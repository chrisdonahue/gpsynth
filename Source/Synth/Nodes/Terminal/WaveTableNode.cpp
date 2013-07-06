/*
  ==============================================================================

    WaveTableNode.cpp
    Author:  cdonahue

  ==============================================================================
*/

#include "WaveTableNode.h"

/*
    =========
    OVERRIDES
    =========
*/

void WaveTableNode::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {
    for (unsigned i = 0; i < numSamples; i++) {
        // produce a sine wave at LFO rate
        buffer[i] = sin(w * (sampleTimes[i]));
    }
}

void WaveTableNode::updateMutatedParams() {
    GPNode::updateMutatedParams();

	// update angular frequency constant
    rate = mutatableParams[0]->getValue();
    w = 2.0 * M_PI * rate;
	
    // minimum/maximum constant and declared in constructor
}

void WaveTableNode::toString(std::stringstream& ss) {
    ss << "(" << symbol;
    for (unsigned i = 0; i < mutatableParams.size(); i++) {
        ss << " ";
        mutatableParams[i]->toString();
    }
    ss << ")";
}
