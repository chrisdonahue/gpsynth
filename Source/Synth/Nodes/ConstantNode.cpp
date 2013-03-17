/*
  ==============================================================================

    ConstantNode.cpp
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#include "ConstantNode.h"

ConstantNode::ConstantNode(GPMutatableParam* value) {
    mutatableParams.push_back(value);

    isBinary = false;
    isTerminal = true;
}

ConstantNode::~ConstantNode() {
}

ConstantNode* ConstantNode::getCopy() {
    return new ConstantNode(mutatableParams[0].getCopy());
}

double ConstantNode::evaluate(double* t, double* v) {
    return mutatableParams[0].getValue();
}

void ConstantNode::evaluateBlock(double* t, unsigned nv, double* v, unsigned n, float* buffer) {
    double value = mutatableParams[0].getValue();
	for (int i = 0; i < n; i++) {
        buffer[i] = value;
    }
}

std::string ConstantNode::toString() {
    double value = mutatableParams[0].getValue();
    char buffer[10];
    if (value == M_PI) {
        snprintf(buffer, 10, "(pi)");
    }
    else {
        snprintf(buffer, 10, "(%.2lf)", value);
    }
    return std::string(buffer);
}
