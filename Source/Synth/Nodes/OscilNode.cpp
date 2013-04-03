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

OscilNode::OscilNode(bool terminal, GPMutatableParam* p, int vn, GPMutatableParam* i, GPNode* mod) {
    terminalOscil = terminal;
    partial = p->getDValue();
    variableNum = vn;
    w = 2.0 * M_PI * partial;
    index = 0;

    mutatableParams.push_back(p);

    if (!terminalOscil) {
        index = i->getCValue();
        mutatableParams.push_back(i);
        descendants.push_back(mod);
        arity = 1;
    }
}

OscilNode::~OscilNode() {
}

OscilNode* OscilNode::getCopy() {
    if (terminalOscil)
        return new OscilNode(terminalOscil, mutatableParams[0]->getCopy(), variableNum, NULL, NULL);
    else
        return new OscilNode(terminalOscil, mutatableParams[0]->getCopy(), variableNum, mutatableParams[1]->getCopy(), descendants[0] == NULL ? NULL : descendants[0]->getCopy());
}

void OscilNode::evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer) {
    if (!terminalOscil) {
        descendants[0]->evaluateBlock(fn, t, nv, v, min, max, n, buffer);
    }
    *min = -1;
    *max = 1;
    double* currentIndex = v + variableNum;
    if (terminalOscil) {
        for (unsigned i = 0; i < n; i++) {
            // produce a sine wave at frequency *currentIndex * p
            buffer[i] = sin(w * (t[i]) * (*currentIndex));
            currentIndex += nv;
        }
    }
    else {
        for (unsigned i = 0; i < n; i++) {
            // equivalent to chowning 1973 FM synthesis assuming buffer is a sine wave
            buffer[i] = sin( (w * (t[i]) * (*currentIndex)) + (index * buffer[i]));
        }
    }
}

void OscilNode::toString(bool printRange, std::stringstream& ss) {
    if (terminalOscil)
        ss << "(osc p";
        mutatableParams[0]->toString(printRange, ss);
        ss << " v" << variableNum << ")";
    else {
        ss << "(fm p"; 
        mutatableParams[0]->toString(printRange, ss);
        ss << " v" << variableNum << " ";
        mutatableParams[1]->toString(printRange, ss);
        ss << " ";
        descendants[0]->toString(printRange, ss);
        ss << ")";
    }
}

void OscilNode::updateMutatedParams() {
    partial = mutatableParams[0]->getDValue();
    w = 2.0 * M_PI * partial;

    if (!terminalOscil) {
        index = mutatableParams[1]->getCValue();
        descendants[0]->updateMutatedParams();
    }
}
