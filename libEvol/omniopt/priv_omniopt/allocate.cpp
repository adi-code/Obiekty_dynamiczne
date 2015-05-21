#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../OmniOptOptimizer.h"
#include "rand.h"

void OmniOptOptimizer::allocate_memory_pop (OmniOptOptimizer::population *pop, int size)
{
    int i;
    pop->ind = (individual *)malloc(size*sizeof(individual));
    for (i=0; i<size; i++)
    {
        allocate_memory_ind (&(pop->ind[i]));
    }
    return;
}

void OmniOptOptimizer::allocate_memory_ind (individual *ind)
{
    int j;
    if (nreal != 0)
    {
        ind->xreal = (double *)malloc(nreal*sizeof(double));
    }
    if (nbin != 0)
    {
        ind->xbin = (double *)malloc(nbin*sizeof(double));
    }
    ind->obj = (double *)malloc(nobj*sizeof(double));
    if (ncon != 0)
    {
        ind->constr = (double *)malloc(ncon*sizeof(double));
    }
    return;
}

void OmniOptOptimizer::deallocate_memory_pop (OmniOptOptimizer::population *pop, int size)
{
	int i;
	for (i=0; i<size; i++)
	{
		deallocate_memory_ind(&(pop->ind[i]));
	}
	free (pop->ind);
	return;
}

void OmniOptOptimizer::deallocate_memory_ind (individual *ind)
{
	int j;
	if (nreal!=0)
	{
		free (ind->xreal);
	}
	if (nbin!=0)
	{
		free (ind->xbin);
	}
	free (ind->obj);
	if (ncon!=0)
	{
		free(ind->constr);
	}
	return;
}
