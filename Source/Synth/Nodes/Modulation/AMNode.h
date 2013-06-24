/*
  ==============================================================================

    AMNode.h
    Author:  cdonahue

  ==============================================================================
*/

#ifndef AMNODE_H
#define AMNODE_H

#include "../GPNode.h"

class AMNode: public GPNode {
public:
    AMNode(GPMutatableParam* vn, GPMutatableParam* p, GPMutatableParam* o, GPMutatableParam* a, GPNode* mod);
    ~AMNode();

	// overrides
    AMNode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
    void toString(std::stringstream& ss);

    // optional overrides
	void updateMutatedParams();

private:
    int variableNum;
    float partial;
    float offset;
    float alpha;
    double w;
};

#endif
