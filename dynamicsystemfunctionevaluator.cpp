#include "dynamicsystemfunctionevaluator.h"

#include "regulator.h"

#include <limits>

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
        Regulator *regulator = new Regulator(p_kr, p_ti, p_td, p_kd);
//        DynamicalSystem control_system(m_controlled_process, regulator);
        DynamicalSystem controlled_process(m_controlled_process_dimension,
                        m_numerator_parameters,
                        m_denominator_parameters);
        DynamicalSystem control_system(&controlled_process, regulator);
        if(control_system.IsSystemStable())
        {
            p_overshoot = 0;
            p_integral_of_square_error = 0;
            p_control_time = p_max_time;
            boost::numeric::ublas::vector<double> state_vector;
            boost::numeric::ublas::vector<double> y;
            state_vector.resize(control_system.GetSystemDimension());
            state_vector.clear();
            y.resize(1, 1);
            y.clear();
            std::vector<double> full_step_resposne;
            full_step_resposne.clear();
            full_step_resposne.push_back(0);
            double time_interval = 0.01;
            int max_time_index = p_max_time / time_interval;

            m_numeric_solver.adjust_size(state_vector);

            for(int time_index = 0;
                time_index <= max_time_index;
                time_index++)
            {
                p_integral_of_square_error += (full_step_resposne[time_index] - 1) *
                        (full_step_resposne[time_index] - 1) * time_interval;
                try {
                    m_numeric_solver.do_step(control_system,
                                      state_vector,
                                      time_index * time_interval,
                                      time_interval);
                    axpy_prod(control_system.GetCMatrix(), state_vector, y);
                    y += column(control_system.GetDMatrix(), 0);
                } catch(const boost::numeric::ublas::bad_size& e) {
                }

                if(fabs(y(0) - 1) > pow(10, 50))
                {
                    throw too_big_value;
                }
                full_step_resposne.push_back(y(0));
            }
            int time_index = 2;
            while(p_overshoot == 0 && time_index <= max_time_index)
            {
                if(full_step_resposne[time_index] < full_step_resposne[time_index - 1] &&
                        full_step_resposne[time_index - 2] < full_step_resposne[time_index - 1] &&
                        full_step_resposne[time_index - 1] > 1)
                {
                    p_overshoot = (full_step_resposne[time_index - 1] - 1) * 100;
                }
                time_index++;
            }
            if(fabs(full_step_resposne[max_time_index] - 1) <= 0.05)
            {
                time_index = max_time_index - 1;
                while(time_index >= 0)
                {
                    if(fabs(full_step_resposne[time_index] - 1) > 0.05)
                    {
                        p_control_time = (time_index + 1) * time_interval;
                        break;
                    }
                    time_index--;
                }
            }
        }
        else
        {
            p_control_time = std::numeric_limits<double>::max();//10000000000000;
            p_overshoot = std::numeric_limits<double>::max();//10000;
            p_integral_of_square_error = std::numeric_limits<double>::max();//1000000000000;
        }

        delete regulator;
    }
    else
    {
        p_control_time = std::numeric_limits<double>::max();//10000000000000;
        p_overshoot = std::numeric_limits<double>::max();//10000;
        p_integral_of_square_error = std::numeric_limits<double>::max();//1000000000000;
    }

    std::vector<double> result = {p_control_time, p_overshoot, p_integral_of_square_error};
    return result;
}
