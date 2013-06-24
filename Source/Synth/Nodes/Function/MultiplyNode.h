/*
  ==============================================================================

    MultiplyNode.h
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

	// overrides
    MultiplyNode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);

    // optional overrides
	void updateMutatedParams();

private:
};

#endif
