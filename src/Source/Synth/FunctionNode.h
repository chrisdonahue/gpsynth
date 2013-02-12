/*
  ==============================================================================

    FunctionNode.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef __FUNCTIONNODE_H_444F39A9__
#define __FUNCTIONNODE_H_444F39A9__

#include <stdio.h>
#include "GPNode.h"

class FunctionNode: public GPNode {
    public:
        FunctionNode(double (*fun)(double, double), std::string sym, GPNode* l, GPNode* r);
        ~FunctionNode();
        FunctionNode* getCopy();

        void setFunction(double (*fun)(double, double), std::string sym, GPNode* rSub);
        double evaluate(double* t, float* f);
        std::string toString();
        void traceLineage();

    private:
        double (*function)(double, double);
        std::string symbol;
};

#endif  // __FUNCTIONNODE_H_444F39A9__
