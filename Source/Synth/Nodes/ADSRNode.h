/*
  ==============================================================================

    ADSRNode.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef ADSRNODE_H
#define ADSRNODE_H

#include "../GPNode.h"

class ADSRNode: public GPNode {
public:
    ADSRNode(bool store, bool terminal, double sr, GPMutatableParam* del, GPMutatableParam* atk, GPMutatableParam* atkh, GPMutatableParam* dec, GPMutatableParam* sus, GPMutatableParam* sush, GPMutatableParam* rel, GPNode* signal);
    ~ADSRNode();
    ADSRNode* getCopy();

    void evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer);
    inline float getEnvelopeValue(unsigned fn);
    void toString(bool printRange, std::stringstream& ss);
    void fillFromParams();
    void updateMutatedParams();

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
