#include "dynamicsystemfunctionevaluator.h"

#include "regulator.h"

#include <limits>
#include <fstream>
#include <iomanip>

DynamicSystemFunctionEvaluator::DynamicSystemFunctionEvaluator()
{

}

DynamicSystemFunctionEvaluator::~DynamicSystemFunctionEvaluator()
{

}

void DynamicSystemFunctionEvaluator::SetControlledProcessDimension(
        unsigned int p_controlled_process_dimension)
{
    m_controlled_process_dimension = p_controlled_process_dimension;
}

void DynamicSystemFunctionEvaluator::SetMaxResponseTime(double p_response_time)
{
    m_response_time = p_response_time;
}

void DynamicSystemFunctionEvaluator::SetNumeratorParameters(
        std::vector<double> p_numerator_parameters)
{
    m_numerator_parameters = p_numerator_parameters;
}

void DynamicSystemFunctionEvaluator::SetDenominatorParameters(std::vector<double> p_denominator_parameters)
{
    m_denominator_parameters = p_denominator_parameters;
}

void DynamicSystemFunctionEvaluator::SetOutputFile(const char *p_filename) {
    m_filename = p_filename;
}

std::vector<double> DynamicSystemFunctionEvaluator::evaluateImpl(double* p_pf, unsigned int /*p_nf*/) {
    double p_kr = p_pf[0];
    double p_ti = p_pf[1];
    double p_td = p_pf[2];
    double p_kd = p_pf[3];

    // TODO: remove p_max_time (or rename)
    double p_max_time = m_response_time;
    double p_control_time = std::numeric_limits<double>::max();//10000000000000;
    double p_overshoot = std::numeric_limits<double>::max();//10000;
    double p_integral_of_square_error = std::numeric_limits<double>::max();//1000000000000;

    if(p_kr != 0)
    {
//        m_controlled_process = new DynamicalSystem(
//                        m_controlled_process_dimension,
//                        m_numerator_parameters,
//                        m_denominator_parameters);
//        Regulator *regulator = new Regulator(p_kr, p_ti, p_td, p_kd);
        Regulator regulator(p_kr, p_ti, p_td, p_kd);
//        DynamicalSystem control_system(m_controlled_process, regulator);
        DynamicalSystem controlled_process(m_controlled_process_dimension,
                        m_numerator_parameters,
                        m_denominator_parameters);
//        DynamicalSystem control_system(&controlled_process, regulator);
        DynamicalSystem control_system(&controlled_process, &regulator);
        if(control_system.IsSystemStable())
        {
            p_overshoot = 0;
            p_integral_of_square_error = 0;
            p_control_time = p_max_time;
            boost::numeric::ublas::vector<double> state_vector;
            boost::numeric::ublas::vector<double> y;
            state_vector.resize(control_system.GetSystemDimension());
//            state_vector.clear();
            y.resize(1, 1);
//            y.clear();
            std::vector<double> full_step_response;
//            full_step_response.clear();
            full_step_response.push_back(0);

            std::pair<double, double> time_res_params = control_system.GetTimeResponseParams();
            // don't assign max response time here
            // beacause it's often too long
//            p_max_time = time_res_params.second;
            double time_interval = time_res_params.first;
            double set_point = 1.0;
            int max_time_index = p_max_time / time_interval;

            m_numeric_solver.adjust_size(state_vector);

            DSMatrix c_matrix = control_system.GetCMatrix();
            DSMatrix d_matrix = control_system.GetDMatrix();
            matrix_column<DSMatrix> u_part = column(d_matrix, 0);


            try {
                for(int time_index = 0;
                    time_index <= max_time_index;
                    time_index++)
                {

                        m_numeric_solver.do_step(control_system,
                                          state_vector,
                                          time_index * time_interval,
                                          time_interval);
                        axpy_prod(c_matrix, state_vector, y);
                        y += u_part;


                    if(fabs(y(0) - set_point) > pow(10, 50))
                    {
                       // throw too_big_value;
                        p_control_time = std::numeric_limits<double>::max();
                        p_overshoot = std::numeric_limits<double>::max();
                        p_integral_of_square_error = std::numeric_limits<double>::max();
                        std::vector<double> result = {p_control_time, p_overshoot, p_integral_of_square_error};
                        return result;
                    }

                    full_step_response.push_back(y(0));

                    p_integral_of_square_error += (y(0) - set_point) * (y(0) - set_point) * time_interval;
                }
            } catch(const boost::numeric::ublas::bad_size& /*e*/) {
            }

            p_overshoot = computeOvershoot(full_step_response, set_point);
            p_control_time = computeControlTime(full_step_response, set_point, time_interval);

//            int time_index = 2;
//            while(p_overshoot == 0 && time_index <= max_time_index)
//            {
//                if(full_step_response[time_index] < full_step_response[time_index - 1] &&
//                        full_step_response[time_index - 2] < full_step_response[time_index - 1] &&
//                        full_step_response[time_index - 1] > set_point)
//                {
//                    p_overshoot = (full_step_response[time_index - 1] - set_point) * 100;
//                }
//                time_index++;
//            }
//            if(fabs(full_step_response[max_time_index] - set_point) <= 0.05)
//            {
//                time_index = max_time_index - 1;
//                while(time_index >= 0)
//                {
//                    if(fabs(full_step_response[time_index] - set_point) > 0.05)
//                    {
//                        p_control_time = (time_index + 1) * time_interval;
//                        break;
//                    }
//                    time_index--;
//                }
//            }

            bool save = !m_filename.empty();

            if(save) {
                saveResponse(m_filename, time_interval, p_kr, p_ti, p_td, p_kd,
                             full_step_response, controlled_process, regulator, control_system);
            }
        }
        else
        {
            p_control_time = std::numeric_limits<double>::max();
            p_overshoot = std::numeric_limits<double>::max();
            p_integral_of_square_error = std::numeric_limits<double>::max();
        }

//        delete regulator;
    }
    else
    {
        p_control_time = std::numeric_limits<double>::max();
        p_overshoot = std::numeric_limits<double>::max();
        p_integral_of_square_error = std::numeric_limits<double>::max();
    }

//    p_control_time = std::numeric_limits<double>::max();
//    p_overshoot = std::numeric_limits<double>::max();
//    p_integral_of_square_error = std::numeric_limits<double>::max();

    std::vector<double> result = {p_control_time, p_overshoot, p_integral_of_square_error};
    return result;
}

double DynamicSystemFunctionEvaluator::computeOvershoot(const std::vector<double>& p_step_response,
                                                        const double& p_set_point) {
    unsigned int time_index = 2;
    double overshoot = 0.0;
    unsigned int max_time_index = p_step_response.size() - 1;

    while(overshoot == 0 && time_index <= max_time_index)
    {
        if(p_step_response[time_index] < p_step_response[time_index - 1] &&
                p_step_response[time_index - 2] < p_step_response[time_index - 1] &&
                p_step_response[time_index - 1] > p_set_point)
        {
            overshoot = (p_step_response[time_index - 1] - p_set_point) * 100;
        }
        time_index++;
    }

    return overshoot;
}

double DynamicSystemFunctionEvaluator::computeControlTime(const std::vector<double>& p_step_response,
                                                          const double& p_set_point,
                                                          const double& p_sampling_time) {
    unsigned int max_time_index = p_step_response.size() - 1;
    double control_time = max_time_index * p_sampling_time;

    if(fabs(p_step_response[max_time_index] - p_set_point) <= 0.05)
    {
        int time_index = max_time_index - 1;
        while(time_index >= 0)
        {
            if(fabs(p_step_response[time_index] - p_set_point) > 0.05)
            {
                control_time = (time_index + 1) * p_sampling_time;
                break;
            }
            time_index--;
        }
    }

    return control_time;
}

void DynamicSystemFunctionEvaluator::saveResponse(const std::string& p_filename, double p_sampling_time,
                                                  double p_kr, double p_ti, double p_td, double p_kd,
                                                  const std::vector<double>& response,
                                                  DynamicalSystem& proc, DynamicalSystem& reg,
                                                  DynamicalSystem& loop) {

    std::ofstream out(p_filename.c_str());
    auto writeVector = [](std::ostream& out, const std::vector<double>& v) {
        for(unsigned int i=0;i<v.size();++i) {
            out << v[i] << ' ';
        }
        out << '\n';
    };
    out << p_kr << ' ' << p_ti << ' ' << p_td << ' ' << p_kd << ' ' << p_sampling_time << '\n';
    writeVector(out, proc.GetNumerator());
    writeVector(out, proc.GetDenominator());
    writeVector(out, reg.GetNumerator());
    writeVector(out, reg.GetDenominator());
    writeVector(out, loop.GetNumerator());
    writeVector(out, loop.GetDenominator());
    writeVector(out, response);
    out.close();
}
