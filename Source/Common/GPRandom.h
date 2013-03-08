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
        GPRandom(unsigned s);

        void normalizeDistribution(std::vector<double>* weights);
        int sampleFromDistribution(std::vector<double>* weights);
        double random();
        int random(int m);

    private:
        unsigned seed;
        std::mt19937 engine;
        std::uniform_real_distribution<double> uni_real;
        std::uniform_real_distribution<double> noise;
};

#endif
