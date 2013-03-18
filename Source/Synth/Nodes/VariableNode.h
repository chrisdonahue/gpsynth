/*
  ==============================================================================

    VariableNode.h
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef VARIABLENODE_H
#define VARIABLENODE_H

#include "../GPNode.h"

class VariableNode: public GPNode {
    public:
        VariableNode(int vn);
        ~VariableNode();
        VariableNode* getCopy();

        double evaluate(double* t, double* v);
        void evaluateBlock(double* t, unsigned nv, double* v, unsigned n, float* buffer);
        std::string toString();
        void updatedMutatedParams();

    private:
        int variableNum;
};

#endif
