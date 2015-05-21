#ifndef EFILE_H
#define EFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <limits>
#include "Particle.h"

class EFILE{

  std::vector<double> tlb,tub;
  int ndimensions;
  int nobjectives;
  int nconstr;

  int maxbests;
  int nclusters;
  int allnsolutions;
  int maxsolutions;
  bool hasentered;
  bool firsttime;
  std::vector<double> xtlb,xtub;
  //std::vector<int> nsolutionsbyswarm;
  bool hasclusteredfirsttime;
  int pmsize;
  double ** proximitymatrix;

public:
 std::vector<int> nsolutionsbyswarm;
 int ** proximitymatrixindex;
int nsolutions;
  bool updatematrix;
  std::vector<double> EPS;
  std::vector<Particle> solutions;
  std::vector<Particle> allsolutions;
  void init(int _ndimensions, int _nobjectives, int _nclusters,int _maxsolutions, int _maxbests, int nconstr);
  EFILE();
  int nClusteredSolutions(int _whichcluster);
  int selectClusteredSolution(int _whichcluster,int _whichsolution);
  //  void add(vector<Particle> &_sol);
  //void add(Particle _s);
  bool add(Particle &_s,int _whichcluster=-1);
  bool addAll(Particle &_s);
  void clear();
  void update();
  bool reAdd();
  void finalSolutions();
  void deleteExcedent();
  int falseReAdd();
  void printSolutions();
  int  domine(Particle &_a, Particle &_b);
  int  dominePBest(Particle &_a);
  int  domine1(Particle&_a, Particle&_b);
  int  domine1PBest(Particle &_a);
  int  domine2(Particle&_a, Particle&_b);
  int  domine2PBest(Particle&_a);
  double  euclideanDistance(std::vector<double> &_a,std::vector<double> &_b);
  void normalizeDistances1();
  void normalizeDistances2();

  int deleteFromCluster(int _whichsolution);
  void add2Cluster(int _whichcluster);
  int substractFromTo(int _from, int _to, int _quantity);
  int updateClusters(std::vector<int> &_whichparticles);

  void normalizeDistances();
  void hierarchicalClustering();
  void createProximityMatrix();
  void copy2Column(int _i);
  void copy2Row(int _i);
  void findLowestValue(int *_i,int *_j);
  void deleteRow(int _j);
  void deleteColumn(int _i);
  void joinColumns(int _i,int _j);

  int selectRandomSolution(int _whichcluster);
  int selectClusteredRandomSolution(int _whichcluster);
  double rnd(double _min,double _max);
  int nSolutions();

  double EFILE_MAX;
  double EFILE_MIN;
};
#endif // EFILE_H
