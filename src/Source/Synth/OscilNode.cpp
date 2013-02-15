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

OscilNode::OscilNode(int p, GPNode* l, GPNode* r) {
    partial = p;
    left = l;
    right = r;
    parent = NULL;
    w = 2.0 * partial * M_PI;
}

OscilNode::~OscilNode() {
    delete left;
    delete right;
}

OscilNode* OscilNode::getCopy() {
    return new OscilNode(partial, left->getCopy(), right->getCopy());
}

double OscilNode::evaluate(double* t, float* f) {
    return sin(w*(*t)*(*f));
}

std::string OscilNode::toString() {
    char buffer[30];
    snprintf(buffer, 30, "(SIN WAVE PARTIAL: %d)", partial);
    return std::string(buffer);
}

void OscilNode::traceLineage() {

}
