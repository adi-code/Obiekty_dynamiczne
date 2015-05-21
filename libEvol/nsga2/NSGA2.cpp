//---------------------------------------------------------------------------




#include "NSGA2.h"

//---------------------------------------------------------------------------
#include "CNSGA2Optimizer.h"
#include <messages.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


namespace wutNSGA2 {

  int maxpop = 500;
  int maxchrom = 200;
  int maxvar = 20;
  int maxfun = 10;
  int maxcons = 20;
  void initVariables() {
    maxpop = CNSGA2Optimizer::gNSGA2Optimizer->getPopSize();
    maxchrom = CNSGA2Optimizer::gNSGA2Optimizer->getBinarySize();
    maxvar = CNSGA2Optimizer::gNSGA2Optimizer->getFloatSize() + CNSGA2Optimizer::gNSGA2Optimizer->getBinarySize();
    maxfun = CNSGA2Optimizer::gNSGA2Optimizer->getObjectiveFncCnt();
    maxcons = CNSGA2Optimizer::gNSGA2Optimizer->getConstraintFncCnt();
  }

  globpop::globpop() {
    rankar = new int*[2*maxpop];
    rankno = new int[2*maxpop];
    genes = new int*[2*maxpop];
    rank = new int[2*maxpop];
    flag = new int[2*maxpop];
    fitness = new float*[2*maxpop];
    cub_len = new float[2*maxpop];
    xreal = new float*[2*maxpop];
    xbin = new float*[2*maxpop];
    error = new float[2*maxpop];
    constr = new float*[2*maxpop];
    for (int i=0; i < 2*maxpop; i++) {
      rankar[i] = new int [2*maxpop];
      genes[i] = new int [maxchrom];
      fitness[i] = new float [maxfun];
      xreal[i] = new float[maxvar];
      xbin[i] = new float[maxvar];
      constr[i] = new float[maxcons];
    }

  }
  globpop::globpop(const globpop& src) {
    globpop();
    for (int i = 0; i < 2*maxpop; i++) {
      for (int j=0; j < 2*maxpop; j++) {
        rankar[i][j] = src.rankar[i][j];
      }
      for (int j=0; j < maxchrom; j++) {
        genes[i][j] = src.genes[i][j];
      }
      for (int j=0; j < maxfun; j++) {
        fitness[i][j] = src.fitness[i][j];
      }
      for (int j=0; j < maxvar; j++) {
        xreal[i][j] = src.xreal[i][j];
        xbin[i][j] = src.xbin[i][j];
      }
      for (int j=0; j < maxcons; j++) {
        constr[i][j] = src.constr[i][j];
      }
    }
  }
  globpop::~globpop() {

    for (int i=0; i < 2*maxpop; i++) {
      delete [] rankar[i];
      delete [] genes[i];
      delete [] fitness[i];
      delete [] xreal[i];
      delete [] xbin[i];
      delete [] constr[i];
    }

    delete [] rankar;
    delete [] rankno;
    delete [] genes;
    delete [] rank;
    delete [] flag;

    delete [] cub_len;
    delete [] xreal;
    delete [] xbin;
    delete [] error;
    delete [] constr;
  }

  individual::individual() {
    genes =  new int[maxchrom];
    xreal = new float[maxvar];
    xbin = new float[maxvar];
    fitness = new float[maxfun];
    constr = new float[maxcons];
  }
  individual::individual(const individual& src) {
    individual();
    for (int i = 0; i < maxchrom; i++) {
      genes[i] = src.genes[i];
    }
    for (int i = 0; i < maxvar; i++) {
      xreal[i] = src.xreal[i];
      xbin[i] = src.xbin[i];
    }
    for (int i = 0; i < maxfun; i++) {
      fitness[i] = src.fitness[i];
	}
    for (int i = 0; i < maxcons; i++) {
      constr[i] = src.constr[i];
    }
  }
  individual::~individual() {
    delete [] genes;
    delete [] xreal;
    delete [] xbin;
    delete [] fitness;
    delete [] constr;
  }

  population::population() {
    rankrat = new float[maxpop];
    rankno = new int[maxpop];
    ind = new individual[maxpop];
  }
  population::~population() {
    delete [] rankrat;
    delete [] rankno;
    delete [] ind;
  }
  population::population(const population& src) {
    population();
    for (int i = 0; i < maxpop; i++) {
      rankrat[i] = src.rankrat[i];
      rankno[i] = src.rankno[i];
      ind[i] = src.ind[i];
    }
  }
  NSGA2::NSGA2() {
    jrand=1;
    fpara1 = new float*[2*maxpop];
    for (int i = 0; i < 2*maxpop; i++) {
      fpara1[i] = new float[2];
    }
    vlen = new int[maxvar];
    lim_b = new float*[maxvar];
    lim_r = new float*[maxvar];
    for (int i = 0; i < maxvar; i++) {
      lim_b[i] = new float[2];
      lim_r[i] = new float[2];
    }
    coef = new double[maxvar];

    for (int i = 0; i < 55; i++) {
      oldrand[i] = double(rand())/RAND_MAX;
    }

  }

  NSGA2::~NSGA2() {
    for (int i = 0; i < 2*maxpop; i++) {
      delete [] fpara1[i];
    }
    for (int i = 0; i < maxvar; i++) {
      delete [] lim_b[i];
      delete [] lim_r[i];
    }
    delete [] fpara1;

    delete [] vlen;
    delete [] lim_b;
    delete [] lim_r;
    delete [] coef;
  }


  void NSGA2::crossover(population *new_pop_ptr,population *mate_pop_ptr)
  {
    int i,j,k,l,m,n,y,mating_site,*par1,*par2,*chld1,*chld2,c;
    float rnd;
    int r;
    rnd=randomperc();

    new_pop_ptr->ind_ptr=&(new_pop_ptr->ind[0]);

    mate_pop_ptr->ind_ptr=&(mate_pop_ptr->ind[0]);

    for (i = 0,y = 0,n = 0;i < popsize/2;i++)
    {
      new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[n]);
      chld1=&(new_pop_ptr->ind_ptr->genes[0]);
      n = n+1;

      new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[n]);
      chld2=&(new_pop_ptr->ind_ptr->genes[0]);
      n = n+1;

      mate_pop_ptr->ind_ptr = &(mate_pop_ptr->ind[y]);
      par1 = &(mate_pop_ptr->ind_ptr->genes[0]);
      y = y+1;

      mate_pop_ptr->ind_ptr = &(mate_pop_ptr->ind[y]);
      par2 = &(mate_pop_ptr->ind_ptr->genes[0]);
      y = y+1;

      rnd = randomperc();
      if (rnd < pcross)
      {
        ncross++;
        rnd = randomperc();
        c = floor(rnd*(chrom+10));
        mating_site = c;
        if(mating_site >= chrom)
        {
          mating_site = mating_site/2;
        }

        for(k = 0;k < chrom;k++)
        {
          if(k > mating_site-1)
          {
            *chld1++ = *par2++;
            *chld2++ = *par1++;
          }
          else
          {
            *chld1++ = *par1++;
            *chld2++ = *par2++;
          }
        }
      }
      else
      {
        for (k = 0;k < chrom;k++)
        {
          *chld1++ = *par1++;
          *chld2++ = *par2++;
        }
      }
    }
    return;
  }
  void NSGA2::decode(population *pop_ptr)
  {
    float *real_ptr;
    int i,sum,b,k,c,d,*gene_ptr,m,x;

    pop_ptr->ind_ptr = &(pop_ptr->ind[0]);

    for(i = 0; i < popsize; i++)
    {
      real_ptr = &(pop_ptr->ind_ptr->xbin[0]);
      gene_ptr = &(pop_ptr->ind_ptr->genes[0]);
      for(m = 0; m < nchrom; m++)
      {
        /*finding out the co-efficient 2 to the power of
      (l-1) where l is the no of bits assigned to this variable

      For More Info Study DEB's Book*/
        sum = 0;
        for(k = 0; k < vlen[m]; k++)
        {
          b = *gene_ptr;
		//  d = vlen[m] - k - 1;
		 // c = pow(2,d);
		  sum =/*sum + c **/ b;
          gene_ptr++;
        }

        x = vlen[m];
		//coef[m] = pow(2,x) - 1;
		*real_ptr =sum;//lim_b[m][0] + (sum/coef[m])*(lim_b[m][1]-lim_b[m][0]);
        real_ptr++;
      }
      pop_ptr->ind_ptr = &(pop_ptr->ind[i+1]);
    }
    return ;
  }
  void NSGA2::func(population *pop_ptr)
  {
    /*File ptr to the file to store the value of the g for last iteration
  g is the parameter required for a particular problem
  Every problem is not required*/

    float *realx_ptr, /*Pointer to the array of x values*/
    *binx_ptr,      /* Pointer to the binary variables */
    *fitn_ptr,      /*Pointer to the array of fitness function*/


    *err_ptr;      /*Pointer to the error */


    float* x = new float[2*maxvar];
    float* cstr = new float[maxcons];
    float* f = new float[maxfun];     /*array of fitness values*/
    int i,j,k;
    float error, cc;

    pop_ptr->ind_ptr= &(pop_ptr->ind[0]);

    /*Initializing the max rank to zero*/
    pop_ptr->maxrank = 0;
    for(i = 0;i < popsize;i++)
    {
      pop_ptr->ind_ptr = &(pop_ptr->ind[i]);
      realx_ptr = &(pop_ptr->ind_ptr->xreal[0]);
      binx_ptr = &(pop_ptr->ind_ptr->xbin[0]);

      for(j = 0; j < nvar; j++)
      { // Real-coded variables
        x[j] = *realx_ptr++;
      }

      for(j = 0; j < nchrom; j++)
      { // Binary-codced variables
        x[nvar+j] = *binx_ptr++;
      }

      fitn_ptr = &(pop_ptr->ind_ptr->fitness[0]);
      err_ptr = &(pop_ptr->ind_ptr->error);



      /*   DO NOT CHANGE ANYTHING ABOVE   */
      /*----------------------CODE YOUR OBJECTIVE FUNCTIONS HERE------------*/
      /*All functions must be of minimization type, negate maximization
            functions */
      /*============Start Coding Your Function From This Point=============*/
      // First fitness function
      double* xf = new double[nvar+nchrom];
      for (int j = 0; j < nvar; j++) {
        xf[j] = pop_ptr->ind_ptr->xreal[j];
      }
      for (int j = nvar; j < nvar+nchrom; j++) {
        xf[j] = pop_ptr->ind_ptr->xbin[j-nvar];
      }
      for (unsigned int j=0; j < CNSGA2Optimizer::gNSGA2Optimizer->getObjectiveFncCnt(); j++) {
        f[j] = CNSGA2Optimizer::gNSGA2Optimizer->calculateObjectiveFnc(xf, nvar+nchrom, j);
      }
      //  f[0] = (1-x[0])*(1-x[0]) + 100*(x[1]-x[0]*x[0])*(x[1]-x[0]*x[0]);
      // Second Fitness Function
      //	  f[1] = (x[0]*x[0]+x[1]-11)*(x[0]*x[0]+x[1]-11)+ (x[0]+x[1]*x[1]-7)*(x[0]+x[1]*x[1]-7);
      /*=========End Your Coding Upto This Point===============*/

      /******************************************************************/
      /*              Put The Constraints Here                          */
      /******************************************************************/
      // g(x) >= 0 type (normalize g(x) as in the cstr[1] below)
      /*===========Start Coding Here=============*/
      for (unsigned int j=0; j < CNSGA2Optimizer::gNSGA2Optimizer->getConstraintFncCnt(); j++) {
        cstr[j] = -CNSGA2Optimizer::gNSGA2Optimizer->calculateConstraintFnc(xf, nvar+nchrom, j);
      }
      //cstr[0] = 1;//x[0]*x[0]+x[1]*x[1]-1.0-0.1*cos(16.0*(x[0]/x[1]));
      // cstr[1] = 1;//(-square(x[0]-0.5) - square(x[1]-0.5) + 0.5)/0.5;

      delete [] xf;
      /*===========Constraints Are Coded Upto Here=============*/
      /*   DO NOT CHANGE ANYTHING BELOW  */



      for(k = 0 ; k < nfunc ;k++)
      {
        *fitn_ptr++  = f[k];
      }

      for (k = 0;k < ncons; k++)
      {
        pop_ptr->ind_ptr->constr[k] = cstr[k];
      }
      error = 0.0;
      for (k = 0;k < ncons;k++)
      {
        cc = cstr[k];
        if(cc < 0.0)
          error = error - cc;
      }
      *err_ptr = error;
    }

    /*---------------------------* RANKING *------------------------------*/

    if(ncons == 0)
      ranking(pop_ptr);
    else
      rankcon(pop_ptr);



    delete [] x;
    delete [] cstr;
    delete [] f;
    return;
  }
  void NSGA2::init(population *pop_ptr)
  {
	int i,j,r, d2;
	float d, d1;
	pop_ptr->ind_ptr = &(pop_ptr->ind[0]);

	/*Loop Over the population size*/
	for (i = 0 ; i < popsize ; i++)
	{

	  /*Loop over the chromosome length*/
	  for (j = 0;j < chrom;j++)
	  {
		/*Generate a Random No. if it is less than 0.5 it
	  generates a 0 in the string otherwise 1*/
		d = randomperc();
		d1 = 2*d - 1;

		double ivalue = d*(lim_b[j][1] - lim_b[j][0])+lim_b[j][0];
		 if (ivalue < lim_b[j][0]) {
			ivalue = lim_b[j][0];
		 } else if (ivalue > lim_b[j][1]) {
			ivalue =lim_b[j][1];
		 }
		int l = floor(ivalue);
		int h = ceil(ivalue);
		if (l!=h) {
		  if (randomperc() < 0.5) {
			  ivalue = l;
		  } else {
			  ivalue = h;
		  }
		} else {
			ivalue = l;
		}
		pop_ptr->ind_ptr->genes[j]  =ivalue;

//		if(d >= 0.5)
//		{
//		  pop_ptr->ind_ptr->genes[j] = 1;
//		}
//		else
//		{
//		  pop_ptr->ind_ptr->genes[j] = 0;
//        }
	  }
      pop_ptr->ind_ptr = &(pop_ptr->ind[i+1]);
	}
    pop_ptr->ind_ptr = &(pop_ptr->ind[0]);
    return;
  }
  void NSGA2::input(FILE *rep_ptr)
  {
    int i;

    float cc;

    //  printf("------------------------------------------------------------\n\n\n");
    //
    //  printf("This is a Multi-Objective GA program to solve the constraint problem\n\n");
    //
    //  printf("-----------------------------------------------------------\n\n");
    //
    //  printf("Give problem specification\n\n");
    //
    //  printf("-------------------------------------------------------------\n");
    //
    //  printf("-------------------------------------------------------------\n");

    /*Asks for number of the variables*/

    // printf("Give no. of real and binary-coded variables\n\n");
    // scanf("%d %d",&nvar,&nchrom);
    nvar=CNSGA2Optimizer::gNSGA2Optimizer->getFloatSize();
    nchrom=CNSGA2Optimizer::gNSGA2Optimizer->getBinarySize();
    /*Checks if the number of variables are more than specified in the
  program if it fails here goto tunsga1.c file and increase the
  maxvar to the number equal to your requirement*/

    if ((nvar > maxvar) || (nchrom > maxvar))
    {
      printf(" Increase No. of the variables in the program. Currently set %d.\n",maxvar);
      exit(1);
    }

    /*Asks for number of the functions*/

    // printf("Give no. of objective functions\n\n");
    //scanf("%d",&nfunc);
    nfunc=CNSGA2Optimizer::gNSGA2Optimizer->getObjectiveFncCnt();
    /*Checks if the number of functions are more than specified in the
   program if it fails here goto tunsga1.c file and increase the
   maxfun to the number equal to your requirement*/

    if(nfunc > maxfun)
    {
      printf("Increase the number of functions in the program. Curently set %d.\n",maxfun);
      exit(1);
    }

    // printf("Give no. of constraints\n\n");
    // scanf("%d",&ncons);
    ncons=CNSGA2Optimizer::gNSGA2Optimizer->getConstraintFncCnt();
    /*Checks if the number of constraints are more than specified in the
  program if it fails here goto tunsga1.c file and increase the
    maxcons to the number equal to your requirement*/

    if(ncons > maxcons)
    {
      printf("Increase the number of constraints in the program. Currently set %d.\n",maxcons);
      exit(1);
    }

    //printf("------------------------------------------------------------\n");
    //  printf("Give GA parameters\n");
    // printf("------------------------------------------------------------\n");

    fflush(stdin);
    /*Asks for number of the individuals in the population*/
    /*Default value = 30 to 100*/

    //printf("Give Population size (an even no.) \n");
    //scanf("%d",&popsize);
    popsize=CNSGA2Optimizer::gNSGA2Optimizer->getPopSize();
    /*Checks if the number of individuals in the population  are more
   than specified in the program if it fails here goto tunsga1.c file and
   increase the maxpop to the number equal to your requirement*/


    if(popsize > maxpop)
    {
      printf("Increase Population size. Currently set %d\n",maxpop);
      exit(1);
    }

    /*No. of generations for which the GA will let the population evolve
    Default value is 100
    Too large value will take very long time and very small value will
    not let the GA reach the global Pareto front to the problem*/

    // printf("Give the no.of generations \n");
    //scanf("%d",&gener);
    gener=CNSGA2Optimizer::gNSGA2Optimizer->getGenerations();
    /*No. of generations for which the GA will let the population evolve
    Default value is 100
    Too large value will take very long time and very small value will
  not let the GA reach the global Pareto front to the problem*/

    // printf("Give the cross-over probability (between 0.5 and 1)\n");
    //scanf("%f",&pcross);
    pcross=CNSGA2Optimizer::gNSGA2Optimizer->getCross();
    if (nvar > 0)
    {
      cc = 1.0/nvar;
      //printf("Give the mutation probability for real-coded vectors (between 0 and %f)\n",cc);
      //scanf("%f",&pmut_r);
      pmut_r = CNSGA2Optimizer::gNSGA2Optimizer->getFloatPMut();
      /*Asks for Distribution Index for Cross-over (Default = 20)*/
      /*For Mutation (Default = 10 to 500)*/

      // printf("Give Distribution Index for real-coded crossover between 0.5 to 100\n");
      // scanf("%f",&di);
      di = CNSGA2Optimizer::gNSGA2Optimizer->getDi();
      //printf("Give Distribution Index for real-coded mutation between 0.5 to 500\n");
      //scanf("%f",&dim);
      dim = CNSGA2Optimizer::gNSGA2Optimizer->getDiM();
      for(i = 0; i < nvar; i++)
      {
        /*Specify the limits of the variables*/

        //printf("Give Lower & Upper limits of the %dth real-coded variable\n",i+1);
        //scanf("%f %f",&lim_r[i][0],&lim_r[i][1]);
        lim_r[i][0]=CNSGA2Optimizer::gNSGA2Optimizer->getFLLimit(i);
        lim_r[i][1]=CNSGA2Optimizer::gNSGA2Optimizer->getFHLimit(i);

      }

      //printf("If limits on real-coded variable are rigid (1 if yes)\n");
      //scanf("%d",&ans);
      ans=CNSGA2Optimizer::gNSGA2Optimizer->getRigidConstraints();
    }

    chrom = 0;
    if (nchrom > 0)
    {
      /*Asking for Crossover Type*/
      //  printf("Give Crossover type 1 for Simple one & 2 for Uniform X-over\n");
      //  scanf("%d",&optype);
      optype=1;
      /*Specify the Cross-over Probability for the Simple or Real Coded
  Cross-over suited Range is 0.9 to 0.99*/

	  for (i = 0; i < nchrom; i++)
      {
        /*For Binary Coded Problem
      Specify the no of bits for each variable
    Total Sum of the bit value must be equal to
      chromosome length*/
        //printf("Give the no.of bits assigned to the %d variable\n",i+1);
        //scanf("%d",&vlen[i]);
		vlen[i] = 1;//gNSGA2Optimizer->getBitsNumber(i);
        chrom += vlen[i];

        //printf("Give lower & the upper limits of the %d variable\n",i+1);
        //scanf("%f %f",&lim_b[i][0],&lim_b[i][1]);
        lim_b[i][0] = CNSGA2Optimizer::gNSGA2Optimizer->getBLLimit(i);
        lim_b[i][1] = CNSGA2Optimizer::gNSGA2Optimizer->getBHLimit(i);
      }
      if(chrom > maxchrom)
      {
        printf("Increase chromosome size. Currently set %d.\n",maxchrom);
        exit(1);
      }

      /* Ask for mutation probablity */
      cc = 1.0/chrom;

      //  printf("Give the mutation probability for binary strings (between 0 and %f)\n",cc);
      //  scanf("%f",&pmut_b);
      pmut_b = CNSGA2Optimizer::gNSGA2Optimizer->getBinaryPMut();
    }

    // printf("----------------------------------------------------------------\n");

    /*Give the initial seed*/
    //printf("\nGive random seed(between 0 and 1)\n");
    //scanf("%f",&seed);
    seed=0.5;
    // end of reading parameters


    /*Print the GA parameters and problem parameters
    in the file output.dat*/

    if (CNSGA2Optimizer::gNSGA2Optimizer->saveToFile()) {
      fprintf(rep_ptr,"GA PARAMETERS\n");
      fprintf(rep_ptr,"-------------------------------------------------------\n");

      fprintf(rep_ptr,"Population Size ->%d\n",popsize);

      fprintf(rep_ptr,"Chromosome Length ->%d\n",chrom);

      fprintf(rep_ptr,"No. of generations ->%d\n",gener);

      fprintf(rep_ptr,"No. of Functions ->%d\n",nfunc);

      fprintf(rep_ptr,"No. of Constraints ->%d\n",ncons);

      if (nchrom > 0)
        fprintf(rep_ptr,"No. of binary-coded variables ->%d\n",nchrom);
      if (nvar > 0)
        fprintf(rep_ptr,"No. of real-coded variables ->%d\n",nvar);

      fprintf(rep_ptr,"Selection Strategy is Tournament Selection\n");

      for(i = 0;i < nchrom;i++)
      {

        fprintf(rep_ptr,"Binary-coded variable No.-> %d\n",i);

        fprintf(rep_ptr,"No. of bits assigned to it ->%d\n",vlen[i]);

        fprintf(rep_ptr,"Lower limits on %dth variable-> %f\n",i,lim_b[i][0]);

        fprintf(rep_ptr,"Upper limits on %dth variable ->%f\n",i,lim_b[i][1]);
      }

      for(i = 0; i < nvar; i++)
      {

        fprintf(rep_ptr,"Real-coded variable No.-> %d\n",i);

        fprintf(rep_ptr,"Lower limits on %dth variable-> %f\n",i,lim_r[i][0]);

        fprintf(rep_ptr,"Upper limits on %dth variable ->%f\n",i,lim_r[i][1]);
        if (ans == 1)
          fprintf(rep_ptr,"Variable bounds are rigid\n");
        else
          fprintf(rep_ptr,"Variable bounds are not rigid\n");
      }

      if (nchrom > 0)
      {
        if(optype == 1)
          fprintf(rep_ptr,"X-over on binary string is SINGLE POINT X-OVER\n");

        if (optype ==2)
          fprintf(rep_ptr,"X-over on binary strings is UNIFORM X-OVER \n");
      }
      if (nvar > 0)
        fprintf(rep_ptr,"Crossover parameter in the SBX operator is %f\n",di);

      fprintf(rep_ptr,"Cross-over Probability ->%f\n",pcross);

      if (nchrom > 0)
        fprintf(rep_ptr,"Mutation Probability for binary strings -> %f\n",pmut_b);

      if (nvar > 0)
        fprintf(rep_ptr,"Mutation Probability for real-coded vectors -> %f\n",pmut_r);

      fprintf(rep_ptr,"Random Seed ->%f\n",seed);
    }
    return;
  }
  void NSGA2::keepalive(population *pop1_ptr,population *pop2_ptr,population *pop3_ptr,int gen)
  {
    int i,j,jj,k,m,a1,l,rec;

    int *front_pop = new int[maxpop];

    int sum,st,str,pool,poolf,sel,r1;

    int *gene1_ptr, *gene2_ptr,leftsum,x;

    float rnd,a,*gene3_ptr,x3,*gene4_ptr,*xbin1_ptr,*xbin2_ptr;

    /*Forming the global mating pool*/

    for(i = 0;i < popsize;i++)
    {
      if(nchrom > 0)
      {
        /*Binary Coded GA genes are copied*/
        for(k = 0;k < chrom;k++)
        {
          globalpop.genes[i][k]=pop1_ptr->ind[i].genes[k];
          globalpop.genes[i+popsize][k] = pop2_ptr->ind[i].genes[k];
        }
        for (k=0; k < nchrom; k++)
        {
          globalpop.xbin[i][k] = pop1_ptr->ind[i].xbin[k];
          globalpop.xbin[i+popsize][k] = pop2_ptr->ind[i].xbin[k];
        }
      }
      if (nvar > 0)
      {
        /*For Real Coded GA x values are copied */
        for(k = 0;k < nvar;k++)
        {
          globalpop.xreal[i][k] = pop1_ptr->ind[i].xreal[k];
          globalpop.xreal[i+popsize][k] = pop2_ptr->ind[i].xreal[k];
        }
      }

      /*Fitness is copied to the global pool */
      for(l = 0;l < nfunc;l++)
      {
        globalpop.fitness[i][l] = pop1_ptr->ind[i].fitness[l];
        globalpop.fitness[i+popsize][l] = pop2_ptr->ind[i].fitness[l];
      }

      /*Initial;ising the dummyfitness to zero */
      globalpop.cub_len[i] = 0;
      globalpop.cub_len[i+popsize] = 0;
      globalpop.error[i] = pop1_ptr->ind[i].error;
      globalpop.error[i+popsize] = pop2_ptr->ind[i].error;
      for (jj=0; jj<ncons; jj++)
      {
        globalpop.constr[i][jj] = pop1_ptr->ind[i].constr[jj];
        globalpop.constr[i+popsize][jj] = pop2_ptr->ind[i].constr[jj];
      }
    }


    global_pop_ptr = &(globalpop);

    /*Finding the global ranks */
    if(ncons == 0)
      grank(gen);
    else
      grankc(gen);

    m = globalpop.maxrank;

    /* Sharing the fitness to get the dummy fitness */
    for(i = 0;i < m;i++)
    {
      gshare(i+1);
    }

    poolf = popsize;
    pool = 0;


    /*Initializing the flags of population to zero */
    for(i = 0;i < 2*popsize;i++)
    {
      globalpop.flag[i] = 0;
    }
    // decide which all solutions belong to the pop3
    rec = 0;
    st = 0;
    for(i = 0 ;i < m ; i++)
    {
      /*    Elitism Applied Here     */
      st = pool;
      pool += globalpop.rankno[i];

      if(pool <= popsize)
      {
        for(k = 0;k < 2*popsize ;k++)
        {
          if(globalpop.rank[k] == i+1)
            globalpop.flag[k] = 1;
        }
        pop3_ptr->rankno[i] = globalpop.rankno[i];
      }
      else
      {
        sel = popsize - st;
        Lastrank = i+1;
        pop3_ptr->rankno[i] = sel;
        gsort(i+1,sel);
        break;
      }
    }

    k = 0;
    for(i = 0,k = 0;i < 2*popsize && k < popsize; i++)
    {
      if(nchrom > 0)
      {
        if(globalpop.flag[i] == 1)
        {
		  gene1_ptr = &(globalpop.genes[i][0]);
          xbin1_ptr = &(globalpop.xbin[i][0]);
          pop3_ptr->ind_ptr = &(pop3_ptr->ind[k]);
          gene2_ptr = &(pop3_ptr->ind_ptr->genes[0]);
          xbin2_ptr = &(pop3_ptr->ind_ptr->xbin[0]);

          for(j = 0 ; j < chrom; j++)
          {
            *gene2_ptr++ = *gene1_ptr++;
          }
          for (j=0; j < nchrom; j++)
            *xbin2_ptr++ = *xbin1_ptr++;
        }
      }
      if (nvar > 0)
      {
        if(globalpop.flag[i] == 1)
        {
          gene3_ptr = &(globalpop.xreal[i][0]);
          pop3_ptr->ind_ptr = &(pop3_ptr->ind[k]);
          gene4_ptr = &(pop3_ptr->ind_ptr->xreal[0]);

          for(j = 0 ;j < nvar;j++)
          {
            *gene4_ptr++ = *gene3_ptr++;
          }
        }
      }
      if(globalpop.flag[i] == 1)
      {
        for(j = 0;j < nfunc;j++)
          pop3_ptr->ind[k].fitness[j] = globalpop.fitness[i][j];
        pop3_ptr->ind[k].cub_len = globalpop.cub_len[i];
        if(ncons != 0)
          pop3_ptr->ind[k].error = globalpop.error[i];
        for (jj=0; jj<ncons; jj++)
          pop3_ptr->ind[k].constr[jj] = globalpop.constr[i][jj];
        pop3_ptr->ind[k].rank = globalpop.rank[i];
        k++;  // increment the pop3 counter
      }
    }

    pop3_ptr->maxrank = Lastrank;
    delete [] front_pop;
    return;
  }

  void NSGA2::grank(int gen)
  {
    int i,j,k,rnk,val,nondom,popsize1,q;

    int *gflg =  new int[2*maxpop];

    float *ptr1,*ptr2;
    FILE *gr;

    if (CNSGA2Optimizer::gNSGA2Optimizer->saveToFile()) {
      gr = fopen("g_rank_record.out","a");
      fprintf(gr,"Genration no. = %d\n",gen);
    }


    /*----------------------------* RANKING *---------------------------------*/
    rnk = 0;
    nondom = 0;
    popsize1 = 2*popsize;

    for(i = 0;i < popsize1;i++)
    {
      gflg[i] = 2;
    }

    for(k = 0;k < popsize1;k++)
    {
      q =  0;
      for(j = 0;j < popsize1;j++)
      {
        if (gflg[j] != 1) break;
      }
      if(j == popsize1) break;
      rnk = rnk +1;
      for( j = 0 ;j < popsize1; j++)
      {
        if(gflg[j] == 0) gflg[j] = 2;
      }
      for(i = 0;i < popsize1 ; i++)
      {
        if(gflg[i] != 1 && gflg[i] != 0)
        {
          ptr1 = &(global_pop_ptr->fitness[i][0]);
          for(j = 0;j < popsize1 ; j++)
          {
            if( i!= j)
            {
              if(gflg[j] != 1)
              {
                ptr2 = &(global_pop_ptr->fitness[j][0]);
                val = indcmp1(ptr1,ptr2);
                if( val == 2)
                {
                  gflg[i] = 0;/* individual 1 is dominated */
                  break;
                }
                if(val == 1)
                {
                  gflg[j] = 0;/* individual 2 is dominated */
                }
                if(val == 3)
                {
                  nondom++;/* individual 1 & 2 are non dominated */
                  if(gflg[j] != 0)gflg[j] = 3;
                }
              }
            }
          }
          if( j == popsize1)
          {
            global_pop_ptr->rank[i] = rnk;
            gflg[i] = 1;
            global_pop_ptr->rankar[rnk-1][q] =  i;
            q++;
          }
        }
      }
      global_pop_ptr->rankno[rnk-1] = q;
    }
    global_pop_ptr->maxrank = rnk;
    if (CNSGA2Optimizer::gNSGA2Optimizer->saveToFile()) {
      fprintf(gr,"   RANK     No Of Individuals\n");
      for(i = 0;i < rnk;i++) {
        fprintf(gr,"\t%d\t%d\n",i+1,globalpop.rankno[i]);
      }
      fclose(gr);
    }
    delete [] gflg;
    return;
  }

  void NSGA2::grankc(int gen)
  {
    int i,j,k,rnk,val,nondom,popsize1,q;
    int *gflg =  new int[2*maxpop];
    float *ptr1,*ptr2;
    float *err_ptr1,*err_ptr2;
    FILE *gr;
    if (CNSGA2Optimizer::gNSGA2Optimizer->saveToFile()) {
      gr = fopen("g_rank_record.out","a");
      fprintf(gr,"Genration no. = %d\n",gen);
    }
    /*----------------------------* RANKING *---------------------------------*/
    rnk = 0;
    nondom = 0;
    popsize1 = 2*popsize;
    min_fit = popsize1;
    delta_fit = 0.1 *popsize1;
    for(i=0;i<popsize1;i++)
    {
      gflg[i] = 2;
    }
    for(k = 0;k < popsize1;k++)
    {
      q =  0;
      for(j = 0;j < popsize1;j++)
      {
        if (gflg[j] != 1) break;
      }
      if(j == popsize1) break;
      rnk = rnk +1;
      for( j = 0 ;j < popsize1; j++)
      {
        if(gflg[j] == 0) gflg[j] = 2;
      }
      for(i = 0;i< popsize1 ; i++)
      {
        if(gflg[i] != 1 && gflg[i] != 0)
        {
          ptr1 = &(global_pop_ptr->fitness[i][0]);
          err_ptr1 = &(global_pop_ptr->error[i]);
          for(j = 0;j < popsize1 ; j++)
          {
            if( i!= j)
            {
              if(gflg[j] != 1)
              {
                ptr2 = &(global_pop_ptr->fitness[j][0]);
                err_ptr2 = &(global_pop_ptr->error[j]);

                if(*err_ptr1 < 1.0e-6 && *err_ptr2 > 1.0e-6)
                {/* first feasible second individaul is infeasible*/
                  gflg[j] = 0;
                }
                else
                {
                  if(*err_ptr1 >1.0e-6 && *err_ptr2 < 1.0e-6)
                  {/*first individual is infeasible and second is feasible*/
                    gflg[i] = 0;
                    break;
                  }
                  else
                  {/*both feasible or both infeasible*/
                    if(*err_ptr1 > *err_ptr2)
                    {
                      gflg[i] = 0;
                      /*first individual is more infeasible*/
                      break;
                    }
                    else
                    {
                      if(*err_ptr1 < *err_ptr2)
                        gflg[j] = 0;
                      /*second individual is more infeasible*/

                      else
                      {
                        val = indcmp1(ptr1,ptr2);
                        if( val == 2)
                        {
                          gflg[i] = 0;
                          /* individual 1 is dominated */
                          break;
                        }
                        if(val == 1)
                        {
                          gflg[j] = 0;
                          /* individual 2 is dominated */
                        }
                        if(val == 3)
                        {
                          nondom++;/* individual 1 & 2 are non dominated */
                          if(gflg[j] != 0) gflg[j] = 3;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
          if( j == popsize1)
          {
            global_pop_ptr->rank[i] = rnk;
            gflg[i] = 1;
            global_pop_ptr->rankar[rnk-1][q] =  i;
            q++;
          }
        }
      }
      global_pop_ptr->rankno[rnk-1] = q;
    }
    global_pop_ptr->maxrank = rnk;
    if (CNSGA2Optimizer::gNSGA2Optimizer->saveToFile()) {
      fprintf(gr,"   RANK     No Of Individuals\n");
      for(i = 0;i < rnk;i++)
      {
        fprintf(gr,"\t%d\t%d\n",i+1,globalpop.rankno[i]);
      }
      fclose(gr);
    }

    delete [] gflg;
    return;
  }

  int NSGA2::indcmp1(float *ptr1,float *ptr2)
  {
    float* fit1 = new float[maxfun];
    float* fit2 = new float[maxfun];

    int i,value,m,n;
    for(i = 0;i < nfunc ;i++)
    {
      fit1[i] = *ptr1++;
      fit2[i] = *ptr2++;
    }
    m = 0;n=0;
    while(m < nfunc && fit1[m] <= fit2[m])
    {
      if((fit2[m] - fit1[m]) < 1e-7) n++;
      m++;
    }
    if(m == nfunc)
    {
      if(n == nfunc) value = 3;
      else value = 1;                    /*value = 1 for dominating*/
    }
    else
    {
      m = 0;n = 0;
      while(m < nfunc && fit1[m] >= fit2[m])
      {
        if((fit1[m] - fit2[m]) < 1e-7) n++;
        m++;
      }
      if(m == nfunc)
      {
        if(n != nfunc)
          value = 2;                       /*value =  2 for dominated */
        else value = 3;
      }
      else value = 3;                   /*value = 3 for incomparable*/
    }
    delete [] fit1;
    delete [] fit2;
    return value;
  }

  void NSGA2::gsort(int rnk,int sel)
  {
    int i,j,a,q;
    float temp,temp1;

    float**array = new float*[2*maxpop];
    for (int i=0; i < 2*maxpop; i++) {
      array[i] = new float[2];
    }

    q = globalpop.rankno[rnk-1];

    for(i = 0 ;i < q ;i++)
    {
      array[i][0] = globalpop.rankar[rnk-1][i];
      a = globalpop.rankar[rnk-1][i];
      array[i][1] = globalpop.cub_len[a];
    }
    for(i = 0;i < q ;i++)
    {
      for(j = i+1;j < q;j++)
      {
        if(array[i][1] < array[j][1])
        {
          temp = array[i][1];
          temp1 = array[i][0];
          array[i][1] = array[j][1];
          array[i][0] = array[j][0];

          array[j][1] = temp;
          array[j][0] = temp1;
        }
      }
    }

    for(i = 0;i < sel;i++)
    {
      a = array[i][0];
      globalpop.flag[a] = 1;
    }

    for (int i=0; i < 2*maxpop; i++) {
      delete [] array[i];
    }
    delete [] array;
    return;
  }

  void NSGA2::gshare(int rnk)
  {
    float max;

    float**length =  new float*[2*maxpop];
    for (int i=0; i < 2*maxpop; i++) {
      length[i] = new float[2];
    }

    int i,j,m1,a ;
    float min, Diff;  // Added 18.08.2003

    m1 = globalpop.rankno[rnk-1];

    for(j = 0;j < nfunc;j++)
    {
      for(i = 0;i < m1;i++)
      {
        fpara1[i][0] = 0;
        fpara1[i][1] = 0;
      }

      for(i = 0;i < m1;i++)
      {
        a = globalpop.rankar[rnk-1][i];
        fpara1[i][0] = (float)a ;
        fpara1[i][1] = globalpop.fitness[a][j];
      }

      sort(m1); /*Sort the arrays in ascending order of the fitness*/

      max = fpara1[m1-1][1];
      min = fpara1[0][1];  // Added 18.08.2003
      Diff = max-min;      // Added 18.08.2003 and 5 subsequent lines
      if (Diff < 0.0)
      {
        printf("Something wrong in keepaliven.h\n");
        exit(1);
      }
      for(i = 0;i < m1;i++)
      {
        if(i == 0 ||i == (m1-1))
        {
          length[i][0] = fpara1[i][0];
          length[i][1] = 100*max;
        }
        else
        {
		  length[i][0] = fpara1[i][0];
		  length[i][1] = fabs(fpara1[i+1][1]- fpara1[i-1][1]);
		  if (Diff!=0) length[i][1]/=Diff; // crowding distances are normalized 18.08.2003
        }
      }
      for(i = 0;i < m1;i++)
      {
        a = length[i][0];
        globalpop.cub_len[a] += length[i][1];
      }
    }
    for (int i=0; i < 2*maxpop; i++) {
      delete [] length[i];
    }
    delete [] length;
    return;
  }

  void NSGA2::sort(int m1)
  {
    float temp,temp1;
    int i1,j1,k1;
    for(k1 = 0;k1 < m1-1;k1++)
    {
      for(i1 = k1+1;i1 < m1;i1++)
      {
        if(fpara1[k1][1] > fpara1[i1][1])
        {
          temp = fpara1[k1][1];
          temp1 = fpara1[k1][0];
          fpara1[k1][1] = fpara1[i1][1];
          fpara1[k1][0] = fpara1[i1][0];
          fpara1[i1][1] = temp;
          fpara1[i1][0] = temp1;
        }
      }
    }
    return;
  }

  void NSGA2::mutate(population *new_pop_ptr)
  {
//    int i,*ptr,j,r;
//    float rand1,*rand_float_ptr;
//
//    rand1=randomperc();
//    new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[0]);
//
//    for(j = 0;j < popsize;j++)
//    {
//      ptr= &(new_pop_ptr->ind_ptr->genes[0]);
//      new_pop_ptr->ind_ptr =&(new_pop_ptr->ind[j+1]);
//
//      /*Select bit */
//      for (i = 0;i < chrom;i++)
//      {
//        rand1 = randomperc();
//
//        /*Check whether to do mutation or not*/
//        if(rand1 <= pmut_b)
//        {
//          if(*ptr == 0)
//            *ptr =1;
//		  else
//            *ptr=0;
//		  nmut++;
//        }
//        ptr++;
//      }
//    }
    int i,j,r;
	float rnd,delta,indi,*ptr,*ptr1,deltaq;
	float y,yl,yu,val,xy;

    for(j = 0;j < popsize;j++)
	{
	  for (i = 0;i < nchrom; i++)
	  {
        rnd = randomperc();

        /*For each variable find whether to do mutation or not*/
		if(rnd <= pmut_b)
        {
		  double orig = new_pop_ptr->ind[j].genes[i];
		  y = new_pop_ptr->ind[j].genes[i];
		  yl = lim_b[i][0];
		  yu = lim_b[i][1];

		  if(y > yl)
		  {
			/*Calculate delta*/

			if((y-yl) < (yu-y))
              delta = (y - yl)/(yu - yl);
			else
              delta = (yu - y)/(yu-yl);

			rnd = randomperc();

            indi = 1.0/(dim +1.0);

            if(rnd <= 0.5)
			{
              xy = 1.0-delta;
			  val = 2*rnd+(1-2*rnd)*(pow(xy,(dim+1)));
			  deltaq =  pow(val,indi) - 1.0;
			}
			else
			{
			  xy = 1.0-delta;
			  val = 2.0*(1.0-rnd)+2.0*(rnd-0.5)*(pow(xy,(dim+1)));
			  deltaq = 1.0 - (pow(val,indi));
			}

			/*Change the value for the parent */
			//  *ptr  = *ptr + deltaq*(yu-yl);
			// Added by Deb (31/10/01)
			y = y + deltaq * (yu-yl);
			if (y < yl) y=yl;
			if (y > yu) y=yu;
			new_pop_ptr->ind[j].genes[i] = y;
		  }
		  else // y == yl
		  {
			xy = randomperc();
			new_pop_ptr->ind[j].genes[i] = xy*(yu - yl) + yl;
		  }
		 if (new_pop_ptr->ind[j].genes[i] < lim_b[i][0]) {
			new_pop_ptr->ind[j].genes[i] = lim_b[i][0];
		 } else if (new_pop_ptr->ind[j].genes[i] > lim_b[i][1]) {
			new_pop_ptr->ind[j].genes[i] =lim_b[i][1];
		 }

		 int l = floor(new_pop_ptr->ind[j].genes[i]);
		 int h = ceil(new_pop_ptr->ind[j].genes[i]);
		 if (l!=h) {
			if (randomperc() < 0.5) {
			  new_pop_ptr->ind[j].genes[i] = l;
			} else {
			  new_pop_ptr->ind[j].genes[i] = h;
			}
		 } else {
			new_pop_ptr->ind[j].genes[i] = l;
		}
		if (orig ==new_pop_ptr->ind[j].genes[i]) {
				if (orig==lim_b[i][1]) {
					new_pop_ptr->ind[j].genes[i] = lim_b[i][1]-1;
				} else if (orig == lim_b[i][0]) {
					new_pop_ptr->ind[j].genes[i] = lim_b[i][0]+1;
				} else {
                    if (randomperc() < 0.5) {
						new_pop_ptr->ind[j].genes[i] = new_pop_ptr->ind[j].genes[i] - 1;
					} else {
						new_pop_ptr->ind[j].genes[i] = new_pop_ptr->ind[j].genes[i] +1;
					}
				}
			}
		  nmut++;
		}
		//  ptr++;
	  }
	}


    return;
  }

  void NSGA2::run()
  {

    /*Some Local variables to this Problem (Counters And some other pointers*/

    int i,j,l,f,maxrank1;
    float *ptr,tot;
    FILE
    *rep_ptr,
    *gen_ptr,
    *rep2_ptr,
    *end_ptr,
    *g_var,
    *lastit;
    /*File Pointers*/

    if (CNSGA2Optimizer::gNSGA2Optimizer->saveToFile()) {
	  rep_ptr = fopen("NSGA2_output.txt","w");
	  gen_ptr =fopen("NSGA2_all_fitness.txt","w");
	  rep2_ptr = fopen("NSGA2_ranks.txt","w");
	  end_ptr = fopen("NSGA2_final_fitness.txt","w");
	  g_var = fopen("NSGA2_final_var.txt","w");
	  lastit = fopen("NSGA2_plot.txt","w");
    }
    /*Opening the files*/

    old_pop_ptr = &(oldpop);

    nmut = 0;
    ncross = 0;

    /*Get the input from the file input.h*/
    input(rep_ptr);

    if (CNSGA2Optimizer::gNSGA2Optimizer->saveToFile()) {
      fprintf(rep_ptr,"Results in a file\n");
      fprintf(end_ptr,"# Last generation population (Feasible and non-dominated)\n");
      fprintf(end_ptr,"# Fitness_vector (first %d)  Constraint_violation (next %d)  Overall_penalty\n",nfunc,ncons);
      fprintf(g_var,"#Feasible Variable_vectors for non-dominated solutions at last generation\n");
      fprintf(g_var,"# Real (first %d)  Binary (next %d)\n",nvar,nchrom);
      fprintf(lastit,"# Feasible and Non-dominated Objective Vector\n");
    }
    /*Initialize the random no generator*/
    warmup_random(seed);

    /*Binary Initializaton*/
    if (nchrom > 0)
      init(old_pop_ptr);
    if (nvar > 0)
      realinit(old_pop_ptr);

    old_pop_ptr = &(oldpop);

    // decode binary strings
    decode(old_pop_ptr);

    old_pop_ptr = &(oldpop);
    new_pop_ptr = &(newpop);

    for(j = 0;j < popsize;j++)
    {
      /*Initializing the Rank array having different individuals
  at a particular  rank to zero*/
      old_pop_ptr->rankno[j] = 0;
      new_pop_ptr->rankno[j] = 0;
    }

    old_pop_ptr = &(oldpop);

    func(old_pop_ptr);
    /*Function Calculaiton*/
    if (CNSGA2Optimizer::gNSGA2Optimizer->saveToFile()) {
      fprintf(rep_ptr,"----------------------------------------------------\n");
      fprintf(rep_ptr,"Statistics at Generation 0 ->\n");
      fprintf(rep_ptr,"--------------------------------------------------\n");
    }


    /********************************************************************/
    /*----------------------GENERATION STARTS HERE----------------------*/

    for (i = 0;i < gener;i++)
    {
        CNSGA2Optimizer::gNSGA2Optimizer->fireOnGenerationDoneNotify(i);
      if (CNSGA2Optimizer::gNSGA2Optimizer->isTerminated()) {break;}
      //printf("Generation = %d\n",i+1);
      old_pop_ptr = &(oldpop);
      mate_pop_ptr = &(matepop);
      if (CNSGA2Optimizer::gNSGA2Optimizer->saveToFile()) {
        fprintf(rep_ptr,"Population at generation no. -->%d\n",i+1);
        fprintf(gen_ptr,"#Generation No. -->%d\n",i+1);
        fprintf(gen_ptr,"#Variable_vector  Fitness_vector Constraint_violation Overall_penalty\n");
      }
      /*--------SELECT----------------*/
      nselect(old_pop_ptr ,mate_pop_ptr );

      new_pop_ptr = &(newpop);
      mate_pop_ptr = &(matepop);

      /*CROSSOVER----------------------------*/
      if (nchrom > 0)
	  {

        if(optype == 1)
        {
          crossover(new_pop_ptr ,mate_pop_ptr );
          /*Binary Cross-over*/
        }

        if(optype == 2)
        {
          unicross(new_pop_ptr ,mate_pop_ptr );
          /*Binary Uniform Cross-over*/
        }
      }
      if (nvar > 0)
        realcross(new_pop_ptr ,mate_pop_ptr );
      /*Real Cross-over*/


      /*------MUTATION-------------------*/
      new_pop_ptr = &(newpop);

      if (nchrom > 0)
        mutate(new_pop_ptr );
      /*Binary Mutation */

      if (nvar > 0)
        real_mutate(new_pop_ptr );
      /*Real Mutation*/

      new_pop_ptr = &(newpop);

      /*-------DECODING----------*/
      if(nchrom > 0)
        decode(new_pop_ptr );
      /*Decoding for binary strings*/

      /*----------FUNCTION EVALUATION-----------*/
      new_pop_ptr = &(newpop);
      func(new_pop_ptr );

      /*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
      old_pop_ptr = &(oldpop);
      new_pop_ptr = &(newpop);
      mate_pop_ptr = &(matepop);

      /*Elitism And Sharing Implemented*/
      keepalive(old_pop_ptr ,new_pop_ptr ,mate_pop_ptr,i+1);

      mate_pop_ptr = &(matepop);
      if(nchrom > 0)
        decode(mate_pop_ptr );

      mate_pop_ptr = &(matepop);
      /*------------------REPORT PRINTING--------------------------------*/
      if (CNSGA2Optimizer::gNSGA2Optimizer->saveToFile()) {
        report(i ,old_pop_ptr ,mate_pop_ptr ,rep_ptr ,gen_ptr, lastit );
      }
      /*==================================================================*/

      /*----------------Rank Ratio Calculation------------------------*/
      new_pop_ptr = &(matepop);
      old_pop_ptr = &(oldpop);

      /*Finding the greater maxrank among the two populations*/

      if(old_pop_ptr->maxrank > new_pop_ptr->maxrank)
        maxrank1 = old_pop_ptr->maxrank;
      else
        maxrank1 = new_pop_ptr->maxrank;

      if (CNSGA2Optimizer::gNSGA2Optimizer->saveToFile()) {
        fprintf(rep2_ptr,"--------RANK AT GENERATION %d--------------\n",i+1);
        fprintf(rep2_ptr,"Rank old ranks   new ranks     rankratio\n");

        for(j = 0;j < maxrank1 ; j++)
        {
          /*Sum of the no of individuals at any rank in old population
      and the new populaion*/

          int oprr=old_pop_ptr->rankno[j];
          int nprr =new_pop_ptr->rankno[j];

          tot = (old_pop_ptr->rankno[j])+ (new_pop_ptr->rankno[j]);
          if (tot==0) {
            new_pop_ptr->rankrat[j] = 0.0;
          } else {
            new_pop_ptr->rankrat[j] = (new_pop_ptr->rankno[j])/tot;
          }
          /*Finding the rank ratio for new population at this rank*/




          /*Printing this rank ratio to a file called ranks.dat*/

          fprintf(rep2_ptr," %d\t  %d\t\t %d\t %f\n",j+1,old_pop_ptr->rankno[j],new_pop_ptr->rankno[j],new_pop_ptr->rankrat[j]);

        }

        fprintf(rep2_ptr,"-----------------Rank Ratio-------------------\n");
      }

      /*==================================================================*/

      /*=======Copying the new population to old population======*/

      old_pop_ptr = &(oldpop);
      new_pop_ptr = &(matepop);

      for(j = 0;j < popsize;j++)
      {
        old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
        new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
        if(nchrom > 0)
        {
          /*For Binary GA copying of the chromosome*/

          for(l = 0;l < chrom;l++)
            old_pop_ptr->ind_ptr->genes[l]=new_pop_ptr->ind_ptr->genes[l];

          for(l = 0;l < nchrom;l++)
            old_pop_ptr->ind_ptr->xbin[l] = new_pop_ptr->ind_ptr->xbin[l];
        }
        if(nvar > 0)
        {
          /*For Real Coded GA copying of the chromosomes*/
          for(l = 0;l < nvar;l++)
            old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
        }

        /*Copying the fitness vector */
        for(l = 0 ; l < nfunc ;l++)
          old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

        /*Copying the dummy fitness*/
        old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

        /*Copying the rank of the individuals*/
        old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

        /*Copying the error and constraints of the individual*/

        old_pop_ptr->ind_ptr->error = new_pop_ptr->ind_ptr->error;
        for(l = 0;l < ncons;l++)
        {
          old_pop_ptr->ind_ptr->constr[l] = new_pop_ptr->ind_ptr->constr[l];
        }

        /*Copying the flag of the individuals*/
        old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
      }   // end of j

      maxrank1 = new_pop_ptr->maxrank ;

      /*Copying the array having the record of the individual
  at different ranks */
      for(l = 0;l < popsize;l++)
      {
        old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
      }

      /*Copying the maxrank */
      old_pop_ptr->maxrank = new_pop_ptr->maxrank;

      /*Printing the fitness record for last generation in a file last*/

    }  // end of i
    // for the last generation
    old_pop_ptr = &(matepop);
    CMultiObjectiveSolutionSet* solset =  new CMultiObjectiveSolutionSet();
    for(f = 0;f < popsize ; f++) // for printing
    {

      old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

      if ((old_pop_ptr->ind_ptr->error <= 0.0) && (old_pop_ptr->ind_ptr->rank == 1))  // for all feasible solutions and non-dominated solutions
      {
        CMultiObjectiveSolution * sol = new CMultiObjectiveSolution();
        sol->FObjFncVals.resize(nfunc);
        for(l = 0;l < nfunc;l++){
          sol->FObjFncVals[l] = old_pop_ptr->ind_ptr->fitness[l];
          if (CNSGA2Optimizer::gNSGA2Optimizer->saveToFile()) fprintf(end_ptr,"%f\t",old_pop_ptr->ind_ptr->fitness[l]);
        }
        sol->FConstraintVals.resize(ncons);
        for(l = 0;l < ncons;l++)
        {
          sol->FConstraintVals[l] = old_pop_ptr->ind_ptr->constr[l];
          if (CNSGA2Optimizer::gNSGA2Optimizer->saveToFile()) fprintf(end_ptr,"%f\t",old_pop_ptr->ind_ptr->constr[l]);
        }
        if (ncons > 0) {
          if (CNSGA2Optimizer::gNSGA2Optimizer->saveToFile()) fprintf(end_ptr,"%f\t",old_pop_ptr->ind_ptr->error);
        }

        if (CNSGA2Optimizer::gNSGA2Optimizer->saveToFile()) fprintf(end_ptr,"\n");


        if (nvar > 0)
        {
          sol->FFloatVars.resize(nvar);
          for(l = 0;l < nvar ;l++) {
            sol->FFloatVars[l] = old_pop_ptr->ind_ptr->xreal[l];
            if (CNSGA2Optimizer::gNSGA2Optimizer->saveToFile()) fprintf(g_var,"%f\t",old_pop_ptr->ind_ptr->xreal[l]);
          }
          if (CNSGA2Optimizer::gNSGA2Optimizer->saveToFile()) fprintf(g_var,"  ");
        }

        if(nchrom > 0)
        {
          sol->FBinaryVars.resize(nchrom);
          for(l = 0;l < nchrom;l++) {
            sol->FBinaryVars[l] = old_pop_ptr->ind_ptr->xbin[l];
            if (CNSGA2Optimizer::gNSGA2Optimizer->saveToFile()) fprintf(g_var,"%f\t",old_pop_ptr->ind_ptr->xbin[l]);
          }
        }
        if (CNSGA2Optimizer::gNSGA2Optimizer->saveToFile()) fprintf(g_var,"\n");
        solset->solutions.push_back(sol);
      }  // feasibility check

    } // end of f (printing)
    CNSGA2Optimizer::gNSGA2Optimizer->setSolution(solset);
    if (solset->solutions.size()==0) {
      CNSGA2Optimizer::gNSGA2Optimizer->fireOnAfterStopNotify(M_OPT_NO_FSOL);
    }
    // for the last generation
    /*                   Generation Loop Ends                                */
    /************************************************************************/


    if (CNSGA2Optimizer::gNSGA2Optimizer->saveToFile()) {
      fprintf(rep_ptr,"NO. OF CROSSOVER = %d\n",ncross);
      fprintf(rep_ptr,"NO. OF MUTATION = %d\n",nmut);
      fprintf(rep_ptr,"------------------------------------------------------------\n");
      fprintf(rep_ptr,"---------------------------------Thanks---------------------\n");
      fprintf(rep_ptr,"-------------------------------------------------------------\n");
      //printf("NOW YOU CAN LOOK IN THE FILE OUTPUT2.DAT\n");
    }
    /*Closing the files*/
    if (CNSGA2Optimizer::gNSGA2Optimizer->saveToFile()) {
      fclose(rep_ptr);
      fclose(gen_ptr);
      fclose(rep2_ptr);
      fclose(end_ptr);
      fclose(g_var);
      fclose(lastit);
    }
  }

  void NSGA2::rankcon(population *pop_ptr)
  {
    int i,j,k,       /*counters*/
    rnk,           /*rank*/
    val,           /*value obtained after comparing two individuals*/
    nondom,        /*no of non dominated members*/
    maxrank1,      /*Max rank of the population*/
    q;

    float* rankarr=new float[maxpop]; /*Array storing the individual number at a rank*/
    float *ptr1,*ptr2,*err_ptr1,*err_ptr2;

    /*------------------------------* RANKING *------------------------------*/

    /*Initializing the ranks to zero*/
    rnk = 0 ;

    nondom = 0 ;
    maxrank1 = 0;
    /*min_fit is initialize to start distributing the dummy fitness =
    popsize to the rank one individuals and keeping the record such
    that the minimum fitness of the better rank individual is always
    greater than max fitness of the relatively worse rank*/

    min_fit = popsize;


    /*Difference in the fitness of minimum dummy fitness of better rank
    and max fitness of the next ranked individuals*/

    delta_fit = 0.1 * popsize;

    /*Initializing all the flags to 2*/

    for( j = 0 ;j < popsize; j++)
    {
      pop_ptr->ind[j].flag = 2;
    }

    q = 0;

    for(k =  0;k < popsize;k++,q=0)
    {
      for(j = 0;j < popsize;j++)
      {
        if (pop_ptr->ind[j].flag != 1)break;
        /*Break if all the individuals are assigned a rank*/
      }
      if(j == popsize)break;

      rnk = rnk + 1;

      for( j = 0 ;j < popsize; j++)
      {
        if(pop_ptr->ind[j].flag == 0) pop_ptr->ind[j].flag = 2;
        /*Set the flag of dominated individuals to 2*/
      }

      for(i = 0;i < popsize ; i++)
      {
        /*Select an individual which rank to be assigned*/

        pop_ptr->ind_ptr = &(pop_ptr->ind[i]);

        if(pop_ptr->ind_ptr->flag != 1 && pop_ptr->ind_ptr->flag != 0)
        {
          ptr1 = &(pop_ptr->ind_ptr->fitness[0]);
          err_ptr1 = &(pop_ptr->ind_ptr->error);

          for(j = 0;j < popsize ; j++)
          {

            /*Select the other individual which has not got a rank*/
            if( i!= j)
            {
              if(pop_ptr->ind[j].flag != 1)
              {
                pop_ptr->ind_ptr = &(pop_ptr->ind[j]);
                ptr2 = &(pop_ptr->ind_ptr->fitness[0]);
                err_ptr2 = &(pop_ptr->ind_ptr->error);

                if(*err_ptr1 < 1.0e-6 && *err_ptr2 > 1.0e-6)
                {
                  /*first ind is feasible second individaul
        is infeasible*/
                  pop_ptr->ind[j].flag = 0;
                }
                else
                {
                  if(*err_ptr1 > 1.0e-6 && *err_ptr2 < 1.0e-6)
                  {
                    /*first individual is infeasible and
            second is feasible*/
                    pop_ptr->ind[i].flag = 0;
                    break;
                  }
                  else
                  {
                    /*both are feasible or both are infeasible*/
                    if(*err_ptr1 > *err_ptr2)
                    {
                      pop_ptr->ind[i].flag = 0;
                      /*first individual is more infeasible*/
                      break;
                    }
                    else
                    {
                      if(*err_ptr1 < *err_ptr2)
                      {
                        pop_ptr->ind[j].flag = 0;
                        /*second individual is more
              infeasible*/
                      }
                      else
                      {
                        /*Compare the two individuals for
              fitness*/
                        val = indcmp3(ptr1,ptr2);

                        /*VAL = 2 for dominated individual
              which rank to be given*/

                        /*VAL = 1 for dominating individual
              which rank to be given*/

                        /*VAL = 3 for non comparable
              individuals*/

                        if( val == 2)
                        {
                          pop_ptr->ind[i].flag = 0;
                          /* individual 1 is dominated */
                          break;
                        }

                        if(val == 1)
                        {
                          pop_ptr->ind[j].flag = 0;
                          /* individual 2 is dominated */
                        }

                        if(val == 3)
                        {
                          nondom++;
                          /* individual 1 & 2 are
             non dominated */
                          if(pop_ptr->ind[j].flag != 0)
                            pop_ptr->ind[j].flag = 3;
                        }

                      }   /*if loop ends*/
                    }       /* i != j loop ends*/
                  }
                }
              }
            }
          }        /*loop over j ends*/
          if( j == popsize)
          {
            /*Assign the rank and set the flag*/
            pop_ptr->ind[i].rank = rnk;
            pop_ptr->ind[i].flag = 1;
            rankarr[q] = i;
            q++;
          }
        }       /*Loop over flag check ends*/
      }           /*Loop over i ends */
      pop_ptr->rankno[rnk-1] = q ;
    }
    maxrank1 = rnk;


    /*     Find Max Rank of the population    */
    for(i = 0;i < popsize;i++)
    {
      rnk = pop_ptr->ind[i].rank;

      if(rnk > maxrank1)maxrank1 = rnk;

    }

    pop_ptr->maxrank = maxrank1;
    delete [] rankarr;
    return;
  }

  /*Routine Comparing the two individuals*/

  int NSGA2::indcmp3(float *ptr1,float *ptr2)
  {
    float* fit1 = new float[maxfun];
    float* fit2 = new float[maxfun];
    int i,value,m,n;
    for(i = 0;i < nfunc ;i++)
    {
      fit1[i] = *ptr1++;
      fit2[i] = *ptr2++;
    }
    m = 0;
    n = 0;
    while(m < nfunc && fit1[m] <= fit2[m])
    {
      if(fit1[m]== fit2[m]) n++;
      m++;
    }
    if(m == nfunc)
    {
      if(n == nfunc) value = 3;
      else value = 1;             /*value = 1 for dominationg*/
    }
    else
    {
      m = 0;
      n = 0;
      while(m < nfunc && fit1[m] >= fit2[m])
      {
        if(fit1[m]== fit2[m]) n++;
        m++;
      }
      if(m == nfunc)
      {
        if(n != nfunc)
          value = 2;                       /*value =  2 for dominated */
        else value =3;
      }
      else value = 3;                   /*value = 3 for incomparable*/
    }
    delete [] fit1;
    delete [] fit2;
    return value;
  }

  void NSGA2::advance_random()
  /* Create next batch of 55 random numbers */
  {
    int j1;
    double new_random;

    for(j1 = 0; j1 < 24; j1++)
    {
      new_random = oldrand[j1] - oldrand[j1+31];
      if(new_random < 0.0) new_random = new_random + 1.0;
      oldrand[j1] = new_random;
    }
    for(j1 = 24; j1 < 55; j1++)
    {
      new_random = oldrand [j1] - oldrand [j1-24];
      if(new_random < 0.0) new_random = new_random + 1.0;
      oldrand[j1] = new_random;
    }
  }
  int  NSGA2::flip(float prob)
  {

    if(randomperc() <= prob)
      return(1);
    else
      return(0);
  }
  void NSGA2::initrandomnormaldeviate()
  /* initialization routine for randomnormaldeviate */
  {
    rndcalcflag = 1;
  }
  double NSGA2::noise(double mu , double sigma)
  /* normal noise with specified mean & std dev: mu & sigma */
  {

    return((randomnormaldeviate()*sigma) + mu);
  }

  double NSGA2::randomnormaldeviate()
  /* random normal deviate after ACM algorithm 267 / Box-Muller Method */
  {
    double sqrt(), log(), sin(), cos();
    double t, rndx1;

    if(rndcalcflag)
    {
      rndx1 = ::sqrt(- 2.0* ::log((double) randomperc()));
      t = 6.2831853072 * (double) randomperc();
      rndx2 = ::sin(t);
      rndcalcflag = 0;
      return(rndx1 * ::sin(t));
    }
    else
    {
      rndcalcflag = 1;
      return(rndx2);
    }
  }
  double NSGA2::randomperc()
  /* Fetch a single random number between 0.0 and 1.0 - Subtractive Method */
  /* See Knuth, D. (1969), v. 2 for details */
  /* name changed from random() to avoid library conflicts on some machines*/
  {
    jrand++;
    if(jrand >= 55)
    {
      jrand = 1;
      advance_random();
    }
    return(oldrand[jrand]);
  }
  int NSGA2::rnd(int low, int high)
  /* Pick a random integer between low and high */
  {
    int i;

    if(low >= high)
      i = low;
    else
    {
      i = (randomperc() * (high - low + 1)) + low;
      if(i > high) i = high;
    }
    return(i);
  }
  float NSGA2::rndreal(float lo , float hi)
  /* real random number between specified limits */
  {
    return((randomperc() * (hi - lo)) + lo);
  }
  void NSGA2::warmup_random(float random_seed)
  /* Get random off and running */
  {
    int j1, ii;
    double new_random, prev_random;

    oldrand[54] = random_seed;
    new_random = 0.000000001;
    prev_random = random_seed;
    for(j1 = 1 ; j1 <= 54; j1++)
    {
      ii = (21*j1)%54;
      oldrand[ii] = new_random;
      new_random = prev_random-new_random;
      if(new_random<0.0) new_random = new_random + 1.0;
      prev_random = oldrand[ii];
    }

    advance_random();
    advance_random();
    advance_random();

    jrand = 0;
  }

  void NSGA2::realcross(population *new_pop_ptr,population *mate_pop_ptr)
  {
    int i,j,y,n,r;
    float rnd,par1,par2,chld1,chld2,betaq,beta,alpha;
    float y1,y2,yu,yl,expp;

    y=0; n=0;
    for(i = 0; i < popsize/2; i++)
    {
      rnd = randomperc();

      /*Check Whether the cross-over to be performed*/
      if(rnd <= pcross)
      {

        /*Loop over no of variables*/
        for(j = 0;j < nvar;j++)
        {
          /*Selected Two Parents*/
          par1 = mate_pop_ptr->ind[y].xreal[j];
          par2 = mate_pop_ptr->ind[y+1].xreal[j];

          yl = lim_r[j][0];
          yu = lim_r[j][1];

          rnd = randomperc();

          /* Check whether variable is selected or not*/
          if(rnd <= 0.5)
          {
            /*Variable selected*/
            ncross++;

            if(fabs(par1 - par2) > 0.000001) // changed by Deb (31/10/01)
            {
              if(par2 > par1)
              {
                y2 = par2;
                y1 = par1;
              }
              else
              {
                y2 = par1;
                y1 = par2;
              }

              /*Find beta value*/
              if((y1 - yl) > (yu - y2))
              {
                beta = 1 + (2*(yu - y2)/(y2 - y1));
                //printf("beta = %f\n",beta);
              }
              else
              {
                beta = 1 + (2*(y1-yl)/(y2-y1));
                //printf("beta = %f\n",beta);
              }

              /*Find alpha*/
              expp = di + 1.0;

              beta = 1.0/beta;

              alpha = 2.0 - pow(beta,expp);

              if (alpha < 0.0)
              {
                printf("ERRRROR %f %d %d %f %f\n",alpha,y,n,par1,par2);
                exit(-1);
              }

              rnd = randomperc();

              if (rnd <= 1.0/alpha)
              {
                alpha = alpha*rnd;
                expp = 1.0/(di+1.0);
                betaq = pow(alpha,expp);
              }
              else
              {
                alpha = alpha*rnd;
                alpha = 1.0/(2.0-alpha);
                expp = 1.0/(di+1.0);
                if (alpha < 0.0)
                {
                  printf("ERRRORRR \n");
                  exit(-1);
                }
                betaq = pow(alpha,expp);
              }

              /*Generating two children*/
              chld1 = 0.5*((y1+y2) - betaq*(y2-y1));
              chld2 = 0.5*((y1+y2) + betaq*(y2-y1));

            }
            else
            {

              betaq = 1.0;
              y1 = par1; y2 = par2;

              /*Generation two children*/
              chld1 = 0.5*((y1+y2) - betaq*(y2-y1));
              chld2 =  0.5*((y1+y2) + betaq*(y2-y1));

            }
            // added by deb (31/10/01)
            if (chld1 < yl) chld1 = yl;
            if (chld1 > yu) chld1 = yu;
            if (chld2 < yl) chld2 = yl;
            if (chld2 > yu) chld2 = yu;
          }
          else
          {

            /*Copying the children to parents*/
            chld1 = par1;
            chld2 = par2;
          }
          new_pop_ptr->ind[n].xreal[j] = chld1;
          new_pop_ptr->ind[n+1].xreal[j] = chld2;
        }
      }
      else
      {
        for(j = 0;j < nvar;j++)
        {
          par1 = mate_pop_ptr->ind[y].xreal[j];
          par2 = mate_pop_ptr->ind[y+1].xreal[j];
          chld1 = par1;
          chld2 = par2;
          new_pop_ptr->ind[n].xreal[j] = chld1;
          new_pop_ptr->ind[n+1].xreal[j] = chld2;
        }
      }
      n = n+2; y=y+2;
    }
    return;
  }

  void NSGA2::realinit(population *pop_ptr)
  {
    int i,j,r,d2;
    double d,d1;

    for (i = 0 ; i < popsize ; i++)
    {
      for (j = 0; j < nvar; j++)
      {
        d = randomperc();
        d1 = 2*d - 1;
        /*if limits are not specified then generates any number between
      zero and infinity*/
        if(ans != 1)
        {
          pop_ptr->ind[i].xreal[j] = 1/d1 ;
        }

        /*if limits are specified it generates the value in
      range of minimum and maximum value of the variable*/
        else
        {
          pop_ptr->ind[i].xreal[j] = d*(lim_r[j][1] - lim_r[j][0])+lim_r[j][0];
        }
      }
      /* pop_ptr->ind_ptr = &(pop_ptr->ind[i+1]); */
    }
    /*pop_ptr->ind_ptr = &(pop_ptr->ind[0]); */
    return;
  }

  void NSGA2::real_mutate(population *new_pop_ptr)
  {
	int i,j,r;
	float rnd,delta,indi,*ptr,*ptr1,deltaq;
	float y,yl,yu,val,xy;

    for(j = 0;j < popsize;j++)
	{
      for (i = 0;i < nvar; i++)
	  {
        rnd = randomperc();

        /*For each variable find whether to do mutation or not*/
		if(rnd <= pmut_r)
        {
		  y = new_pop_ptr->ind[j].xreal[i];
          yl = lim_r[i][0];
		  yu = lim_r[i][1];

		  if(y > yl)
		  {
			/*Calculate delta*/

			if((y-yl) < (yu-y))
              delta = (y - yl)/(yu - yl);
			else
              delta = (yu - y)/(yu-yl);

            rnd = randomperc();

            indi = 1.0/(dim +1.0);

            if(rnd <= 0.5)
			{
              xy = 1.0-delta;
			  val = 2*rnd+(1-2*rnd)*(pow(xy,(dim+1)));
			  deltaq =  pow(val,indi) - 1.0;
			}
			else
			{
			  xy = 1.0-delta;
			  val = 2.0*(1.0-rnd)+2.0*(rnd-0.5)*(pow(xy,(dim+1)));
			  deltaq = 1.0 - (pow(val,indi));
			}

			/*Change the value for the parent */
			//  *ptr  = *ptr + deltaq*(yu-yl);
			// Added by Deb (31/10/01)
			y = y + deltaq * (yu-yl);
			if (y < yl) y=yl;
			if (y > yu) y=yu;
			new_pop_ptr->ind[j].xreal[i] = y;
		  }
		  else // y == yl
		  {
			xy = randomperc();
			new_pop_ptr->ind[j].xreal[i] = xy*(yu - yl) + yl;
		  }
		  nmut++;
		}
		//  ptr++;
	  }
	}
	return ;
  }

  void  NSGA2::report(int t,population *pop1_ptr,population *pop2_ptr,FILE *rep_ptr,FILE *gen_ptr, FILE *lastit )
  {
    int i,j,*be,*rptr,*rptr1;

    float *ptr1,*ptr,*fptr,*fptr1, *ptr1_b, *ptr2_b;

    float *ptr2,*cons_ptr1,*cons_ptr2, *err2;

    int *gene_ptr1 ,*gene_ptr2 ;

    fprintf(rep_ptr,"\n\n---------------------------------------------------\n");
    fprintf(rep_ptr,"Generation No.     ->%d\n",t+1);
    fprintf(rep_ptr,"------------------------------------------------------\n");
    if(ncons == 0)
      fprintf(rep_ptr," variables (real %d binary %d)  fitness (%d)  rank cublen || variables  fitness rank cublen\n",nvar,nchrom,nfunc);
    else
      fprintf(rep_ptr," variables (real %d binary %d)  fitness (%d) constraint (%d) penalty rank cublen || variables  fitness constraint penalty rank cublen\n",nvar,nchrom,nfunc,ncons);

    pop1_ptr->ind_ptr = &(pop1_ptr->ind[0]);

    pop2_ptr->ind_ptr = &(pop2_ptr->ind[0]); // Deb 31/10/01


    for(i = 0;i < popsize;i++)
    {
      fprintf(rep_ptr,"\n------------------------------------------------\n");

      ptr1_b = &(pop1_ptr->ind_ptr->xbin[0]);
      ptr2_b = &(pop2_ptr->ind_ptr->xbin[0]);

      ptr1 = &(pop1_ptr->ind_ptr->xreal[0]);
      ptr2 = &(pop2_ptr->ind_ptr->xreal[0]);  // Deb 31/10/01

      fptr = &(pop1_ptr->ind_ptr->fitness[0]);
      fptr1 = &(pop2_ptr->ind_ptr->fitness[0]);

      rptr = &(pop1_ptr->ind_ptr->rank);
      rptr1 = &(pop2_ptr->ind_ptr->rank);

      cons_ptr1 = &(pop1_ptr->ind_ptr->constr[0]);
      cons_ptr2 = &(pop2_ptr->ind_ptr->constr[0]);

      err2 = &(pop2_ptr->ind_ptr->error);

      for(j = 0;j < nvar;j++)
        fprintf(rep_ptr,"%f ",*ptr1++);

      for(j = 0;j < nchrom; j++)
        fprintf(rep_ptr,"%f ",*ptr1_b++);
      if (t == gener-1)
      {
        for(j = 0;j < nfunc;j++)
        {
          if ((*err2 <= 0.0) && (*rptr1 == 1))
            fprintf(lastit,"%f\t",*fptr1++);
          else *fptr1++;
        }
        if ((*err2 <= 0.0) && (*rptr1 == 1))
          fprintf(lastit,"\n");
      }
      fptr =  &(pop1_ptr->ind_ptr->fitness[0]);
      fptr1 = &(pop2_ptr->ind_ptr->fitness[0]);

      for(j = 0;j < nfunc;j++)
        fprintf(rep_ptr,"  %.4f",*fptr++);

      if(ncons != 0)
      {
        for(j = 0;j < ncons;j++)
          fprintf(rep_ptr,"  %.2e",*cons_ptr1++);
        fprintf(rep_ptr," %.2e",pop1_ptr->ind_ptr->error);
      }

      fprintf(rep_ptr," %d ",*rptr);

      fprintf(rep_ptr,"%f ",pop1_ptr->ind_ptr->cub_len);
      fprintf(rep_ptr,"|**|");

      for(j = 0;j < nvar;j++)
      {
        fprintf(rep_ptr," %f ",*ptr2);
        fprintf(gen_ptr," %f ",*ptr2++);
      }
      for(j = 0;j < nchrom; j++)
      {
        fprintf(rep_ptr,"%f ",*ptr2_b);
        fprintf(gen_ptr,"%f ",*ptr2_b++);
      }
      for(j = 0;j < nfunc;j++)
      {
        fprintf(rep_ptr,"  %f",*fptr1);
        fprintf(gen_ptr,"  %f",*fptr1++);
      }
      fprintf(rep_ptr," %d ",*rptr1);

      if(ncons != 0)
      {
        for(j = 0;j < ncons;j++)
        {
          fprintf(rep_ptr,"  %.2e",*cons_ptr2);
          fprintf(gen_ptr,"  %.2e",*cons_ptr2++);
        }
        fprintf(rep_ptr," %.2e",pop2_ptr->ind_ptr->error);
        fprintf(gen_ptr," %.2e",pop2_ptr->ind_ptr->error);
      }

      fprintf(rep_ptr," %f ",pop2_ptr->ind_ptr->cub_len);

      pop1_ptr->ind_ptr = &(pop1_ptr->ind[i+1]);
      pop2_ptr->ind_ptr = &(pop2_ptr->ind[i+1]);

      fprintf(gen_ptr,"\n");
    }

    fprintf(rep_ptr,"\n--------------------------------------------------\n\n");
    fprintf(rep_ptr,"-------------------------------------------------------\n");
    fprintf(gen_ptr,"\n--------------------------------------------------\n\n");
    return;
  }
  /*
void NSGA2::roulette(population *old_pop_ptr,population *mate_pop_ptr)
{
  int i,j,k,r,*gene1,*gene2,maxrank;
  float sum,c,cumprob[maxpop],rnd,*fit;

  //Find Sum of the fitness
  sum = 0;

  for (i = 0;i < popsize;i++)
    {
      old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[i]);
    fit = &(old_pop_ptr->ind_ptr->dumfit);
      r = old_pop_ptr->ind_ptr->rank;
      sum = sum +  *fit*1000*(popsize-r);
    }

  c = 0;

  //*Finding the cumulative probability for the different individuals/
  for(i = 0;i < popsize;i++)
    {
      old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[i]);

      fit = &(old_pop_ptr->ind_ptr->dumfit);

      r = old_pop_ptr->ind_ptr->rank;

      c = c +  *fit*1000*(popsize-r) ;

      cumprob[i] = c/sum ;

    }

  for(i = 0;i < popsize;i++)
   {
     rnd = randomperc();
     j = 0;

     //Generate a Random No to find which individual to select/

     while(rnd > cumprob[j])
       {
   j++;
       }

     old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
     mate_pop_ptr->ind_ptr = &(mate_pop_ptr->ind[i]);

     if(optype != 3)
       {
   gene1 = &(old_pop_ptr->ind_ptr->genes[0]);

   gene2 = &(mate_pop_ptr->ind_ptr->genes[0]);

   for(k = 0;k < chrom;k++)
   *gene2++ = *gene1++;

       }
     else
       {
   for(k = 0;k < chrom;k++)
     mate_pop_ptr->ind_ptr->gener[k] = old_pop_ptr->ind_ptr->gener[k];
       }
     for(k = 0;k < nvar;k++)
       mate_pop_ptr->ind_ptr->xreal[k] = old_pop_ptr->ind_ptr->xreal[k];
   }
  return;
}
   */

  void NSGA2::nselect(population *old_pop_ptr,population *pop2_ptr)
  {
    int *fit_ptr1,*fit_ptr2;

    float rnd2,*f1_ptr,*f2_ptr;

    int *s1_ptr,*s2_ptr,*select_ptr,*ptry;
    float *select_ptr_r, *s1_ptr_r, *s2_ptr_r;

    individual *j,*j1;

    int c,i,rnd,rnd1,k,n,j2,r,s,r1;

    old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[0]);

    pop2_ptr->ind_ptr= &(pop2_ptr->ind[0]);

    j =  &(old_pop_ptr->ind[popsize-1]);

    old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[0]);
    j2 = 0;
    r = popsize;
    s = chrom;

    for(n = 0,k = 0;n < popsize;n++,k++)
    {
      pop2_ptr->ind_ptr = &(pop2_ptr->ind[k]);
      select_ptr = &(pop2_ptr->ind_ptr->genes[0]);
      select_ptr_r = &(pop2_ptr->ind_ptr->xreal[0]);

      rnd2 = randomperc();

      rnd2 = popsize* rnd2;

      rnd = floor(rnd2);

      if(rnd == 0)
        rnd = popsize - k;

      if(rnd == popsize)
        rnd = (popsize-2)/2;

      /*Select first parent randomly*/
      j = &(old_pop_ptr->ind[rnd-1]);

      rnd2 = randomperc();

      rnd2 = popsize * rnd2;

      rnd1 = floor(rnd2);

      if (rnd1 == 0)
        rnd1 = popsize - n;

      if(rnd1 == popsize)
        rnd1 = (popsize - 4)/2;


      /*Select second parent randomly*/
      j1 = &(old_pop_ptr->ind[rnd1-1]);

      old_pop_ptr->ind_ptr = j;

      s1_ptr = &(old_pop_ptr->ind_ptr->genes[0]);
      s1_ptr_r = &(old_pop_ptr->ind_ptr->xreal[0]);
      fit_ptr1 = &(old_pop_ptr->ind_ptr->rank);
      f1_ptr = &(old_pop_ptr->ind_ptr->cub_len);

      old_pop_ptr->ind_ptr = j1;
      s2_ptr = &(old_pop_ptr->ind_ptr->genes[0]);
      s2_ptr_r = &(old_pop_ptr->ind_ptr->xreal[0]);
      fit_ptr2 = &(old_pop_ptr->ind_ptr->rank);
      f2_ptr = &(old_pop_ptr->ind_ptr->cub_len);
      /*--------------------------------------------------------------------------*/

      /*------------------SELECTION PROCEDURE------------------------------------*/

      /*Comparing the fitnesses*/

      if(*fit_ptr1 > *fit_ptr2)
      {
        for(i = 0;i < chrom;i++)
          *select_ptr++=*s2_ptr++;
        for(i = 0;i < nvar;i++)
          *select_ptr_r++=*s2_ptr_r++;
      }
      else
      {
        if(*fit_ptr1 < *fit_ptr2)
        {
          for(i = 0;i < chrom;i++)
            *select_ptr++=*s1_ptr++;
          for(i = 0;i < nvar;i++)
            *select_ptr_r++=*s1_ptr_r++;
        }
        else
        {
          if(*f1_ptr < *f2_ptr)
          {
            for(i = 0;i < chrom;i++)
              *select_ptr++=*s2_ptr++;
            for(i = 0;i < nvar;i++)
              *select_ptr_r++=*s2_ptr_r++;
          }
          else
          {
            for(i = 0;i < chrom;i++)
              *select_ptr++=*s1_ptr++;
            for(i = 0;i < nvar;i++)
              *select_ptr_r++=*s1_ptr_r++;
          }
        }
      }
    }
    return;
  }

  void NSGA2::unicross(population *new_pop_ptr, population *mate_pop_ptr)
  {
    int i,j,r,*gene,y,n,*par1,*par2,*chld1,*chld2;
    float rnd;
    for(i = 0,y = 0,n = 0;i < popsize;i++)
    {
      for(j = 0;j < chrom;j++)
      {

        /*Select a bit for doing cross-over*/
        new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[y]);
        chld1 = &(new_pop_ptr->ind_ptr->genes[j]);

        new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[y+1]);
        chld2 = &(new_pop_ptr->ind_ptr->genes[j]);

        mate_pop_ptr->ind_ptr = &(mate_pop_ptr->ind[n]);
        par1 = &(mate_pop_ptr->ind_ptr->genes[j]);

        mate_pop_ptr->ind_ptr = &(mate_pop_ptr->ind[n+1]);
        par2 = &(mate_pop_ptr->ind_ptr->genes[j]);

        rnd = randomperc();

        /*Checking whether to do cross-over or not*/
        if(rnd <= pcross)
        {
          ncross++;
          *chld1 = *par2;
          *chld2 = *par2;
        }

        else
        {
          *chld1 = *par1;
          *chld2 = *par2;
        }
      }

      y = y+2;
      n = n+2;

    }

    for(i = 0;i < popsize;i++)
    {
      new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[i]);
      gene = &(new_pop_ptr->ind_ptr->genes[0]);
      for(j = 0;j < chrom;j++)
      {
        gene = &(new_pop_ptr->ind_ptr->genes[j]);
      }
    }
    return;
  }
  void NSGA2::ranking(population *pop_ptr)
  {
    int i,j,k,       /*counters*/
    rnk,           /*rank*/
    val,           /*value obtained after comparing two individuals*/
    nondom,        /*no of non dominated members*/
    maxrank1,      /*Max rank of the population*/
    q;

    int* rankarr = new int[maxpop]; /*Array storing the individual number at a rank*/
    float *ptr1,*ptr2;

    /*------------------------------* RANKING *------------------------------*/

    /*Initializing the ranks to zero*/
    rnk = 0 ;

    nondom = 0 ;
    maxrank1 = 0;

    /*min_fit is initialize to start distributing the dummy fitness =
    popsize to the rank one individuals and keeping the record such
    that the minimum fitness of the better rank individual is always
    greater than max fitness of the relatively worse rank*/


    /*Difference in the fitness of minimum dummy fitness of better rank
  and max fitness of the next ranked individuals*/

    /*Initializing all the flags to 2*/

    for( j = 0 ;j < popsize; j++)
    {
      pop_ptr->ind[j].flag = 2;
    }

    q = 0;

    for(k =  0;k < popsize;k++,q=0)
    {
      for(j = 0;j < popsize;j++)
      {
        if (pop_ptr->ind[j].flag != 1)break;
        /*Break if all the individuals are assigned a rank*/
      }
      if(j == popsize)break;

      rnk = rnk + 1;

      for( j = 0 ;j < popsize; j++)
      {
        if(pop_ptr->ind[j].flag == 0) pop_ptr->ind[j].flag = 2;
        /*Set the flag of dominated individuals to 2*/
      }

      for(i = 0;i < popsize ; i++)
      {
        /*Select an individual which rank to be assigned*/

        pop_ptr->ind_ptr = &(pop_ptr->ind[i]);

        if(pop_ptr->ind_ptr->flag != 1 && pop_ptr->ind_ptr->flag != 0)
        {
          ptr1 = &(pop_ptr->ind_ptr->fitness[0]);

          for(j = 0;j < popsize ; j++)
          {

            /*Select the other individual which has not got a rank*/
            if( i != j)
            {
              if(pop_ptr->ind[j].flag != 1)
              {
                pop_ptr->ind_ptr = &(pop_ptr->ind[j]);
                ptr2 = &(pop_ptr->ind_ptr->fitness[0]);

                /*Compare the two individuals for fitness*/
                val = indcmp(ptr1,ptr2);

                /*VAL = 2 for dominated individual which rank to be given*/
                /*VAL = 1 for dominating individual which rank to be given*/

                /*VAL = 3 for non comparable individuals*/

                if( val == 2)
                {
                  pop_ptr->ind[i].flag = 0;/* individual 1 is dominated */
                  break;
                }

                if(val == 1)
                {
                  pop_ptr->ind[j].flag = 0;/* individual 2 is dominated */
                }

                if(val == 3)
                {
                  nondom++;/* individual 1 & 2 are non dominated */
                  if(pop_ptr->ind[j].flag != 0)
                    pop_ptr->ind[j].flag = 3;
                }

              }   /*if loop ends*/
            }       /* i != j loop ends*/
          }           /*loop over j ends*/
          if( j == popsize)
          {

            /*Assign the rank and set the flag*/
            pop_ptr->ind[i].rank = rnk;
            pop_ptr->ind[i].flag = 1;
            rankarr[q] = i;
            q++;
          }
        }       /*Loop over flag check ends*/
      }           /*Loop over i ends */
      pop_ptr->rankno[rnk-1] = q ;
    }
    maxrank1 = rnk;


    /* Find Max Rank of the population    */
    for(i = 0;i < popsize;i++)
    {
      rnk = pop_ptr->ind[i].rank;

      if(rnk > maxrank1)maxrank1 = rnk;

    }

    pop_ptr->maxrank = maxrank1;
    delete [] rankarr;
    return;
  }



  /*Routine Comparing the two individuals*/

  int NSGA2::indcmp(float *ptr1,float *ptr2)
  {
    float *fit1 = new float[maxfun];
    float *fit2 = new float[maxfun];
    int i,value,m,n;
    for(i = 0;i < nfunc ;i++)
    {
      fit1[i] = *ptr1++;
      fit2[i] = *ptr2++;
    }
    m = 0;
    n = 0;
    while(m < nfunc && fit1[m] <= fit2[m])
    {
      if((fit2[m] -  fit1[m]) < 1e-7) n++;
      m++;
    }
    if(m == nfunc)
    {
      if(n == nfunc) value = 3;
      else value = 1;             /*value = 1 for dominationg*/
    }
    else
    {
      m = 0;
      n = 0;
      while(m < nfunc && fit1[m] >= fit2[m])
      {
        if((fit1[m] - fit2[m]) < 1e-7) n++;
        m++;
      }
      if(m == nfunc)
      {
        if(n != nfunc)
          value = 2;                       /*value =  2 for dominated */
        else value =3;
      }
      else value = 3;                   /*value = 3 for incomparable*/
    }

    delete [] fit1;
    delete [] fit2;
    return value;
  }
} //namespace end
