#include "CosineNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

CosineNode::CosineNode(GPNode* zero) {
    arity = 1;

    descendants.push_back(zero);

    symbol = "cos";
}

CosineNode::~CosineNode() {
}

/*
    =========
    OVERRIDES
    =========
*/

CosineNode* CosineNode::getCopy() {
    CosineNode* ret = new CosineNode(descendants[0] == NULL ? NULL : descendants[0]->getCopy());
    return ret;
}

void CosineNode::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {
    descendants[0]->evaluateBlockPerformance(firstFrameNumber, numSamples, sampleTimes, numConstantVariables, constantVariables, buffer);
    for (unsigned i = 0; i < numSamples; i++) {
        buffer[i] = cos(buffer[i]);
    }
}

void CosineNode::updateMutatedParams() {
    FunctionNode::updateMutatedParams();
    minimum = -1.0;
    maximum = 1.0;
}
