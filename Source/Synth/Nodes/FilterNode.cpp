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

FilterNode::FilterNode(unsigned m, unsigned n, GPRandom* r, bool erc, GPMutatableParam* z, GPMutatableParam* p, GPNode* signal) :
{
    // allocate zero buffer
    numXCoefficients = m;
    x = (float*) calloc(numXCoefficients, sizeof(float));

    // allocate pole buffer
    numYCoefficients = n;
    y = (float*) calloc(numYCoefficients, sizeof(float));

    // make mutatable params for x coefficients
    for (unsigned i = 0; i < numXCoefficients; i++) {
        GPMutatableParam* copy = z->getCopy();
        if (erc)
            copy->ephemeralRandom(r);
        mutatableParams.push_back(copy);
    }
    delete z;
    
    // make mutatable params for y coefficients
    for (unsigned i = 0; i < numYCoefficients; i++) {
        GPMutatableParam* copy = p->getCopy();
        if (ephemeralRandomConstants)
            copy->ephemeralRandom(rng);
        mutatableParams.push_back(copy);
    }
    delete p;

    // add signal to descendants
    descendants.push_back(signal);
}

FilterNode::FilterNode(std::vector<GPMutatableParam*>& xcoef, std::vector<GPMutatableParam*>& ycoef, GPNode* signal) {
    numXCoefficients = xcoef.size();
    xcoefficients = xcoef;
    x = (float*) calloc(numXCoefficients, sizeof(float));

    numYCoefficients = ycoef.size();
    ycoefficients = ycoef;
    y = (float*) calloc(numYCoefficients, sizeof(float));

    for (unsigned i = 0; i < xcoefficients.size(); i++) {
        mutatableParams.push_back(xcoefficients[i]);
    }

    for (unsigned i = 0; i < ycoefficients.size(); i++) {
        mutatableParams.push_back(ycoefficients[i]);
    }

    descendants.push_back(signal);
}

FilterNode::~FilterNode() {
    free(x);
    free(y);
}

FilterNode* FilterNode::getCopy() {
    std::vector<GPMutatableParam*> xcoefcopies(numXCoefficients);
    for (unsigned i = 0; i < numXCoefficients; i++) {
        xcoefcopies[i] = xcoefficients[i]->getCopy();
    }
    std::vector<GPMutatableParam*> ycoefcopies(numYCoefficients);
    for (unsigned i = 0; i < numYCoefficients; i++) {
        ycoefcopies[i] = ycoefficients[i]->getCopy();
    }
    return new FilterNode(xcoefcopies, ycoefcopies, descendants[0]->getCopy();
}

void FilterNode::evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer) {
    *min = min;
    *max = max;
}

void FilterNode::toString(std::stringstream& ss) {

}

void FilterNode::fillFromParams() {
}

void FilterNode::updateMutatedParams() {
    fillFromParams();
    
    descendants[0]->updateMutatedParams();
}
