/*
  ==============================================================================

    VariableNode.cpp
    Author:  cdonahue

  ==============================================================================
*/

#include "VariableNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

VariableNode::VariableNode(GPMutatableParam* vn, GPMutatableParam* range) {
    assert(!(vn->isMutatable));
    variableNum = vn->getDValue();
    mutatableParams.push_back(vn);
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
    return new VariableNode(mutatableParams[0]->getCopy(), mutatableParams[1]->getCopy());
}

void VariableNode::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {
    firstFrameNumber;
    sampleTimes;
    numConstantVariables;

    for (unsigned i = 0; i < numSamples; i++) {
        buffer[i] = constantVariables[variableNum];
    }
}

void VariableNode::updateMutatedParams() {
    GPNode::updateMutatedParams();
    minimum = mutatableParams[1]->getCMin();
    maximum = mutatableParams[1]->getCMax();
}

void VariableNode::toString(std::stringstream& ss) {
    ss << "(var " << variableNum;
    mutatableParams[0]->toString(ss);
    ss << " ";
    mutatableParams[1]->toString(ss);
    ss << ")";
}
