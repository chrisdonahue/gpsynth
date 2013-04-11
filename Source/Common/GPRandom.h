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
#ifdef _WIN32
#include <random>
#else
#include "boost/random.hpp"
#endif

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
#ifdef _WIN32
	std::mt19937 engine;
	std::uniform_real_distribution<double> uni_real;
#else
	boost::mt19937 engine;
    boost::uniform_real<double> uni_real;
#endif

    // GNU GAUSS MEMBER VARIABLES
    double fac;
    double r;
    double v1;
    double v2;
    double x;
    char s;
};

#endif
