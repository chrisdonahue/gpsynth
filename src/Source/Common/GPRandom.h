/*
  ==============================================================================

    GPRandom.h
    Created: 6 Feb 2013 7:19:11pm
    Author:  cdonahue

  ==============================================================================
*/

#ifndef GPRANDOM_H
#define GPRANDOM_H

#include <random>

class GPRandom {
    GPRandom(unsigned seed);

    unsigned seed;
    std::mt19937_64 engine;
    std::uniform_real_distribution<double> uni_real;

    void normalizeDistribution(bool lowerIsBetter, std::vector<double>* weights);
    int sampleFromDistribution(bool lowerIsBetter, std::vector<double>* weights);
    double random();
};

#endif
