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
    // overrides
    void toString(std::stringstream& ss);
	void updateMutatedParams();

	// subclass overrides
    virtual WaveTableNode* getCopy() = 0
	virtual void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) = 0;

    // oscillator symbol
    std::string symbol;
private:
};

#endif
