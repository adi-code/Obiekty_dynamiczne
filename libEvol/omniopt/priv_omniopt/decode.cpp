#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../OmniOptOptimizer.h"
#include "rand.h"

void OmniOptOptimizer::decode_pop (OmniOptOptimizer::population *pop)
{
    int i;
    if (nbin!=0)
    {
        for (i=0; i<popsize; i++)
        {
            decode_ind (&(pop->ind[i]));
        }
    }
    return;
}

void OmniOptOptimizer::decode_ind (OmniOptOptimizer::individual *ind)
{
//    int j, k;
//    int sum;
//    if (nbin!=0)
//    {
//        for (j=0; j<nbin; j++)
//        {
//            sum=0;
//            for (k=0; k<nbits[j]; k++)
//            {
//                if (ind->gene[j][k]==1)
//                {
//					sum += (int)pow(2, nbits[j]-1-k);
//                }
//            }
//            ind->xbin[j] = min_binvar[j] + (double)sum*(max_binvar[j] - min_binvar[j])/(double)(pow(2,nbits[j])-1);
//        }
//    }
    return;
}
