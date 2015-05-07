//---------------------------------------------------------------------------

#ifndef CNSGA2OptimizerH
#define CNSGA2OptimizerH

//---------------------------------------------------------------------------
#include <stdio.h>
#include <math.h>

#include "../libEvolOptimizer/ICMultiObjectiveOptimizer.h"

class CNSGA2Optimizer  : public ICMultiObjectiveOptimizer {
  public:
    /** Constructor
     */
    CNSGA2Optimizer();

    /** Destructor
     */
    ~CNSGA2Optimizer();

    /** Sets Cross-over propability (between 0.5 and 1)
  @param Cross-over propability
     */
    void setCross(double _v);

    /** Returns Cross-over propability (between 0.5 and 1)
  @return Cross-over propability
     */
    double getCross();

    /** Sets the mutation probability for real-coded vectors (between 0 and 1.0/FFloatVarSize)
   @param the mutation probability
     */
    void setFloatPMut(double _v);

    /** Returns the mutation probability for real-coded vectors (between 0 and 1.0/FFloatVarSize)
   @return the mutation probability
     */
    double getFloatPMut();

    /** Sets the mutation probability for binary-coded vectors (between 0 and 1.0/FBinaryVarSize)
   @param the mutation probability
     */
    void setBinaryPMut(double _v);

    /** Returns the mutation probability for binary-coded vectors (between 0 and 1.0/FBinaryVarSize)
   @return the mutation probability
     */
    double getBinaryPMut();

    /** Sets distribution Index for real-coded crossover between 0.5 to 100 (Default = 20)
   @param distribution Index for real-coded crossover
     */
    void setDi(double _v);

    /** Returns distribution Index for real-coded crossover between 0.5 to 100 (Default = 20)
   @return distribution Index for real-coded crossover
     */
    double getDi();

    /** Sets distribution Index for real-coded mutation between (Default = 10 to 500)
   @param distribution Index for real-coded mutation
     */
    void setDiM(double _v);

    /** Returns distribution Index for real-coded mutation between (Default = 10 to 500)
   @return distribution Index for real-coded mutation
     */
    double getDiM();

    /** Method starts optimization*/
    virtual void run();

    virtual const char* getName() { return "NSGA2";}
  private:

    /** Cross-over propability (between 0.5 and 1)*/
    double FPCross;

    /** The mutation probability for real-coded vectors (between 0 and 1.0/FFloatVarSize)*/
    double FPMutF;

    /** The mutation probability for real-coded vectors (between 0 and 1.0/FBinaryVarSize)*/
    double FPMutB;

    /** Distribution Index for real-coded crossover between 0.5 to 100 (Default = 20)*/
    double FDi;

    /** Distribution Index for real-coded mutation between (Default = 10 to 500)*/
    double FDiM;

  private:

};

extern CNSGA2Optimizer* gNSGA2Optimizer;
#endif
