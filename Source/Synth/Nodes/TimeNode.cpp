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

TimeNode::TimeNode() {
}

TimeNode::~TimeNode() {
}

/*
    =========
    OVERRIDES
    =========
*/

TimeNode* TimeNode::getCopy() {
    return new TimeNode();
}

void TimeNode::prepareToPlay() {

}

void TimeNode::evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer) {
	v;
	nv;
	fn;
	max;
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

}

void TimeNode::toString(bool printRange, std::stringstream& ss) {
	printRange;
    ss << "(time)";
}
