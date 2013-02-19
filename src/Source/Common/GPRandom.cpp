/*
   =============
   PROBABILISTIC
   =============
*/

#include "GPRandom.h"

void GPRandom::normalizeDistribution(std::vector<double>* weights) {
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

int GPRandom::sampleFromDistribution(std::vector<double>* weights) {
    // sample from normalized distribution
    return -1;
}

double GPRandom::random() {
    return uni_real(engine);
}
