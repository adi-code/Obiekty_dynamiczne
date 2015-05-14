#ifndef coptim_particleH
#define coptim_particleH

#include "ICSingleObjOptimizer.h"

class ICParticle
{
  public:
    //ICParticle() { }

    virtual ~ICParticle() {};

    // copy function
    virtual void copy(const ICParticle *src) = 0;

    /** clone particle */
    virtual ICParticle* clone() const = 0;

    virtual ICParticle* CompareFitness(ICParticle* _p)=0;

    /** Method returns solution set found
	@return set of evaluated solutions
     */
    virtual CSolution* getCurrentSolution() const =0;

    /** Method sets solution set found
	@param _s set of evaluated solutions
     */
    virtual void setCurrentSolution(CSolution* _s)=0;

    /** Method returns solution set found
	@return set of evaluated solutions
     */
    virtual CSolution* getBestSolution() const =0;

    /** Method sets solution set found
	@param _s set of evaluated solutions
     */
    virtual void setBestSolution(CSolution* _s)=0;

    virtual double getFloatVel(unsigned int _idx) const = 0;
    virtual void setFloatVel(unsigned int, double _v) = 0;

    virtual double getBinVel(unsigned int _idx) const = 0;
    virtual void setBinVel(unsigned int, double _v) = 0;

    virtual double getFloatPos(unsigned int _idx) const=0;
    virtual void setFloatPos(unsigned int, double _v)=0;

    virtual unsigned int getBinPos(unsigned int _idx) const=0;
    virtual void setBinPos(unsigned int, unsigned int _v)=0;

    virtual void resize(unsigned int _fsize, unsigned int _basize) = 0;

    virtual void setLambda(double _v)=0;
    /** randomize particle. Random values are choosen for all properties and initial velocity */
    virtual void randomFloat() = 0;
    virtual void randomBin() = 0;

    virtual unsigned int getBinSize()=0;

    virtual unsigned int getFloatSize()=0;

    /** evaluator method - need to be defined in any derived class */
    virtual CSolution* evaluate() = 0;

    /** initiation method - default random initiation */
    virtual void initParticle() = 0;

    /** moving particle to the new position */
    virtual void fly() = 0;

    /** get parent optimizer */
    virtual ICSingleObjOptimizer* getParent() const=0;

    /** set optimizer object */
    virtual void setParent(ICSingleObjOptimizer* _parent)=0;

    /** update velocity vector */
    virtual void updateVel(double _dec, double _c1m, double _c2m, double _c3m, const ICParticle *_lb, const ICParticle *_gb) = 0;

  protected:
    /** update real-coded velocity vector */
    virtual void updateRealVel(double _dec, double _c1m, double _c2m, double _c3m, const ICParticle *_lb, const ICParticle *_gb) = 0;

    /** update binary-coded velocity vector */
    virtual void updateBinVel(double _dec, double _c1m, double _c2m, double _c3m, const ICParticle *_lb, const ICParticle *_gb) = 0;
};

#endif
