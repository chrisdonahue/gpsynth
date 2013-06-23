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
    LFONode(bool terminal, GPMutatableParam* rate, GPNode* mod);
    ~LFONode();

	// overrides
    LFONode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
    void toString(std::stringstream& ss);

    // optional overrides
	void updateMutatedParams();

private:
    bool terminalLFO;
    float rate;
    float w;
};

#endif
