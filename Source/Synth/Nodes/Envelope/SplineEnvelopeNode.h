/*
  ==============================================================================

    SplineEnvelopeNode.h
    Author:  cdonahue

  ==============================================================================
*/

#ifndef SPLINEENVELOPENODE_H
#define SPLINEENVELOPENODE_H

#include "../../GPNode.h"

class SplineEnvelopeNode: public GPNode {
public:
    SplineEnvelopeNode(GPRandom* r, bool ephemeralRandom, GPMutatableParam* splineType, GPMutatableParam* numPoints, std::vector<GPMutatableParam*>& points, GPNode* signal);
    ~SplineEnvelopeNode();

	// overrides
    SplineEnvelopeNode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
    void toString(std::stringstream& ss);

    // optional overrides
	void setRenderInfo(float sr, unsigned blockSize, unsigned maxNumFrames, float maxTime);
    void doneRendering();
	void updateMutatedParams();

	// class specific
    void fillFromParams();

private:
    int splineType;
    int numPoints;
    GPRandom* rng;
    float maxSegmentLength;

    unsigned maxNumFrames;
    float sampleRate;

    bool releaseFinished;
    unsigned framesInEnvelope;
    float* envelope;
};

#endif
