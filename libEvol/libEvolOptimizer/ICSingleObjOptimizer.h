//---------------------------------------------------------------------------

#ifndef ICSingleObjOptimizerH
#define ICSingleObjOptimizerH

#include "ICEvolOptimizer.h"

/** Structure stores one solution of the optimized problem*/
class CSObjSolution : public CSolution {
  public:

    /** virtual method comapring two solutions.
		@param _s compared solution
		pointer to a better one
     */
    virtual CSolution* compare(CSolution* _s);

    CSObjSolution();

    CSObjSolution(const CSObjSolution& _src);

    virtual CSolution* copy();

    /**Vector of real coded variables values*/
    std::vector<double> FFloatVars;

    /**Vector of binary coded variables values*/
    std::vector<double> FBinaryVars;

    /**Vector of objective functions values*/
    double FObjFncVal;

    /**Vector of constraint functions values*/
    std::vector<double> FConstraintVals;

    /**Sum of constraint functions values*/
    double err;
};


class ICSingleObjOptimizer : public  ICEvolOptimizer {
  public:
    ICSingleObjOptimizer();

    virtual ~ICSingleObjOptimizer() {};

    virtual void initVariables();

    /** This method adds an objective function used in optimization
	@param _f pointer to the objective function object
     */
    void setObjectiveFnc(ICQualityFunction* _f);

    /** This method calculates objective function value
	@param _pf vector of double values of the decision variables
	@param _nf size of vector of double values of the decision variables
	@return double value of a calucated objective function
     */
	double calculateObjectiveFnc(double* _pf, unsigned int _nf);

    /** Method sets solution set found
	@param _s set of evaluated solutions
     */
    virtual void setSolution(CSolution* _s);

   /** Method starts optimization*/
    virtual void run()=0;

    /** Method check algorithm configuration
	@return true if an algorithm is properly configured
    	false if it is not
     */
    virtual bool checkConf();

  protected:
    /** Pointer to the objective function*/
    ICQualityFunction* FObjectiveFnc;
};
//---------------------------------------------------------------------------
#endif
