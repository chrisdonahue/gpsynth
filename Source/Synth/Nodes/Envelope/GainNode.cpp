/*
  ==============================================================================

    GainNode.cpp
    Author:  cdonahue

  ==============================================================================
*/

#include "GainNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

GainNode::GainNode(GPMutatableParam* v, GPNode* signal) {
    mutatableParams.push_back(v);

    arity = 1;
    descendants.push_back(signal);

    value = 0.0;
}

GainNode::~GainNode() {
}

/*
    =========
    OVERRIDES
    =========
*/

GainNode* GainNode::getCopy() {
    return new GainNode(mutatableParams[0]->getCopy(), descendants[0] == NULL ? NULL : descendants[0]->getCopy());
}

void GainNode::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {
	// use unused variables
	firstFrameNumber;
	sampleTimes;
	numConstantVariables;
	constantVariables;

    descendants[0]->evaluateBlockPerformance(firstFrameNumber, numSamples, sampleTimes, numConstantVariables, constantVariables, buffer);
    for (unsigned i = 0; i < numSamples; i++) {
        buffer[i] *= value;
    }
}

void GainNode::setRenderInfo(float sr, unsigned blockSize, unsigned maxNumFrames, float maxTime) {
    GPNode::setRenderInfo(sr, blockSize, maxNumFrames, maxTime);
    // TODO: maybe fill an array with value and use memcpy for terminals? will this be faster?
}

void GainNode::doneRendering() {
    GPNode::doneRendering();
    // TODO: free if we do above TODO
}

void GainNode::updateMutatedParams() {
    GPNode::updateMutatedParams();
    value = mutatableParams[0]->getValue();
    intervalMultiply(&minimum, &maximum, mutatableParams[0]->getMin(), mutatableParams[0]->getMax(), descendants[0]->minimum, descendants[0]->maximum);
}

void GainNode::toString(std::stringstream& ss) {
    ss << "(gain ";
    mutatableParams[0]->toString(ss);
    ss << " ";
    descendants[0]->toString(ss);
    ss << ")";
}
