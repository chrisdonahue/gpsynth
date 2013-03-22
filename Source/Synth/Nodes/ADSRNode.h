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
        ADSRNode(bool terminal, double sr, GPMutatableParam* del, GPMutatableParam* atk, GPMutatableParam* atkh, GPMutatableParam* dec, GPMutatableParam* sus, GPMutatableParam* sush, GPMutatableParam* rel, GPNode* signal);
        ~ADSRNode();
        ADSRNode* getCopy();

        void evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer);
        std::string toString();
        void updateMutatedParams();

    private:
        bool terminalADSR;
        double sampleRate;
        bool releaseFinished;
        unsigned framesInEnvelope;
        float* envelope;

        double delay;
        double attack;
        double decay;
        double sustain;
        double release;
        double attackheight;
        double sustainheight;

        double minheight;
        double maxheight;
};

#endif
