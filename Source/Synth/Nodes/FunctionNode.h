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
        FunctionNode(GPFunction fun, std::string sym, GPNode* l, GPNode* r);
        ~FunctionNode();
        FunctionNode* getCopy();

        void setFunction(GPFunction fun, std::string sym, GPNode* rSub);
        double evaluate(double* t, double* v);
        std::string toString();
        bool equals(GPNode* other);
        void mutate(GPParams* e);

    private:
        GPFunction* function;
        std::string symbol;
};

#endif
