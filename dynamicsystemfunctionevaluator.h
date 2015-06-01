#ifndef DYNAMICSYSTEMFUNCTIONEVALUATOR_H
#define DYNAMICSYSTEMFUNCTIONEVALUATOR_H

#include <vector>

#include "functionevaluator.h"

#include "dynamicalsystem.h"

#include <boost/numeric/odeint.hpp>
#include <boost/numeric/ublas/vector.hpp>

using namespace boost::numeric;

class DynamicSystemFunctionEvaluator : public FunctionEvaluator
{
public:
    DynamicSystemFunctionEvaluator();
    ~DynamicSystemFunctionEvaluator();

    void SetControlledProcessDimension(unsigned int p_controlled_process_dimension);
    void SetMaxResponseTime(double p_response_time);
    void SetNumeratorParameters(std::vector<double> p_numerator_parameters);
    void SetDenominatorParameters(std::vector<double> p_denominator_parameters);
    void SetOutputFile(const char* p_filename);

protected:
    std::vector<double> evaluateImpl(double* p_pf, unsigned int p_nf);
    double computeOvershoot(const std::vector<double>& p_step_response,
                            const double& p_set_point);
    double computeControlTime(const std::vector<double>& p_step_response,
                              const double& p_set_point,
                              const double& p_sampling_time);
    void saveResponse(const std::string& p_filename, double p_sampling_time,
                      double p_kr, double p_ti, double p_td, double p_kd,
                      const std::vector<double>& response,
                      DynamicalSystem& proc, DynamicalSystem& reg,
                      DynamicalSystem& loop);

private:
    unsigned int m_controlled_process_dimension;
    double m_response_time;
    std::vector<double> m_numerator_parameters;
    std::vector<double> m_denominator_parameters;
    odeint::runge_kutta4<ublas::vector<double> > m_numeric_solver;
    DynamicalSystem* m_controlled_process;
    std::string m_filename;
};

#endif // DYNAMICSYSTEMFUNCTIONEVALUATOR_H
