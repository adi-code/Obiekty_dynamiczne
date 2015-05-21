//---------------------------------------------------------------------------

#ifndef NSGA2H
#define NSGA2H
//---------------------------------------------------------------------------

#include <stdio.h>

//
//#define square(x) ((x)*(x))
//#define maxpop   500  /*Max population */
//#define maxchrom 200  /*Max chromosome length*/
//#define maxvar    20  /*Max no. of variables*/
//#define maxfun    10  /*Max no. of functions */
//#define maxcons   20  /*Max no. of Constraints*/
/** Nondominated Sorting Genetic Algorithm II namespace: has to be used with this algorithm*/
namespace wutNSGA2 {
  void initVariables();

  extern int maxpop;
  extern int maxchrom;
  extern int maxvar;
  extern int maxfun;
  extern int maxcons;

  class globpop
  {
    public:
      globpop();
      globpop(const globpop& src);
      ~globpop();
      int maxrank,   /*Max rank of the global population*/
      **rankar, /*record of array of individual numbers at
				  a particular rank */
      *rankno;           /*record of no. of individuals at a particular rank*/

      int **genes,
      *rank,            /*rank of different individuals*/
      *flag;            /*Setting the flag */

      float **fitness, /*Fitness function values for the different
			 individuals*/
      *cub_len,              /*Dummyfitness*/
      **xreal,       /*value of the decoded variables for different individuals */
      **xbin,   /* binray-coded variables */
      *error,               /*Error Values of the individuals*/
      **constr;
  };

  class individual      /*individual properties*/
  {
    public:
      individual();
      individual(const individual& src);
      ~individual();

      int *genes, /*bianry chromosome*/
      rank,              /*Rank of the individual*/
      flag;              /*Flag for ranking*/
      float *xreal, /*list of real variables*/
      *xbin;      /*list of decoded value of the chromosome */
      float *fitness,/*Fitness values */
      *constr,     /*Constraints values*/
      cub_len,             /*crowding distance of the individual*/
      error;              /* overall constraint violation for the individual*/
  };        /*Structure defining individual*/

  class population
  {
    public:
      population();
      ~population();
      population(const population& src);
      int maxrank;            /*Maximum rank present in the population*/
      float *rankrat;  /*Rank Ratio*/
      int *rankno;     /*Individual at different ranks*/
      individual *ind, /*Different Individuals*/
      *ind_ptr;
  };
/** Nondominated Sorting Genetic Algorithm II */
  class NSGA2 {
    public:
      /** Nondominated Sorting Genetic Algorithm II constructor*/
	  NSGA2();
      ~NSGA2();
      void run();
    private:
      globpop globalpop,*global_pop_ptr;
      int left,Lastrank;
      float **fpara1;
      int gener,       /*No of generations*/
      nvar,nchrom,          /*No of variables*/
      ncons,         /*No of Constraints*/
      *vlen,  /*Array to store no of bits for each variable*/
      nmut,          /* No of Mutations */
      ncross,        /*No of crossovers*/
      ans;
      float seed,      /*Random Seed*/
      pcross,        /*Cross-over Probability*/
      pmut_b, pmut_r,          /*Mutation Probability*/
      **lim_b, **lim_r;/*Limits of variable in array*/
      float di,        /*Distribution Index for the Cross-over*/
      dim,           /*Distribution Index for the Mutation*/
      delta_fit,     /* variables required forfitness for fitness sharing */
      min_fit,
      front_ratio;
      int optype,      /*Cross-over type*/
      nfunc,         /*No of functions*/
      sharespace;    /*Sharing space (either parameter or fitness)*/
      double *coef; /*Variable used for decoding*/

      int popsize,  /*Population Size*/
      chrom;             /*Chromosome size*/

      population oldpop,
      newpop, matepop,
      *old_pop_ptr, *new_pop_ptr, *mate_pop_ptr;

      double oldrand[55];     /* Array of 55 random numbers */
      int jrand;              /*current random number */
      double rndx2;           /* used with random normal deviate */
      int rndcalcflag;        /* used with random normal deviate */


      void crossover(population *new_pop_ptr,population *mate_pop_ptr) ;
      void decode(population *pop_ptr);
      void func(population *pop_ptr);
      void init(population *pop_ptr);
      void input(FILE *rep_ptr);
      void keepalive(population *pop1_ptr,population *pop2_ptr,population *pop3_ptr,int gen);
      void grank(int gen);
      void grankc(int gen);
      int indcmp1(float *ptr1,float *ptr2);
      void gsort(int rnk,int sel);
      void gshare(int rnk);
      void sort(int rnk);
      void mutate(population *new_pop_ptr);
      void rankcon(population *pop_ptr);
      int indcmp3(float *ptr1,float *ptr2);
      void advance_random();
      int flip(float prob);
      void initrandomnormaldeviate();
      double noise(double mu ,double sigma);
      double randomnormaldeviate();
      double randomperc();
      int rnd(int low, int high);
      float rndreal(float lo , float hi);
      void warmup_random(float random_seed);
      void realcross(population *new_pop_ptr,population *mate_pop_ptr);
      void realinit(population *pop_ptr);
      void real_mutate(population *new_pop_ptr);
      void report(int t,population *pop1_ptr, population *pop2_ptr,FILE *rep_ptr,FILE *gen_ptr, FILE *lastit);
      /*void roulette(population *old_pop_ptr,population *mate_pop_ptr);*/
      void nselect(population *old_pop_ptr,population *pop2_ptr);
      void unicross(population *new_pop_ptr,population *mate_pop_ptr);
      void ranking(population *pop_ptr);
      int indcmp(float *ptr1,float *ptr2);

  };
}
#endif
