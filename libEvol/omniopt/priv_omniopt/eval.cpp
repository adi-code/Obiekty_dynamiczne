#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../OmniOptOptimizer.h"
#include "rand.h"

void OmniOptOptimizer::evaluate_pop (OmniOptOptimizer::population *pop)
{
    int i;
    for (i=0; i<popsize; i++)
    {
        evaluate_ind (&(pop->ind[i]));
    }
    return;
}

void OmniOptOptimizer::evaluate_ind (OmniOptOptimizer::individual *ind)
{

	double* fulVar = new double [this->nreal + this->nbin];
	for (int i = 0; i < this->nreal; i++) {
		fulVar[i] = ind->xreal[i];
	}
	for (int i = 0; i < this->nbin; i++) {
		fulVar[i+this->nreal] = ind->xbin[i];
	}

	for (int i = 0; i != this->nobj; i++) {
		ind->obj[i] = this->calculateObjectiveFnc(fulVar, this->nreal+ this->nbin, i);
	}

	for (int i = 0; i != this->ncon; i++) {
		ind->constr[i] = -1*this->calculateConstraintFnc(fulVar, this->nreal+ this->nbin, i);
	}


	ind->constr_violation = 0.0;
	for (int j=0; j<ncon; j++)
	{
		if (ind->constr[j]<0.0)
		{
			ind->constr_violation += ind->constr[j];
		}
	}
    delete [] fulVar;
    return;
}

/* Routine to evaluate objective function values and constraints (and constraint violation
   based on a different consideration) for an OmniOptOptimizer::individual */
/*
void OmniOptOptimizer::evaluate_ind (OmniOptOptimizer::individual *ind)
{
    int j;
    test_problem (ind->xreal, ind->xbin, ind->obj, ind->constr);
    if (ncon==0)
    {
        ind->constr_violation = 0.0;
    }
    else
    {
        ind->constr_violation = 0.0;
        for (j=0; j<ncon; j++)
        {
            if (ind->constr_violation > ind->constr[j])
            {
                ind->constr_violation = ind->constr[j];
            }
        }
    }
    return;
}
*/
