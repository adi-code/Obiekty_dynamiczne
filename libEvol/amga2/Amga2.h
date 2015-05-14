#ifndef AMGA2_H
#define AMGA2_H



#include "Individual.h"

#include "../libEvolOptimizer/ICMultiObjectiveOptimizer.h"

#include <vector>
#include <string>
#include <iostream>
#include <list>

class RandomNumberGenerator;

/** Archive-based Micro Genetic Algorithm 2*/
class Amga2 : public ICMultiObjectiveOptimizer {
	//friend std::ostream& operator<<(std::ostream&, const Amga2&);
    friend class Individual;

  public:
    /** Archive-based Micro Genetic Algorithm 2 constructor*/
	Amga2();
    ~Amga2();
    virtual const char* getName() { return "AMGA2";}
    virtual void run();
    void setCross(double _v);
    double getCross();
    void setPMut(double _v);
    double getPMut();
    void setDi(double _v);
    double getDi();
    void setDiM(double _v);
    double getDiM();
    void setArchiveSize(int size);
    int getArchiveSize();

  private:

    //this section was public - it was the previous interface of amga2

    void alocateMemory();
    void evolvePopulation();
	void writeParetoFile(const std::string&);
	void writeParetoObjectives(const std::string&);


  private:
	//void printPopulation();
    void assignDiversityMetric(std::vector<Individual> &population, std::list<unsigned int> &elite);
    void assignInfiniteDiversity(std::vector<Individual> &population, std::list<unsigned int> &elite);
    void createInitialPopulation();
    void createParentPopulation();
    void createOffspringPopulation();
    void crossIndividuals(const Individual &parent, const Individual& r1, const Individual &r2, const Individual &r3, Individual &offspring);
    void finalizePopulation();
    void evaluatePopulation(std::vector<Individual>& population, unsigned int size);
    void evaluateIndividual(Individual * individual);
    void normalizePopulation(std::vector<Individual>& population, unsigned int size);
    void normalizeIndividualValues(Individual * individual);
	void extractBestRank(const std::vector<Individual>& population, std::list<unsigned int>& pool, std::list<unsigned int>& elite);
    void extractDistinctIndividuals(std::vector<Individual> &population, std::list<unsigned int> &elite, std::list<unsigned int> &distinct);
    void extractENNSPopulation(const std::vector<Individual> &population, std::list<unsigned int> &pool, std::list<unsigned int> &elite, unsigned int desiredEliteSize);
    void extractExtremeSolutions(const std::vector<Individual> &population, std::list<unsigned int> &pool, std::list<unsigned int> &extreme);
    void fillBestPopulation(std::vector<Individual> &mixedPopulation, unsigned int mixedLength, std::vector<Individual> &population, unsigned int desiredBestSize);
    void fillDiversePopulation(std::vector<Individual> &mixedPopulation, std::list<unsigned int> &pool, std::vector<Individual> &population, unsigned int startLocation, unsigned int desiredSize);
    void mutateOffspringPopulation();
    void updateArchivePopulation();
    void quickSortObj(const std::vector<Individual>& population, unsigned int objCount, unsigned int* objArray, int left, int right) const;
    void quickSortDiversity(const std::vector<Individual>& population, unsigned int* diversityArray, int left, int right) const;
    void updateObjectiveConstraintRange();
   // void printObjectives(const std::vector<Individual>& population, unsigned int size, const std::string& fileName) const;
    double calcEuclideanDistance(const Individual &a, const Individual &b) const;
    int checkDomination(const Individual& a, const Individual& b) const;

  private:
    unsigned int numVar;

	unsigned int numIntVar;

	double *minIntVar;
	double *maxIntVar;


	unsigned int numObj;
    unsigned int numConstr;
	double *minVar;
	double *maxVar;
    double *minObj;
    double *maxObj;
	double *minConstr;
	double *maxConstr;
    unsigned int archiveSize;
    unsigned int initialSize;
    unsigned int populationSize;
    unsigned int eliteSize;
    unsigned int numEvaluations;
    double probCross;
    double probMut;
    double etaCross;
    double etaMut;
    unsigned int debugMode;
    unsigned int evaluationCounter;
    static const double INF;
    static const double EPS;
    RandomNumberGenerator* randGen;
    std::vector<Individual> offspringPopulation;
    std::vector<Individual> parentPopulation;
    std::vector<Individual> archivePopulation;
    std::vector<Individual> combinedPopulation;
    unsigned int currentArchiveSize;
    bool isEvolutionEnded;

    class DistanceMatrix {
      public:
        int index1;
        int index2;
        double distance;

        DistanceMatrix() : index1(-1), index2(-1), distance(0.0) {
        }

        DistanceMatrix(int m_index1, int m_index2, double m_distance) : index1(m_index1), index2(m_index2), distance(m_distance) {
        }

        DistanceMatrix(const DistanceMatrix& rhs) : index1(rhs.index1), index2(rhs.index2), distance(rhs.distance) {
        }

        ~DistanceMatrix() {
        }

        const DistanceMatrix & operator=(const DistanceMatrix& rhs) {
          if (this == &rhs) {
            return (*this);
          }
          distance = rhs.distance;
          index1 = rhs.index1;
          index2 = rhs.index2;
          return (*this);
        }

        bool operator<(const DistanceMatrix& o) const {
          if (distance < o.distance) {
            return (true);
          } else if (distance > o.distance) {
            return (false);
          } else if (index1 < o.index1) {
			return (true);
          } else if (index1 > o.index1) {
            return (false);
          } else if (index2 < o.index2) {
            return (true);
          } else if (index2 > o.index2) {
            return (false);
          } else {
            return (false);
          }
        }
    };
};

#endif
