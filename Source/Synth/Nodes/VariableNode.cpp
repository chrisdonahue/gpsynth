/*
  ==============================================================================

    VariableNode.cpp
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#include "VariableNode.h"

VariableNode::VariableNode(int vn, double min, double max) {
    variableNum = vn;

    minimum = min;
    maximum = max;
}

VariableNode::~VariableNode() {
}

VariableNode* VariableNode::getCopy() {
    return new VariableNode(variableNum, minimum, maximum);
}

void VariableNode::evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer) {
    double* currentIndex = v + variableNum;
    *min = minimum;
    *max = maximum;
    for (int i = 0; i < n; i++) {
        buffer[i] = *currentIndex;
        currentIndex += nv;
    }
}

std::string VariableNode::toString() {
    char buffer[10];
    snprintf(buffer, 10, "(v%d)", variableNum);
    return std::string(buffer);
}

void VariableNode::updateMutatedParams() {
}