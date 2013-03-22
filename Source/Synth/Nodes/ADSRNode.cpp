/*
  ==============================================================================

    ADSRNode.cpp
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#include "ADSRNode.h"

/*
    ==============
    PUBLIC METHODS
    ==============
*/

ADSRNode::ADSRNode(double sr, GPMutatableParam* del, GPMutatableParam* atk, GPMutatableParam* atkh, GPMutatableParam* dec, GPMutatableParam* sus, GPMutatableParam* sush, GPMutatableParam* rel, GPNode* signal) :
sampleRate(sr)
{
    sampleRate = sr;
    releaseFinished = false;
    framesInEnvelope = 0;
    envelope = NULL;

    mutatableParams.push_back(del);
    mutatableParams.push_back(atk);
    mutatableParams.push_back(atkh);
    mutatableParams.push_back(dec);
    mutatableParams.push_back(sus);
    mutatableParams.push_back(sush);
    mutatableParams.push_back(rel);

    descendants.push_back(signal);
    arity = 1;

    updateMutatedParams();
}

ADSRNode::~ADSRNode() {
    free(envelope);
}

ADSRNode* ADSRNode::getCopy() {
    return new ADSRNode(sampleRate, mutatableParams[0]->getCopy(), mutatableParams[1]->getCopy(), mutatableParams[2]->getCopy(), mutatableParams[3]->getCopy(), mutatableParams[4]->getCopy(), mutatableParams[5]->getCopy(), mutatableParams[6]->getCopy(), descendants[0] == NULL ? NULL : descendants[0]->getCopy());
}

void ADSRNode::evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer) {
    *min = 0.0;
    *max = maxheight;

    descendants[0]->evaluateBlock(fn, t, nv, v, min, max, n, buffer);

    // if frame number is within the envelope
    if (fn < framesInEnvelope)
        releaseFinished = false;

    // if ADSR hasn't finished releasing but will within these n frames
    if (!releaseFinished && fn + n > framesInEnvelope) {
        for (int i = 0; fn + i < framesInEnvelope; i++) {
            buffer[i] = buffer[i] * envelope[fn + i];
        }
        for (int i = framesInEnvelope - fn; i < n; i++) {
            buffer[i] = 0.0;
        }
        releaseFinished = true;
    }
    // else if ADSR hasn't finished releasing and won't within these n frames
    else if (!releaseFinished) {
        for (int i = 0; i < n; i++) {
            buffer[i] = buffer[i] * envelope[fn + i];
        }
    }
    // else if ADSR has finished releasing for all n frames
    else {
        for (int i = 0; i < n; i++) {
            buffer[i] = 0.0;
        }
    }
}

std::string ADSRNode::toString() {
    char buffer[2048];
    snprintf(buffer, 2048, "(adsr %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %s)", delay, attack, attackheight, decay, sustain, sustainheight, release, descendants[0]->toString().c_str());
    return std::string(buffer);
}

void ADSRNode::updateMutatedParams() {
    delay = mutatableParams[0]->getValue();
    attack = mutatableParams[1]->getValue();
    attackheight = mutatableParams[2]->getValue();
    decay = mutatableParams[3]->getValue();
    sustain = mutatableParams[4]->getValue();
    sustainheight = mutatableParams[5]->getValue();
    release = mutatableParams[6]->getValue();
    
    maxheight = std::max(attackheight, sustainheight);

    framesInEnvelope = (unsigned) (delay * sampleRate) + (unsigned) (attack * sampleRate) + (unsigned) (decay * sampleRate) + (unsigned) (sustain * sampleRate) + (unsigned) (release * sampleRate);
    if (envelope != NULL)
        free(envelope);
    envelope = (float*) malloc(sizeof(float) * framesInEnvelope);

    // delay
    unsigned framesFilled = 0;
    for (int i = framesFilled; i < (unsigned) (delay * sampleRate); i++, framesFilled++) {
        envelope[i] = 0.0;
    }
    // attack
    for (int i = framesFilled; i < (unsigned) (attack * sampleRate); i++, framesFilled++) {
        envelope[i] = (i / (attack * sampleRate)) * attackheight;
    }
    // decay
    for (int i = framesFilled; i < (unsigned) (decay * sampleRate); i++, framesFilled++) {
        envelope[i] = attackheight - ((i / (decay * sampleRate)) * (attackheight - sustainheight));
    }
    // sustain
    for (int i = framesFilled; i < (unsigned) (decay * sampleRate); i++, framesFilled++) {
        envelope[i] = sustainheight;
    }
    // release
    for (int i = framesFilled; i < (unsigned) (decay * sampleRate); i++, framesFilled++) {
        envelope[i]  = sustainheight - ((i / (release * sampleRate)) * (sustainheight));
    }

    descendants[0]->updateMutatedParams();
}
