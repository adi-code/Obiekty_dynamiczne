//---------------------------------------------------------------------------


#include "ICMultiObjectiveOptimizer.h"

//---------------------------------------------------------------------------

#include <assert.h>
#include "messages.h"
CSolution*CMultiObjectiveSolution::compare(CSolution* _s){
  return this;
};
CSolution*  CMultiObjectiveSolution::copy() {
  CMultiObjectiveSolution* ns = new CMultiObjectiveSolution();
  ns->FFloatVars = this->FFloatVars;
  ns->FBinaryVars = this->FBinaryVars;
  ns->FObjFncVals = this->FObjFncVals;
  ns->FConstraintVals = this->FConstraintVals;
  ns->err = this->err;
  return ns;
};

CMultiObjectiveSolution::CMultiObjectiveSolution() {}

bool ICMultiObjectiveOptimizer::checkConf() {
    if (FFloatVarSize==0 && FBinaryVarSize==0) {
    fireOnErrorNotify(E_OPT_FBV_NS);
    return false;
  }

  if (FObjectiveFncs.size()<2) {
    fireOnErrorNotify(E_OPT_NMO);
    return false;
  }
  return true;
}

CMultiObjectiveSolution::CMultiObjectiveSolution(const CMultiObjectiveSolution& _src){
  FFloatVars = _src.FFloatVars;
  FBinaryVars = _src.FBinaryVars;
  FObjFncVals = _src.FObjFncVals;
  FConstraintVals = _src.FConstraintVals;
  err= _src.err;
}

CSolution* CMultiObjectiveSolutionSet::copy() {
  CMultiObjectiveSolutionSet* ss = new CMultiObjectiveSolutionSet();
  for (unsigned int i=0; i < this->solutions.size(); i++) {
    ss->solutions.push_back(this->solutions[i]);
  }
  return ss;
}
CMultiObjectiveSolutionSet::CMultiObjectiveSolutionSet(const CMultiObjectiveSolutionSet& _src) {
  solutions = _src.solutions;
}

CMultiObjectiveSolutionSet::CMultiObjectiveSolutionSet(){}

ICMultiObjectiveOptimizer::ICMultiObjectiveOptimizer() : ICEvolOptimizer() {

}

void ICMultiObjectiveOptimizer::initVariables() {
  ICEvolOptimizer::initVariables();
}

void ICMultiObjectiveOptimizer::addObjectiveFnc(ICQualityFunction* _f) {
  FObjectiveFncs.push_back(_f);
}

unsigned int ICMultiObjectiveOptimizer::getObjectiveFncCnt() {
  return FObjectiveFncs.size();
}

double ICMultiObjectiveOptimizer::calculateObjectiveFnc(double* _pf, unsigned int _nf, unsigned int _i) {
  assert(_i < FObjectiveFncs.size());
  assert(_nf == FFloatVarSize + FBinaryVarSize);
  return FObjectiveFncs[_i]->calculate(_pf, _nf);
}

// \cond PRIVATE
std::deque<ICQualityFunction*> ICMultiObjectiveOptimizer::getObjectiveFunctions() {
  return FObjectiveFncs;
}
// \endcond
