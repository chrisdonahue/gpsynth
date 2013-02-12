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
    symbol = sym;
}

double FunctionNode::evaluate(double* t, float* f) {
    if (right != NULL) {
        return function(left->evaluate(t, f), right->evaluate(t, f));
    }
    else {
        return function(left->evaluate(t, f), 0.0);
    }
}

std::string FunctionNode::toString() {
    char buffer[1024];
    if (right != NULL) {
        sprintf(buffer, "(%s %s %s)", symbol.c_str(), left->toString().c_str(), right->toString().c_str());
    }
    else {
        sprintf(buffer, "(%s %s)", symbol.c_str(), left->toString().c_str());
    }
    return std::string(buffer);
}

void traceLineage() {
    // IMPLEMENT LATER
}
