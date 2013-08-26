#ifndef GAINNODE_H
#define GAINNODE_H

#include "../../GPNode.h"

class GainNode: public GPNode {
public:
    GainNode(GPMutatableParam* v, GPNode* signal);
    ~GainNode();

	// overrides
    GainNode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);

    // optional overrides
	void setRenderInfo(float sr, unsigned blockSize, unsigned maxNumFrames, float maxTime);
    void doneRendering();
	void updateMutatedParams();

private:
    float value;
};

#endif
