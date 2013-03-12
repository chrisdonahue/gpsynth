/*
  ==============================================================================

    ConstantNode.cpp
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#include "ConstantNode.h"

ConstantNode::ConstantNode(double v) {
    left = NULL;
    right = NULL;
    parent = NULL;

    value = v;

    isBinary = false;
    isTerminal = true;
}

ConstantNode::~ConstantNode() {
}

ConstantNode* ConstantNode::getCopy() {
    return new ConstantNode(value);
}

double ConstantNode::evaluate(double* t, double* v) {
    return value;
}

void evaluateBlock(double* t, double** v, unsigned n, float* buffer) {
	for (int i = 0; i < n; i++) {
        buffer[i] = value;
    }
}

std::string ConstantNode::toString() {
    char buffer[10];
    if (value == M_PI) {
        snprintf(buffer, 10, "(pi)", value);
    }
    else {
        snprintf(buffer, 10, "(%.2lf)", value);
    }
    return std::string(buffer);
}

void ConstantNode::mutate(GPParams* p) {
    // TODO += instead of = then wrap values around
    value = (p->rng->random() * (p->valueNodeMaximum - p->valueNodeMinimum) + p->valueNodeMinimum);
    if (value > p->valueNodeMaximum) {
        // TODO: wrap values around
    }
    else if (value < p->valueNodeMinimum) {
        // TODO: wrap values aroud
    }
}
