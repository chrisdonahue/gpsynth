/*
  ==============================================================================

    VariableNode.cpp
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#include "VariableNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

VariableNode::VariableNode(int vn, GPMutatableParam* range) {
    variableNum = vn;

    mutatableParams.push_back(range);

    arity = 0;
}

VariableNode::~VariableNode() {
}

/*
    =========
    OVERRIDES
    =========
*/

VariableNode* VariableNode::getCopy() {
    return new VariableNode(variableNum, mutatableParams[0]->getCopy());
}

void VariableNode::prepareToPlay() {

}

void VariableNode::evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer) {
	fn;
	t;
    double* currentIndex = v + variableNum;
    *min = minimum;
    *max = maximum;
    for (unsigned i = 0; i < n; i++) {
        buffer[i] = *currentIndex;
        currentIndex += nv;
    }
}

void VariableNode::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {

}

void VariableNode::getRangeTemp(float* min, float* max) {

}

void VariableNode::updateMutatedParams() {
    minimum = mutatableParams[0]->getCMin();
    maximum = mutatableParams[0]->getCMax();
}

void VariableNode::toString(bool printRange, std::stringstream& ss) {
	if (printRange) {
        ss << "(v " << variableNum;
        mutatableParams[0]->toString(printRange, ss);
        ss << ")";
    }
}
