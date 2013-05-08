/*
  ==============================================================================

    SilenceNode.cpp
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#include "SilenceNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

SilenceNode::SilenceNode() {
    arity = 0;
}

SilenceNode::~SilenceNode() {
}

/*
    =========
    OVERRIDES
    =========
*/

SilenceNode* SilenceNode::getCopy() {
    return new SilenceNode();
}

void SilenceNode::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {
	// use unused variables
	firstFrameNumber;
	sampleTimes;
	numConstantVariables;
	constantVariables;

    for (unsigned i = 0; i < numSamples; i++) {
        buffer[i] = 0.0;
    }
}

void SilenceNode::toString(std::stringstream& ss) {
    ss << "(silence)";
}
