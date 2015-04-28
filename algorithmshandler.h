#ifndef ALGORITHMSHANDLER_H
#define ALGORITHMSHANDLER_H

#include <QObject>
#include "regulator.h"

using namespace boost::numeric;

class AlgorithmsHandler : public QObject
{
    Q_OBJECT
private:
    unsigned int m_controlled_process_dimension;
    double m_max_response_time;
    std::vector<double> m_numerator_parameters;
    std::vector<double> m_denominator_parameters;

public:
    AlgorithmsHandler();
    void SetControlledProcessDimension(
            unsigned int p_controlled_process_dimension);
    void SetMaxResponseTime(double p_max_response_time);
    void SetNumeratorParameters(std::vector<double> p_numerator_parameters);
    void SetDenominatorParameters(std::vector<double> p_denominator_parameters);

private:
    void Evaluate(DynamicalSystem p_system,
                  odeint::runge_kutta4<ublas::vector<double> > p_numeric_solver,
                  double p_max_time,
                  double &p_control_time,
                  double &p_overshoot,
                  double &p_integral_of_square_error);

private slots:
    void RunAlgorithm();

signals:
    void EndOfAlgorithm();
};

#endif // ALGORITHMSHANDLER_H
