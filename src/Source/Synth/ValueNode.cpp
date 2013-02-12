/*
  ==============================================================================

    ValueNode.cpp
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#include "ValueNode.h"

ValueNode::ValueNode(double* v, bool t, bool f) {
    value = v;
    isTime = t;
    isFreq = f;
}

ValueNode::~ValueNode() {
    delete value;
}

ValueNode* ValueNode::getCopy() {
    return new ValueNode(value, isTime, isFreq);
}

double ValueNode::evaluate(double* t, float* f) {
    if (isTime) {
        return *t;
    }
    else if (isFreq) {
        return *f;
    }
    else {
        return *value;
    }
}

std::string ValueNode::toString() {
    char buffer[10];
    if (isTime) {
        snprintf(buffer, 10, "%s", "time");
    }
    else if (isFreq) {
        snprintf(buffer, 10, "%s", "freq");
    }
    else {
        snprintf(buffer, 10, "%lf", *value);
    }
    return std::string(buffer);
}
