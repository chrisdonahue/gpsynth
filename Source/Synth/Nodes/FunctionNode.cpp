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
    FunctionNode* ret = new FunctionNode(gpfunction, left == NULL ? left : left->getCopy(), right == NULL ? right : right->getCopy());
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

std::string FunctionNode::toString() {
    char buffer[1024];
    if (isBinary) {
        snprintf(buffer, 1024, "(%s %s %s)", gpfunction.symbol, left->toString().c_str(), right->toString().c_str());
    }
    else {
        snprintf(buffer, 1024, "(%s %s)", gpfunction.symbol, left->toString().c_str());
    }
    return std::string(buffer);
}

bool FunctionNode::equals(GPNode* other) {
    return false;
}

void FunctionNode::mutate(GPParams* p) {
    return;
}
