/*
  ==============================================================================

    GainNode.h
    Author:  cdonahue

  ==============================================================================
*/

#ifndef GAINNODE_H
#define GAINNODE_H

#include "../GPNode.h"

class GainNode: public GPNode {
public:
    GainNode(bool pi, GPMutatableParam* v, GPNode* signal);
    ~GainNode();

	// overrides
    GainNode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
    void toString(std::stringstream& ss);

    // optional overrides
	void setRenderInfo(float sr, unsigned blockSize, unsigned maxNumFrames, float maxTime);
    void doneRendering();
	void updateMutatedParams();

private:
    float value;
};

#endif
