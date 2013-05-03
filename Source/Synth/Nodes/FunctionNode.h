/*
  ==============================================================================

    FunctionNode.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef FUNCTIONNODE_H
#define FUNCTIONNODE_H

#include "../GPNode.h"

class FunctionNode: public GPNode {
public:
	// overrides
	void prepareToPlay();
    void evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer);
	void updateMutatedParams();
    void toString(bool printRange, std::stringstream& ss);

    // subclass overrides
    virtual FunctionNode* getCopy() = 0;
	virtual void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) = 0;

private:
    std::string symbol;
};

#endif
