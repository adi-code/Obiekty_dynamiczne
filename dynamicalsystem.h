#ifndef DYNAMICALSYSTEM_H
#define DYNAMICALSYSTEM_H

#include "dsidentitymatrix.h"
#include "boost/numeric/ublas/vector.hpp"
#include <vector>
#include <iostream>

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
    matrix_column<DSMatrix> m_u_part;

public:
    /**
     * @brief DynamicalSystem
     * Creates transfer function:
     *
     *         b(n)*s^n + b(n-1)*s^(n-1) + ... + b(1)*s + b(0)
     * G(s) = -------------------------------------------------
     *         a(n)*s^n + a(n-1)*s^(n-1) + ... + a(1)*s + a(0)
     *
     *
     * @param p_system_dimension the system dimension (n)
     * @param p_numerator_parameters numerator parameters ([b(0) b(1) ... b(n)])
     * @param p_denominator_parameters denominator parameters ([b(0) b(1) ... b(n)])
     */
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
