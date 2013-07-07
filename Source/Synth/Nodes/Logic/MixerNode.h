/*
  ==============================================================================

    MixerNode.h
    Author:  cdonahue

  ==============================================================================
*/

#ifndef MIXERNODE_H
#define MIXERNODE_H

#include "../../GPNode.h"

class MixerNode: public GPNode {
public:
    MixerNode(GPNode* mod, GPNode* sigone, GPNode* sigtwo);
    ~MixerNode();

	// overrides
    MixerNode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
    void toString(std::stringstream& ss);

    // optional overrides
	void updateMutatedParams();

private:
	float controlMin;
	float controlMax;
};

#endif
