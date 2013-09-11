#ifndef SUBTRACTNODE_H
#define SUBTRACTNODE_H

#include "FunctionNode.h"

class SubtractNode: public FunctionNode {
public:
    SubtractNode(GPNode* zero, GPNode* one);
    ~SubtractNode();

	// overrides
    SubtractNode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);

    // optional overrides
	void updateMutatedParams();

private:
};

#endif
