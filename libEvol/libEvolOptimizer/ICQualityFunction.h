#ifndef ICQUALITYFNC_H
#define ICQUALITYFNC_H
class ICQualityFunction
{
  public:
    /** Virtual destructor */
    virtual ~ICQualityFunction() {};

    /** This method calculates objective function value
		@param _pf vector of double values of the decision variables
		@param _nf size of vector of double values of the decision variables
		@param _usrParams pointer to additipnal parameters defined by the library user
		@return double value of a calucated objective function
     */
    virtual double calculate(double* _pf, unsigned int _nf)=0;

    /** Description of selected algorithm
	@return brief description of chosen optimization algorithm
     */
    virtual const char *getDescription() {
      return "most general description you will ever get";
    }
};
#endif
