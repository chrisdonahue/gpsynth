/*
  ==============================================================================

    AddNode.h
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef CONSTANTNODE_H
#define CONSTANTNODE_H

#include "FunctionNode.h"

class AddNode: public FunctionNode {
public:
    AddNode(GPNode* signalone, GPNode* signaltwo);
    ~AddNode();

	// GPNode overrides
    AddNode* getCopy();
	void prepareToPlay();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);

    // FunctionNode overrides
    float operate(float a, float b);
    void operateInterval(double minone, double maxone, double mintwo, double maxtwo, double* lower, double* upper);

private:
};

#endif
