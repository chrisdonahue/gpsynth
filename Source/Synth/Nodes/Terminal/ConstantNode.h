/*
  ==============================================================================

    ConstantNode.h
    Author:  cdonahue

  ==============================================================================
*/

#ifndef CONSTANTNODE_H
#define CONSTANTNODE_H

#include "../../GPNode.h"

class ConstantNode: public GPNode {
public:
    ConstantNode(bool pi, GPMutatableParam* v, GPNode* signal);
    ~ConstantNode();

	// overrides
    ConstantNode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
    void toString(std::stringstream& ss);

    // optional overrides
	void setRenderInfo(float sr, unsigned blockSize, unsigned maxNumFrames, float maxTime);
    void doneRendering();
	void updateMutatedParams();

private:
    bool isPi;
    double value;
};

#endif
