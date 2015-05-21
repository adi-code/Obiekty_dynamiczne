#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../OmniOptOptimizer.h"
#include "rand.h"

void OmniOptOptimizer::input ()
{
//    int i;
//    popsize = 128;
//    ngen = 1000;
//    nobj = 2;
//    min_obj = (double *)malloc(nobj*sizeof(double));
//    max_obj = (double *)malloc(nobj*sizeof(double));
//    epsilon = (double *)malloc(nobj*sizeof(double));
//    ncon = 0;
//    nreal = 4;
//
//    min_realvar = (double *)malloc(nreal*sizeof(double));
//    max_realvar = (double *)malloc(nreal*sizeof(double));
//
//	for (i=0; i<nreal; i++)
//	{
//		min_realvar[i] = -5;
//		max_realvar[i] = 5;
//	}
//	pcross_real = 0.8;
//	pmut_real = 0.8;
//	eta_c = 10; //distribution_index for crossover
//	eta_m = 10; //distribution_index for mutation
//	nbin = 0;
//    if (nbin != 0)
//    {
//        nbits = (int *)malloc(nbin*sizeof(int));
//        min_binvar = (double *)malloc(nbin*sizeof(double));
//        max_binvar = (double *)malloc(nbin*sizeof(double));
//        for (i=0; i<nbin; i++)
//        {
//            nbits[i] = 1;
//            min_binvar[i] = 0;
//            max_binvar[i] = 1;
//        }
//        pcross_bin = 0.8;
//        pmut_bin = 0.1;
//    }
//    mate = 0; //printf("\n Enter the choice for selection restriction, 0 for normal selection, 1 for restricted selection : ");
//    delta = 0.5; //printf("\n Enter the value of delta (0.0 - 1.0) for loose domination : ");
//    var_option = 1; //space niching one of them must be 1
//    obj_option = 0; //objective niching
//    input_type = 0; //printf("\n Enter the choice for OmniOptOptimizer::population initialization, 0 for random, 1 for latin-hypercube based sampling, 2 for reading initial OmniOptOptimizer::population from a file : ");
//    frequency = 2; //printf("\n Enter the frequency with which the OmniOptOptimizer::population information (only the variables) is to be stored : ");
//    run_mode =1; //printf("\n Enter the simulation mode, 0 for Analysis mode, 1 for Turbo mode : ");
}
