/*
  ==============================================================================

    WaveTableNode.h
    Author:  cdonahue

  ==============================================================================
*/

#ifndef WAVETABLENODE_H
#define WAVETABLENODE_H

#include "../GPNode.h"

class WaveTableNode: public GPNode {
public:
    WaveTableNode(GPMutatableParam* rate;
    ~WaveTableNode();

	// overrides
    WaveTableNode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
    void toString(std::stringstream& ss);

    // optional overrides
	void updateMutatedParams();

private:
    float rate;
    double w;
};

#endif
