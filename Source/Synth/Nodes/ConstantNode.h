/*
  ==============================================================================

    ConstantNode.h
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef CONSTANTNODE_H
#define CONSTANTNODE_H

#include "../GPNode.h"

class ConstantNode: public GPNode {
    public:
        ConstantNode(GPMutatableParam* v);
        ~ConstantNode();
        ConstantNode* getCopy();

        void evaluateBlock(double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer);
        std::string toString();
        void updateMutatedParams();

    private:
        double value;
};

#endif
