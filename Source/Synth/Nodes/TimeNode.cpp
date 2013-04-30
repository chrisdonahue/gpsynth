/*
  ==============================================================================

    TimeNode.cpp
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#include "TimeNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

TimeNode::TimeNode(GPMutatableParam* tmax) {
    mutatableParams.push_back(tmax);

    arity = 0;
}

TimeNode::~TimeNode() {
}

/*
    =========
    OVERRIDES
    =========
*/

TimeNode* TimeNode::getCopy() {
    return new TimeNode(mutatableParams[0]->getCopy());
}

void TimeNode::prepareToPlay() {

}

void TimeNode::evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer) {
	v;
	nv;
	fn;
	*max = maximum;
    *min = 0;
    for (unsigned i = 0; i < n; i++) {
        buffer[i] = t[i];
    }
}

void TimeNode::evaluateBlockPerformance(unsigned fn, float* t, unsigned nv, float* v, float* min, float* max, unsigned n, float* buffer) {

}

void TimeNode::getRangeTemp(float* min, float* max) {

}

void TimeNode::updateMutatedParams() {
    maximum = mutatableParams[0]->getValue();
}

void TimeNode::toString(bool printRange, std::stringstream& ss) {
	printRange;
    ss << "(time)";
}
