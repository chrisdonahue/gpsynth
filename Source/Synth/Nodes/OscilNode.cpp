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

OscilNode::OscilNode(GPMutatableParam* p, int vn) {
    partial = p->getDValue();
    variableNum = vn;
    w = 2.0 * partial * M_PI;

    mutatableParams.push_back(p);

    isBinary = false;
    isTerminal = true;
}

OscilNode::~OscilNode() {
}

OscilNode* OscilNode::getCopy() {
    return new OscilNode(mutatableParams[0]->getCopy(), variableNum);
}

double OscilNode::evaluate(double* t, double* v) {
    return sin(w*(*t)*(v[variableNum]));
}

void OscilNode::evaluateBlock(double* t, unsigned nv, double* v, unsigned n, float* buffer) {
    double* currentIndex = v + variableNum;
	for (int i = 0; i < n; i++) {
        buffer[i] = sin(w*(t[i])*(*currentIndex));
        currentIndex += nv;
    }
}

std::string OscilNode::toString() {
    char buffer[30];
    snprintf(buffer, 30, "(osc p%d v%d)", partial, variableNum);
    return std::string(buffer);
}

void OscilNode::updateMutatedParams() {
    partial = mutatableParams[0]->getDValue();
}
