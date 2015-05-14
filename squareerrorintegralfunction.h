#ifndef SQUAREERRORINTEGRALFUNCTION_H
#define SQUAREERRORINTEGRALFUNCTION_H

#include "function.h"

class SquareErrorIntegralFunction : public Function
{
public:
    SquareErrorIntegralFunction();
    ~SquareErrorIntegralFunction();

    double calculate(double* p_pf, unsigned int p_nf);
};

#endif // SQUAREERRORINTEGRALFUNCTION_H
