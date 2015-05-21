//---------------------------------------------------------------------------

#ifndef ICEvolOptimizerH
#define ICEvolOptimizerH

#include "ICQualityFunction.h"
#include "ICNotifier.h"
#include <deque>
#include <vector>
//---------------------------------------------------------------------------
class CSolution{
public:
    enum EnCompare {enBetter, enWorse, enSame};

    virtual ~CSolution(){}
  /** virtual method comapring two solutions.
  @param _s compared solution
    pointer to a better one
  */
  virtual CSolution* compare(CSolution* _s)=0;
  virtual CSolution* copy()=0;
};

class ICEvolOptimizer  {
public:
	// \cond PRIVATE
   ICEvolOptimizer();

  /** default destructor
  */
  virtual ~ICEvolOptimizer() {};
  /** Method initating fields of this, basic class
  */

  // \endcond
  virtual void initVariables();


  /** This method adds an constraint function used in optimization
  @param _f pointer to the constraint function object
  */
  void addConstraintFnc(ICQualityFunction* _f);

  /** This method gives the number of stored constraint functions
  @return number of constraint functions
  */
  unsigned int getConstraintFncCnt();

  /** This method calculates constraint function value
  @param _pf vector of double values of the decision variables
  @param _nf size of vector of double values of the decision variables
  @param _i number of the calculated constraint functions
  @return double value of a calucated constraint function
  */
  double calculateConstraintFnc(double* _pf, unsigned int _nf, unsigned int _i);

  /** This method sets a number of decision variables vector
  @param _v a number of decision variables vector
    \cond PRIVATE
  */
  void setFloatSize(unsigned int _v);

  /** This method returns a size of a decision vector variables
  @return a number of decision variables vector
  */
  unsigned int getFloatSize();

  /** This method sets a number of binary-coded decision variables vector
  @param _v a number of decision variables vector
  */
  void setBinarySize(unsigned int _v);

  /** This method returns a size of a binary-coded decision vector variables
  @return a number of decision variables vector
  */
  unsigned int getBinarySize();

  /** Sets a number of the individuals in the population
   @param _v number of the individuals in the population
  */
  void setPopSize(unsigned int _v);

  /**
   Returns number of the individuals in the population
   @return number of the individuals in the population
  */
  unsigned int getPopSize();

  /**  Sets upper limits of the i-th real-coded variable
  @param _v limit value
  @param _i variable index
  */
  void setFHLimit(double _v, unsigned int _i);

  /** Returns upper limits of the i-th real-coded variable
  @param _i variable index
  @return limit value
  */
  double getFHLimit(unsigned int _i);

  /**  Sets lower limits of the i-th real-coded variable
  @param _v limit value
  @param _i variable index
  */
  void setFLLimit(double _v, unsigned int _i);

  /** Returns lower limits of the i-th real-coded variable
  @param _i variable index
  @return limit value
  */
  double getFLLimit(unsigned int _i);

  /**  Sets upper limits of the i-th binary-coded variable
  @param _v limit value
  @param _i variable index
  */
  void setBHLimit(int _v, unsigned int _i);

  /** Returns upper limits of the i-th binary-coded variable
  @param _i variable index
  @return limit value
  */
  double getBHLimit(unsigned int _i);

  /**  Sets lower limits of the i-th binary-coded variable
  @param _v limit value
  @param _i variable index
  */
  void setBLLimit(int _v, unsigned int _i);

  /** Returns lower limits of the i-th binary-coded variable
  @param _i variable index
  @return limit value
  */
  double getBLLimit(unsigned int _i);

  /** Sets no. of generations for which the GA will let the population evolve
  @param number of generations
  */
  void setGenerations(unsigned int _v);

  /** Returns no. of generations for which the GA will let the population evolve
  @return number of generations
  */
  unsigned int getGenerations();

  unsigned int getCurrentGeneration();

   /** Sets limits on real-coded variable are rigid (Default = true)
   @param if limits on real-coded variable are rigid
   */
   void setRigidConstraints(bool _v);

   /** Returns limits on real-coded variable are rigid (Default = true)
   @return if limits on real-coded variable are rigid
   */
   bool getRigidConstraints();

   /** Sets random seed(between 0 and 1)
   @param random seed
   */
   void setSeed(double _v);

   /** Returns random seed(between 0 and 1)
   @return random seed
   */
   double getSeed();

   /** \endcond */
   /** Method starts optimization*/
   virtual void run()=0;

   /** Method check algorithm configuration
  @return true if an algorithm is properly configured
      false if it is not
   */
   virtual bool checkConf()=0;

   /** Save to File flag setter
  @param _v flag value
  \cond PRIVATE
  */
   void setSaveToFile(bool _v);

   /** Save to file flag getter
  @return flag value
   */
   bool saveToFile();

   // \endcond

   /** Method terminates calculations before assumed calculations */
   void terminate();

   /** Method returns termination status*/
   bool isTerminated();

   /** Notifier setter */
   void setNotifier(ICNotifier* _notifier);

   /** Notifier getter*/
   ICNotifier* getNotifier();

   /**On before optimization start notifications firerer */
   void fireOnBeforeStartNotify(const char* _name);

  /**On after stop notifications firerer
  @params _name message from stop criterion after optimization was stopped
  */
   void fireOnAfterStopNotify(const char* _name);

   /** Notifications shown after error detection
  @params _name error description
  */
   void fireOnErrorNotify(const char* _error);

  /** Notifications shown after generation evaluation
  @params _g generation number
  */
  void fireOnGenerationDoneNotify(unsigned int _g);

   /** Method returns solution set found
  @return set of evaluated solutions

   */
   virtual CSolution* getSolution();

   /** Method sets solution set found
  @param _s set of evaluated solutions
   */
   virtual void setSolution(CSolution* _s);
   // \endcond


   virtual const char* getName() {return "Nonamed algorithm";}
    /** Method used to decode binary-coded variable to decimal notation \cond PRIVATE*/
   std::vector<double> decode(std::vector<unsigned int> _b);
   // \endcond

protected:

   /** Deque of pointers to the constraints functions*/
   std::deque<ICQualityFunction*> FConstraintFncs;

   /** Real coded decision variables size*/
   unsigned int FFloatVarSize;

   /** Bianry coded decision variables size*/
   unsigned int FBinaryVarSize;

   /** Upper limits of the real-coded variables*/
   std::vector<double> FFHighLimits;
   /** Lower limits of the real-coded variables*/
   std::vector<double> FFLowLimits;

   /** Upper limits of the binary-coded variables*/
   std::vector<int> FBHighLimits;
   /** Lower limits of the binary-coded variables*/
   std::vector<int> FBLowLimits;

   /** Number of the individuals in the population*/
   unsigned int FPopSize;

  /** No. of generations for which the GA will let the population evolve
    Default value is 100
    Too large value will take very long time and very small value will
    not let the GA reach the global Pareto front to the problem*/
   unsigned int FGenerations;

   unsigned int FCurrentGeneration;

   /** Save to file flag (Default = true)
   This library generates few output files. Reports for each algorithm might differ.
   */
   bool FSaveToFile;

   /**Variable used to stop calculations before assumed generations*/
   bool FTerminated;

   /** limits on real-coded variable are rigid (Default = true)
    * \cond PRIVATE
    * */
   bool FRigidConstraints;
   // \endcond

   /** Random seed(between 0 and 1) */
   double FSeed;

   /** Pointer to the notifier object*/
   ICNotifier* FNotifier;

   /** Solution of the optimized problem*/
   CSolution* FSolution;
 };
#endif
