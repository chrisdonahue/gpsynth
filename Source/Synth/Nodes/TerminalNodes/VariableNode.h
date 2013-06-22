/*
  ==============================================================================

    VariableNode.h
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef VARIABLENODE_H
#define VARIABLENODE_H

#include "../GPNode.h"

class VariableNode: public GPNode {
public:
    VariableNode(GPMutatableParam* vn, GPMutatableParam* range);
    ~VariableNode();

	// overrides
    VariableNode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
    void toString(std::stringstream& ss);

    // optional overrides
	void updateMutatedParams();

private:
    int variableNum;
};

#endif
