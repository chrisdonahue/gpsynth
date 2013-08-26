#include "LFOTerminalNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

LFOTerminalNode::LFOTerminalNode(GPMutatableParam* rate)
{
    mutatableParams.push_back(rate);

    arity = 0;
    minimum = -1;
    maximum = 1;
    
    symbol = "lfo";
}

LFOTerminalNode::~LFOTerminalNode() {
}

/*
    =========
    OVERRIDES
    =========
*/

LFOTerminalNode* LFOTerminalNode::getCopy() {
    return new LFOTerminalNode(mutatableParams[0]->getCopy());
}

void LFOTerminalNode::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {
    for (unsigned i = 0; i < numSamples; i++) {
        // produce a sine wave at LFO rate
        buffer[i] = sin(w * (sampleTimes[i]));
    }
}

void LFOTerminalNode::updateMutatedParams() {
    GPNode::updateMutatedParams();

	// update angular frequency constant
    rate = mutatableParams[0]->getValue();
    w = 2.0 * M_PI * rate;
	
    // minimum/maximum constant and declared in constructor
}
