#include "controltimefunction.h"

ControlTimeFunction::ControlTimeFunction()
{

}

ControlTimeFunction::~ControlTimeFunction()
{

}

double ControlTimeFunction::calculate(double* p_pf, unsigned int p_nf) {
    std::vector<double> result = m_evaluator->evaluate(p_pf, p_nf);
    return result[0];
}
