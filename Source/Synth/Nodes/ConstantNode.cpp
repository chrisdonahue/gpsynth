/*
  ==============================================================================

    ConstantNode.cpp
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#include "ConstantNode.h"

ConstantNode::ConstantNode(GPMutatableParam* v, double min, double max) {
    value = v->getValue();
    mutatableParams.push_back(v);

    minimum = min;
    maximum = max;
}

ConstantNode::~ConstantNode() {
}

ConstantNode* ConstantNode::getCopy() {
    return new ConstantNode(mutatableParams[0]->getCopy());
}

void ConstantNode::evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer) {
    *min = minimum;
    *max = maximum;
	for (int i = 0; i < n; i++) {
        buffer[i] = value;
    }
}

std::string ConstantNode::toString() {
    char buffer[10];
    if (value == M_PI) {
        snprintf(buffer, 10, "(pi)");
    }
    else {
        snprintf(buffer, 10, "(%.2lf)", value);
    }
    return std::string(buffer);
}

void ConstantNode::updateMutatedParams() {
    value = mutatableParams[0]->getValue();
}
