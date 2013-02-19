/*
  ==============================================================================

    GPRandom.cpp
    Created: 6 Feb 2013 7:19:11pm
    Author:  cdonahue

  ==============================================================================
*/

#include "GPRandom.h"

GPRandom::GPRandom(unsigned s, bool lowerbetter) :
seed(s), engine(seed), uni_real(0, 1),
lowerIsBetter(lowerbetter) {}

void GPRandom::setNormalizationSymantics(bool lowerbetter) {
    lowerIsBetter = lowerbetter;
}

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
    for (int i = 0; i < weights->size(); i++) {
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
