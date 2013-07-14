/*
  ==============================================================================

    AMNode.cpp
    Author:  cdonahue

  ==============================================================================
*/

#include "AMNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

AMNode::AMNode(GPMutatableParam* vn, GPMutatableParam* p, GPMutatableParam* o, GPMutatableParam* a, GPNode* mod) {
    assert(vn->isUnmutatable() && vn->isDiscrete());
    variableNum = vn->getDValue();

    mutatableParams.push_back(vn);
    mutatableParams.push_back(p);
    mutatableParams.push_back(o);
    mutatableParams.push_back(a);

    descendants.push_back(mod);
    arity = 1;
}

AMNode::~AMNode() {
}

/*
    =========
    OVERRIDES
    =========
*/

AMNode* AMNode::getCopy() {
    return new AMNode(mutatableParams[0]->getCopy(), mutatableParams[1]->getCopy(), mutatableParams[2]->getCopy(), mutatableParams[3]->getCopy(), descendants[0] == NULL ? NULL : descendants[0]->getCopy());
}

void AMNode::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {
    descendants[0]->evaluateBlockPerformance(firstFrameNumber, numSamples, sampleTimes, numConstantVariables, constantVariables, buffer);
    for (unsigned i = 0; i < numSamples; i++) {
        buffer[i] = (offset + alpha * buffer[i]) * sin (w * (sampleTimes[i]) * (constantVariables[variableNum]));
    }
}

void AMNode::updateMutatedParams() {
    GPNode::updateMutatedParams();

	// update angular frequency constant
    partial = mutatableParams[1]->getValue();
    w = 2.0 * M_PI * partial;
	
	// update AM params
    offset = mutatableParams[2]->getCValue();
    alpha = mutatableParams[3]->getCValue();
    
    // minimum/maximum constant and declared in constructor
    intervalMultiply(&minimum, &maximum, descendants[0]->minimum, descendants[0]->maximum, alpha, alpha);
    intervalAdd(&minimum, &maximum, minimum, maximum, offset, offset);
}

void AMNode::toString(std::stringstream& ss) {
    ss << "(am "; 
    mutatableParams[0]->toString(ss);
    ss << " ";
    mutatableParams[1]->toString(ss);
    ss << " ";
    mutatableParams[2]->toString(ss);
    ss << " ";
    mutatableParams[3]->toString(ss);
    ss << " ";
    descendants[0]->toString(ss);
    ss << ")";
}
