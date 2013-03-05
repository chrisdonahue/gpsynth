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
        VariableNode(double v, int p);
        ~VariableNode();
        VariableNode* getCopy();

        double evaluate(double* t, double* v);
        std::string toString();
        void mutate(GPParams* e);

    private:
        int variableNum;
};

#endif
