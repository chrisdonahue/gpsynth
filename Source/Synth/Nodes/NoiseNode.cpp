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

    left = NULL;
    right = NULL;
    parent = NULL;

    isBinary = false;
    isTerminal = true;
}

NoiseNode::~NoiseNode() {
}

NoiseNode* NoiseNode::getCopy() {
    return new NoiseNode(rng);
}

double NoiseNode::evaluate(double* t, double* v) {
    return rng->gauss();
}

void NoiseNode::evaluateBlock(double* t, unsigned nv, double* v, unsigned n, float* buffer) {
    for (int i = 0; i < n; i++) {
        buffer[i] = rng->gauss();
    }
}

std::string NoiseNode::toString() {
    char buffer[20];
    snprintf(buffer, 20, "(noise)");
    return std::string(buffer);
}

void NoiseNode::mutate(GPParams* e) {
}
