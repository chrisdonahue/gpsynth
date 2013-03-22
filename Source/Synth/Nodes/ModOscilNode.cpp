/*
  ==============================================================================

    ModOscilNode.cpp
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#include "ModOscilNode.h"

/*
    ==============
    PUBLIC METHODS
    ==============
*/

ModOscilNode::ModOscilNode(GPNode* freq, GPNode* phase) {
    w = 2.0 * M_PI;

    descendants.push_back(freq);
    descendants.push_back(phase);
    arity = 2;
}

ModOscilNode::~ModOscilNode() {
}

ModOscilNode* ModOscilNode::getCopy() {
    return new ModOscilNode(descendants[0] == NULL ? NULL : descendants[0]->getCopy(), descendants[1] == NULL ? NULL : descendants[1]->getCopy());
}

void ModOscilNode::evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer) {
    float* phaseBlock = (float*) malloc(sizeof(float) * n);
    double onemin;
    double onemax;
    descendants[0]->evaluateBlock(fn, t, nv, v, min, max, n, buffer);
    descendants[1]->evaluateBlock(fn, t, nv, v, &onemin, &onemax, n, phaseBlock);
    *min = -1;
    *max = 1;
    for (int i = 0; i < n; i++) {
        buffer[i] = sin(w * (t[i]) * (buffer[i]) + phaseBlock[i]);
    }
    free(phaseBlock);
    return;
}

std::string ModOscilNode::toString() {
    char buffer[2048];
    snprintf(buffer, 2048, "(modosc %s %s)", descendants[0]->toString().c_str(), descendants[1]->toString().c_str());
    return std::string(buffer);
}

void ModOscilNode::updateMutatedParams() {
    descendants[0]->updateMutatedParams();
    descendants[1]->updateMutatedParams();
}
