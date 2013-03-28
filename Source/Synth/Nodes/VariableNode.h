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
        VariableNode(int vn, double min, double max);
        ~VariableNode();
        VariableNode* getCopy();

        void evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer);
        std::string toString(unsigned* childStringLength);
        void updateMutatedParams();

    private:
        int variableNum;
        double minimum;
        double maximum;
};

#endif
