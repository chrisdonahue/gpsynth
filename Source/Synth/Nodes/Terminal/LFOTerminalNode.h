/*
  ==============================================================================

    LFOTerminalNode.h
    Author:  cdonahue

  ==============================================================================
*/

#ifndef LFOTERMINALNODE_H
#define LFOTERMINALNODE_H

#include "../GPNode.h"

class LFOTerminalNode: public GPNode {
public:
    LFOTerminalNode(GPMutatableParam* rate;
    ~LFOTerminalNode();

	// overrides
    LFOTerminalNode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
    void toString(std::stringstream& ss);

    // optional overrides
	void updateMutatedParams();

private:
    float rate;
    double w;
};

#endif
