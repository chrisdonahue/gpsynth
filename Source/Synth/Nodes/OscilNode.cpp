/*
  ==============================================================================

    OscilNode.cpp
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#include "OscilNode.h"

/*
    ==============
    PUBLIC METHODS
    ==============
*/

OscilNode::OscilNode(int p, int vn, GPNode* l, GPNode* r) {
    partial = p;
    variableNum = vn - 1;
    left = l;
    right = r;
    parent = NULL;
    w = 2.0 * partial * M_PI;
    isBinary = false;
    isTerminal = true;
}

OscilNode::~OscilNode() {
    delete left;
    delete right;
}

OscilNode* OscilNode::getCopy() {
    return new OscilNode(partial, variableNum + 1, left->getCopy(), right->getCopy());
}

double OscilNode::evaluate(double* t, double* v) {
    return sin(w*(*t)*(v[variableNum]));
}

std::string OscilNode::toString() {
    char buffer[30];
    snprintf(buffer, 30, "(SIN WAVE PARTIAL: %d)", partial);
    return std::string(buffer);
}

void OscilNode::mutate(GPNodeParams* e) {

}
