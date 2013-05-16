/*
  ==============================================================================

    MixerNode.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef MIXERNODE_H
#define MIXERNODE_H

#include "../GPNode.h"

class MixerNode: public GPNode {
public:
    MixerNode(bool fade, GPNode* mod, GPNode* sigone, GPNode* sigtwo);
    ~MixerNode();

	// overrides
    MixerNode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
    void toString(std::stringstream& ss);

    // optional overrides
	void updateMutatedParams();

private:
    bool fadeMixer;
    float signalOneMin;
    float signalOneMax;
    float signalTwoMin;
    float signalTwoMax;
};

#endif
