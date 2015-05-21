//---------------------------------------------------------------------------


#include "ICEvolOptimizer.h"

#include "messages.h"
#include "assert.h"
#include <stddef.h>
#include <math.h>
//---------------------------------------------------------------------------

ICEvolOptimizer::ICEvolOptimizer() {
  FSaveToFile =true;
  FNotifier = NULL;
  FTerminated =false;
  FFloatVarSize=0;
  FBinaryVarSize=0;
  FRigidConstraints=true;
  FSeed=0.5;
  FGenerations = 100;
  FPopSize = 100;
  FSolution=NULL;
}

void ICEvolOptimizer::initVariables() {

}

void ICEvolOptimizer::addConstraintFnc(ICQualityFunction* _f) {
  FConstraintFncs.push_back(_f);
}

unsigned int ICEvolOptimizer::getConstraintFncCnt() {
  return FConstraintFncs.size();
}

double ICEvolOptimizer::calculateConstraintFnc(double* _pf, unsigned int _nf, unsigned int _i) {
  assert(_i < FConstraintFncs.size());
  assert(_nf == FFloatVarSize + FBinaryVarSize);
  return FConstraintFncs[_i]->calculate(_pf, _nf);
}

void ICEvolOptimizer::setFloatSize(unsigned int _v) {
    if (_v<0) {
        fireOnErrorNotify(E_OPT_FV_NO);
        return;
    }
  FFloatVarSize = _v;
  FFHighLimits.resize(FFloatVarSize, 0.0);
  FFLowLimits.resize(FFloatVarSize, 0.0);
}

unsigned int ICEvolOptimizer::getFloatSize() {
  return FFloatVarSize;
}

void ICEvolOptimizer::setBinarySize(unsigned int _v) {
    if (_v<0) {
        fireOnErrorNotify(E_OPT_BV_NO);
      return;
  }
  FBinaryVarSize = _v;
  FBHighLimits.resize(FBinaryVarSize, 0.0);
  FBLowLimits.resize(FBinaryVarSize, 0.0);
}

unsigned int ICEvolOptimizer::getBinarySize() {
  return FBinaryVarSize;
}
void ICEvolOptimizer::setPopSize(unsigned int _v) {
  FPopSize = _v;
}

unsigned int ICEvolOptimizer::getPopSize() {
  return FPopSize;
}

void ICEvolOptimizer::setFHLimit(double _v, unsigned int _i) {
  if (FFloatVarSize==0) {
    fireOnErrorNotify(E_OPT_FV_NS);
    return;
  }
  assert(_i < FFHighLimits.size());
  FFHighLimits[_i]=_v;
}

double ICEvolOptimizer::getFHLimit(unsigned int _i) {
  assert(_i < FFHighLimits.size());
  return FFHighLimits[_i];
}

void ICEvolOptimizer::setFLLimit(double _v, unsigned int _i) {
  if (FFloatVarSize==0) {
    fireOnErrorNotify(E_OPT_FV_NS);
    return;
  }
  assert(_i < FFLowLimits.size());
  FFLowLimits[_i]=_v;
}

double ICEvolOptimizer::getFLLimit(unsigned int _i){
  assert(_i < FFLowLimits.size());
  return FFLowLimits[_i];
}

void ICEvolOptimizer::setBHLimit(int _v, unsigned int _i) {
  if (FBinaryVarSize==0) {
    fireOnErrorNotify(E_OPT_BV_NS);
    return;
  }
  assert(_i < FBHighLimits.size());
  FBHighLimits[_i]=_v;
}

double ICEvolOptimizer::getBHLimit(unsigned int _i) {
  assert(_i < FBHighLimits.size());
  return FBHighLimits[_i];
}

void ICEvolOptimizer::setBLLimit(int _v, unsigned int _i) {
  if (FBinaryVarSize==0) {
    fireOnErrorNotify(E_OPT_BV_NS);
    return;
  }
  assert(_i < FBLowLimits.size());
  FBLowLimits[_i]=_v;
}

double ICEvolOptimizer::getBLLimit(unsigned int _i){
  assert(_i < FBLowLimits.size());
  return FBLowLimits[_i];
}

void ICEvolOptimizer::setGenerations(unsigned int _v) {
  if (_v<=0) {
    fireOnErrorNotify(E_OPT_PARAM_GENERATIONS);
    return;
  }
  FGenerations = _v;
}

unsigned int ICEvolOptimizer::getGenerations() {
  return FGenerations;
}

unsigned int ICEvolOptimizer::getCurrentGeneration() {
  return FCurrentGeneration;
}

void ICEvolOptimizer::setRigidConstraints(bool _v) {
  FRigidConstraints = _v;
}

bool ICEvolOptimizer::getRigidConstraints() {
  return FRigidConstraints;
}

void ICEvolOptimizer::setSeed(double _v) {
    if (_v > 1.0) {
       fireOnErrorNotify(E_OPT_PARAM_SEED);
    } else if (_v < 0) {
       fireOnErrorNotify(E_OPT_PARAM_SEED);
    } else {
       FSeed = _v;
    }
}

double ICEvolOptimizer::getSeed() {
  return FSeed;
}


void ICEvolOptimizer::setSaveToFile(bool _v) {
  FSaveToFile = _v;
}

bool ICEvolOptimizer::saveToFile() {
  return FSaveToFile;
}

void ICEvolOptimizer::terminate() {
  FTerminated = true;
}

bool ICEvolOptimizer::isTerminated() {
  return FTerminated;
}

void ICEvolOptimizer::setNotifier(ICNotifier* _notifier) {
  FNotifier= _notifier;
}

ICNotifier* ICEvolOptimizer::getNotifier() {
  return FNotifier;
}

void ICEvolOptimizer::fireOnBeforeStartNotify(const char* _name) {
  if (FNotifier) {
    FNotifier->onBeforeStart(_name);
  }
}

void ICEvolOptimizer::fireOnAfterStopNotify(const char* _name) {
  if (FNotifier) {
    FNotifier->onAfterStop(_name);
  }
}

void ICEvolOptimizer::fireOnErrorNotify(const char* _error) {
  if (FNotifier) {
    FNotifier->onError(_error);
  }
}

void ICEvolOptimizer::fireOnGenerationDoneNotify(unsigned int _g) {
  FCurrentGeneration = _g;
  if (FNotifier) {
    FNotifier->onGenerationDone(_g);
  }
}

CSolution* ICEvolOptimizer::getSolution() {
  return FSolution;
}

void ICEvolOptimizer::setSolution(CSolution* _s) {
  if (FSolution) {
    //delete FSolution; algorithm is responsible for deleting the solution on exit
  }
  FSolution = _s;
}

