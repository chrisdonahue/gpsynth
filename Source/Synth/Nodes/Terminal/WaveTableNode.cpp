/*
  ==============================================================================

    WaveTableNode.cpp
    Author:  cdonahue

  ==============================================================================
*/

#include "WaveTableNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

WaveTableNode::WaveTableNode(GPMutatableParam* rate)
{
    mutatableParams.push_back(rate);

    arity = 0;
    minimum = -1;
    maximum = 1;   
}

WaveTableNode::~WaveTableNode() {
}

/*
    =========
    OVERRIDES
    =========
*/

WaveTableNode* WaveTableNode::getCopy() {
    return new WaveTableNode(terminalLFO, mutatableParams[0]->getCopy(), NULL);
}

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
    ss << "(lfo ";
    mutatableParams[0]->toString(ss);
    ss << ")";
}
