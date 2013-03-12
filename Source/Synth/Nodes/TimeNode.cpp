/*
  ==============================================================================

    TimeNode.cpp
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#include "TimeNode.h"

TimeNode::TimeNode() {
    left = NULL;
    right = NULL;
    parent = NULL;

    isBinary = false;
    isTerminal = true;
}

TimeNode::~TimeNode() {
}

TimeNode* TimeNode::getCopy() {
    return new TimeNode();
}

double TimeNode::evaluate(double* t, double* v) {
    return *t;
}

void TimeNode::evaluateBlock(double* t, double** v, unsigned n, float* buffer) {
    for (int i = 0; i < n; i++) {
        buffer[i] = t[i];
    }
}

std::string TimeNode::toString() {
    char buffer[10];
    snprintf(buffer, 10, "(%s)", "time");
    return std::string(buffer);
}

void TimeNode::mutate(GPParams* p) {
    return;
}
