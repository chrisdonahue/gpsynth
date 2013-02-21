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

FunctionNode::FunctionNode(double (*fun)(double, double), std::string sym, GPNode* l, GPNode* r) {
    function = fun;
    symbol = sym;
    left = l;
    right = r;
    parent = NULL;
    if (function == sine || function == cosine)
        isBinary = false;
    else
        isBinary = true;
    isTerminal = false;
}

FunctionNode::~FunctionNode() {
    delete left;
    delete right;
    delete &symbol;
}

FunctionNode* FunctionNode::getCopy() {
    return new FunctionNode(function, symbol, left, right);
}

void FunctionNode::setFunction(double (*fun)(double, double), std::string sym, GPNode* rSub) {
    if (right == NULL) {
        right = rSub;
    }
    function = fun;
    if (fun == sine || fun == cosine)
        isBinary = false;
    else
        isBinary = true;
    symbol = sym;
}

double FunctionNode::evaluate(double* t, double* v) {
    if (right != NULL) {
        return function(left->evaluate(t, v), right->evaluate(t, v));
    }
    else {
        return function(left->evaluate(t, v), 0.0);
    }
}

std::string FunctionNode::toString() {
    char buffer[1024];
    if (right != NULL) {
        snprintf(buffer, 1024, "(%s %s %s)", symbol.c_str(), left->toString().c_str(), right->toString().c_str());
    }
    else {
        snprintf(buffer, 1024, "(%s %s)", symbol.c_str(), left->toString().c_str());
    }
    return std::string(buffer);
}

void FunctionNode::mutate(GPNodeParams* e) {
}
