/*
  ==============================================================================

    AddNode.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef ADDNODE_H
#define ADDNODE_H

#include "../GPNode.h"

class AddNode: public GPNode {
public:
    AddNode(bool terminal, bool store, double sr, GPMutatableParam* del, GPMutatableParam* atk, GPMutatableParam* atkh, GPMutatableParam* dec, GPMutatableParam* sus, GPMutatableParam* sush, GPMutatableParam* rel, GPNode* signal);
    ~AddNode();

	// overrides
    AddNode* getCopy();
	void prepareToPlay();
	void setRenderInfo(float sr, unsigned blockSize, float maxTime);
    void evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer);
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
	void updateMutatedParams();
    void toString(bool printRange, std::stringstream& ss);

	// class specific
    void fillFromParams();
    inline float getEnvelopeValue(unsigned fn);

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
};

#endif
