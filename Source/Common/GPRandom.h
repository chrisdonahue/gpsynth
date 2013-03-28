/*
  ==============================================================================

    GPRandom.h
    Created: 6 Feb 2013 7:19:11pm
    Author:  cdonahue

  ==============================================================================
*/

#ifndef GPRANDOM_H
#define GPRANDOM_H

#include <cmath>
#include <random>
#include <iostream>

class GPRandom {
    public:
        GPRandom(unsigned s);

        void normalizeDistribution(std::vector<double>* weights);
        int sampleFromDistribution(std::vector<double>* weights);
        double random();
        double whitenoise();
        double gauss();
        int random(int m);

    private:
        unsigned seed;
        std::mt19937 engine;
        std::uniform_real_distribution<double> uni_real;
        std::uniform_real_distribution<double> noise;

        // GNU GAUSS MEMBER VARIABLES
        double fac;
        double r;
        double v1;
        double v2;
        double x;
        char s;
};

#endif
