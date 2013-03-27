/*
  ==============================================================================

    NoiseNode.cpp
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#include "NoiseNode.h"

/*
    ==============
    PUBLIC METHODS
    ==============
*/

NoiseNode::NoiseNode(GPRandom* r) {
    rng = r;
}

NoiseNode::~NoiseNode() {
}

NoiseNode* NoiseNode::getCopy() {
    return new NoiseNode(rng);
}

void NoiseNode::evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer) {
    *min = -1;
    *max = 1;
    for (int i = 0; i < n; i++) {
        buffer[i] = rng->gauss();
    }
}

std::string NoiseNode::toString() {
    char buffer[20];
    snprintf(buffer, 20, "(noise)");
    return std::string(buffer);
}

void NoiseNode::updateMutatedParams() {
}