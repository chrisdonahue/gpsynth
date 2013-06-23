/*
  ==============================================================================

    SineNode.cpp
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#include "SineNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

SineNode::SineNode(GPNode* zero) {
    arity = 1;

    descendants.push_back(zero);

    symbol = "sin";
}

SineNode::~SineNode() {
}

/*
    =========
    OVERRIDES
    =========
*/

SineNode* SineNode::getCopy() {
    SineNode* ret = new SineNode(descendants[0] == NULL ? NULL : descendants[0]->getCopy());
    return ret;
}

void SineNode::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {
    descendants[0]->evaluateBlockPerformance(firstFrameNumber, numSamples, sampleTimes, numConstantVariables, constantVariables, buffer);
    for (unsigned i = 0; i < numSamples; i++) {
        buffer[i] = sin(buffer[i]);
    }
}

void SineNode::updateMutatedParams() {
    FunctionNode::updateMutatedParams();
    minimum = -1.0;
    maximum = 1.0;
}
