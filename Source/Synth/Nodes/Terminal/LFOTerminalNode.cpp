/*
  ==============================================================================

    LFONode.cpp
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#include "LFONode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

LFONode::LFONode(bool terminal, GPMutatableParam* rate, GPNode* mod)
{
	terminalLFO = terminal;

    mutatableParams.push_back(rate);

    if (terminalLFO) {
        arity = 0;
        minimum = -1;
        maximum = 1;   
    }
    else {
        descendants.push_back(mod);
        arity = 1;
    }
}

LFONode::~LFONode() {
}

/*
    =========
    OVERRIDES
    =========
*/

LFONode* LFONode::getCopy() {
    if (terminalLFO)
        return new LFONode(terminalLFO, mutatableParams[0]->getCopy(), NULL);
    else
        return new LFONode(terminalLFO, mutatableParams[0]->getCopy(), descendants[0] == NULL ? NULL : descendants[0]->getCopy());
}

void LFONode::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {
    if (terminalLFO) {
        for (unsigned i = 0; i < numSamples; i++) {
            // produce a sine wave at LFO rate
            buffer[i] = sin(w * (sampleTimes[i]));
        }
    }
    else {
        descendants[0]->evaluateBlockPerformance(firstFrameNumber, numSamples, sampleTimes, numConstantVariables, constantVariables, buffer);
        for (unsigned i = 0; i < numSamples; i++) {
            // produce a sine wave at LFO rate
            buffer[i] = buffer[i] * sin(w * (sampleTimes[i]));
        }
    }
}

void LFONode::updateMutatedParams() {
    GPNode::updateMutatedParams();

	// update angular frequency constant
    rate = mutatableParams[0]->getValue();
    w = 2.0 * M_PI * rate;
	
    // minimum/maximum constant and declared in constructor
    if (!terminalLFO) {
        intervalMultiply(&minimum, &maximum, -1.0f, 1.0f, descendants[0]->minimum, descendants[0]->maximum);
    }
}

void LFONode::toString(std::stringstream& ss) {
    if (terminalLFO) {
        ss << "(lfo ";
        mutatableParams[0]->toString(ss);
        ss << ")";
    }
    else {
        ss << "(lfo* "; 
        mutatableParams[0]->toString(ss);
        ss << " ";
        descendants[0]->toString(ss);
        ss << ")";
    }
}
