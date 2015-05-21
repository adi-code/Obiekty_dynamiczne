#include <cmath>

#include "RandomNumberGenerator.h"

using namespace std;

RandomNumberGenerator::RandomNumberGenerator() : randomSeed(0.12345), jrand(0) {
  randomize();
}

RandomNumberGenerator::RandomNumberGenerator(const RandomNumberGenerator& rhs) {
  copy(rhs);
}

RandomNumberGenerator::RandomNumberGenerator(double seed) : randomSeed(seed), jrand(0) {
  randomize();
}

RandomNumberGenerator::~RandomNumberGenerator() {

}

const RandomNumberGenerator& RandomNumberGenerator::operator=(const RandomNumberGenerator& rhs) {
  if (this == &rhs) {
    return (*this);
  }
  copy(rhs);
  return (*this);
}

void RandomNumberGenerator::setSeed(double seed) {
  randomSeed = seed;
  randomize();
  return;
}

double RandomNumberGenerator::getSeed() const {
  return (randomSeed);
}

void RandomNumberGenerator::copy(const RandomNumberGenerator& rhs) {
  randomSeed = rhs.randomSeed;
  jrand = rhs.jrand;
  for (register unsigned int i = 0; i < 55; i++) {
    oldRand[i] = rhs.oldRand[i];
  }
  return;
}

void RandomNumberGenerator::randomize() {
  for (register unsigned int j1 = 0; j1 <= 54; j1++) {
    oldRand[j1] = 0.0;
  }
  jrand = 0;
  warmupRandom();
  return;
}

void RandomNumberGenerator::warmupRandom() {
  register unsigned int index = 0;
  register double newRandom = 0.000000001;
  register double prevRandom = randomSeed;
  oldRand[54] = randomSeed;
  for (register unsigned int i = 1; i <= 54; i++) {
    index = (21 * i) % 54;
    oldRand[index] = newRandom;
    newRandom = prevRandom - newRandom;
    if (newRandom < 0.0) {
      newRandom += 1.0;
    }
    prevRandom = oldRand[index];
  }
  advanceRandom();
  advanceRandom();
  advanceRandom();
  jrand = 0;
  return;
}

void RandomNumberGenerator::advanceRandom() {
  register double newRandom = 0.0;
  for (register unsigned int i = 0; i < 24; i++) {
    newRandom = oldRand[i] - oldRand[i + 31];
    if (newRandom < 0.0) {
      newRandom += 1.0;
    }
    oldRand[i] = newRandom;
  }
  for (register unsigned int i = 24; i < 55; i++) {
    newRandom = oldRand[i] - oldRand[i - 24];
    if (newRandom < 0.0) {
      newRandom += 1.0;
    }
    oldRand[i] = newRandom;
  }
  return;
}

double RandomNumberGenerator::randomperc() {
  jrand++;
  if (jrand >= 55) {
    jrand = 1;
    advanceRandom();
  }
  return (oldRand[jrand]);
}

int RandomNumberGenerator::fetchRandomInteger(int low, int high) {
  register int res = 0;
  if (low >= high) {
    res = low;
  } else {
    res = low + (int) (randomperc()*(high - low + 1));
    if (res > high) {
      res = high;
    }
  }
  return (res);
}

double RandomNumberGenerator::fetchRandomReal(double low, double high) {
  register double res = 0.0;
  res = low + (high - low) * randomperc();
  if (low >= high) {
    res = low;
  } else if (res < low) {
    res = low;
  } else if (res > high) {
    res = high;
  }
  return (res);
}
