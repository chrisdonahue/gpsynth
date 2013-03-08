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

NoiseNode::NoiseNode(GPParams* p) {
    bufferSize = p->noiseNodeMaxBufferSize;
    currentBufferIndex = 0;
    buffer = (double*) malloc(sizeof(double) * bufferSize);

    for (int i = 0; i < bufferSize; i++) {
        buffer[i] = p->rng->noise();
    }

    left = NULL;
    right = NULL;
    parent = NULL;
    isBinary = false;
    isTerminal = true;
}

NoiseNode::~NoiseNode() {
    free(buffer);
}

NoiseNode* NoiseNode::getCopy() {
    return new NoiseNode(function, symbol, left->getCopy(), right->getCopy());
}

double NoiseNode::evaluate(double* t, double* v) {
    if (currentBufferIndex == bufferSize)
        currentBufferIndex = 0;
    return buffer[currentBufferIndex++];
}

std::string NoiseNode::toString() {
    char buffer[20];
    snprintf(buffer, 20, "(white noise)");
    return std::string(buffer);
}

void NoiseNode::mutate(GPParams* e) {
    for (int i = 0; i < bufferSize; i++) {
        buffer[i] = p->rng->whitenoise();
    }
}
