/*
  ==============================================================================

    SilenceNode.h
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef SILENCENODE_H
#define SILENCENODE_H

#include "../GPNode.h"

class SilenceNode: public GPNode {
public:
    SilenceNode();
    ~SilenceNode();

	// overrides
    SilenceNode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
    void toString(bool printRange, std::stringstream& ss);

private:
};

#endif
