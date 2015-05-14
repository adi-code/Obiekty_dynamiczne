#ifndef CONTROLTIMEFUNCTION_H
#define CONTROLTIMEFUNCTION_H

#include "function.h"

class ControlTimeFunction : public Function
{
public:
    ControlTimeFunction();
    ~ControlTimeFunction();

    double calculate(double* p_pf, unsigned int p_nf);
};

#endif // CONTROLTIMEFUNCTION_H
