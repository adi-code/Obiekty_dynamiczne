#ifndef RANDOM_NUMBER_GENERATOR_H
#define RANDOM_NUMBER_GENERATOR_H 1

class RandomNumberGenerator {
  public:
    RandomNumberGenerator();
    RandomNumberGenerator(const RandomNumberGenerator&);
    RandomNumberGenerator(double);
    const RandomNumberGenerator & operator=(const RandomNumberGenerator&);
    void setSeed(double);
    double getSeed() const;
    int fetchRandomInteger(int, int);
    double fetchRandomReal(double, double);
    ~RandomNumberGenerator();

  private:
    void copy(const RandomNumberGenerator&);
    void randomize();
    void warmupRandom();
    void advanceRandom();
    double randomperc();

  private:
    double randomSeed;
    double oldRand[55];
    int jrand;

};

#endif
