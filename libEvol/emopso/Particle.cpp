#include "Particle.h"

Particle::Particle(int _dimensions, int _objectives, int _constraints){
  vel.resize(_dimensions);
  fx.resize(_objectives);
  fxpbest.resize(_objectives);
  x.resize(_dimensions);
  nx.resize(_dimensions);
  xpbest.resize(_dimensions);
  constraints.resize(_constraints);
}

CMultiObjectiveSolution * Particle::convertToCMultiObjectiveSolution() {
  CMultiObjectiveSolution * result = new CMultiObjectiveSolution;
  result->FBinaryVars.clear();
  result->FFloatVars = this->x;
  result->FObjFncVals = this->fx;
  result->FConstraintVals = this->constraints;
  return result;
}
