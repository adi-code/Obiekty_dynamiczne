#include "controltimefunction.h"

ControlTimeFunction::ControlTimeFunction()
{

}

ControlTimeFunction::~ControlTimeFunction()
{

}

double ControlTimeFunction::calculate(double* p_pf, unsigned int p_nf) {
    double a[] = {p_pf[0], p_pf[1], p_pf[2], p_pf[3]};
    std::vector<double> result = m_evaluator->evaluate(p_pf, p_nf);
    return result[0];
}
