#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../OmniOptOptimizer.h"
#include "rand.h"

void OmniOptOptimizer::initialize_pop (OmniOptOptimizer::population *pop)
{
    if (input_type==0)
    {
        initialize_random_pop(pop);
    }
    else
    {
        if (input_type==1)
        {
            initialize_latin_pop(pop);
        }
        else
        {
            initialize_file_pop(pop);
        }
    }
    return;
}

void OmniOptOptimizer::initialize_random_pop (OmniOptOptimizer::population *pop)
{
    int i;
    for (i=0; i<popsize; i++)
    {
        int j, k;
        double rand;
        if (nreal!=0)
        {
            for (j=0; j<nreal; j++)
            {
				pop->ind[i].xreal[j] = rndreal (min_realvar[j], max_realvar[j]);
            }
        }
        if (nbin!=0)
        {
            for (i=0; i<popsize; i++)
            {
                for (j=0; j<nbin; j++)
                {
					pop->ind[i].xbin[j] = rndreal (min_binvar[j], max_binvar[j]);
					int l = floor(pop->ind[i].xbin[j]);
					int h = ceil(pop->ind[i].xbin[j]);
					if (h>max_binvar[j]) {
						h=max_binvar[j];
					} else if (l < min_binvar[j]) {
						l=  min_binvar[j];
					}

					if (l!=h) {
						if (randomperc() < 0.5) {
							pop->ind[i].xbin[j] = l;
						} else {
							pop->ind[i].xbin[j] = h;
						}
					} else {
							pop->ind[i].xbin[j] = l;
					}

				}
            }
        }
    }
    return;
}

void OmniOptOptimizer::initialize_latin_pop (OmniOptOptimizer::population *pop)
{
    int i, j, k;
    int *a;
    int temp;
    int rand;
	double random;
    double *grid, *val;
    a = (int *)malloc(popsize*sizeof(int));
    grid = (double *)malloc(nreal*sizeof(double));
    val = (double *)malloc(popsize*sizeof(double));
    if (nreal!=0)
    {
        for (j=0; j<nreal; j++)
        {
            grid[j] = (max_realvar[j]-min_realvar[j])/(double)popsize;
        }
        for (i=0; i<popsize; i++)
        {
            a[i] = i;
        }
        for (j=0; j<nreal; j++)
        {
            for (i=0; i<popsize; i++)
            {
                rand = rnd (i, popsize-1);
                temp = a[rand];
                a[rand] = a[i];
                a[i] = temp;
            }
            for (i=0; i<popsize; i++)
            {
                val[i] = rndreal(min_realvar[j]+grid[j]*i, min_realvar[j]+grid[j]*(i+1));
                if (val[i] < min_realvar[j])
                    val[i] = min_realvar[j];
                if (val[i] > max_realvar[j])
                    val[i] = max_realvar[j];
                pop->ind[a[i]].xreal[j] = val[i];
            }
        }
        if (nbin!=0)
        {
            for (i=0; i<popsize; i++)
            {
                for (j=0; j<nbin; j++)
                {
					pop->ind[i].xbin[j] = rndreal (min_binvar[j], max_binvar[j]);
					int l = floor(pop->ind[i].xbin[j]);
					int h = ceil(pop->ind[i].xbin[j]);
					if (h>max_binvar[j]) {
						h=max_binvar[j];
					} else if (l < min_binvar[j]) {
						l=  min_binvar[j];
					}

					if (l!=h) {
						if (randomperc() < 0.5) {
							pop->ind[i].xbin[j] = l;
						} else {
							pop->ind[i].xbin[j] = h;
						}
					} else {
							pop->ind[i].xbin[j] = l;
					}

				}
            }
        }
    }
    free (a);
    free (grid);
    free (val);
    return;
}

void OmniOptOptimizer::initialize_file_pop (OmniOptOptimizer::population *pop)
{
    FILE *fpt;
    int i, j, k;
    fpt = fopen("input_var.in","r");
    if (fpt==NULL)
    {
        printf("\n Input data file for initialization-from-file not found.");
        printf("\n The file 'input_var.in' does not exist, exiting \n");
        exit(1);
    }
    for (i=0; i<popsize; i++)
    {
        if (nreal!=0)
        {
            for (j=0; j<nreal; j++)
            {
                fscanf(fpt,"%lf\t",&(pop->ind[i].xreal[j]));
            }
        }
        if (nbin!=0)
        {
            for (j=0; j<nbin; j++)
            {
//                for (k=0; k<nbits[j]; k++)
//                {
//                    fscanf(fpt,"%d\t",&(pop->ind[i].gene[j][k]));
//                }
            }
        }
        fscanf(fpt,"\n");
    }
    fclose(fpt);
    return;
}
