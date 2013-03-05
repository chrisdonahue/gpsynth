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

std::string TimeNode::toString() {
    char buffer[10];
    snprintf(buffer, 10, "(%s)", "time");
    return std::string(buffer);
}

void TimeNode::mutate(GPParams* p) {
    return;
}
