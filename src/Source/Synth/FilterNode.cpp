/*
  ==============================================================================

    FilterNode.cpp
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#include "FilterNode.h"

/*
    ==============
    PUBLIC METHODS
    ==============
*/

FilterNode::FilterNode(int x, int y, GPNode* l, GPNode* r) {
    numX = x;
    numY = y;
    numFilled = 0;
    xMem = (double*) calloc(sizeof(double) * numX);
    yMem = (double*) calloc(sizeof(double) * numY);
    left = l;
    right = r;
    parent = NULL;
}

FilterNode::~FilterNode() {
    delete left;
    delete right;
    free(xMem);
    free(yMem);
}

FilterNode* FilterNode::getCopy() {
    return new FilterNode(function, symbol, left, right);
}

void FilterNode::setMemoryConstants(int x, int y) {
    numX = x;
    numY = y;
    free(xMem);
    free(yMem);
    xMem = (double*) calloc(sizeof(double) * numX);
    yMem = (double*) calloc(sizeof(double) * numY);
}

double FilterNode::evaluate(double* t, float* f) {


    if (right != NULL) {
        return function(left->evaluate(t, f), right->evaluate(t, f));
    }
    else {
        return function(left->evaluate(t, f), 0.0);
    }
}

std::string FilterNode::toString() {
    char buffer[1024];
    if (right != NULL) {
        snprintf(buffer, 1024, "(%s %s %s)", symbol.c_str(), left->toString().c_str(), right->toString().c_str());
    }
    else {
        snprintf(buffer, 1024, "(%s %s)", symbol.c_str(), left->toString().c_str());
    }
    return std::string(buffer);
}

void traceLineage() {
    // IMPLEMENT LATER
}
