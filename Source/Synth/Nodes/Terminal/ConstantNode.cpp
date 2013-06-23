/*
  ==============================================================================

    ConstantNode.cpp
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#include "ConstantNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

ConstantNode::ConstantNode(bool terminal, bool pi, GPMutatableParam* v, GPNode* signal) {
	terminalConstant = terminal;
    isPi = pi;
    
    if (!isPi) {
        mutatableParams.push_back(v);
    }

	if (terminalConstant) {
    	arity = 0;
   	}
  	else {
  		arity = 1;
  		descendants.push_back(signal);
  	}

    value = 0.0;
}

ConstantNode::~ConstantNode() {
}

/*
    =========
    OVERRIDES
    =========
*/

ConstantNode* ConstantNode::getCopy() {
	if (terminalConstant) {
    	return new ConstantNode(terminalConstant, isPi, isPi ? NULL : mutatableParams[0]->getCopy(), NULL);
    }
    else {
    	return new ConstantNode(terminalConstant, isPi, isPi ? NULL : mutatableParams[0]->getCopy(), descendants[0] == NULL ? NULL : descendants[0]->getCopy());
    }
}

void ConstantNode::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {
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

void ConstantNode::setRenderInfo(float sr, unsigned blockSize, unsigned maxNumFrames, float maxTime) {
    GPNode::setRenderInfo(sr, blockSize, maxNumFrames, maxTime);
    // TODO: maybe fill an array with value and use memcpy for terminals? will this be faster?
}

void ConstantNode::doneRendering() {
    GPNode::doneRendering();
    // TODO: free if we do above TODO
}

void ConstantNode::updateMutatedParams() {
    GPNode::updateMutatedParams();
    if (isPi) {
        value = M_PI;
        if (terminalConstant) {
            minimum = M_PI - 1;
            maximum = M_PI + 1;
        }
        else {
            descendants[0]->updateMutatedParams();
            intervalMultiply(&minimum, &maximum, M_PI - 1, M_PI + 1, descendants[0]->minimum, descendants[0]->maximum);
        }
    }
    else {
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
}

void ConstantNode::toString(std::stringstream& ss) {
    if (isPi) {
    	if (terminalConstant) {
        	ss << "(pi";
        }
        else {
			ss << "(pi* ";
        	descendants[0]->toString(ss);
		}
		ss << ")";
    }
    else {
    	if (terminalConstant) {
        	ss << "(const ";
       	}
       	else {
       		ss << "(const* ";
       	}
        mutatableParams[0]->toString(ss);
        if (!terminalConstant) {
        	ss << " ";
        	descendants[0]->toString(ss);
        }
        ss << ")";
    }
}
