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

	// overrides
    LFONode* getCopy();
	void prepareToPlay();
    void evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer);
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
	void setRenderInfo(float sr, unsigned blockSize, float maxTime);
	void updateMutatedParams();
    void toString(bool printRange, std::stringstream& ss);

private:
    double frequency;
    double w;
};

#endif
