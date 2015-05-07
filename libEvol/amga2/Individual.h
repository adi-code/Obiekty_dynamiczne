#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H 1

#include <iostream>

class Amga2;
class CMultiObjectiveSolution;

class Individual {
    friend class Amga2;

  public:
    Individual(int _numVar, int _numIntVar, int _numObj, int _numConstr);
    Individual(const Individual&);
    const Individual & operator=(const Individual&);
	double * getVar() {return var;}

    int * getIntVar() {return intVar;}


    double* getObj() {return obj;}
    double* getConstr() {return constr;}
    double* getNormalVar() {return normalVar;}
    double* getNormalObj() {return normalObj;}
    double* getNormalConstr() {return normalConstr;}


    CMultiObjectiveSolution * convertToCMultiObjectiveSolution();
    void setRank(int _rank);
    int getRank();
    void setDiversity(int div);
    int getDiversity();

    double getConstrViolation();
    void setConstrViolation(double _constrViolation);

    int getEvalCounter();
    void setEvalCounter(int _evalCounter);


    ~Individual();

  private:
    void copy(const Individual&);

  private:

    double* var;

	int* intVar;

	double* obj;
	double* constr;
	int numVar;

    int numIntVar;

	int numObj;
    int numConstr;
    double* normalVar;
    double* normalObj;
    double* normalConstr;
    double constrViolation;
    unsigned int evalCounter;
    unsigned int rank;
    double diversity;
};

#endif
