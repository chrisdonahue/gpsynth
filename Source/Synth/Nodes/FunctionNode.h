/*
  ==============================================================================

    FunctionNode.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef FUNCTIONNODE_H
#define FUNCTIONNODE_H

#include "../GPNode.h"

class FunctionNode: public GPNode {
    public:
        FunctionNode(double (*fun)(double, double), std::string sym, GPNode* l, GPNode* r);
        ~FunctionNode();
        FunctionNode* getCopy();

        void setFunction(double (*fun)(double, double), std::string sym, GPNode* rSub);
        double evaluate(double* t, double* v);
        std::string toString();
        void mutate(GPNodeParams* e);

    private:
        double (*function)(double, double);
        std::string symbol;
};

#endif
