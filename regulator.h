#ifndef REGULATOR_H
#define REGULATOR_H

#include <dynamicalsystem.h>

class Regulator : public DynamicalSystem
{
public:
    double m_kr;
    double m_ti;
    double m_td;
    double m_kd;

public:
    Regulator(double p_kr,
              double p_ti,
              double p_td,
              double p_kd);
};

#endif // REGULATOR_H
