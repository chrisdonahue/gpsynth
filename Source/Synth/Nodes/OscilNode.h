/*
  ==============================================================================

    OscilNode.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef OSCILNODE_H
#define OSCILNODE_H

#include <cmath>
#include "../GPNode.h"

class OscilNode: public GPNode {
    public:
        OscilNode(GPMutatableParam* p, int vn);
        ~OscilNode();
        OscilNode* getCopy();

        double evaluate(double* t, double* v);
        void evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer);
        std::string toString();
        void updateMutatedParams();

    private:
        int partial;
        int variableNum;
        double w;
};

#endif
