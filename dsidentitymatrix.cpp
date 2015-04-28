#include "dsidentitymatrix.h"

DSIdentityMatrix::DSIdentityMatrix(unsigned int p_size) :
    DSMatrix (p_size, p_size)
{
    for(unsigned int row_size = 1; row_size <= p_size; row_size ++)
    {
        for(int column_size = 1; column_size <= p_size; column_size++)
        {
            if(row_size == column_size)
            {
                data()[(row_size - 1) * size1() + (column_size - 1)] = 1;
            }
            else
            {
                data()[(row_size - 1) * size1() + (column_size - 1)] = 0;
            }
        }
    }
}
