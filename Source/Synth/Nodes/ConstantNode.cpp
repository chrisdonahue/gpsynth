/*
  ==============================================================================

    ConstantNode.cpp
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#include "ConstantNode.h"

ConstantNode::ConstantNode(GPMutatableParam* v) {
    value = v->getValue();
    mutatableParams.push_back(v);

    minimum = v->getMin();
    maximum = v->getMax();
}

ConstantNode::~ConstantNode() {
}

ConstantNode* ConstantNode::getCopy() {
    return new ConstantNode(mutatableParams[0]->getCopy());
}

void ConstantNode::evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer) {
	v;
	nv;
	t;
	fn;
    *min = minimum;
    *max = maximum;
    for (unsigned i = 0; i < n; i++) {
        buffer[i] = value;
    }
}

void ConstantNode::toString(bool printRange, std::stringstream& ss) {
    if (value == M_PI) {
        ss << "(pi)";
    }
    else {
        ss << "(";
        mutatableParams[0]->toString(printRange, ss);
        ss << ")";
    }
}

void ConstantNode::updateMutatedParams() {
    value = mutatableParams[0]->getValue();
}
