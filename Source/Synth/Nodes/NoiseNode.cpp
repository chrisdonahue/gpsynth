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

NoiseNode::NoiseNode(unsigned buffersize, GPRandom* r) {
    bufferSize = buffersize;
    rng = r;
    currentBufferIndex = 0;
    noiseBuffer = (double*) malloc(sizeof(double) * bufferSize);

    for (int i = 0; i < bufferSize; i++) {
        noiseBuffer[i] = rng->whitenoise();
    }

    left = NULL;
    right = NULL;
    parent = NULL;
    isBinary = false;
    isTerminal = true;
}

NoiseNode::~NoiseNode() {
    free(noiseBuffer);
}

NoiseNode* NoiseNode::getCopy() {
    return new NoiseNode(bufferSize, rng);
}

double NoiseNode::evaluate(double* t, double* v) {
    if (currentBufferIndex == bufferSize)
        currentBufferIndex = 0;
    return noiseBuffer[currentBufferIndex++];
}

void NoiseNode::evaluateBlock(double* t, unsigned nv, double* v, unsigned n, float* buffer) {
    for (int i = 0; i < n; i++) {
        if (currentBufferIndex == bufferSize)
            currentBufferIndex = 0;
        buffer[i] = noiseBuffer[currentBufferIndex++];
    }
}

std::string NoiseNode::toString() {
    char buffer[20];
    snprintf(buffer, 20, "(white noise)");
    return std::string(buffer);
}

void NoiseNode::mutate(GPParams* e) {
    for (int i = 0; i < bufferSize; i++) {
        noiseBuffer[i] = rng->whitenoise();
    }
}
