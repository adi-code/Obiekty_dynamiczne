//---------------------------------------------------------------------------


#include "CParticleRegular.h"

//---------------------------------------------------------------------------

#include <stdlib.h>
#include "ICSingleObjOptimizer.h"
#include <math.h>
CParticleRegular::CParticleRegular() {
  FBestSolution = NULL;
  FCurrentSolution = NULL;

  FRealSize=0;
  FBinSize=0;

  FParent = NULL;

  FLambda = 0.1;
}

CParticleRegular::~CParticleRegular() {
  if(FCurrentSolution) delete FCurrentSolution;
  if(FBestSolution) delete FBestSolution;
}

void CParticleRegular::copy(const ICParticle *_src) {
  const CParticleRegular *src = dynamic_cast<const CParticleRegular *>(_src);
  if(FBestSolution)FBestSolution = src->FBestSolution->copy();
  if (FCurrentSolution) FCurrentSolution = src->FBestSolution->copy();

  FRealSize=src->FRealSize;
  FBinSize=src->FBinSize;

  FRealVelocity=src->FRealVelocity;
  FBinVelocity=src->FBinVelocity;

  FVelMax=src->FVelMax;

  FRealPos=src->FRealPos;
  FBinPos=src->FBinPos;

  FLambda=src->FLambda;

  FRealSize=src->FRealSize;
  FBinSize=src->FBinSize;
  FParent=src->FParent;
}
CParticleRegular::CParticleRegular(const CParticleRegular& src) {
  if(FBestSolution)FBestSolution = src.FBestSolution->copy();
  if (FCurrentSolution) FCurrentSolution = src.FBestSolution->copy();

  FRealSize=src.FRealSize;
  FBinSize=src.FBinSize;

  FRealVelocity=src.FRealVelocity;
  FBinVelocity=src.FBinVelocity;

  FVelMax=src.FVelMax;

  FRealPos=src.FRealPos;
  FBinPos=src.FBinPos;

  FLambda=src.FLambda;

  FRealSize=src.FRealSize;
  FBinSize=src.FBinSize;
  FParent=src.FParent;
}

ICParticle* CParticleRegular::clone() const {
  ICParticle* rval = new CParticleRegular(*this);
  return rval;
}
ICParticle* CParticleRegular::CompareFitness(ICParticle* _p) {
  if (_p == NULL) {
    return this;
  }
  CSolution* s = FBestSolution->compare(_p->getBestSolution());
  if(s == FBestSolution) {
    return this;
  }
  return _p;
}

void CParticleRegular::resize(unsigned int _fsize, unsigned int _bsize) {
  FRealSize=_fsize;
  FBinSize=_bsize;

  FRealVelocity.resize(FRealSize,0.0);
  FVelMax.resize(FRealSize,0.0);
  FBinVelocity.resize(FBinSize,0.0);
  FRealPos.resize(FRealSize,0.0);
  FBinPos.resize(FBinSize,0);
}
CSolution* CParticleRegular::getCurrentSolution() const {
  return FCurrentSolution;
};

void CParticleRegular::setCurrentSolution(CSolution* _s) {
  if (FCurrentSolution) {
    delete FCurrentSolution;
  }
  FCurrentSolution = _s;
  if (FBestSolution) {
    CSolution* s = FCurrentSolution->compare(FBestSolution);
    if (s!=FBestSolution) {
      setBestSolution(FCurrentSolution->copy());
    }
  } else {
    FBestSolution = FCurrentSolution->copy();
  }
};

CSolution* CParticleRegular::getBestSolution() const {
  return FBestSolution;
};

void CParticleRegular::setBestSolution(CSolution* _s) {
  if (FBestSolution) {
    delete FBestSolution;
  }
  FBestSolution = _s;
}

double CParticleRegular::getFloatVel(unsigned int _idx) const {
  assert(_idx < FRealSize);
  return FRealVelocity[_idx];
};

void CParticleRegular::setFloatVel(unsigned int _idx, double _v) {
  assert(_idx < FRealSize);
  FRealVelocity[_idx] = _v;
};

double CParticleRegular::getBinVel(unsigned int _idx) const {
  assert(_idx < FBinSize);
  return FBinVelocity[_idx];
};

void CParticleRegular::setBinVel(unsigned int _idx, double _v) {
  assert(_idx < FBinSize);
  FBinVelocity[_idx] = _v;
};

void CParticleRegular::setLambda(double _v) {
  FLambda = _v;
}

unsigned int CParticleRegular::getBinSize() {
  return FBinSize;
}

unsigned int CParticleRegular::getFloatSize() {
  return FRealSize;
}

double CParticleRegular::getFloatPos(unsigned int _idx) const {
  assert(_idx < FRealSize);
  return FRealPos[_idx];
}

void CParticleRegular::setFloatPos(unsigned int _idx, double _v) {
  assert(_idx < FRealSize);
  FRealPos[_idx] = _v;
}

unsigned int CParticleRegular::getBinPos(unsigned int _idx) const {
  assert(_idx < FBinSize);
  return FBinPos[_idx];
}

void CParticleRegular::setBinPos(unsigned int _idx, unsigned int _v) {
  assert(_idx < FBinSize);
  FBinPos[_idx];
}

void CParticleRegular::randomFloat() {
  for (unsigned int i=0; i < FRealSize; i++) {
    double check = FParent->getFHLimit(i) - FParent->getFLLimit(i);
    if (check > EPS) { FVelMax[i] = FLambda*check; }
    FRealPos[i] = FParent->getFLLimit(i) + check*drand();
    FRealVelocity[i] = FVelMax[i] + 2*FVelMax[i]*drand();
  }
}
void CParticleRegular::randomBin() {
  for (unsigned int i=0; i < FBinSize; i++) {
    FBinVelocity[i] = -4 + 8*drand();
    evaluateBinPos(i, FBinVelocity[i]);
  }
}

void CParticleRegular::evaluateBinPos(unsigned int _idx, double _v) {
  double tmp = drand();
  double s = 1/(1+exp(-1*_v));
  if (tmp<s) {
    FBinPos[_idx] = 1;
  } else {
    FBinPos[_idx] = 0;
  }
}
/** initiation method - default random initiation */
void CParticleRegular::initParticle() {
  // randomize(); is it needed here ? does not compile
  randomFloat();
  randomBin();
}

CSolution* CParticleRegular::evaluate() {
  double res=0;
  unsigned int fs = FParent->getFloatSize();
  unsigned int bs = FParent->getBinarySize();
  //unsigned int al = FParent->getBinaryArrayLength();

  unsigned int xs = fs+bs;

  double* x = new double[xs];
  std::vector<double> flo(fs, 0);

  for (unsigned int i = 0; i < fs; i++) {
    x[i] =  FRealPos[i];
    flo[i] = x[i];
  }
  std::vector<unsigned int> bin(al, 0);

  std::vector<double> xb = FParent->decode(FBinPos);
  for (unsigned int i = fs; i < xs; i++) {
    x[i] =  xb[i-fs];
  }

  CSObjSolution* s = new CSObjSolution();

  res = FParent->calculateObjectiveFnc(x, xs);

  s->FObjFncVal = res;
  s->FBinaryVars = xb;
  s->FFloatVars = flo;
  double sum=0;
  for (unsigned int i=0; i < FParent->getConstraintFncCnt(); i++) {
    double cval = FParent->calculateConstraintFnc(x, xs, i);
    s->FConstraintVals.push_back(cval);
    sum=+cval;
  }
  s->err=sum;
  setCurrentSolution(s);
  delete [] x;
  return s;

}
void CParticleRegular::fly()
{
  for (unsigned int i = 0; i < FRealSize; i++)
  {
    FRealPos[i] += FRealVelocity[i];
  }
  for (unsigned int i=0; i < FBinSize; i++) {
    evaluateBinPos(i, FBinVelocity[i]);
  }
}
void CParticleRegular::updateVel(double _dec, double _c1m, double _c2m, double _c3m, const ICParticle *_lb, const ICParticle *_gb)
{
  updateRealVel(_dec, _c1m, _c2m, _c3m, _lb, _gb);
  updateBinVel(_dec, _c1m, _c2m, _c3m, _lb, _gb);
}

void CParticleRegular::updateRealVel(double _dec, double _c1m, double _c2m, double _c3m, const ICParticle *_lb, const ICParticle *_gb) {
  const CParticleRegular *lb = dynamic_cast<const CParticleRegular*>(_lb);
  const CParticleRegular *gb = dynamic_cast<const CParticleRegular*>(_gb);
  double nv=0;
  double nv_hat=0;
  double prev =0;
  double soc =0;
  double cog =0;
  double glob =0;
  double alfa = 0;
  double beta = 0;
  double gamma = 0;

  for (unsigned int i = 0; i < FRealSize; i++)
  {
    alfa = _c1m*drand();
    beta = _c2m* drand();
    gamma = _c3m* drand();
    prev = FRealVelocity[i]*(1.0 -_dec);
    cog = alfa* (((CSObjSolution*)FBestSolution)->FFloatVars[i] - FRealPos[i]);
    soc =  beta* (((CSObjSolution*)_lb->getBestSolution())->FFloatVars[i] - FRealPos[i]);
    glob = gamma* (((CSObjSolution*)_gb->getBestSolution())->FFloatVars[i] - FRealPos[i]);
    nv_hat = prev + soc + cog + glob;
    if (FVelMax[i] > EPS)
    {nv = (nv_hat < 0 ? -1.0 : 1.0) * min(fabs(nv_hat), FVelMax[i]);}
    else
    { nv = nv_hat;}
    setFloatVel(i, nv);
  }
}

void CParticleRegular::updateBinVel(double _dec, double _c1m, double _c2m, double _c3m, const ICParticle *_lb, const ICParticle *_gb) {
  const CParticleRegular *lb = dynamic_cast<const CParticleRegular*>(_lb);
  const CParticleRegular *gb = dynamic_cast<const CParticleRegular*>(_gb);
  double nv=0;
  double nv_hat=0;
  double prev =0;
  double soc =0;
  double cog =0;
  double glob =0;
  double alfa = 0;
  double beta = 0;
  double gamma = 0;

  for (unsigned int i = 0; i < FBinSize; i++)
  {
    alfa = _c1m*drand();
    beta = _c2m* drand();
    gamma = _c3m* drand();
    prev = FRealVelocity[i]*(1.0 -_dec);
    cog = alfa* (((CSObjSolution*)FBestSolution)->FBinaryVars[i] - FBinPos[i]);
    soc =  beta* (((CSObjSolution*)_lb->getBestSolution())->FBinaryVars[i] - FBinPos[i]);
    glob = gamma* (((CSObjSolution*)_gb->getBestSolution())->FBinaryVars[i] - FBinPos[i]);
    nv_hat = prev + soc + cog + glob;

    {nv = (nv_hat < 0 ? -1.0 : 1.0) * min(fabs(nv_hat), 4);}

    setBinVel(i, nv);
  }
}


double CParticleRegular::min(double _a, double _b)
{
  if (_a < _b) {
    return _a;
  } else {return _b;}
}

ICSingleObjOptimizer* CParticleRegular::getParent() const {
  return FParent;
}

void CParticleRegular::setParent(ICSingleObjOptimizer* _parent) {
  FParent = _parent;
}
