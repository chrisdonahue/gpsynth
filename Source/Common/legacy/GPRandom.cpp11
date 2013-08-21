/*
  ==============================================================================

    GPRandom.cpp
    Created: 6 Feb 2013 7:19:11pm
    Author:  cdonahue

  ==============================================================================
*/

#include "GPRandom.h"

GPRandom::GPRandom(unsigned s) :
    seed(s), engine(seed), uni_real(0, 1), noise(-1, 1), s(0)
{}

void GPRandom::normalizeDistribution(std::vector<double>* weights) {
    double sum = 0;
    for (std::vector<double>::iterator i = weights->begin(); i != weights->end(); i++) {
        sum += *i;
    }
    for (std::vector<double>::iterator i = weights->begin(); i != weights->end(); i++) {
        *i = (*i) / sum;
    }
}

int GPRandom::sampleFromDistribution(std::vector<double>* weights) {
    // sample from normalized distribution
    double rand = random();
    double sum = 0;
    for (unsigned i = 0; i < weights->size(); i++) {
        sum += weights->at(i);
        if (sum > rand)
            return i;
    }
    std::cerr << "Bad distribution sample. This shouldn't happen." << std::endl;
    return -1;
}

double GPRandom::random() {
    return uni_real(engine);
}

double GPRandom::gauss() {
    /*
        Copyright Tony Kirke from the Signal Processing Using C++ (SPUC) library
        GNU GPL don't distribute with this code!
    */
    if (s == 0) {
        do {
            v1 = (2.0 * random()) - 1.0;
            v2 = (2.0 * random()) - 1.0;
            r = (v1*v1) + (v2*v2);
        } while (r >= 1.0);
        fac = sqrt(-2.0 * log(r) / r);
        x = v1 * fac;
        s = 1;
        return (v2*fac);
    }
    else {
        s = 0;
        return x;
    }
}

double GPRandom::whitenoise() {
    return noise(engine);
}

// m cannot be returned
int GPRandom::random(int m) {
    if (m <= 0) {
        return -1;
    }
    return (int) (random() * m);
}
