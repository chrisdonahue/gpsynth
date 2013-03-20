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

double FunctionNode::evaluate(double* t, double* v) {
    if (arity == 2) {
        return gpfunction.function(descendants[0]->evaluate(t, v), descendants[1]->evaluate(t, v));
    }
    else {
        return gpfunction.function(descendants[0]->evaluate(t, v), 0.0);
    } 
}

void FunctionNode::evaluateBlock(double* t, unsigned nv, double* v, unsigned n, float* buffer) {
    descendants[0]->evaluateBlock(t, nv, v, n, buffer);
    if (arity == 2) {
        float* oneBlock = (float*) malloc(sizeof(float) * n);
        descendants[1]->evaluateBlock(t, nv, v, n, oneBlock);
        for (int i = 0; i < n; i++) {
            buffer[i] = gpfunction.function(buffer[i], oneBlock[i]);
        }
        free(oneBlock);
    }
    else {
        for (int i = 0; i < n; i++) {
            buffer[i] = gpfunction.function(buffer[i], 0.0);
        }
    }
}

std::string FunctionNode::toString() {
    char buffer[2048];
    if (arity == 2) {
        snprintf(buffer, 2048, "(%s %s %s)", gpfunction.symbol, descendants[0]->toString().c_str(), descendants[1]->toString().c_str());
    }
    else {
        snprintf(buffer, 2048, "(%s %s)", gpfunction.symbol, descendants[0]->toString().c_str());
    }
    return std::string(buffer);
}

void FunctionNode::updateMutatedParams() {
    descendants[0]->updateMutatedParams();
    if (arity == 2)
        descendants[1]->updateMutatedParams();
}
