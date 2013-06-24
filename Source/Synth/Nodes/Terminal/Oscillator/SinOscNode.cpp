/*
  ==============================================================================

    SinOscNode.cpp
    Author:  cdonahue

  ==============================================================================
*/

#include "SinOscNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

SinOscNode::SinOscNode(GPMutatableParam* vn, GPMutatableParam* partial, GPMutatableParam* phase) {
    terminalOsc = terminal;
    assert(!(vn->isMutatable) && !(vn->isContinuous));
    variableNum = vn->getDValue();

    mutatableParams.push_back(vn);
    mutatableParams.push_back(partial);
    mutatableParams.push_back(phase);

    arity = 0;
    
    minimum = -1;
    maximum = 1;
}

SinOscNode::~SinOscNode() {
}

/*
    =========
    OVERRIDES
    =========
*/

SinOscNode* SinOscNode::getCopy() {
    return new SinOscNode(mutatableParams[0]->getCopy(), mutatableParams[1]->getCopy());
}

void SinOscNode::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {
    for (unsigned i = 0; i < numSamples; i++) {
        // produce a sine wave at frequency *currentIndex * p
        buffer[i] = sin(( w * (sampleTimes[i]) * (constantVariables[variableNum]) ) + phase);
    }
}

void SinOscNode::updateMutatedParams() {
    GPNode::updateMutatedParams();

	// update angular frequency constant
    partial = mutatableParams[1]->getValue();
    phase = mutatableParams[2]->getValue();
    w = 2.0 * M_PI * partial;
	
    // minimum/maximum constant and declared in constructor
}

void SinOscNode::toString(std::stringstream& ss) {
    ss << "(sinosc ";
    mutatableParams[0]->toString(ss);
    ss << " ";
    mutatableParams[1]->toString(ss);
    ss << " ";
    mutatableParams[2]->toString(ss);
    ss << ")";
}
