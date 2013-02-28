/*
  ==============================================================================

    ValueNode.cpp
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#include "ValueNode.h"

ValueNode::ValueNode(double v, int vn) {
    left = NULL;
    right = NULL;
    parent = NULL;

    value = v;
    variableNum = vn - 1;

    isTime = false;
    isVariable = false;
    isConstant = false;
    if (vn == -1)
        isConstant = true;
    if (vn == 0)
        isTime = true;
    else
        isVariable = true;

    isBinary = false;
    isTerminal = true;
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
        snprintf(buffer, 10, "(%.2lf)", value);
    }
    else if (isTime) {
        snprintf(buffer, 10, "(%s)", "time");
    }
    else if (isVariable) {
        snprintf(buffer, 10, "(v%d)", variableNum);
    }
    else {
        std::cerr << "ValueNode has no method for evaluation." << std::endl;
    }
    return std::string(buffer);
}

bool ValueNode::equals(GPNode* other) {
    return false;
}

void ValueNode::mutate(GPParams* p) {
    if (isConstant) {
        value = (p->rng->random() * (p->valueNodeMaximum - p->valueNodeMinimum) + p->valueNodeMinimum);
    }
    else if (isTime) {
        return;
    }
    else if (isVariable) {
        variableNum = (int) (p->rng->random() * p->numVariables);
    }
    else {
        std::cerr << "ValueNode has no method for evaluation." << std::endl;
    }
}
