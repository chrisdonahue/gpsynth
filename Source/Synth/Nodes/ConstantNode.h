/*
  ==============================================================================

    ConstantNode.h
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef CONSTANTNODE_H
#define CONSTANTNODE_H

#include "../GPNode.h"

class ConstantNode: public GPNode {
public:
    ConstantNode(bool terminal, bool pi, GPMutatableParam* v, GPNode* signal);
    ~ConstantNode();

	// overrides
    ConstantNode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
    void toString(bool printRange, std::stringstream& ss);

    // optional overrides
	void updateMutatedParams();

private:
	bool terminalConstant;
    bool isPi;
    double value;
};

#endif
