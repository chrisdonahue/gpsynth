/*
  ==============================================================================

    ValueNode.cpp
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#include "ValueNode.h"

ValueNode::ValueNode(double v, int vn) {
    value = v;
    isTime = false;
    isVariable = false;
    isConstant = false;
    if (vn == -1)
        isConstant = true;
    if (vn == 0)
        isTime = true;
    else
        isVariable = true;
    variableNum = vn - 1;
}

ValueNode::~ValueNode() {
}

ValueNode* ValueNode::getCopy() {
    return new ValueNode(value, variableNum + 1);
}

double ValueNode::evaluate(double* t, double* v) {
    if (isConstant) {
        return value;
    }
    else if (isTime) {
        return *t;
    }
    else if (isVariable) {
        return v[variableNum];
    }
    else {
        std::cerr << "ValueNode has no method for evaluation." << std::endl;
    }
    return -1;
}

std::string ValueNode::toString() {
    char buffer[10];
    if (isConstant) {
        snprintf(buffer, 10, "%lf", value);
    }
    else if (isTime) {
        snprintf(buffer, 10, "%s", "time");
    }
    else if (isVariable) {
        snprintf(buffer, 10, "Variable: %d", variableNum);
    }
    else {
        std::cerr << "ValueNode has no method for evaluation." << std::endl;
    }
    return std::string(buffer);
}

void ValueNode::mutate(GPNodeParams* e) {
    if (isConstant) {
        return;
    }
    else if (isTime) {
        return;
    }
    else if (isVariable) {
        return;
    }
    else {
        std::cerr << "ValueNode has no method for evaluation." << std::endl;
    }
}
