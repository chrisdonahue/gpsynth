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
#include <iostream>

class GPRandom {
    public:
        GPRandom(unsigned s, bool lowerbetter);

        void setNormalizationSymantics(bool lowerbetter);
        void normalizeDistribution(std::vector<double>* weights);
        int sampleFromDistribution(std::vector<double>* weights);
        double random();

    private:
        bool lowerIsBetter;
        unsigned seed;
        std::mt19937 engine;
        std::uniform_real_distribution<double> uni_real;
};

#endif
