/*
  ==============================================================================

    TimeNode.h
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef TIMENODE_H
#define TIMENODE_H

#include "../GPNode.h"

class TimeNode: public GPNode {
public:
    TimeNode();
    ~TimeNode();

	// overrides
    TimeNode* getCopy();
	void prepareToPlay();
    void evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer);
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
	void updateMutatedParams();
    void toString(bool printRange, std::stringstream& ss);

    // optional overrides
	void setRenderInfo(float sr, unsigned blockSize, float maxTime);
private:
};

#endif
