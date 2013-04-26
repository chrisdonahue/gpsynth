/*
  ==============================================================================

    ConstantNode.cpp
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#include "ConstantNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

ConstantNode::ConstantNode(GPMutatableParam* v) {
    value = v->getValue();
    mutatableParams.push_back(v);

    minimum = v->getMin();
    maximum = v->getMax();
}

ConstantNode::~ConstantNode() {
}

/*
    =========
    OVERRIDES
    =========
*/

ConstantNode* ConstantNode::getCopy() {
    return new ConstantNode(mutatableParams[0]->getCopy());
}

void ConstantNode::prepareToPlay() {
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

void ConstantNode::evaluateBlockPerformance(unsigned fn, float* t, unsigned nv, float* v, float* min, float* max, unsigned n, float* buffer) {
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

void ConstantNode::getRange(float* min, float* max) {

}

void ConstantNode::updateMutatedParams() {
    value = mutatableParams[0]->getValue();
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
