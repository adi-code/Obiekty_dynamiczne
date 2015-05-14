#include "overshootfunction.h"

OvershootFunction::OvershootFunction()
{

}

OvershootFunction::~OvershootFunction()
{

}

double OvershootFunction::calculate(double* p_pf, unsigned int p_nf) {
    std::vector<double> result = m_evaluator->evaluate(p_pf, p_nf);
    return result[1];
}
