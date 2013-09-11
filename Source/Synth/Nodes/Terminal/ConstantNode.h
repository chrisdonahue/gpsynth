#ifndef CONSTANTNODE_H
#define CONSTANTNODE_H

#include "../../GPNode.h"

class ConstantNode: public GPNode {
public:
    ConstantNode(bool pi, GPMutatableParam* v);
    ~ConstantNode();

	// overrides
    ConstantNode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);

    // optional overrides
	void setRenderInfo(float sr, unsigned blockSize, unsigned maxNumFrames, float maxTime);
    void doneRendering();
	void updateMutatedParams();

private:
    bool isPi;
    double value;
};

#endif
