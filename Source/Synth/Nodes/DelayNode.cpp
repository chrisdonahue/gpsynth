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
    parent = NULL;
    isBinary = false;
    isTerminal = false;
}

DelayNode::~DelayNode() {
    delete left;
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

void DelayNode::evaluateBlock(double* t, unsigned nv, double* v, unsigned n, float* buffer) {
	return;
}

std::string DelayNode::toString() {
    char buffer[1024];
    snprintf(buffer, 1024, "(delay %d %s)", numSamples, left->toString().c_str());
    return std::string(buffer);
}
