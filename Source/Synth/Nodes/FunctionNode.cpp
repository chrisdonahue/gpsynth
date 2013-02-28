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

FunctionNode::FunctionNode(GPFunction* gpfun, GPNode* l, GPNode* r) {
    function = gpfun->function;
    symbol = *(gpfun->symbol);
    isBinary = gpfun->isBinary;
    left = l;
    right = r;
    parent = NULL;
    isTerminal = false;
}

FunctionNode::~FunctionNode() {
    delete left;
    delete right;
    delete &symbol;
}

FunctionNode* FunctionNode::getCopy() {
    FunctionNode* ret = new FunctionNode(function, symbol, left == NULL ? left : left->getCopy(), right == NULL ? right : right->getCopy());
    ret->parent = parent;
    return ret;
}

void FunctionNode::setFunction(GPFunction* fun, std::string sym, GPNode* rSub) {
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
    if (left != NULL && right != NULL) {
        return function(left->evaluate(t, v), right->evaluate(t, v));
    }
    else if (left != NULL && right == NULL) {
        if (isBinary)
            return left->evaluate(t, v);
        else
            return function(left->evaluate(t, v), -1.0);
    }
    else {
        return 0.0;
    }
}

std::string FunctionNode::toString() {
    char buffer[1024];
    if (left != NULL && right != NULL) {
        snprintf(buffer, 1024, "(%s %s %s)", symbol.c_str(), left->toString().c_str(), right->toString().c_str());
    }
    else if (left != NULL && right == NULL) {
        snprintf(buffer, 1024, "(%s %s)", symbol.c_str(), left->toString().c_str());
    }
    else {
        snprintf(buffer, 1024, "(%s)", symbol.c_str());
    }
    return std::string(buffer);
}

bool FunctionNode::equals(GPNode* other) {
    return false;
}

void FunctionNode::mutate(GPParams* p) {
    return;
}
