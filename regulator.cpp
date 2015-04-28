#include "regulator.h"

Regulator::Regulator(double p_kr,
                     double p_ti,
                     double p_td,
                     double p_kd) :
    DynamicalSystem(2),
    m_kr(p_kr),
    m_ti(p_ti),
    m_td(p_td),
    m_kd(p_kd)
{
    m_numerator_parameters.clear();
    m_numerator_parameters.push_back(m_kr * m_kd);
    m_numerator_parameters.push_back(m_kr * (m_kd * m_ti + m_td));
    m_numerator_parameters.push_back(m_kr * m_ti * m_td * (m_kd + 1));
    m_denominator_parameters.clear();
    m_denominator_parameters.push_back(0);
    m_denominator_parameters.push_back(m_kd * m_ti);
    m_denominator_parameters.push_back(m_ti * m_td);

    CalculateMatrices();

}
