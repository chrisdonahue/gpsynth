/*
  ==============================================================================

    PMNode.h
    Author:  cdonahue

  ==============================================================================
*/

#ifndef PMNODE_H
#define PMNODE_H

#include "../../GPNode.h"

class PMNode: public GPNode {
public:
    PMNode(GPMutatableParam* vn, GPMutatableParam* p, GPMutatableParam* i, GPNode* mod);
    ~PMNode();

	// overrides
    PMNode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
    void toString(std::stringstream& ss);

    // optional overrides
	void updateMutatedParams();

private:
    int variableNum;
    float partial;
    float index;
    double w;
};

#endif
