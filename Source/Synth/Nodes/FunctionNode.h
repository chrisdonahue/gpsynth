/*
  ==============================================================================

    FunctionNode.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef FUNCTIONNODE_H
#define FUNCTIONNODE_H

#include "../../Common/GPFunctions.h"
#include "../GPNode.h"

class FunctionNode: public GPNode {
    public:
        FunctionNode(GPFunction gpfun, GPNode* l, GPNode* r);
        ~FunctionNode();
        FunctionNode* getCopy();

        void setFunction(GPFunction fun, GPNode* rSub);
        double evaluate(double* t, double* v);
        void evaluateBlock(double* t, unsigned nv, double* v, unsigned n, float* buffer);
        std::string toString();
        void updateMutatedParams();

    private:
        GPFunction gpfunction;
};

#endif
