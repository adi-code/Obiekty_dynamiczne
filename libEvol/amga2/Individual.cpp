#include <iostream>
#include <cstdlib>
#include <cmath>

#include "Individual.h"
#include "Amga2.h"
#include <ICMultiObjectiveOptimizer.h>

using namespace std;

Individual::Individual(int _numVar, int _numIntVar, int _numObj, int _numConstr) {
  constrViolation = 0.0;
  evalCounter = 0;
  rank = 0;
  diversity = 0.0;

  numVar = _numVar;

  numIntVar = _numIntVar;

  numObj = _numObj;
  numConstr = _numConstr;

  var = new double[_numVar];

  intVar = new int[_numIntVar];

  normalVar = new double[_numVar];
  obj = new double[_numObj];
  normalObj = new double[_numObj];
  for (register unsigned int i = 0; i < _numVar; i++) {
    var[i] = normalVar[i] = 0.0;
  }

  for (register unsigned int i = 0; i < _numIntVar; i++) {
	intVar[i] = 0.0;
  }

  for (register unsigned int i = 0; i < _numObj; i++) {
    obj[i] = normalObj[i] = 0.0;
  }
  if (_numConstr > 0) {
    constr = new double[_numConstr];
    normalConstr = new double[_numConstr];
    for (register unsigned int i = 0; i < _numConstr; i++) {
      constr[i] = normalConstr[i] = 0.0;
    }
  }
}

Individual::Individual(const Individual& rhs) {
  numVar = rhs.numVar;
  numObj = rhs.numObj;
  numConstr = rhs.numConstr;

  numIntVar = rhs.numIntVar;

  var = new double[numVar];
  intVar = new int[numIntVar];

  normalVar = new double[numVar];
  obj = new double[numObj];
  normalObj = new double[numObj];
  if (numConstr > 0) {
    constr = new double[numConstr];
    normalConstr = new double[numConstr];
  }
  copy(rhs);
}

Individual::~Individual() {
  //delete[](var);
  //delete[](normalVar);
  //delete[](obj);
  //delete[](normalObj);
  if (numConstr > 0) {
    delete[](constr);
    delete[](normalConstr);
  }
}

const Individual& Individual::operator=(const Individual& rhs) {
  if (this == &rhs) {
    return (*this);
  }
  copy(rhs);
  return (*this);
}

void Individual::copy(const Individual& rhs) {
  for (register unsigned int i = 0; i < numVar; i++) {
    var[i] = rhs.var[i];
    normalVar[i] = rhs.normalVar[i];
  }

  for (register unsigned int i = 0; i < numIntVar; i++) {
	intVar[i] = rhs.intVar[i];
  }

  for (register unsigned int i = 0; i < numObj; i++) {
    obj[i] = rhs.obj[i];
    normalObj[i] = rhs.normalObj[i];
  }
  if (numConstr > 0) {
    for (register unsigned int i = 0; i < numConstr; i++) {
      constr[i] = rhs.constr[i];
      normalConstr[i] = rhs.normalConstr[i];
    }
  }
  constrViolation = rhs.constrViolation;
  evalCounter = rhs.evalCounter;
  rank = rhs.rank;
  diversity = rhs.diversity;
  return;
}

void Individual::setRank(int _rank) {
  rank = _rank;
}

int Individual::getRank() {
  return rank;
}

void Individual::setDiversity(int div) {
  diversity = div;
}

int Individual::getDiversity() {
  return diversity;
}

double Individual::getConstrViolation() {
  return constrViolation;
}

void Individual::setConstrViolation(double _constrViolation) {
  constrViolation = _constrViolation;
}


CMultiObjectiveSolution * Individual::convertToCMultiObjectiveSolution() {
  CMultiObjectiveSolution * result = new CMultiObjectiveSolution;

  for (int i = 0; i < numObj; i++) {
    result->FObjFncVals.push_back(obj[i]);
  }

  for (int i = 0; i < numVar; i++) {
    result->FFloatVars.push_back(var[i]);
  }

  for (int i = 0; i < numIntVar; i++) {
	result->FBinaryVars.push_back(intVar[i]);
  }

  for (int i = 0; i < numConstr; i++) {
    result->FConstraintVals.push_back(constr[i]);
    result->err += constr[i];
  }

  return result;
}

int Individual::getEvalCounter() {
  return evalCounter;
}

void Individual::setEvalCounter(int _evalCounter) {
  evalCounter = _evalCounter;
}
