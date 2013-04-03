/*
  ==============================================================================

    TimeNode.cpp
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#include "TimeNode.h"

TimeNode::TimeNode() {
}

TimeNode::~TimeNode() {
}

TimeNode* TimeNode::getCopy() {
    return new TimeNode();
}

void TimeNode::evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer) {
    *min = 0;
    for (unsigned i = 0; i < n; i++) {
        buffer[i] = t[i];
    }
}

void TimeNode::toString(bool printRange, std::stringstream& ss) {
    ss << "(time)";
}

void TimeNode::updateMutatedParams() {
}
