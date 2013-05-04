/*
  ==============================================================================

    OscilNode.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef OSCILNODE_H
#define OSCILNODE_H

#include "../GPNode.h"

class OscilNode: public GPNode {
public:
    OscilNode(bool terminal, GPMutatableParam* vn, GPMutatableParam* p, GPMutatableParam* i, GPNode* mod);
    ~OscilNode();

	// overrides
    OscilNode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
    void toString(bool printRange, std::stringstream& ss);

    // optional overrides
	void updateMutatedParams();

private:
    bool terminalOscil;
    int partial;
    int variableNum;
    double w;
    double index;
};

#endif
