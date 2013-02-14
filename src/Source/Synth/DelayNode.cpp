/*
  ==============================================================================

    DelayNode.cpp
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#include "DelayNode.h"

/*
    ==============
    PUBLIC METHODS
    ==============
*/

DelayNode::DelayNode(int n, GPNode* l, GPNode* r) {
    numFilled = 0;

    numSamples = n;
    delayLine();

    left = l;
    right = r;
    parent = NULL;
}

DelayNode::~DelayNode() {
    delete left;
    delete right;
    delete delayLine;
}

DelayNode* DelayNode::getCopy() {
    return new DelayNode(n, left->getCopy(), right->getCopy());
}

void DelayNode::setMemoryConstants(int n) {
    numSamples = n;
    delayLine.clear();
}

double DelayNode::evaluate(double* t, float* f) {
}

std::string DelayNode::toString() {
    char buffer[100];
    snprintf(buffer, 100, "(DELAY %d)", numSamples);
    return std::string(buffer);
}

void traceLineage() {
    // IMPLEMENT LATER
}
