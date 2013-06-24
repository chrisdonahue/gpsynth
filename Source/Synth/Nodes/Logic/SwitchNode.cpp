/*
  ==============================================================================

    SwitchNode.cpp
    Author:  cdonahue

  ==============================================================================
*/

#include "SwitchNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

SwitchNode::SwitchNode(GPNode* mod, GPNode* sigone, GPNode* sigtwo) {
    arity = 3;
    descendants.push_back(mod);
    descendants.push_back(sigone);
    descendants.push_back(sigtwo);
}

SwitchNode::~SwitchNode() {
}

/*
    =========
    OVERRIDES
    =========
*/

SwitchNode* SwitchNode::getCopy() {
    return new SwitchNode(descendants[0] == NULL ? NULL : descendants[0]->getCopy(), descendants[1] == NULL ? NULL : descendants[1]->getCopy(), descendants[2] == NULL ? NULL : descendants[2]->getCopy());
}

void SwitchNode::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {
    descendants[0]->evaluateBlockPerformance(firstFrameNumber, numSamples, sampleTimes, numConstantVariables, constantVariables, buffer);
    descendants[1]->evaluateBlockPerformance(firstFrameNumber, numSamples, sampleTimes, numConstantVariables, constantVariables, descendantBuffers[0]);
    descendants[2]->evaluateBlockPerformance(firstFrameNumber, numSamples, sampleTimes, numConstantVariables, constantVariables, descendantBuffers[1]);

    for (unsigned i = 0; i < numSamples; i++) {
        if (buffer[i] <= 0) {
            buffer[i] = descendantBuffers[0][i];
        }
        else {
            buffer[i] = descendantBuffers[1][i];
        }
    }
}

void SwitchNode::updateMutatedParams() {
    GPNode::updateMutatedParams();

    // update min/max values from descendants
    float signalOneMin = descendants[1]->minimum;
    float signalOneMax = descendants[1]->maximum;
    float signalTwoMin = descendants[2]->minimum;
    float signalTwoMax = descendants[2]->maximum;
    minimum = signalOneMin < signalTwoMin ? signalOneMin : signalTwoMin;
    maximum = signalOneMax > signalTwoMax ? signalOneMax : signalTwoMax;
}

void SwitchNode::toString(std::stringstream& ss) {
    ss << "(switch "; 
    descendants[0]->toString(ss);
    ss << " ";
    descendants[1]->toString(ss);
    ss << " ";
    descendants[2]->toString(ss);
    ss << ")";
}
