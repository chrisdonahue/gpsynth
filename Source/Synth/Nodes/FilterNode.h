/*
  ==============================================================================

    FilterNode.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef FILTERNODE_H
#define FILTERNODE_H

#include "../GPNode.h"

class FilterNode: public GPNode {
    public:
        FilterNode(bool terminal, double sr, GPMutatableParam* del, GPMutatableParam* atk, GPMutatableParam* atkh, GPMutatableParam* dec, GPMutatableParam* sus, GPMutatableParam* sush, GPMutatableParam* rel, GPNode* signal);
        ~FilterNode();
        FilterNode* getCopy();

        void evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer);
        void toString(std::stringstream& ss);
        void fillFromParams();
        void updateMutatedParams();

    private:
        // difference equation zeroes
        unsigned numXCoefficients;
        std::vector<GPMutatableParam*> xcoefficients;
        float* x;

        // difference equation poles
        unsigned numYCoefficients;
        std::vector<GPMutatableParam*> ycoefficients;
        float* y;
};

#endif
