#ifndef OVERSHOOTFUNCTION_H
#define OVERSHOOTFUNCTION_H

#include "function.h"

class OvershootFunction : public Function
{
public:
    OvershootFunction();
    ~OvershootFunction();

    double calculate(double* p_pf, unsigned int p_nf);
};

#endif // OVERSHOOTFUNCTION_H
