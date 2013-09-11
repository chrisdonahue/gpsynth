#ifndef TIMENODE_H
#define TIMENODE_H

#include "../../GPNode.h"

class TimeNode: public GPNode {
public:
    TimeNode();
    ~TimeNode();

	// overrides
    TimeNode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);

    // optional overrides
	void setRenderInfo(float sr, unsigned blockSize, unsigned maxNumFrames, float maxTime);
	void doneRendering();

private:
};

#endif
