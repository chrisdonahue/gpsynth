/*
  ==============================================================================

    VariableNode.cpp
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#include "VariableNode.h"

VariableNode::VariableNode(int vn) {
    left = NULL;
    right = NULL;
    parent = NULL;

    variableNum = vn;

    isBinary = false;
    isTerminal = true;
}

VariableNode::~VariableNode() {
}

VariableNode* VariableNode::getCopy() {
    return new VariableNode(variableNum);
}

double VariableNode::evaluate(double* t, double* v) {
    return v[variableNum];
}

std::string VariableNode::toString() {
    char buffer[10];
    snprintf(buffer, 10, "(v%d)", variableNum);
    return std::string(buffer);
}

void VariableNode::mutate(GPParams* p) {
        variableNum = (int) (p->rng->random() * p->numVariables);
}
