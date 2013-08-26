#ifndef COSINENODE_H
#define COSINENODE_H

#include <math.h>
#include "FunctionNode.h"

class CosineNode: public FunctionNode {
public:
    CosineNode(GPNode* zero);
    ~CosineNode();

	// overrides
    CosineNode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);

    // optional overrides
	void updateMutatedParams();

private:
};

#endif
