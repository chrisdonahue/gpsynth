#include "ConstantNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

ConstantNode::ConstantNode(bool pi, GPMutatableParam* v) {
    isPi = pi;
    
    if (!isPi) {
        mutatableParams.push_back(v);
    }

    arity = 0;

    value = 0.0;

    symbol = "const";
}

ConstantNode::~ConstantNode() {
}

/*
    =========
    OVERRIDES
    =========
*/

ConstantNode* ConstantNode::getCopy() {
    return new ConstantNode(isPi, isPi ? NULL : mutatableParams[0]->getCopy());
}

void ConstantNode::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {
	// use unused variables
	firstFrameNumber;
	sampleTimes;
	numConstantVariables;
	constantVariables;

    // fill buffer with value
    for (unsigned i = 0; i < numSamples; i++) {
        buffer[i] = value;
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
        minimum = M_PI - 1;
        maximum = M_PI + 1;
    }
    else {
        value = mutatableParams[0]->getValue();
        minimum = mutatableParams[0]->getMin();
        maximum = mutatableParams[0]->getMax();
    }
}
