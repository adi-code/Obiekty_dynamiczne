#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../OmniOptOptimizer.h"
#include "rand.h"

void OmniOptOptimizer::mutation_pop (OmniOptOptimizer::population *pop)
{
    int i;
    for (i=0; i<popsize; i++)
    {
        mutation_ind(&(pop->ind[i]));
    }
    return;
}

void OmniOptOptimizer::mutation_ind (OmniOptOptimizer::individual *ind)
{
    if (nreal!=0)
    {
        real_mutate_ind(ind);
    }
    if (nbin!=0)
    {
        bin_mutate_ind(ind);
    }
    return;
}

void OmniOptOptimizer::bin_mutate_ind (OmniOptOptimizer::individual *ind)
{
//    int j, k;
//    double prob;
//    for (j=0; j<nbin; j++)
//    {
//        for (k=0; k<nbits[j]; k++)
//        {
//            prob = randomperc();
//            if (prob <=pmut_bin)
//            {
//                if (ind->gene[j][k] == 0)
//                {
//                    ind->gene[j][k] = 1;
//                }
//                else
//                {
//                    ind->gene[j][k] = 0;
//                }
//                nbinmut+=1;
//            }
//        }
//    }

	int j;
	double rnd, delta1, delta2, mut_pow, deltaq, orig;
	double y, yl, yu, val, xy;
	for (j=0; j<nbin; j++)
	{
        if (randomperc() <= pmut_bin)
		{
			orig = ind->xbin[j];
			y = ind->xbin[j];
			yl = min_binvar[j];
			yu = max_binvar[j];
            delta1 = (y-yl)/(yu-yl);
			delta2 = (yu-y)/(yu-yl);
			rnd = randomperc();
            mut_pow = 1.0/(eta_m+1.0);
			if (rnd <= 0.5)
			{
                xy = 1.0-delta1;
				val = 2.0*rnd+(1.0-2.0*rnd)*(pow(xy,(eta_m+1.0)));
				deltaq =  pow(val,mut_pow) - 1.0;
            }
			else
			{
				xy = 1.0-delta2;
				val = 2.0*(1.0-rnd)+2.0*(rnd-0.5)*(pow(xy,(eta_m+1.0)));
				deltaq = 1.0 - (pow(val,mut_pow));
			}
			y = y + deltaq*(yu-yl);
			if (y<yl)
				y = yl;
			if (y>yu)
				y = yu;
			ind->xbin[j] = y;

			int l = floor(ind->xbin[j]);
			int h = ceil(ind->xbin[j]);
            if (h>max_binvar[j]) {
				h=max_binvar[j];
			} else if (l < min_binvar[j]) {
				l=  min_binvar[j];
			}

			if (l!=h) {
				if (randomperc() < 0.5) {
					ind->xbin[j] = l;
				} else {
					ind->xbin[j] = h;
						}
				} else {
					ind->xbin[j] = l;
				}
			if (orig ==ind->xbin[j]) {
				if (orig==max_binvar[j]) {
					ind->xbin[j] = max_binvar[j]-1;
				} else if (orig == min_binvar[j]) {
					ind->xbin[j] = min_binvar[j]+1;
				} else {
                    if (randomperc() < 0.5) {
						ind->xbin[j] = ind->xbin[j] - 1;
					} else {
						ind->xbin[j] = ind->xbin[j] +1;
					}
				}
			}
			nrealmut+=1;
		}
	}
    return;
}

void OmniOptOptimizer::real_mutate_ind (OmniOptOptimizer::individual *ind)
{
	int j;
	double rnd, delta1, delta2, mut_pow, deltaq;
	double y, yl, yu, val, xy;
	for (j=0; j<nreal; j++)
	{
        if (randomperc() <= pmut_real)
		{
			y = ind->xreal[j];
			yl = min_realvar[j];
			yu = max_realvar[j];
            delta1 = (y-yl)/(yu-yl);
			delta2 = (yu-y)/(yu-yl);
			rnd = randomperc();
            mut_pow = 1.0/(eta_m+1.0);
			if (rnd <= 0.5)
			{
                xy = 1.0-delta1;
				val = 2.0*rnd+(1.0-2.0*rnd)*(pow(xy,(eta_m+1.0)));
				deltaq =  pow(val,mut_pow) - 1.0;
            }
			else
			{
				xy = 1.0-delta2;
				val = 2.0*(1.0-rnd)+2.0*(rnd-0.5)*(pow(xy,(eta_m+1.0)));
				deltaq = 1.0 - (pow(val,mut_pow));
			}
			y = y + deltaq*(yu-yl);
			if (y<yl)
				y = yl;
			if (y>yu)
				y = yu;
			ind->xreal[j] = y;
			nrealmut+=1;
		}
	}
    return;
}
