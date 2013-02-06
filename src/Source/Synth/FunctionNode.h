/*
  ==============================================================================

    FunctionNode.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef __FUNCTIONNODE_H_444F39A9__
#define __FUNCTIONNODE_H_444F39A9__

#include "GPNode.h"

class FunctionNode: public GPNode {
public:
    FunctionNode(void (*fun)(GPNode&, GPNode&), std::string sym, GPNode& l, GPNode& r);
    ~FunctionNode();
    FunctionNode& getCopy();

    void setFunction(void (*fun)(GPNode&, GPNode&), std::string sym, GPNode& rSub);
    float evaluate();
    std::string toString();
    void traceLineage();

private:
    void (*function)(GPNode&, GPNode&);
    std::string symbol;
};

#endif  // __FUNCTIONNODE_H_444F39A9__
