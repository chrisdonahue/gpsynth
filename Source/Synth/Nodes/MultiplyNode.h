/*
  ==============================================================================

    MultiplyNode.h
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef MULTIPLYNODE_H
#define MULTIPLYNODE_H

#include "FunctionNode.h"

class MultiplyNode: public FunctionNode {
public:
    MultiplyNode(GPNode* zero, GPNode* one);
    ~MultiplyNode();

	// GPNode overrides
    MultiplyNode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
	void updateMutatedParams();

private:
};

#endif
