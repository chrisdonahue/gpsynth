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
    return new TimeNode();
}

void TimeNode::setRenderInfo(float sr, unsigned blockSize, float maxTime) {
    maximum = maxtime;
}

void TimeNode::prepareToPlay() {

}

void TimeNode::evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer) {
	v;
	nv;
	fn;
    *min = 0;
	*max = maximum;
    if (t[n - 1] < maximum) {
        for (unsigned i = 0; i < n; i++) {
            buffer[i] = t[i];
        }
    }
    else if (t[n - 1] > maximum) {
        unsigned i = 0;
        while (t[i] < maximum) {
            buffer[i] = t[i];
            i++;
        }
        for (; i < n; i++) {
            buffer[i] = maximum;
        }
    }
}

void TimeNode::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {
    firstFrameNumber;
    numConstantVariables;
    constantVariables;
    for (unsigned i = 0; i < numSamples; i++) {
        buffer[i] = sampleTimes[i];
    }
}

void TimeNode::updateMutatedParams() {
}

void TimeNode::toString(bool printRange, std::stringstream& ss) {
	printRange;
    ss << "(time)";
}
