#ifndef DSIDENTITYMATRIX_H
#define DSIDENTITYMATRIX_H

#include "dsmatrix.h"

class DSIdentityMatrix : public DSMatrix
{
public:
    DSIdentityMatrix(unsigned int p_size);
};

#endif // DSIDENTITYMATRIX_H
