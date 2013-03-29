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
    xcoefficients(), ycoefficients()
{
    // allocate zero buffer
    numXCoefficients = m;
    x = (float*) calloc(numXCoefficients, sizeof(float));
    xcoefs = (float*) calloc(numXCoefficients, sizeof(float));

    // allocate pole buffer
    numYCoefficients = n;
    y = (float*) calloc(numYCoefficients, sizeof(float));
    ycoefs = (float*) calloc(numYCoefficients, sizeof(float));

    // make mutatable params for x coefficients
    for (unsigned i = 0; i < numXCoefficients; i++) {
        GPMutatableParam* copy = z->getCopy();
        if (erc)
            copy->ephemeralRandom(r);
        xcoefs[i] = copy->getCValue();
        xcoefficients.push_back(copy);
        mutatableParams.push_back(copy);
    }
    delete z;

    // make mutatable params for y coefficients
    for (unsigned i = 0; i < numYCoefficients; i++) {
        GPMutatableParam* copy = p->getCopy();
        if (ephemeralRandomConstants)
            copy->ephemeralRandom(rng);
        ycoefs[i] = copy->getCValue();
        ycoefficients.push_back(copy);
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
    free(xcoefs);
    free(x);
    free(ycoefs);
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
    descendants[0]->evaluateBlock(fn, t, nv, v, min, max, n, buffer);

    for (unsigned i = 0; i < n; i++) {
        unsigned j;
        unsigned k;
        for (j = 1; j < numXCoefficients; j++) {
            x[j] = x[j-1];
        }
        x[0] = buffer[i];
        for (k = 1; k < numYCoefficients; j++) {
            y[k] = y[k-1];
        }
        buffer[i] = 0;
        for (j = 0; j < numXCoefficients; j++) {
            buffer[i] += xcoefs[j] * x[j];
        }
        for (k = 1; k < numYCoefficients; k++) {
            buffer[i] += ycoefs[k] * y[k];
        }
        y[0] = buffer[i];
    }
    for (unsigned i = 0; i < n; i++) {
        for (unsigned j = 1;
    }
        *min = min;
*max = max;
}

void FilterNode::toString(std::stringstream& ss) {
    ss << "(iirfilter z" << numXCoefficients << ":";
    for (unsigned i = 0; i < numXCoefficients; i++) {
        ss << " " << xcoefs[i];
    }
    ss << " p" << numYCoefficients << ":";
    for (unsigned i = 0; i < numYCoefficients; i++) {
        ss << " " << ycoefs[i];
    }
    descendants[0]->toString(ss);
    ss << ")";
}

void FilterNode::updateMutatedParams() {
    for (unsigned i = 0; i < numXCoefficients; i++) {
        xcoefs[i] = xcoefficients[i]->getCValue();
    }
    for (unsigned i = 0; i < numYCoefficients; i++) {
        ycoefs[i] = ycoefficients[i]->getCValue();
    }

    descendants[0]->updateMutatedParams();
}
