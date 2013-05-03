/*
  ==============================================================================

    AddNode.cpp
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#include "AddNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

AddNode::AddNode(GPNode* zero, GPNode* one) {
    arity = 2;

    descendants.push_back(signalone);
    descendants.push_back(one);

    symbol = "+";
}

AddNode::~AddNode() {

}

/*
    =========
    OVERRIDES
    =========
*/

AddNode* AddNode::getCopy() {
	if (terminalConstant) {
    	return new AddNode(terminalConstant, mutatableParams[0]->getCopy(), NULL);
    }
    else {
    	return new AddNode(terminalConstant, mutatableParams[0]->getCopy(), descendants[0]->getCopy());
    }
}

void AddNode::prepareToPlay() {
}

void AddNode::evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer) {
	// use unused variables
	v;
	nv;
	t;
	fn;
	
	// if this is a leaf constant
	if (terminalConstant) {
		for (unsigned i = 0; i < n; i++) {
		    buffer[i] = value;
		}
	}
	// if this is an enveloping constant
	else {
		descendants[0]->evaluateBlock(fn, t, nv, v, min, max, n, buffer);
		for (unsigned i = 0; i < n; i++) {
			buffer[i] *= value;
		}
	}
	*min = minimum;
	*max = maximum;
}

void AddNode::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {
	// use unused variables
	firstFrameNumber;
	sampleTimes;
	numConstantVariables;
	constantVariables;

	// if this is a leaf constant	
	if (terminalConstant) {
		for (unsigned i = 0; i < numSamples; i++) {
		    buffer[i] = value;
		}
	}
	// if this is an enveloping constant
	else {
		descendants[0]->evaluateBlockPerformance(firstFrameNumber, numSamples, sampleTimes, numConstantVariables, constantVariables, buffer);
		for (unsigned i = 0; i < numSamples; i++) {
			buffer[i] *= value;
		}
	}
}

void AddNode::setRenderInfo(float sr, unsigned blockSize, float maxTime) {

}

void AddNode::updateMutatedParams() {
    value = mutatableParams[0]->getValue();
    if (terminalConstant) {
		minimum = mutatableParams[0]->getMin();
		maximum = mutatableParams[0]->getMax();
	}
	else {
        descendants[0]->updateMutatedParams();
        intervalMultiply(&minimum, &maximum, mutatableParams[0]->getMin(), mutatableParams[0]->getMax(), descendants[0]->minimum, descendants[0]->maximum);
	}
}
