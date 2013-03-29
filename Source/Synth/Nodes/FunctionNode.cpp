/*
  ==============================================================================

    FunctionNode.cpp
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#include "FunctionNode.h"

/*
    ==============
    PUBLIC METHODS
    ==============
*/

FunctionNode::FunctionNode(GPFunction gpfun, GPNode* zero, GPNode* one) {
    gpfunction = gpfun;

    if (gpfunction.isBinary) {
        descendants.push_back(zero);
        descendants.push_back(one);
        arity = 2;
    }
    else {
        descendants.push_back(zero);
        arity = 1;
    }
}

FunctionNode::~FunctionNode() {
}

FunctionNode* FunctionNode::getCopy() {
    FunctionNode* ret = new FunctionNode(gpfunction, descendants[0] == NULL ? NULL : descendants[0]->getCopy(), descendants[1] == NULL ? NULL : descendants[1]->getCopy());
    return ret;
}

void FunctionNode::evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer) {
    double zeromin = std::numeric_limits<double>::min();
    double zeromax = std::numeric_limits<double>::max();
    descendants[0]->evaluateBlock(fn, t, nv, v, &zeromin, &zeromax, n, buffer);
    if (arity == 2) {
        double onemin = std::numeric_limits<double>::min();
        double onemax = std::numeric_limits<double>::max();
        float* oneBlock = (float*) malloc(sizeof(float) * n);
        descendants[1]->evaluateBlock(fn, t, nv, v, &onemin, &onemax, n, oneBlock);
        gpfunction.calculateRange(min, max, zeromin, zeromax, onemin, onemax);
        for (unsigned i = 0; i < n; i++) {
            buffer[i] = gpfunction.function(buffer[i], oneBlock[i]);
        }
        free(oneBlock);
    }
    else {
        gpfunction.calculateRange(min, max, zeromin, zeromax, 0.0, 0.0);
        for (unsigned i = 0; i < n; i++) {
            buffer[i] = gpfunction.function(buffer[i], 0.0);
        }
    }
}

void FunctionNode::toString(std::stringstream& ss) {
    ss << "(" << gpfunction.symbol;
    for (unsigned i = 0; i < arity; i++) {
        ss << " ";
        descendants[i]->toString(ss);
    }
    ss << ")";
}

void FunctionNode::updateMutatedParams() {
    descendants[0]->updateMutatedParams();
    if (arity == 2)
        descendants[1]->updateMutatedParams();
}
