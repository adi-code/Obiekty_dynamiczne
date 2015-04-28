#include "algorithmshandler.h"

AlgorithmsHandler::AlgorithmsHandler()
{
}

void AlgorithmsHandler::SetControlledProcessDimension(
        unsigned int p_controlled_process_dimension)
{
    m_controlled_process_dimension = p_controlled_process_dimension;
}

void AlgorithmsHandler::SetMaxResponseTime(double p_max_response_time)
{
    m_max_response_time = p_max_response_time;
}

void AlgorithmsHandler::SetNumeratorParameters(
        std::vector<double> p_numerator_parameters)
{
    m_numerator_parameters = p_numerator_parameters;
}

void AlgorithmsHandler::SetDenominatorParameters(std::vector<double> p_denominator_parameters)
{
    m_denominator_parameters = p_denominator_parameters;
}

void AlgorithmsHandler::Evaluate(DynamicalSystem p_system,
                                 odeint::runge_kutta4<ublas::vector<double> > p_numeric_solver,
                                 double p_max_time,
                                 double &p_control_time,
                                 double &p_overshoot,
                                 double &p_integral_of_square_error)
{
    if(p_system.IsSystemStable())
    {
        p_overshoot = 0;
        p_integral_of_square_error = 0;
        p_control_time = p_max_time;
        ublas::vector<double> state_vector;
        ublas::vector<double> y;
        state_vector.resize(p_system.GetSystemDimension());
        state_vector.clear();
        y.resize(1, 1);
        y.clear();
        std::vector<double> full_step_resposne;
        full_step_resposne.clear();
        full_step_resposne.push_back(0);
        double time_interval = 0.01;
        int max_time_index = p_max_time / time_interval;
        for(int time_index = 0;
            time_index <= max_time_index;
            time_index++)
        {
            p_integral_of_square_error += (full_step_resposne[time_index] - 1) *
                    (full_step_resposne[time_index] - 1) * time_interval;
            p_numeric_solver.do_step(p_system,
                              state_vector,
                              time_index * time_interval,
                              time_interval);
            axpy_prod(p_system.GetCMatrix(), state_vector, y);
            y += column(p_system.GetDMatrix(), 0);
            if(fabs(y(0) - 1) > pow(10, 10))
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
        p_control_time = 10000000000000;
        p_overshoot = 10000;
        p_integral_of_square_error = 1000000000000;
    }
}

void AlgorithmsHandler::RunAlgorithm()
{
    DynamicalSystem *controlled_process = new DynamicalSystem(
                m_controlled_process_dimension,
                m_numerator_parameters,
                m_denominator_parameters);
    Regulator *regulator = new Regulator(2, 20, 10, 10);
    DynamicalSystem control_system(controlled_process, regulator);
    odeint::runge_kutta4<ublas::vector<double> > numeric_solver;
    double control_time;
    double overshoot;
    double integral_of_square_error;
    try
    {
        Evaluate(control_system, numeric_solver, m_max_response_time,
                 control_time, overshoot, integral_of_square_error);
    }
    catch(EXCEPTION_TYPE evaluate_exception)
    {
        if(evaluate_exception == too_big_value)
        {
            control_time = 10000000000000;
            overshoot = 10000;
            integral_of_square_error = 1000000000000;
        }
    }

    std::cout << control_time << " " << overshoot << " " << integral_of_square_error << std::endl;
    delete controlled_process;
    delete regulator;
    emit EndOfAlgorithm();
}
