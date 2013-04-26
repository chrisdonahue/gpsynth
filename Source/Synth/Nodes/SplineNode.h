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
    SplineNode(bool store, bool terminal, double sr, GPMutatableParam* del, GPMutatableParam* atk, GPMutatableParam* atkh, GPMutatableParam* dec, GPMutatableParam* sus, GPMutatableParam* sush, GPMutatableParam* rel, GPNode* signal);
    ~SplineNode();

	// overrides
    SplineNode* getCopy();
	void prepareToPlay();
    void evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer);
	void evaluateBlockPerformance(unsigned fn, float* t, unsigned nv, float* v, float* min, float* max, unsigned n, float* buffer);
	void getRange(float* min, float* max);
	void updateMutatedParams();
    void toString(bool printRange, std::stringstream& ss);

private:
    bool storeBuffer;
    bool terminalADSR;
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
    double sustainheight;

    double minimum;
    double maximum;
};

#endif
