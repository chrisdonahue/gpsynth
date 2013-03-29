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
    for (unsigned i = 0; i < n; i++) {
        buffer[i] = rng->gauss();
    }
}

void NoiseNode::toString(std::stringstream& ss) {
    ss << "(noise)";
}

void NoiseNode::updateMutatedParams() {
}
