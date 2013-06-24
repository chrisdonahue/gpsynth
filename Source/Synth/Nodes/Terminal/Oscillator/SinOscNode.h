/*
  ==============================================================================

    SinOsc.h
    Author:  cdonahue

  ==============================================================================
*/

#ifndef SINOSCNODE_H
#define SINOSCNODE_H

#include "../GPNode.h"

class SinOsc: public GPNode {
public:
    SinOsc(GPMutatableParam* vn, GPMutatableParam* partial, GPMutatableParam* phase);
    ~SinOsc();

	// overrides
    SinOsc* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
    void toString(std::stringstream& ss);

    // optional overrides
	void updateMutatedParams();

private:
    float partial;
    float phase;
    int variableNum;
    double w;
};

#endif
