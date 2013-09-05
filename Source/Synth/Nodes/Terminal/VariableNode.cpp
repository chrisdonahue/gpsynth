#include "VariableNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

VariableNode::VariableNode(GPMutatableParam* vn, GPMutatableParam* range) {
    assert(vn->isUnmutatable());
    variableNum = vn->getDValue();
    mutatableParams.push_back(vn);

    assert(range->isUnmutatable());
    minimum = range->getMin();
    maximum = range->getMax();
    mutatableParams.push_back(range);

    arity = 0;

    symbol = "var";
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
