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

    void evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer);
    void toString(bool printRange, std::stringstream& ss);
    void updateMutatedParams();

private:
    double value;
    double minimum;
    double maximum;
};

#endif
