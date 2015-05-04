#include "dsmatrix.h"

DSMatrix::DSMatrix(unsigned int p_row_size, unsigned int p_column_size) :
    matrix<double> (p_row_size, p_column_size)
{

}

DSMatrix::DSMatrix() : matrix<double> ()
{

}

void DSMatrix::InvertMatrix ()
{
    DSMatrix copied_matrix(*this);
    permutation_matrix<std::size_t> auxilary_matrix(copied_matrix.size1());
    int factorizing_result = lu_factorize(copied_matrix, auxilary_matrix);
    if( factorizing_result != 0 )
    {
        throw singular_matrix;
    }
    this->assign(identity_matrix<double>(copied_matrix.size1()));
    lu_substitute(copied_matrix, auxilary_matrix, *this);
}

void DSMatrix::SetAsResultOfMultiply(const DSMatrix &p_first_matrix,
                                     const DSMatrix &p_second_matrix)
{
    DSMatrix result_matrix;
    result_matrix.resize(p_first_matrix.size1(), p_second_matrix.size2());
    axpy_prod(p_first_matrix, p_second_matrix, result_matrix);
    resize(p_first_matrix.size1(), p_second_matrix.size2());
    assign(result_matrix);
}

double DSMatrix::GetDeterminant()
{
    permutation_matrix<std ::size_t> auxilary_matrix(size1());
        double determinant = 1.0;
        if(lu_factorize(*this, auxilary_matrix) ) {
            determinant = 0.0;
        } else {
            for(int i = 0; i < size1(); i++)
            {
                determinant *= data()[i * size1()+ i];
            }
            int sign = 1;
            for (std::size_t i = 0; i < size1(); ++i)
            {
                if (i != auxilary_matrix(i))
                {
                    sign *= -1.0;
                }
            }
            determinant = determinant * sign;
        }
        return determinant;
}

DSMatrix &DSMatrix::operator = (const DSMatrix &p_matrix)
{
    resize(p_matrix.size1(), p_matrix.size2());
    data () = p_matrix.data ();
    return *this;
}

DSMatrix &DSMatrix::operator =(const matrix<double> &p_matrix)
{
    resize(p_matrix.size1(), p_matrix.size2());
    data () = p_matrix.data ();
    return *this;
}
