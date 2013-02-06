/*
  ==============================================================================

    ValueNode.cpp
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#include "ValueNode.h"

ValueNode::ValueNode(GPInfo* i, float *val) {
    info = i;
    value = val;
}

ValueNode::~ValueNode() {
    delete value;
}

ValueNode* ValueNode::getCopy() {
    return new ValueNode(info, value);
}

float ValueNode::evaluate() {
    return *value;
}

std::string ValueNode::toString() {
    char buffer[10];
    if (value == info->time) {
        sprintf(buffer, "%s", "time");
    }
    else if (value == info->cps) {
        sprintf(buffer, "%s", "f");
    }
    else {
        sprintf(buffer, "%f", *value);
    }
    return std::string(buffer);
}
