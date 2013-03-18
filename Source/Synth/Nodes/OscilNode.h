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
        OscilNode(int p, int vn);
        ~OscilNode();
        OscilNode* getCopy();

        double evaluate(double* t, double* v);
        void evaluateBlock(double* t, unsigned nv, double* v, unsigned n, float* buffer);
        std::string toString();
        void updateMutatedParams();

    private:
        int partial;
        int variableNum;
        double w;
};

#endif
