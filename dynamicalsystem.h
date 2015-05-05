#ifndef DYNAMICALSYSTEM_H
#define DYNAMICALSYSTEM_H

#include "dsidentitymatrix.h"
#include "boost/numeric/odeint.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include <vector>
#include <iostream>

using namespace boost::numeric::ublas;

class DynamicalSystem
{
protected:
    unsigned int m_system_dimension;
    DSMatrix m_a_matrix;
    DSMatrix m_b_matrix;
    DSMatrix m_c_matrix;
    DSMatrix m_d_matrix;
    std::vector<double> m_numerator_parameters;
    std::vector<double> m_denominator_parameters;

public:
    DynamicalSystem(unsigned int p_system_dimension,
                    std::vector<double> p_numerator_parameters,
                    std::vector<double> p_denominator_parameters);
    DynamicalSystem(DynamicalSystem *p_controlled_process,
                    DynamicalSystem *p_regulator);
    DynamicalSystem();

    unsigned int GetSystemDimension();
    DSMatrix GetAMatrix();
    DSMatrix GetBMatrix();
    DSMatrix GetCMatrix();
    DSMatrix GetDMatrix();
    std::vector<double> GetNumerator();
    std::vector<double> GetDenominator();
    bool IsSystemStable();

    void operator() ( const boost::numeric::ublas::vector<double> &x ,
                      boost::numeric::ublas::vector<double> &dxdt , const double t);

protected:
    void CalculateMatrices();



};

#endif // DYNAMICALSYSTEM_H
