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
#include <iostream>
#include "boost/random.hpp"

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
    const unsigned seed;
	boost::mt19937 engine;
    boost::uniform_real<double> uni_real;

    // GNU GAUSS MEMBER VARIABLES
    double fac;
    double r;
    double v1;
    double v2;
    double x;
    char s;
};

#endif
