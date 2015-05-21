#ifndef PARTICLE_H
#define PARTICLE_H

#include "../libEvolOptimizer/ICMultiObjectiveOptimizer.h"

class EPSs{
public:
  std::vector<double> eps;
};

class Particle{
public:
  std::vector<double> vel;
  std::vector<double> constraints;
  std::vector<double>  x;
  std::vector<double>  nx;
  std::vector<double>  fx;
  std::vector<double>  xpbest;
  std::vector<double>  fxpbest;
  std::vector<EPSs> epss;
  int subswarm;
  Particle(int _dimensions, int _objectives, int _constraints);
  CMultiObjectiveSolution * convertToCMultiObjectiveSolution() ;
  //Particle(const Particle & other);

};
#endif // PARTICLE_H
