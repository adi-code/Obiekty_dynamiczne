//---------------------------------------------------------------------------

#ifndef CParticleRegularH
#define CParticleRegularH
//---------------------------------------------------------------------------
#include <iomanip>
#include <assert.h>
#include "ICParticle.h"

#include "mtrand.h"

#define EPS 1.0e-30

class CParticleRegular : public ICParticle
{
  public:
    CParticleRegular();
    CParticleRegular(const CParticleRegular& src);
    virtual ~CParticleRegular();

    // copy function
    virtual void copy(const ICParticle *src);

    /** clone partition */
    virtual ICParticle* clone() const;

    ICParticle* CompareFitness(ICParticle* _p);

    /** Method returns solution set found
	@return set of evaluated solutions
     */
    virtual CSolution* getCurrentSolution() const;

    /** Method sets solution set found
	@param _s set of evaluated solutions
     */
    virtual void setCurrentSolution(CSolution* _s);

    /** Method returns solution set found
	@return set of evaluated solutions
     */
    virtual CSolution* getBestSolution() const;

    /** Method sets solution set found
	@param _s set of evaluated solutions
     */
    virtual void setBestSolution(CSolution* _s);

    double getFloatVel(unsigned int _idx) const;
    void setFloatVel(unsigned int, double _v);

    double getBinVel(unsigned int _idx) const;
    void setBinVel(unsigned int, double _v);

    double getFloatPos(unsigned int _idx) const;
    void setFloatPos(unsigned int, double _v);

    unsigned int getBinPos(unsigned int _idx) const;
    void setBinPos(unsigned int, unsigned int _v);

    void setLambda(double _v);

    virtual void resize(unsigned int _fsize, unsigned int _basize);

    unsigned int getBinSize();

    unsigned int getFloatSize();

    /** randomize particle. Random values are choosen for properties as well as velocity */
    void randomFloat();
    void randomBin();

    /** initiation method - default random initiation */
    virtual void initParticle();

    virtual CSolution* evaluate();

    /** moving particle to the new position */
    void fly();

    /** get parent optimizer */
    virtual ICSingleObjOptimizer* getParent() const;

    /** set optimizer object */
    virtual void setParent(ICSingleObjOptimizer* _parent);

    /** update velocity vector */
    void updateVel(double _dec, double _c1m, double _c2m, double _c3m, const ICParticle *_lb, const ICParticle *_gb);

  protected:
    std::vector<double> FRealVelocity;
    std::vector<double> FBinVelocity;

    std::vector<double> FVelMax;

    std::vector<double> FRealPos;
    std::vector<unsigned int> FBinPos;

    void evaluateBinPos(unsigned int _idx, double _v);

    double FLambda;

    unsigned int FRealSize;
    unsigned int FBinSize;

    CSolution* FCurrentSolution;
    CSolution* FBestSolution;

    MTRand drand;

    ICSingleObjOptimizer* FParent;

    double min(double _a, double _b);

    /** update real-coded velocity vector */
    void updateRealVel(double _dec, double _c1m, double _c2m, double _c3m, const ICParticle *_lb, const ICParticle *_gb);

    /** update binary-coded velocity vector */
    void updateBinVel(double _dec, double _c1m, double _c2m, double _c3m, const ICParticle *_lb, const ICParticle *_gb);

};

#endif
