/*
  ==============================================================================

    AddNode.h
    Created: 6 Feb 2013 11:06:13am
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

	// GPNode overrides
    AddNode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
	void updateMutatedParams();

private:
};

#endif
