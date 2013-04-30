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
    FunctionNode(GPFunction gpfun, GPNode* zero, GPNode* one);
    ~FunctionNode();

	// overrides
    FunctionNode* getCopy();
	void prepareToPlay();
    void evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer);
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
	void getRangeTemp(float* min, float* max);
	void updateMutatedParams();
    void toString(bool printRange, std::stringstream& ss);

private:
    GPFunction gpfunction;
};

#endif
