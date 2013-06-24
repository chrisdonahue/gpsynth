/*
  ==============================================================================

    TimeNode.cpp
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

void TimeNode::setRenderInfo(float sr, unsigned blockSize, unsigned maxNumFrames, float maxTime) {
    maximum = maxTime;
    GPNode::setRenderInfo(sr, blockSize, maxNumFrames, maxTime);
}

void TimeNode::doneRendering() {
    if (renderInfoSet) {
        maximum = 0.0;
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

void TimeNode::toString(std::stringstream& ss) {
    ss << "(time)";
}
