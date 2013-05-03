/*
  ==============================================================================

    FunctionNode.cpp
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#include "FunctionNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

FunctionNode::FunctionNode(GPFunction gpfun, GPNode* zero, GPNode* one) {
    gpfunction = gpfun;

    if (gpfunction.isBinary) {
        arity = 2;
        descendants.push_back(zero);
        descendants.push_back(one);
    }
    else {
        arity = 1;
        descendants.push_back(zero);
    }
}

FunctionNode::~FunctionNode() {
}

/*
    =========
    OVERRIDES
    =========
*/

FunctionNode* FunctionNode::getCopy() {
    FunctionNode* ret = new FunctionNode(gpfunction, descendants[0] == NULL ? NULL : descendants[0]->getCopy(), descendants[1] == NULL ? NULL : descendants[1]->getCopy());
    return ret;
}

void FunctionNode::setRenderInfo(float sr, unsigned blockSize, float maxTime) {

}

void FunctionNode::prepareToPlay() {

}

void FunctionNode::evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer) {
    double zeromin = std::numeric_limits<double>::min();
    double zeromax = std::numeric_limits<double>::max();
    descendants[0]->evaluateBlock(fn, t, nv, v, &zeromin, &zeromax, n, buffer);
    if (arity == 2) {
        double onemin = std::numeric_limits<double>::min();
        double onemax = std::numeric_limits<double>::max();
        float* oneBlock = (float*) malloc(sizeof(float) * n);
        descendants[1]->evaluateBlock(fn, t, nv, v, &onemin, &onemax, n, oneBlock);
        operateInterval(zeromin, zeromax, onemin, onemax, min, max);
        for (unsigned i = 0; i < n; i++) {
            buffer[i] = operate(buffer[i], oneBlock[i]);
        }
        free(oneBlock);
    }
    else {
        operateInterval(min, max, zeromin, zeromax, 0.0, 0.0);
        for (unsigned i = 0; i < n; i++) {
            buffer[i] = operate(buffer[i], 0.0);
        }
    }
}

void FunctionNode::toString(bool printRange, std::stringstream& ss) {
    ss << "(" << symbol;
    for (unsigned i = 0; i < arity; i++) {
        ss << " ";
        descendants[i]->toString(printRange, ss);
    }
    ss << ")";
}

void FunctionNode::updateMutatedParams() {
    descendants[0]->updateMutatedParams();
    if (arity == 2)
        descendants[1]->updateMutatedParams();
}
