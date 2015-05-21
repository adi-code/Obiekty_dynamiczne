/*
 * EMOEAOptimizer.cpp
 *
 *  Created on: 21 paź 2013
 *      Author: tomasz
 */

#include "OmniOptOptimizer.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#define GNUPLOT_COMMAND "gnuplot -persist"

#include "OmniOptOptimizer.h"
#include "priv_omniopt/rand.h"
#include <messages.h>

OmniOptOptimizer::OmniOptOptimizer() {

    this->setPopSize(40);
    this->setBinarySize(0);
    this->setFloatSize(4);
    this->setGenerations(625); // 40 000
    this->setSeed(0.5);

    //genetic parameters for fine tuning
    //problem independent

  setCross(0.9);

  setDi(1); //was 15  and 20
  setDiM(1);
	pcross_real = 0.9;
	pmut_real = 0.05;

	pcross_bin = 0.9;
	pmut_bin = 0.05;

	eta_c = 15; //distribution_index for crossover
	eta_m = 20; //distribution_index for mutation
	nbin = 0;

    mate = 0; //printf("\n Enter the choice for selection restriction, 0 for normal selection, 1 for restricted selection : ");
    delta = 0.001; //printf("\n Enter the value of delta (0.0 - 1.0) for loose domination : ");
    var_option = 0; //space niching one of them must be 1
    obj_option = 1; //objective niching
    input_type = 1; //printf("\n Enter the choice for OmniOptOptimizer::population initialization, 0 for random, 1 for latin-hypercube based sampling, 2 for reading initial OmniOptOptimizer::population from a file : ");
    frequency = 2; //printf("\n Enter the frequency with which the OmniOptOptimizer::population information (only the variables) is to be stored : ");
    run_mode =1; //printf("\n Enter the simulation mode, 0 for Analysis mode, 1 for Turbo mode : ");

}

OmniOptOptimizer::~OmniOptOptimizer() {
	// TODO Auto-generated destructor stub
}

void OmniOptOptimizer::alocateMemory() {
	popsize = this->getPopSize();
	ngen = this->getGenerations();
    nobj = this->getObjectiveFncCnt();
    ncon = this->getConstraintFncCnt();
    nreal = this->getFloatSize();
    seed = this->getSeed();
	nbin= this->getBinarySize();


   //	if (nreal) {setPMut(1/nreal);}
  //	if (nbin) {setPMut(1/nreal);}


    min_obj = (double *)malloc(nobj*sizeof(double));
    max_obj = (double *)malloc(nobj*sizeof(double));
    epsilon = (double *)malloc(nobj*sizeof(double));

    min_realvar = new double[nreal];
    max_realvar = new double[nreal];

	min_binvar = new double[nbin];
	max_binvar = new double[nbin ];

	for (unsigned int i=0; i <  nreal; i++) {
	  min_realvar[i] = this->getFLLimit(i);
	  max_realvar[i] = getFHLimit(i);
	}

	for (unsigned int i=0; i <  nbin; i++) {
	  min_binvar[i] = this->getBLLimit(i);
	  max_binvar[i] = getBHLimit(i);
	}
}
void OmniOptOptimizer::run() {
{

	alocateMemory();

    int i;
    FILE *fpt1;
    FILE *fpt2;
    FILE *fpt3;
    FILE *fpt4;
    FILE *fpt5;
	FILE *fpt6;
    FILE *fpt;
	FILE *fpt7;
    FILE *gp;
    char *s;
    OmniOptOptimizer::population *parent_pop;
    OmniOptOptimizer::population *child_pop;
	OmniOptOptimizer::population *mixed_pop;
	std::string befs = "\n Omni-optimizer code developed at KanGAL, IIT Kanpur. \n Copyright (C) 2006 by Dr. Kalyanmoy Deb and Santosh Tiwari\n";
	fireOnBeforeStartNotify(befs.c_str());

	fflush(stdout);

    if (seed<=0.0 || seed>=1.0)
    {
		fireOnErrorNotify("\n Entered seed value is wrong, seed value must be in (0,1) \n");
		exit(1);
    }
    input();
	pdefinit();
    if (run_mode==0)
    {
//		gp = popen(GNUPLOT_COMMAND,"w");
//		if (gp==NULL)
//		{
//			printf("\n Could not open a pipe to gnuplot, check the definition of GNUPLOT_COMMAND in file OmniOptOptimizer.h\n");
//			printf("\n Edit the string to suit your system configuration, recompile and rerun the program\n");
//			exit(1);
//        }
    }
	if (saveToFile()) {
		fpt1 = fopen("OmniOpt_initial_pop.txt","w");
		fpt2 = fopen("OmniOpt_final_pop.txt","w");
		fpt3 = fopen("OmniOpt_nondominated_pop.txt","w");
		fpt4 = fopen("OmniOpt_output.txt","w");
		fpt5 = fopen("OmniOpt_problem_data.txt","w");
		fpt6 = fopen("OmniOpt_plot.txt","w");
		fpt7 = fopen("OmniOpt_run.txt","a");
	}

    s = (char *)malloc(100*sizeof(double));

	if (saveToFile()) {output (fpt1, fpt2, fpt3, fpt4, fpt5);}

	parent_pop = (OmniOptOptimizer::population *)malloc(sizeof(OmniOptOptimizer::population));
    child_pop = (OmniOptOptimizer::population *)malloc(sizeof(OmniOptOptimizer::population));
    mixed_pop = (OmniOptOptimizer::population *)malloc(sizeof(OmniOptOptimizer::population));
    allocate_memory_pop (parent_pop, popsize);
    allocate_memory_pop (child_pop, popsize);
    allocate_memory_pop (mixed_pop, 2*popsize);
    my_randomize();
    initialize_pop (parent_pop);
    decode_pop(parent_pop);

   // printf("\n Initialization done, now performing first generation");
    fflush(stdout);
    evaluate_pop (parent_pop);
    define_epsilon (parent_pop, popsize, epsilon);
    assign_rank_and_crowding_distance (parent_pop);

	//printf("\n gen = 1");
	if (saveToFile()) {
		fflush(stdout);
		fflush(fpt1);
		fflush(fpt2);
		fflush(fpt3);
		fflush(fpt4);
		fflush(fpt5);
	}
    if (ngen==1)
    {
    }
    else
    {
        for (i=2; i<=ngen; i++)
        {
            if (mate==0)
            {
                selection (parent_pop, child_pop);
            }
            else
            {
                restricted_selection(parent_pop, child_pop);
            }
            mutation_pop (child_pop);
            //decode_pop(child_pop);
            evaluate_pop(child_pop);
            merge (parent_pop, child_pop, mixed_pop);
            define_epsilon (mixed_pop, 2*popsize, epsilon);
            fill_nondominated_sort (mixed_pop, parent_pop);


			//printf("\n gen = %d",i);
			fireOnGenerationDoneNotify(i);
        }
		if (saveToFile()) { report_obj(parent_pop,fpt6);}
	}
    fireOnAfterStopNotify("Omni Optimizer Finished");
	if (saveToFile()) {
		fprintf(fpt7,"%f\t%e\n",seed,parent_pop->ind[0].obj[0]);
		fflush(fpt7);
		fclose(fpt7);
		fflush(stdout);
		fflush(fpt1);
		fflush(fpt2);
		fflush(fpt3);
		fflush(fpt4);
		fflush(fpt5);
		fflush(fpt6);
		fclose(fpt1);
		fclose(fpt2);
		fclose(fpt3);
		fclose(fpt4);
		fclose(fpt5);
		fclose(fpt6);
    }
    free (epsilon);
    free (min_obj);
    free (max_obj);
    if (nreal!=0)
    {
        free (min_realvar);
        free (max_realvar);
    }
    if (nbin!=0)
    {
        free (min_binvar);
        free (max_binvar);
    }
	free(s);
//	if (run_mode==0)
//	{
//		if (gp!=NULL)
//		{
//			pclose(gp);
//		}
//	}
	deallocate_memory_pop (parent_pop, popsize);
	deallocate_memory_pop (child_pop, popsize);
	deallocate_memory_pop (mixed_pop, 2*popsize);
	free(parent_pop);
	free(child_pop);
	free(mixed_pop);
   // printf("\n Routine successfully exited \n");
}
}


CMultiObjectiveSolution * OmniOptOptimizer::createSolutionFromIndividual(OmniOptOptimizer::individual * ind) {
  CMultiObjectiveSolution * solution = new CMultiObjectiveSolution();
	for (int i = 0; i != nobj; i++) {
		solution->FObjFncVals.push_back(ind->obj[i]);
	}

	for (int i = 0; i != nreal; i++) {
		solution->FFloatVars.push_back(ind->xreal[i]);
	}

	for (int i = 0; i != nbin; i++) {
		solution->FBinaryVars.push_back(ind->xbin[i]);
	}

	for (int i = 0; i != ncon; i++) {
		solution->FObjFncVals.push_back(ind->constr[i]);
	}

	return solution;
}

void OmniOptOptimizer::setCross(double _v) {
    if (_v > 1.0) {
       fireOnErrorNotify(E_OPT_PARAM_CROSSOVER);
    } else if (_v < 0) {
       fireOnErrorNotify(E_OPT_PARAM_CROSSOVER);
    } else {
       pcross_real = _v;
    }
}
double OmniOptOptimizer::getCross() {
  return pcross_real;
}

void OmniOptOptimizer::setCrossBin(double _v) {
    if (_v > 1.0) {
        fireOnErrorNotify(E_OPT_PARAM_CROSSOVER);
    } else if (_v < 0) {

       fireOnErrorNotify(E_OPT_PARAM_CROSSOVER);
    } else {
       pcross_bin = _v;
    }
}
double OmniOptOptimizer::getCrossBin() {
  return pcross_bin;
}

void OmniOptOptimizer::setPMutBin(double _v) {
    if (_v > 1.0) {
       fireOnErrorNotify(E_OPT_PARAM_MUTB);
    } else if (_v < 0) {
       fireOnErrorNotify(E_OPT_PARAM_MUTB);
    } else {
       pmut_bin = _v;
    }
}
double OmniOptOptimizer::getPMutBin() {
  return pmut_bin;
}

void OmniOptOptimizer::setPMut(double _v) {
    if (_v > 1.0) {
       fireOnErrorNotify(E_OPT_PARAM_MUTF);
    } else if (_v < 0) {
       fireOnErrorNotify(E_OPT_PARAM_MUTF);
    } else {
       pmut_real = _v;
    }
}
double OmniOptOptimizer::getPMut() {
  return pmut_real;
}
void OmniOptOptimizer::setDi(double _v) {
    if (_v<0) {
     fireOnErrorNotify( E_OPT_PARAM_DI);
     return;
    }
    eta_c = _v;
}
double OmniOptOptimizer::getDi() {
  return eta_c;
}
void OmniOptOptimizer::setDiM(double _v) {
    if (_v<0) {
     fireOnErrorNotify( E_OPT_PARAM_DIM);
     return;
    }
    eta_m = _v;
}
double OmniOptOptimizer::getDiM() {
  return eta_m;
}
