/*
  ==============================================================================

    MixerNode.cpp
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#include "MixerNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

MixerNode::MixerNode(bool fade, GPNode* mod, GPNode* sigone, GPNode* sigtwo) {
    fadeMixer = fade;

    arity = 3;
    descendants.push_back(mod);
    descendants.push_back(sigone);
    descendants.push_back(sigtwo);
}

MixerNode::~MixerNode() {
}

/*
    =========
    OVERRIDES
    =========
*/

MixerNode* MixerNode::getCopy() {
    return new MixerNode(fadeMixer, descendants[0] == NULL ? NULL : descendants[0]->getCopy(), descendants[1] == NULL ? NULL : descendants[1]->getCopy(), descendants[2] == NULL ? NULL : descendants[2]->getCopy());
}

void MixerNode::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {
    descendants[0]->evaluateBlockPerformance(firstFrameNumber, numSamples, sampleTimes, numConstantVariables, constantVariables, buffer);
    descendants[1]->evaluateBlockPerformance(firstFrameNumber, numSamples, sampleTimes, numConstantVariables, constantVariables, descendantBuffers[0]);
    descendants[2]->evaluateBlockPerformance(firstFrameNumber, numSamples, sampleTimes, numConstantVariables, constantVariables, descendantBuffers[1]);
    if (fadeMixer) {
        for (unsigned i = 0; i < numSamples; i++) {
            float levelone;
            float leveltwo;
            buffer[i] = descendantBuffers[1][i] * levelone + descendantBuffers[2][i] * leveltwo;
        }
    }
    else {
        for (unsigned i = 0; i < numSamples; i++) {
            if (buffer[i] <= 0) {
                buffer[i] = descendantBuffers[1][i];
            }
            else {
                buffer[i] = descendantBuffers[2][i];
            }
        }
    }
}

void MixerNode::updateMutatedParams() {
    GPNode::updateMutatedParams();

    // update min/max values from descendants
    signalOneMin = descendants[1]->minimum;
    signalOneMax = descendants[1]->maximum;
    signalTwoMin = descendants[2]->minimum;
    signalTwoMax = descendants[2]->maximum;
    minimum = signalOneMin < signalTwoMin ? signalOneMin : signalTwoMin;
    maximum = signalOneMax > signalTwoMax ? signalOneMax : signalTwoMax;
}

void MixerNode::toString(std::stringstream& ss) {
    if (fadeMixer) {
        ss << "(mix ";
        descendants[0]->toString(ss);
        ss << " ";
        descendants[1]->toString(ss);
        ss << " ";
        descendants[2]->toString(ss);
        ss << ")";
    }
    else {
        ss << "(if "; 
        descendants[0]->toString(ss);
        ss << " ";
        descendants[1]->toString(ss);
        ss << " ";
        descendants[2]->toString(ss);
        ss << ")";
    }
}
