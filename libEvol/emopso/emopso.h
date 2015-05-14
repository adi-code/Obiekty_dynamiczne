#ifndef EMOPSO_H
#define EMOPSO_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>

#include "Particle.h"
#include "../libEvolOptimizer/ICMultiObjectiveOptimizer.h"

class EFILE;

class EMOPSO : public ICMultiObjectiveOptimizer {

public:
  EMOPSO();
  ~EMOPSO();

  void run();
  virtual const char* getName() {return "EMOPSO";}
  void setNumberOfClusters(int n);
  int getNuberOfClusters();

  void setMaxArchiveSize(int _as);
  void setSocialAcc(double _sacc);
  void setCognitiveAcc(double _cacc);
  void setInertia(double _i);
  void setFlyTime(int _ft);
	private :
  int _numberOfClusters;

  double rnd(double _min,double _max);
  void function(int _w);

  void computeConstraints(int i);
  void computeObjectives(int i);

  void alocateMemory();

  void initialize();
  //int selectGBest(int _swarm);
  void flight();
  void execute();
  void copyx(std::vector<double> &_a,std::vector<double> &_b);
  void copyfx(std::vector<double> &_a,std::vector<double> &_b);
  void copy(std::vector<double> &_a,std::vector<double> &_b);
  void addClusters();
  int selectGbest(int _whichcluster);
  void perturbation(int );

  std::vector<double> lb,ub;
  int ndimensions;
  int nobjectives;
  int nconstr;
  int nparticles;
  int gmax;
  std::vector<Particle> particles;
  double W,C1,C2;
  int nclusters;
  EFILE* archive;
  int gen;

  int maxarchiveSize;
  int flyTime;
  CMultiObjectiveSolutionSet * output();
};

#endif

