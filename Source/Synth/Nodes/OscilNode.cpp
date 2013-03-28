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
}

OscilNode::~OscilNode() {
}

OscilNode* OscilNode::getCopy() {
    return new OscilNode(mutatableParams[0]->getCopy(), variableNum);
}

double OscilNode::evaluate(double* t, double* v) {
    return sin(w*(*t)*(v[variableNum]));
}

void OscilNode::evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer) {
    *min = -1;
    *max = 1;
    double* currentIndex = v + variableNum;
	for (unsigned i = 0; i < n; i++) {
        buffer[i] = sin(w*(t[i])*(*currentIndex));
        currentIndex += nv;
    }
}

void OscilNode::toString(std::stringstream& ss) {
  ss << "(osc p" << partial << " v" << variableNum << ")";
}

void OscilNode::updateMutatedParams() {
    partial = mutatableParams[0]->getDValue();
    w = 2.0 * partial * M_PI;
}
