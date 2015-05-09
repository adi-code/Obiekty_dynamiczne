#include "algorithmshandler.h"

#include "algorithmselector.h"
#include "controltimefunction.h"
#include "dynamicsystemfunctionevaluator.h"
#include "overshootfunction.h"
#include "squareerrorintegralfunction.h"

AlgorithmsHandler::AlgorithmsHandler()
{
    m_control_time_function.setEvaluator(&m_evaluator);
    m_overshoot_function.setEvaluator(&m_evaluator);
    m_sqi_function.setEvaluator(&m_evaluator);
}

void AlgorithmsHandler::SetControlledProcessDimension(
        unsigned int p_controlled_process_dimension)
{
    m_controlled_process_dimension = p_controlled_process_dimension;
}

void AlgorithmsHandler::SetMaxResponseTime(double p_response_time)
{
    m_response_time = p_response_time;
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

void AlgorithmsHandler::SetAlgorithmParameters(const std::vector<double> &p_params) {
    m_algorithm_params = p_params;
}

void AlgorithmsHandler::Evaluate(double p_max_time,
                                 double &p_control_time,
                                 double &p_overshoot,
                                 double &p_integral_of_square_error,
                                 double p_kr,
                                 double p_ti,
                                 double p_td,
                                 double p_kd)
{
    if(p_kr != 0)
    {
        Regulator *regulator = new Regulator(p_kr, p_ti, p_td, p_kd);
        DynamicalSystem control_system(m_controlled_process, regulator);
        if(control_system.IsSystemStable())
        {
            p_overshoot = 0;
            p_integral_of_square_error = 0;
            p_control_time = p_max_time;
            ublas::vector<double> state_vector;
            ublas::vector<double> y;
            state_vector.resize(control_system.GetSystemDimension());
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
                m_numeric_solver.do_step(control_system,
                                  state_vector,
                                  time_index * time_interval,
                                  time_interval);
                axpy_prod(control_system.GetCMatrix(), state_vector, y);
                y += column(control_system.GetDMatrix(), 0);
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
            p_control_time = 10000000000000;
            p_overshoot = 10000;
            p_integral_of_square_error = 1000000000000;
        }

        delete regulator;
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
//    m_controlled_process = new DynamicalSystem(
//                m_controlled_process_dimension,
//                m_numerator_parameters,
//                m_denominator_parameters);
//    double control_time;
//    double overshoot;
//    double integral_of_square_error;
//    try
//    {
//        Evaluate(m_response_time, control_time,
//                 overshoot, integral_of_square_error,
//                 2, 20, 10, 10);
//    }
//    catch(EXCEPTION_TYPE evaluate_exception)
//    {
//        if(evaluate_exception == too_big_value)
//        {
//            control_time = 10000000000000;
//            overshoot = 10000;
//            integral_of_square_error = 1000000000000;
//        }
//    }

//    std::cout << control_time << " " << overshoot << " " << integral_of_square_error << std::endl;
//    delete m_controlled_process;
//    emit EndOfAlgorithm();

    AlgorithmRunner* runner = m_alg_selector.getAlgorithmRunner();
    AlgorithmSettings* settings = m_alg_selector.getAlgorithmSettings();

    settings->setParameters(m_algorithm_params);

    settings->addObjectiveFunction(&m_control_time_function);
    settings->addObjectiveFunction(&m_overshoot_function);
    settings->addObjectiveFunction(&m_sqi_function);

    runner->configure(settings);
    runner->run();
}

void AlgorithmsHandler::SelectAlgorithm(unsigned int p_i) {
    m_alg_selector.select(p_i);

    AlgorithmSettings* settings = m_alg_selector.getAlgorithmSettings();
    std::vector<AlgorithmSetting> settingList = settings->getSettingsList();
    for(unsigned int i=0;i<settingList.size();++i) {
        AlgorithmSetting s = settingList[i];
        emit ShowWidget(QString::fromUtf8(s.m_label.c_str(), s.m_label.size()),
                        s.m_type, s.m_min, s.m_max, s.m_default);
    }

//    emit ShowWidget("ABC", 0, 0.0, 1.0, 0.5);
//    emit ShowWidget("DEF", 1, 0, 1000, 100);

//    AlgorithmSelector selector;
//    AlgorithmRunner* runner = selector.getAlgorithmRunner();
//    AlgorithmSettings* settings = selector.getAlgorithmSettings();

//    DynamicSystemFunctionEvaluator evaluator;

//    ControlTimeFunction controlTimeFunction;
//    OvershootFunction overshootFunction;
//    SquareErrorIntegralFunction sqiFunction;

//    controlTimeFunction.setEvaluator(&evaluator);
//    overshootFunction.setEvaluator(&evaluator);
//    sqiFunction.setEvaluator(&evaluator);

//    settings->setParameters(m_algorithm_params);

//    settings->addObjectiveFunction(&controlTimeFunction);
//    settings->addObjectiveFunction(&overshootFunction);
//    settings->addObjectiveFunction(&sqiFunction);

//    runner->configure(settings);
//    runner->run();
}
