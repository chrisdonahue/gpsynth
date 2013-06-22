/*
  ==============================================================================

    AddNode.cpp
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#include "AddNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

AddNode::AddNode(GPNode* zero, GPNode* one) {
    arity = 2;

    descendants.push_back(zero);
    descendants.push_back(one);

    symbol = "+";
}

AddNode::~AddNode() {
}

/*
    =========
    OVERRIDES
    =========
*/

AddNode* AddNode::getCopy() {
    AddNode* ret = new AddNode(descendants[0] == NULL ? NULL : descendants[0]->getCopy(), descendants[1] == NULL ? NULL : descendants[1]->getCopy());
    return ret;
}

void AddNode::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {
    descendants[0]->evaluateBlockPerformance(firstFrameNumber, numSamples, sampleTimes, numConstantVariables, constantVariables, buffer);
    descendants[1]->evaluateBlockPerformance(firstFrameNumber, numSamples, sampleTimes, numConstantVariables, constantVariables, descendantBuffers[0]);
    for (unsigned i = 0; i < numSamples; i++) {
        buffer[i] += descendantBuffers[0][i];
    }
}

void AddNode::updateMutatedParams() {
    FunctionNode::updateMutatedParams();
    minimum = descendants[0]->minimum + descendants[1]->minimum;
    maximum = descendants[0]->maximum + descendants[1]->maximum;
}
