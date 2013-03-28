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

    hasInterval = true;
    minimum = min;
    maximum = max;
}

ConstantNode::ConstantNode(GPMutatableParam* v) {
    value = v->getValue();
    mutatableParams.push_back(v);

    hasInterval = false;
    minimum = value;
    maximum = value;
}

ConstantNode::~ConstantNode() {
}

ConstantNode* ConstantNode::getCopy() {
  if (hasInterval)
    return new ConstantNode(mutatableParams[0]->getCopy(), minimum, maximum);
  else
    return new ConstantNode(mutatableParams[0]->getCopy());
}

void ConstantNode::evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer) {
    *min = minimum;
    *max = maximum;
	for (unsigned i = 0; i < n; i++) {
        buffer[i] = value;
    }
}

std::string ConstantNode::toString(unsigned* childStringLength) {
    char buffer[10];

    if (value == M_PI) {
        formatBuffer(10, buffer, "(pi)");
    }
    else {
        formatBuffer(10, buffer, "(%.2lf)", value);
    }
    return std::string(buffer);
}

void ConstantNode::updateMutatedParams() {
    value = mutatableParams[0]->getValue();
    if (!hasInterval) {
      minimum = value;
      maximum = value;
    }
}
