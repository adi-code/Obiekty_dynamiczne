//---------------------------------------------------------------------------




#include "CNSGA2Optimizer.h"
#include <assert.h>
#include "NSGA2.h"
#include <messages.h>



//---------------------------------------------------------------------------


CNSGA2Optimizer* gNSGA2Optimizer = NULL;

CNSGA2Optimizer::CNSGA2Optimizer() : ICMultiObjectiveOptimizer() {

    if(gNSGA2Optimizer) delete gNSGA2Optimizer;
  gNSGA2Optimizer = this;
  FSaveToFile =true;
  FDi = 20;
  FDiM = 20;
  FPCross = 0.8;
  FPMutF=0.2;
  FPMutB=0;

	this->setPopSize(100);
	this->setBinarySize(0);
	this->setFloatSize(4);
	this->setGenerations(2500);
	this->setSeed(0.5);

}

CNSGA2Optimizer::~CNSGA2Optimizer() {

}

void CNSGA2Optimizer::setCross(double _v) {
  if (_v > 1.0) {
     fireOnErrorNotify(E_OPT_PARAM_CROSSOVER);
  } else if (_v < 0) {
     fireOnErrorNotify(E_OPT_PARAM_CROSSOVER);
  } else {
     FPCross = _v;
  }
}

double CNSGA2Optimizer::getCross() {
  return FPCross;
}

void CNSGA2Optimizer::setFloatPMut(double _v){
    if (FFloatVarSize==0) {
     fireOnErrorNotify(E_OPT_FV_NS);
     return;
  }
  if (_v > 1.0/FFloatVarSize) {
     FPMutF = 1.0/FFloatVarSize;
  } else if (_v < 0) {
     fireOnErrorNotify(E_OPT_PARAM_MUTF);
  } else {
     FPMutF = _v;
  }

}

double CNSGA2Optimizer::getFloatPMut() {
  return FPMutF;
}

void CNSGA2Optimizer::setBinaryPMut(double _v){
  if (FBinaryVarSize==0) {
     fireOnErrorNotify(E_OPT_BV_NS);
     return;
  }
  if (_v > 1.0/FBinaryVarSize) {
     FPMutB = 1.0/FBinaryVarSize;
  } else if (_v < 0) {
     fireOnErrorNotify(E_OPT_PARAM_MUTB);
  } else {
     FPMutB = _v;
  }
}

double CNSGA2Optimizer::getBinaryPMut() {
  return FPMutB;
}

void CNSGA2Optimizer::setDi(double _v) {
  if (_v<0) {
     fireOnErrorNotify(E_OPT_PARAM_DI);
     return;
  }
  FDi = _v;
}

double CNSGA2Optimizer::getDi() {
  return FDi;
}

void CNSGA2Optimizer::setDiM(double _v) {
    if (_v<0) {
     fireOnErrorNotify(E_OPT_PARAM_DIM);
     return;
  }
  FDiM = _v;
}

double CNSGA2Optimizer::getDiM() {
  return FDiM;
}


void CNSGA2Optimizer::run() {
  if (!checkConf()) {
    fireOnErrorNotify(E_OPT_TERM);
    return;
  }
  initVariables();
  FTerminated = false;
    wutNSGA2::initVariables();
  wutNSGA2::NSGA2 nsga;
  fireOnBeforeStartNotify(M_OPT_STARTED);
  nsga.run();
  fireOnAfterStopNotify(M_OPT_FINISHED);
}
