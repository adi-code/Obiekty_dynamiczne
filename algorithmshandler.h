#ifndef ALGORITHMSHANDLER_H
#define ALGORITHMSHANDLER_H

#include <QObject>
#include "algorithmselector.h"
#include "controltimefunction.h"
#include "dynamicsystemfunctionevaluator.h"
#include "overshootfunction.h"
#include "squareerrorintegralfunction.h"
#include "regulator.h"

#include <boost/numeric/odeint.hpp>

using namespace boost::numeric;

class AlgorithmsHandler : public QObject
{
    Q_OBJECT
private:
    unsigned int m_controlled_process_dimension;
    double m_response_time;
    std::vector<double> m_numerator_parameters;
    std::vector<double> m_denominator_parameters;
    odeint::runge_kutta4<ublas::vector<double> > m_numeric_solver;
    DynamicalSystem* m_controlled_process;
    std::vector<double> m_algorithm_params;
    unsigned int m_pop_size;
    unsigned int m_iters;
    double m_kr_max;
    double m_ti_max;
    double m_td_max;
    double m_kd_max;
    QThread* m_parent_thread;

    AlgorithmSelector m_alg_selector;
    DynamicSystemFunctionEvaluator m_evaluator;
    ControlTimeFunction m_control_time_function;
    OvershootFunction m_overshoot_function;
    SquareErrorIntegralFunction m_sqi_function;

    std::vector<std::vector<double> > m_results;

public:
    AlgorithmsHandler();
    void SetControlledProcessDimension(
            unsigned int p_controlled_process_dimension);
    void SetMaxResponseTime(double p_response_time);
    void SetNumeratorParameters(std::vector<double> p_numerator_parameters);
    void SetDenominatorParameters(std::vector<double> p_denominator_parameters);
    void SetAlgorithmParameters(const std::vector<double>& p_params);
    void SetPopulationSize(const unsigned int& p_size);
    void SetIterations(const unsigned int& p_iters);
    void SetMaxValues(const double& p_kr_max, const double& p_ti_max,
                      const double& p_td_max, const double& p_kd_max);
    void SelectAlgorithm(unsigned int p_i);
    std::vector<std::vector<double> > GetResults();
    void Notify(const char* p_msg);
    void setParentThread(QThread *p_parent_thread);
    void SaveResults();

private:
    void Evaluate(double p_max_time,
                  double &p_control_time,
                  double &p_overshoot,
                  double &p_integral_of_square_error,
                  double p_kr,
                  double p_ti,
                  double p_td,
                  double p_kd);

private slots:
    void RunAlgorithm();

signals:
    void EndOfAlgorithm();
    void ShowWidget(const QString& p_label, unsigned int p_type = 0,
                    double p_low_limit = 0, double p_high_limit = 1,
                    double p_value = 0);
    void SendMessage(const QString& p_msg);
};

#endif // ALGORITHMSHANDLER_H
