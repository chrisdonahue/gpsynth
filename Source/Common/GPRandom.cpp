/*
  ==============================================================================

    GPRandom.cpp
    Created: 6 Feb 2013 7:19:11pm
    Author:  cdonahue

  ==============================================================================
*/

#include "GPRandom.h"

GPRandom::GPRandom(unsigned s) {
    seed = s;
}

void GPRandom::normalizeDistribution(bool lowerIsBetter, std::vector<double>* weights) {
    if (lowerIsBetter) {
    }
    else {
        double sum = 0;
        for (std::vector<double>::iterator i = weights->begin(); i != weights->end(); i++) {
            sum += *i;
        }
        for (std::vector<double>::iterator i = weights->begin(); i != weights->end(); i++) {
            *i = (*i) / sum;
        }
    }
}

int GPRandom::sampleFromDistribution(bool lowerIsBetter, std::vector<double>* weights) {
    // sample from normalized distribution
    return -1;
}

double GPRandom::random() {
    return uni_real(engine);
}
