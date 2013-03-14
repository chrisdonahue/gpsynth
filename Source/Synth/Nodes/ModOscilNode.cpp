/*
  ==============================================================================

    ModOscilNode.cpp
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#include "ModOscilNode.h"

/*
    ==============
    PUBLIC METHODS
    ==============
*/

ModOscilNode::ModOscilNode(GPNode* l, GPNode* r) {
    left = l;
    right = r;
    parent = NULL;
    w = 2.0 * M_PI;
    isBinary = true;
    isTerminal = false;
}

ModOscilNode::~ModOscilNode() {
    delete left;
    delete right;
}

ModOscilNode* ModOscilNode::getCopy() {
    return new ModOscilNode(left == NULL ? NULL : left->getCopy(), right == NULL ? NULL : right->getCopy());
}

double ModOscilNode::evaluate(double* t, double* v) {
    return sin(w*(*t)*(left->evaluate(t, v)) + right->evaluate(t, v));
}

void ModOscilNode::evaluateBlock(double* t, double** v, unsigned n, float* buffer) {
    float* rightBlock = (float*) malloc(sizeof(float) * n);
    left->evaluateBlock(t, v, n, buffer);
    right->evaluateBlock(t, v, n, rightBlock);
    for (int i = 0; i < n; i++) {
        buffer[i] = sin(w * (t[i]) * (buffer[i]) + rightBlock[i]);
    }
    free(rightBlock);
    return;
}

std::string ModOscilNode::toString() {
    char buffer[2048];
    snprintf(buffer, 30, "(modosc %s %s)", left->toString().c_str(), right->toString().c_str());
    return std::string(buffer);
}

void ModOscilNode::mutate(GPParams* p) {
    if (p->rng->random() < p->mutateRight) {
        right->mutate(p);
    }
    else {
        left->mutate(p);
    }
}
