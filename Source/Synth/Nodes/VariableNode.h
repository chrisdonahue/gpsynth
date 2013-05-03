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
	void setRenderInfo(float sr, unsigned blockSize, float maxTime);
	void prepareToPlay();
    void evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer);
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
	void updateMutatedParams();
    void toString(bool printRange, std::stringstream& ss);

private:
    int variableNum;
};

#endif
