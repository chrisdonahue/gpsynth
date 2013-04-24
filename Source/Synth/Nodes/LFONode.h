/*
  ==============================================================================

    LFONode.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef LFONODE_H
#define LFONODE_H

#include "../GPNode.h"

class LFONode: public GPNode {
public:
    LFONode(bool terminal, GPMutatableParam* p, int vn, GPMutatableParam* i, GPNode* mod);
    ~LFONode();
    LFONode* getCopy();

    void evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer);
	void evaluateBlockPerformance(unsigned fn, float* t, unsigned nv, float* v, float* min, float* max, unsigned n, float* buffer);
    void toString(bool printRange, std::stringstream& ss);
    void updateMutatedParams();

private:
    double frequency;
    double w;
};

#endif
