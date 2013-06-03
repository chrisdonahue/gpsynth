/*
  ==============================================================================

    SplineNode.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef SPLINENODE_H
#define SPLINENODE_H

#include "../GPNode.h"

class SplineNode: public GPNode {
public:
    SplineNode(bool terminal, GPRandom* r, bool ephemeralRandom, GPMutatableParam* numPoints, std::vector<GPMutatableParam*>* points, GPNode* signal);
    ~SplineNode();

	// overrides
    SplineNode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
    void toString(std::stringstream& ss);

    // optional overrides
	void setRenderInfo(float sr, unsigned blockSize, float maxTime);
    void doneRendering();
	void updateMutatedParams();

	// class specific
    void fillFromParams();

private:
    bool terminalSpline;
    int numPoints;
    GPRandom* rng;
    double sampleRate;

    bool releaseFinished;
    unsigned framesInEnvelope;
    float* envelope;

    double delay;
    unsigned delayFrames;
    double attack;
    unsigned attackFrames;
    double decay;
    unsigned decayFrames;
    double sustain;
    unsigned sustainFrames;
    double release;
    unsigned releaseFrames;
    double attackheight;
};

#endif
