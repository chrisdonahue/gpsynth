#include "PMNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

PMNode::PMNode(GPMutatableParam* vn, GPMutatableParam* p, GPMutatableParam* i, GPNode* mod) {
    assert(vn->isUnmutatable() && vn->isDiscrete());
    variableNum = vn->getDValue();

    mutatableParams.push_back(vn);
    mutatableParams.push_back(p);
    mutatableParams.push_back(i);

    descendants.push_back(mod);
    arity = 1;
    
    minimum = -1;
    maximum = 1;

    symbol = "pm";
}

PMNode::~PMNode() {
}

/*
    =========
    OVERRIDES
    =========
*/

PMNode* PMNode::getCopy() {
    return new PMNode(mutatableParams[0]->getCopy(), mutatableParams[1]->getCopy(), mutatableParams[2]->getCopy(), descendants[0] == NULL ? NULL : descendants[0]->getCopy());
}

void PMNode::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {
    descendants[0]->evaluateBlockPerformance(firstFrameNumber, numSamples, sampleTimes, numConstantVariables, constantVariables, buffer);
    for (unsigned i = 0; i < numSamples; i++) {
        // equivalent to chowning 1973 FM synthesis assuming buffer is a sine wave
        buffer[i] = sin( (w * (sampleTimes[i]) * (constantVariables[variableNum])) + (index * buffer[i]));
    }
}

void PMNode::updateMutatedParams() {
    GPNode::updateMutatedParams();

	// update angular frequency constant
    partial = mutatableParams[1]->getValue();
    w = 2.0 * M_PI * partial;
	
	// update PM params
    index = mutatableParams[2]->getCValue();
    
    // minimum/maximum constant and declared in constructor
}
