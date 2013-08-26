#include "MultiplyNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

MultiplyNode::MultiplyNode(GPNode* zero, GPNode* one) {
    arity = 2;

    descendants.push_back(zero);
    descendants.push_back(one);

    symbol = "*";
}

MultiplyNode::~MultiplyNode() {
}

/*
    =========
    OVERRIDES
    =========
*/

MultiplyNode* MultiplyNode::getCopy() {
    MultiplyNode* ret = new MultiplyNode(descendants[0] == NULL ? NULL : descendants[0]->getCopy(), descendants[1] == NULL ? NULL : descendants[1]->getCopy());
    return ret;
}

void MultiplyNode::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {
    descendants[0]->evaluateBlockPerformance(firstFrameNumber, numSamples, sampleTimes, numConstantVariables, constantVariables, buffer);
    descendants[1]->evaluateBlockPerformance(firstFrameNumber, numSamples, sampleTimes, numConstantVariables, constantVariables, descendantBuffers[0]);
    for (unsigned i = 0; i < numSamples; i++) {
        buffer[i] = buffer[i] * descendantBuffers[0][i];
    }
}

void MultiplyNode::updateMutatedParams() {
    FunctionNode::updateMutatedParams();
    intervalMultiply(&minimum, &maximum, descendants[0]->minimum, descendants[0]->maximum, descendants[1]->minimum, descendants[1]->maximum);
}
