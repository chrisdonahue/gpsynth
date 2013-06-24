/*
  ==============================================================================

    SwitchNode.h
    Author:  cdonahue

  ==============================================================================
*/

#ifndef SWITCHNODE_H
#define SWITCHNODE_H

#include "../GPNode.h"

class SwitchNode: public GPNode {
public:
    SwitchNode(bool fade, GPNode* mod, GPNode* sigone, GPNode* sigtwo);
    ~SwitchNode();

	// overrides
    SwitchNode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
    void toString(std::stringstream& ss);

    // optional overrides
	void updateMutatedParams();

private:
};

#endif
