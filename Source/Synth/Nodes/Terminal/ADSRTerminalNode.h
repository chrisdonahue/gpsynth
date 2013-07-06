/*
  ==============================================================================

    ADSRTerminalNode.h
    Author:  cdonahue

  ==============================================================================
*/

#ifndef ADSRTERMINALNODE_H
#define ADSRTERMINALNODE_H

#include "../../GPNode.h"

class ADSRTerminalNode: public GPNode {
public:
    ADSRTerminalNode(GPMutatableParam* del, GPMutatableParam* atk, GPMutatableParam* atkh, GPMutatableParam* dec, GPMutatableParam* sus, GPMutatableParam* sush, GPMutatableParam* rel);
    ~ADSRTerminalNode();

	// overrides
    ADSRTerminalNode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
    void toString(std::stringstream& ss);

    // optional overrides
	void setRenderInfo(float sr, unsigned blockSize, unsigned maxNumFrames, float maxTime);
    void doneRendering();
	void updateMutatedParams();

	// class specific
    void fillFromParams();

private:
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
