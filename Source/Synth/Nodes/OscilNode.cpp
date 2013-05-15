/*
  ==============================================================================

    OscilNode.cpp
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#include "OscilNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

OscilNode::OscilNode(bool terminal, GPMutatableParam* vn, GPMutatableParam* p, GPMutatableParam* i, GPNode* mod) {
    terminalOscil = terminal;
    assert(!(vn->isMutatable));
    variableNum = vn->getDValue();

    mutatableParams.push_back(vn);
    mutatableParams.push_back(p);

    if (terminalOscil) {
        arity = 0;
    }
    else {
        mutatableParams.push_back(i);
        descendants.push_back(mod);
        arity = 1;
    }
    
    minimum = -1;
    maximum = 1;   
}

OscilNode::~OscilNode() {
}

/*
    =========
    OVERRIDES
    =========
*/

OscilNode* OscilNode::getCopy() {
    if (terminalOscil)
        return new OscilNode(terminalOscil, mutatableParams[0]->getCopy(), mutatableParams[1]->getCopy(), NULL, NULL);
    else
        return new OscilNode(terminalOscil, mutatableParams[0]->getCopy(), mutatableParams[1]->getCopy(), mutatableParams[2]->getCopy(), descendants[0] == NULL ? NULL : descendants[0]->getCopy());
}

void OscilNode::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {
    if (terminalOscil) {
        for (unsigned i = 0; i < numSamples; i++) {
            // produce a sine wave at frequency *currentIndex * p
            buffer[i] = sin(w * (sampleTimes[i]) * (constantVariables[variableNum]));
        }
    }
    else {
        descendants[0]->evaluateBlockPerformance(firstFrameNumber, numSamples, sampleTimes, numConstantVariables, constantVariables, buffer);
        for (unsigned i = 0; i < numSamples; i++) {
            // equivalent to chowning 1973 FM synthesis assuming buffer is a sine wave
            buffer[i] = sin( (w * (sampleTimes[i]) * (constantVariables[variableNum])) + (index * buffer[i]));
        }
    }
}

void OscilNode::updateMutatedParams() {
    GPNode::updateMutatedParams();

	// update angular frequency constant
    partial = mutatableParams[1]->getValue();
    w = 2.0 * M_PI * partial;
	
	// update index of modulation and descendant if this is an FM oscillator
    if (!terminalOscil) {
        index = mutatableParams[2]->getCValue();
    }
    
    // minimum/maximum constant and declared in constructor
}

void OscilNode::toString(std::stringstream& ss) {
    if (terminalOscil) {
        ss << "(osc ";
        mutatableParams[0]->toString(ss);
        ss << " ";
        mutatableParams[1]->toString(ss);
        ss << ")";
    }
    else {
        ss << "(fm "; 
        mutatableParams[0]->toString(ss);
        ss << " ";
        mutatableParams[1]->toString(ss);
        ss << " ";
        mutatableParams[2]->toString(ss);
        ss << " ";
        descendants[0]->toString(ss);
        ss << ")";
    }
}
