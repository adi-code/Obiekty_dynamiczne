//---------------------------------------------------------------------------



#include <assert.h>
#include <stddef.h>
#include "ICSingleObjOptimizer.h"
#include "messages.h"
//---------------------------------------------------------------------------

CSolution* CSObjSolution::compare(CSolution* _s) {
  CSObjSolution* sol = dynamic_cast<CSObjSolution*>(_s);

  if (sol->err + sol->FObjFncVal < this->err+this->FObjFncVal) {
	return _s;
  }
  return this;
}

CSObjSolution::CSObjSolution() {

}

CSObjSolution::CSObjSolution(const CSObjSolution& _src) {
  FFloatVars = _src.FFloatVars;
  FBinaryVars = _src.FBinaryVars;
  FObjFncVal= _src.FObjFncVal;
  FConstraintVals = _src.FConstraintVals;
  err = _src.err;
}
CSolution* CSObjSolution::copy() {
  CSObjSolution* ns = new CSObjSolution();
  ns->FFloatVars = this->FFloatVars;
  ns->FBinaryVars = this->FBinaryVars;
  ns->FObjFncVal = this->FObjFncVal;
  ns->FConstraintVals = this->FConstraintVals;
  ns->err = this->err;
  return ns;
}

bool ICSingleObjOptimizer::checkConf() {
  if (FFloatVarSize==0 && FBinaryVarSize==0) {
	fireOnErrorNotify(E_OPT_FBV_NS);
	return false;
  }
  if (!FObjectiveFnc) {
    fireOnErrorNotify(E_OPT_OFNS);
    return false;
  }
  return true;
}

ICSingleObjOptimizer::ICSingleObjOptimizer() : ICEvolOptimizer() {
  FObjectiveFnc = NULL;
}

void ICSingleObjOptimizer::initVariables() {
  ICEvolOptimizer::initVariables();
}

void ICSingleObjOptimizer::setObjectiveFnc(ICQualityFunction* _f) {
  FObjectiveFnc = _f;
}

double ICSingleObjOptimizer::calculateObjectiveFnc(double* _pf, unsigned int _nf) {
  assert(_nf == FFloatVarSize + FBinaryVarSize);
  CSObjSolution* s =  new CSObjSolution;
  for (int i=0; i < FFloatVarSize; i++) {
    s->FFloatVars.push_back(_pf[i]);
  }
  for (int i=0; i < FBinaryVarSize; i++) {
	s->FBinaryVars.push_back(_pf[FFloatVarSize+i]);
  }

  double res =0;
  for (int i=0; i < FConstraintFncs.size(); i++) {
	   s->FConstraintVals.push_back(FConstraintFncs[i]->calculate(_pf, _nf));
	   res+= s->FConstraintVals[i];
  }

  s->err=res;
  s->FObjFncVal = FObjectiveFnc->calculate(_pf, _nf);
  res+=s->FObjFncVal;

  setSolution(s);

  return res;
}
void ICSingleObjOptimizer::setSolution(CSolution* _s) {
  CSObjSolution* sol = dynamic_cast<CSObjSolution*>(_s);
  assert(sol!=NULL);
  if (!FSolution) {
    FSolution = _s;
    return;
  }
  CSolution* res = FSolution->compare(_s);
  if (res!=FSolution) {
    delete FSolution;
    FSolution = _s;
  }
}



