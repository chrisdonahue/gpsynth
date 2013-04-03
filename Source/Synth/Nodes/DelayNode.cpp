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

DelayNode::DelayNode(int n, GPNode* l)
delayLine(n)
{
    numFilled = 0;
    numSamples = n;

    left = l;
    isBinary = false;
    isTerminal = false;
}

DelayNode::~DelayNode() {
}

DelayNode* DelayNode::getCopy() {
    return new DelayNode(n, left->getCopy());
}

void DelayNode::setMemoryConstants(int n) {
    numSamples = n;
    delayLine.clear();
    delayLine.resize(n);
}

double DelayNode::evaluate(double* t, double* v) {

}

void DelayNode::evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer) {
    return;
}

void DelayNode::toString(bool printRange, std::stringstream& ss) {
    char buffer[1024];
    snprintf(buffer, 1024, "(delay %d %s)", numSamples, left->toString(std::stringstreamstd::stringstreamstd::stringstream ss ss ss).c_str());
}
