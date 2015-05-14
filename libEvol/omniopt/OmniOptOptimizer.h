/*
 * EMOEAOptimizer.h
 *
 *  Created on: 21 paź 2013
 *      Author: tomasz
 */

#ifndef OMNIOPTOPTIMIZER_H_
#define OMNIOPTOPTIMIZER_H_

#include "../libEvolOptimizer/ICMultiObjectiveOptimizer.h"
#include <stdio.h>

#define INF 1.0e14
#define EPS 1.0e-10


class OmniOptOptimizer : public ICMultiObjectiveOptimizer {

public:
	OmniOptOptimizer();

	virtual ~OmniOptOptimizer();

	const char* getName() {return "OmniOpt";}
	void run();
	void setCross(double _v);
  double getCross();
  void setCrossBin(double _v);
  double getCrossBin();
  void setPMut(double _v);
  double getPMut();
  void setPMutBin(double _v);
  double getPMutBin();

  void setDi(double _v);
  double getDi();
  void setDiM(double _v);
  double getDiM();



private:

	void alocateMemory();

	typedef struct
	{
		int rank;
		double constr_violation;
		double *xreal;
		double *xbin;
		double *obj;
		double *constr;
		double realvar_crowd_dist;
		double binvar_crowd_dist;
		double obj_crowd_dist;
		double crowd_dist;
	} individual;

	typedef struct
	{
		individual *ind;
	} population;

	typedef struct lists
	{
		int index;
		struct lists *parent;
		struct lists *child;
	} list;


	int nreal;
	int nbin;
	int nobj;
	int ncon;
	int popsize;
	double pcross_real;
	double pcross_bin;
	double pmut_real;
	double pmut_bin;
	double eta_c;
	double eta_m;
	int ngen;
	int nbinmut;
	int nrealmut;
	int nbincross;
	int nrealcross;
	double *min_realvar;
	double *max_realvar;
	double *min_binvar;
	double *max_binvar;
	double *min_obj;
	double *max_obj;
	double *epsilon;
	double delta;
	int mate;
	int input_type;
	int run_mode;
	int var_option;
	int obj_option;
	int frequency;
	int var1;
	int obj1;
	int obj2;
	int obj3;
	int angle1;
	int angle2;
	int choice;

	void allocate_memory_pop (population *, int);
	void allocate_memory_ind (individual *);
	void deallocate_memory_pop (population *, int);
	void deallocate_memory_ind (individual *);

	void crossover (individual *, individual *, individual *, individual *);
	void realcross (individual *, individual *, individual *, individual *);
	void bincross (individual *, individual *, individual *, individual *);

	void assign_crowding_distance_list (population *, list *, int);
	void assign_crowding_distance_list_obj (population *, list *, int);
	void assign_crowding_distance_list_realvar (population *, list *, int);
	void assign_crowding_distance_list_binvar (population *, list *, int);
	void assign_crowding_distance_indices (population *, int, int);
	void assign_crowding_distance_indices_obj (population *, int, int);
	void assign_crowding_distance_indices_realvar (population *, int, int);
	void assign_crowding_distance_indices_binvar (population *, int, int);

	void decode_pop (population *);
	void decode_ind (individual *);

	void onthefly_display (population *, FILE *, int);

	int check_loose_dominance (individual *, individual *);
	int check_dominance (individual *, individual *);
	void define_epsilon (population *, int , double *);

	void evaluate_pop (population *);
	void evaluate_ind (individual *);

	void fill_nondominated_sort (population *, population *);
	void crowding_fill (population *, population *, int , int , list *);

	void ga_optimizer(void);

	void input(void);

	void initialize_pop (population *);
	void initialize_random_pop (population *);
	void initialize_latin_pop (population *);
	void initialize_file_pop (population *);

	void insert (list *, int);
	list* del (list *);

	void merge(population *, population *, population *);
	void copy_ind (individual *, individual *);

	void mutation_pop (population *);
	void mutation_ind (individual *);
	void bin_mutate_ind (individual *);
	void real_mutate_ind (individual *);

	void output (FILE *, FILE *, FILE *, FILE *, FILE *);

	void pdefinit(void);

	void test_problem (double *, double *, int **, double *, double *);

	void assign_rank_and_crowding_distance (population *);

	void report_pop (population *, FILE *);
	void report_feasible (population *, FILE *);
	void report_var (population *, FILE *);
	void report_obj (population *, FILE *);

	void quicksort_front_obj(population *, int , int *, int);
	void q_sort_front_obj(population *, int , int *, int , int );
	void quicksort_front_realvar(population *, int , int *, int );
	void q_sort_front_realvar(population *, int , int *, int , int );
	void quicksort_front_binvar(population *, int , int *, int );
	void q_sort_front_binvar(population *, int , int *, int , int );
	void quicksort_dist(population *, int *, int );
	void q_sort_dist(population *, int *, int , int );

	void selection (population *, population *);
	void restricted_selection (population *, population *);
	individual* tournament (individual *, individual *);
	list* search_nearest (population *, list *, list *, double *, double *, double *, double *);
	list* search_second_nearest (population *, list *, list *, list *, double *, double *, double *, double *);
	double calc_distance (population *, list *, list *, double *, double *, double *, double *);

	double maximum (double , double);
	double minimum (double , double);
	double square (double);
	double modulus (double *, int);
	double dot (double *, double *, int);
	double mean (double *, int);

	CMultiObjectiveSolution * createSolutionFromIndividual(individual * ind);

};

#endif /* OmniOptOPTIMIZER_H_ */
