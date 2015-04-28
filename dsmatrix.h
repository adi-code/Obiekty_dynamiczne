#ifndef DSMATRIX_H
#define DSMATRIX_H

#include "enumerated_types.h"
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "boost/numeric/ublas/operation.hpp"

using namespace boost::numeric::ublas;

class DSMatrix : public matrix<double>
{
public:
    DSMatrix(unsigned int p_row_size, unsigned int p_column_size);
    DSMatrix();
    void InvertMatrix ();

    void SetAsResultOfMultiply(const DSMatrix &p_first_matrix,
                            const DSMatrix &p_second_matrix);
    double GetDeterminant();

    DSMatrix &operator = (const DSMatrix &p_matrix);
    DSMatrix &operator = (const matrix<double> &p_matrix);




};

#endif // DSMATRIX_H
