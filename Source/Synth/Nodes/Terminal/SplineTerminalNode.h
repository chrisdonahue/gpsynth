/*
  ==============================================================================

    SplineTerminalNode.h
    Author:  cdonahue

  ==============================================================================
*/

#ifndef SPLINETERMINALNODE_H
#define SPLINETERMINALNODE_H

#include "../../GPNode.h"

class SplineTerminalNode: public GPNode {
public:
    SplineTerminalNode(GPMutatableParam* splinetype, GPMutatableParam* numpoints, std::vector<GPMutatableParam*>& pointsOrParams);
    ~SplineTerminalNode();

	// overrides
    SplineTerminalNode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
    void toString(std::stringstream& ss);

    // optional overrides
    void ephemeralRandom(GPRandom* rng);
	void setRenderInfo(float sr, unsigned blockSize, unsigned maxNumFrames, float maxTime);
    void doneRendering();
	void updateMutatedParams();

	// class specific
    void fillFromParams();

private:
    int splineType;
    int numSegments;

    float minSegmentLength;
    float maxSegmentLength;
    float minAmplitude;
    float maxAmplitude;

    unsigned maxNumFrames;
    float sampleRate;

    bool releaseFinished;
    unsigned framesInEnvelope;
    float* envelope;
};

#endif
