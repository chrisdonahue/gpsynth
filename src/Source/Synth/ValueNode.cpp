/*
  ==============================================================================

    ValueNode.cpp
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#include "ValueNode.h"

ValueNode::ValueNode(double* v, int vn) {
    value = v;
    if (vn == 0)
        isTime = true;
    variableNum = vn - 1;
}

ValueNode::~ValueNode() {
    delete value;
}

ValueNode* ValueNode::getCopy() {
    return new ValueNode(value, variableNum + 1);
}

double ValueNode::evaluate(double* t, double* v) {
    if (value != NULL) {
        return *value;
    }
    else if (isTime) {
        return *t;
    }
    else {
        return v[variableNum];
    }
}

std::string ValueNode::toString() {
    char buffer[10];
    if (value != NULL) {
        snprintf(buffer, 10, "%lf", *value);
    }
    else if (isTime) {
        snprintf(buffer, 10, "%s", "time");
    }
    else {
        snprintf(buffer, 10, "Variable: %d", variableNum);
    }
    return std::string(buffer);
}

void ValueNode::mutate(GPParams* e) {
    if (value != NULL) {
    
    }
    else if (isTime) {
    
    }
    else {
    
    }
}
