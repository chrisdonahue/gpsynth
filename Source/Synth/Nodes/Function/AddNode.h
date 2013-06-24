/*
  ==============================================================================

    AddNode.h
    Author:  cdonahue

  ==============================================================================
*/

#ifndef ADDNODE_H
#define ADDNODE_H

#include "FunctionNode.h"

class AddNode: public FunctionNode {
public:
    AddNode(GPNode* zero, GPNode* one);
    ~AddNode();

	// overrides
    AddNode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);

    // optional overrides
	void updateMutatedParams();

private:
};

#endif
