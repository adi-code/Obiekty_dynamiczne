//---------------------------------------------------------------------------

#ifndef ICMultiObjectiveOptimizerH
#define ICMultiObjectiveOptimizerH
//---------------------------------------------------------------------------
#include "ICEvolOptimizer.h"

/** Structure stores one solution of the optimized problem*/
class CMultiObjectiveSolution : public CSolution {
  public:
    CMultiObjectiveSolution();
  CMultiObjectiveSolution(const CMultiObjectiveSolution& _src);
  /** virtual method comapring two solutions.
  @param _s compared solution
  pointer to a better one
  */
  virtual CSolution* compare(CSolution* _s);

  virtual CSolution* copy();

  /**Vector of real coded variables values*/
  std::vector<double> FFloatVars;

  /**Vector of binary coded variables values*/
  std::vector<int> FBinaryVars;

  /**Vector of objective functions values*/
  std::vector<double> FObjFncVals;

  /**Vector of constraint functions values*/
  std::vector<double> FConstraintVals;

  /**Sum of constraint functions values*/
  double err;
};

/** Structure stores best solutions of the optimized problem*/
class CMultiObjectiveSolutionSet : public CSolution {
public:


	typedef std::deque<CMultiObjectiveSolution*> MultiObjectiveSolutionsCollection;
	typedef MultiObjectiveSolutionsCollection::iterator Iterator;

    /** virtual method comapring two solutions.
  @param _s compared solution
  pointer to a better one
  */
  virtual CSolution* compare(CSolution* _s){return this;};
  Iterator begin() {return solutions.begin();}
  Iterator end() {return solutions.end();}

  CMultiObjectiveSolutionSet();

  ~CMultiObjectiveSolutionSet() {
   for (std::deque<CMultiObjectiveSolution*>::iterator solution = solutions.begin();
     solution != solutions.end();
       solution++) {
         delete *solution;
         solution = solutions.erase(solution);
       }
   }

  virtual CSolution* copy();

  CMultiObjectiveSolutionSet(const CMultiObjectiveSolutionSet& _src);

  /**vector of solutions found*/
 MultiObjectiveSolutionsCollection solutions;
};


class ICMultiObjectiveOptimizer : public  ICEvolOptimizer {
public:

  ICMultiObjectiveOptimizer();

  virtual ~ICMultiObjectiveOptimizer() {};

  virtual void initVariables();

    /** This method adds an objective function used in optimization
  @param _f pointer to the objective function object
  */
  void addObjectiveFnc(ICQualityFunction* _f);

  /** This method gives the number of stored objective functions
  @return number of objective functions
  */
  unsigned int getObjectiveFncCnt();

  /** This method calculates objective function value
    @param _pf vector of double values of the decision variables
    @param _nf size of vector of double values of the decision variables
    @param _i number of the calculated objective functions
    @return double value of a calucated objective function
  */
  double calculateObjectiveFnc(double* _pf, unsigned int _nf, unsigned int _i);

  // Probably there we will also need getConstraintFncCnt();
  // and calculateConstraintFnc(double* _pf, unsigned int _nf, unsigned int _i);
  // for AMGA2 - it requires calculation of contraints by an individual

  void clearObjectiveFunctions() {FObjectiveFncs.clear();}
	void clearConstraintFunctions() {FConstraintFncs.clear();}

	CMultiObjectiveSolutionSet * getSolutionSet() {return (CMultiObjectiveSolutionSet*)FSolution;}

  /** Method starts optimization*/
  virtual void run()=0;

    /** Method check algorithm configuration
  @return true if an algorithm is properly configured
      false if it is not
   */
  virtual bool checkConf();

  std::deque<ICQualityFunction*> getObjectiveFunctions();

protected:
  /** Deque of pointers to the objective functions*/
  std::deque<ICQualityFunction*> FObjectiveFncs;
};

#endif
