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

FilterNode::FilterNode(int x, int y, double* xc, double* yc, GPNode* l, GPNode* r) {
    numFilled = 0;

    numX = x;
    xMem();
    xCoefficients = xc;

    numY = y;
    yMem();
    yCoefficients = yc;

    left = l;
    right = r;
    parent = NULL;
    isBinary = false;
    isTerminal = false;
}

FilterNode::~FilterNode() {
    delete left;
    delete right;
    delete xMem;
    delete yMem;
    free(xCoefficients);
    free(yCoefficients);
}

FilterNode* FilterNode::getCopy() {
    return new FilterNode(function, symbol, left->getCopy(), right->getCopy());
}

void FilterNode::setMemoryConstants(int x, int y) {
    numX = x;
    numY = y;
    xMem.clear();
    yMem.clear();
}

void FilterNode::setXCoefficient(int x, double c) {
    if (x < numX)
        xCoefficients[x] = c;
}

void FilterNode::setYCoefficient(int y, double c) {
    if (y < numY)
        yCoefficients[y] = c;
}

double FilterNode::evaluate(double* t, double* v) {
    double xn = left->evaluate();
    xMem.push_front(xn);
    numFilled++;
    double yn = 0.0;

    int i = 0;
    while (i < numFilled) {
        yn += xCoefficients[i] * xMem[i];
        if (i != numFilled - 1) {
            yn -= yCoefficients[i] * yMem[i];
        }
        i++;
    }
    yMem.push_front(yn);
    return yn;
}

std::string FilterNode::toString() {
    char buffer[100];
    snprintf(buffer, 100, "(FILTER %d %d)", numX, numY);
    return std::string(buffer);
}

void FilterNode::mutate(GPParams* e) {

}
