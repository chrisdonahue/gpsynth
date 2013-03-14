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

FunctionNode::FunctionNode(GPFunction gpfun, GPNode* l, GPNode* r) {
    gpfunction = gpfun;

    left = l;
    right = r;
    parent = NULL;

    isBinary = gpfunction.isBinary;
    isTerminal = false;
}

FunctionNode::~FunctionNode() {
    delete left;
    delete right;
}

FunctionNode* FunctionNode::getCopy() {
    FunctionNode* ret = new FunctionNode(gpfunction, left == NULL ? NULL : left->getCopy(), right == NULL ? NULL : right->getCopy());
    ret->parent = parent;
    return ret;
}

void FunctionNode::setFunction(GPFunction fun, GPNode* rSub) {
    gpfunction = fun;
    right = rSub;
    isBinary = fun.isBinary;
}

double FunctionNode::evaluate(double* t, double* v) {
    if (isBinary) {
        return gpfunction.function(left->evaluate(t, v), right->evaluate(t, v));
    }
    else {
        return gpfunction.function(left->evaluate(t, v), 0.0);
    } 
}

void FunctionNode::evaluateBlock(double* t, double** v, unsigned n, float* buffer) {
    left->evaluateBlock(t, v, n, buffer);
    if (isBinary) {
        float* rightBlock = (float*) malloc(sizeof(float) * n);
        right->evaluateBlock(t, v, n, rightBlock);
        for (int i = 0; i < n; i++) {
            buffer[i] = gpfunction.function(buffer[i], rightBlock[i]);
        }
        free(rightBlock);
    }
    else {
        for (int i = 0; i < n; i++) {
            buffer[i] = gpfunction.function(buffer[i], 0.0);
        }
    }
}

std::string FunctionNode::toString() {
    char buffer[2048];
    if (isBinary) {
        snprintf(buffer, 2048, "(%s %s %s)", gpfunction.symbol, left->toString().c_str(), right->toString().c_str());
    }
    else {
        snprintf(buffer, 2048, "(%s %s)", gpfunction.symbol, left->toString().c_str());
    }
    return std::string(buffer);
}

void FunctionNode::mutate(GPParams* p) {
    /*
    std::vector<GPFunction>* functionSet;
    if (isBinary) {
        functionSet = p->availableBinaryFunctions;
    }
    else {
        functionSet = p->availableUnaryFunctions;
    }
    gpfunction = functionSet->at(p->rng->random(p->availableBinaryFunctions->size()));
    */
    if (isBinary && p->rng->random() < p->mutateRight) {
        right->mutate(p);
    }
    else {
        left->mutate(p);
    }
}
