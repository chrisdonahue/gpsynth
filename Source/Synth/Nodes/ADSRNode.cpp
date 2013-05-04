/*
  ==============================================================================

    ADSRNode.cpp
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#include "ADSRNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

ADSRNode::ADSRNode(bool terminal, bool store, GPMutatableParam* del, GPMutatableParam* atk, GPMutatableParam* atkh, GPMutatableParam* dec, GPMutatableParam* sus, GPMutatableParam* sush, GPMutatableParam* rel, GPNode* signal)
{
    terminalADSR = terminal;
    storeBuffer = store;
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

    if (terminalADSR) {
        arity = 0;
    }
    else {
        arity = 1;
        descendants.push_back(signal);
    }
}

ADSRNode::~ADSRNode() {
    if (storeBuffer) {
        free(envelope);
    }
}

/*
    =========
    OVERRIDES
    =========
*/

ADSRNode* ADSRNode::getCopy() {
    if (terminalADSR) {
        return new ADSRNode(terminalADSR, storeBuffer, mutatableParams[0]->getCopy(), mutatableParams[1]->getCopy(), mutatableParams[2]->getCopy(), mutatableParams[3]->getCopy(), mutatableParams[4]->getCopy(), mutatableParams[5]->getCopy(), mutatableParams[6]->getCopy(), NULL);
    }
    else {
        return new ADSRNode(terminalADSR, storeBuffer, mutatableParams[0]->getCopy(), mutatableParams[1]->getCopy(), mutatableParams[2]->getCopy(), mutatableParams[3]->getCopy(), mutatableParams[4]->getCopy(), mutatableParams[5]->getCopy(), mutatableParams[6]->getCopy(), descendants[0] == NULL ? NULL : descendants[0]->getCopy());
    }
}

void ADSRNode::setRenderInfo(float sr, unsigned blockSize, float maxTime) {
    sampleRate = sr;
    GPNode::setRenderInfo(sr, blockSize, maxTime);
}

void ADSRNode::prepareToPlay() {

}

void ADSRNode::evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer) {
    if (storeBuffer) {
        // if frame number is within the envelope
        if (fn < framesInEnvelope)
            releaseFinished = false;
        else
            releaseFinished = true;

        // if this is a terminal node
        if (terminalADSR) {
            *min = minimum;
            *max = maximum;
            if (!releaseFinished) {
                if (fn + n > framesInEnvelope) {
                    for (unsigned i = 0; fn + i < framesInEnvelope; i++) {
                        buffer[i] = envelope[fn + i];
                    }
                    for (unsigned i = framesInEnvelope - fn; i < n; i++) {
                        buffer[i] = 0.0;
                    }
                    releaseFinished = true;
                }
                else {
                    for (unsigned i = 0; i < n; i++) {
                        buffer[i] = envelope[fn + i];
                    }
                }
            }
        }
        // if this is not a terminal node
        // TODO: slight enhancement would be to not evaluateBlock here if release finished
        else {
            descendants[0]->evaluateBlock(fn, t, nv, v, min, max, n, buffer);
            GPmultiplyCalculateRange(min, max, minimum, maximum, *min, *max);
            if (!releaseFinished) {
                // if ADSR hasn't finished releasing but will within these n frames
                if (fn + n > framesInEnvelope) {
                    for (unsigned i = 0; fn + i < framesInEnvelope; i++) {
                        buffer[i] = buffer[i] * envelope[fn + i];
                    }
                    for (unsigned i = framesInEnvelope - fn; i < n; i++) {
                        buffer[i] = 0.0;
                    }
                    releaseFinished = true;
                }
                // else if ADSR hasn't finished releasing and won't within n
                else {
                    for (unsigned i = 0; i < n; i++) {
                        buffer[i] = buffer[i] * envelope[fn + i];
                    }
                }
            }
        }
        // else if ADSR has finished releasing for all n frames
        if (releaseFinished) {
            for (unsigned i = 0; i < n; i++) {
                buffer[i] = 0.0;
            }
        }
    }
    else {
        // if frame number is within the envelope
        if (fn < framesInEnvelope)
            releaseFinished = false;
        else
            releaseFinished = true;

        // if this is a terminal node
        if (terminalADSR) {
            *min = minimum;
            *max = maximum;
            if (!releaseFinished) {
                if (fn + n > framesInEnvelope) {
                    for (unsigned i = 0; fn + i < framesInEnvelope; i++) {
                        buffer[i] = getEnvelopeValue(fn + i);
                    }
                    for (unsigned i = framesInEnvelope - fn; i < n; i++) {
                        buffer[i] = 0.0;
                    }
                    releaseFinished = true;
                }
                else {
                    for (unsigned i = 0; i < n; i++) {
                        buffer[i] = getEnvelopeValue(fn + i);
                    }
                }
            }
        }
        // if this is not a terminal node
        else {
            descendants[0]->evaluateBlock(fn, t, nv, v, min, max, n, buffer);
            GPmultiplyCalculateRange(min, max, minimum, maximum, *min, *max);
            if (!releaseFinished) {
                // if ADSR hasn't finished releasing but will within these n frames
                if (fn + n > framesInEnvelope) {
                    for (unsigned i = 0; fn + i < framesInEnvelope; i++) {
                        buffer[i] = buffer[i] * getEnvelopeValue(fn + i);
                    }
                    for (unsigned i = framesInEnvelope - fn; i < n; i++) {
                        buffer[i] = 0.0;
                    }
                    releaseFinished = true;
                }
                // else if ADSR hasn't finished releasing and won't within n
                else {
                    for (unsigned i = 0; i < n; i++) {
                        buffer[i] = buffer[i] * getEnvelopeValue(fn + i);
                    }
                }
            }
        }
        // else if ADSR has finished releasing for all n frames
        if (releaseFinished) {
            for (unsigned i = 0; i < n; i++) {
                buffer[i] = 0.0;
            }
        }
    }
}

void ADSRNode::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {
    // if frame number is within the envelope
    if (firstFrameNumber < framesInEnvelope)
        releaseFinished = false;
    else
        releaseFinished = true;

    // if this is a terminal node
    if (terminalADSR) {
        if (!releaseFinished) {
            if (firstFrameNumber + numSamples > framesInEnvelope) {
                for (unsigned i = 0; firstFrameNumber + i < framesInEnvelope; i++) {
                    buffer[i] = envelope[firstFrameNumber + i];
                }
                for (unsigned i = framesInEnvelope - firstFrameNumber; i < numSamples; i++) {
                    buffer[i] = 0.0;
                }
                releaseFinished = true;
            }
            else {
                for (unsigned i = 0; i < numSamples; i++) {
                    buffer[i] = envelope[firstFrameNumber + i];
                }
            }
        }
    }
    // if this is not a terminal node
    // TODO: slight enhancement would be to not evaluateBlock here if release finished
    else {
        descendants[0]->evaluateBlockPerformance(firstFrameNumber, numSamples, sampleTimes, numConstantVariables, constantVariables, buffer);
        if (!releaseFinished) {
            // if ADSR hasn't finished releasing but will within these n frames
            if (firstFrameNumber + numSamples > framesInEnvelope) {
                for (unsigned i = 0; firstFrameNumber + i < framesInEnvelope; i++) {
                    buffer[i] = buffer[i] * envelope[firstFrameNumber + i];
                }
                for (unsigned i = framesInEnvelope - firstFrameNumber; i < numSamples; i++) {
                    buffer[i] = 0.0;
                }
                releaseFinished = true;
            }
            // else if ADSR hasn't finished releasing and won't within n
            else {
                for (unsigned i = 0; i < numSamples; i++) {
                    buffer[i] = buffer[i] * envelope[firstFrameNumber + i];
                }
            }
        }
    }
    // else if ADSR has finished releasing for all n frames
    if (releaseFinished) {
        for (unsigned i = 0; i < numSamples; i++) {
            buffer[i] = 0.0;
        }
    }
}

void ADSRNode::updateMutatedParams() {
	// get minimum value for attack or sustain
    float minAttackHeight = mutatableParams[2]->getCMin();
    if (mutatableParams[5]->getCMin() < minAttackHeight)
    	minAttackHeight = mutatableParams[5]->getCMin();
    
    // get maximum value for attack or sustain
    float maxAttackHeight = mutatableParams[2]->getCMax();
    if (mutatableParams[5]->getCMax() > maxAttackHeight)
    	maxAttackHeight = mutatableParams[5]->getCMax();
    
    // update min/max of terminal ADSR 
    if (terminalADSR) {
		minimum = minAttackHeight;
		maximum = maxAttackHeight;
	}
	// update descendants and min/max of non-terminal ADSR
	else {
        descendants[0]->updateMutatedParams();
        intervalMultiply(&minimum, &maximum, minAttackHeight, maxAttackHeight, descendants[0]->minimum, descendants[0]->maximum);
	}

    fillFromParams();
}

void ADSRNode::toString(bool printRange, std::stringstream& ss) {
	if (terminalADSR) {
	    ss << "(adsr";
	}
	else {
		ss << "(adsr*";
	}
    for (unsigned i = 0; i < mutatableParams.size(); i++) {
      ss << " ";
      mutatableParams[i]->toString(printRange, ss);
    }
    if (!terminalADSR) {
        ss << " ";
        descendants[0]->toString(printRange, ss);
    }
    ss << ")";
}

/*
    ==============
    CLASS SPECIFIC
    ==============
*/

void ADSRNode::fillFromParams() {
	// update class values from mutatable params
    delay = mutatableParams[0]->getCValue();
    delayFrames = delay * sampleRate;

    attack = mutatableParams[1]->getCValue();
    attackFrames = delayFrames + attack * sampleRate;
    attackheight = mutatableParams[2]->getCValue();

    decay = mutatableParams[3]->getCValue();
    decayFrames = attackFrames + decay * sampleRate;

    sustain = mutatableParams[4]->getCValue();
    sustainFrames = decayFrames + sustain * sampleRate;
    sustainheight = mutatableParams[5]->getCValue();

    release = mutatableParams[6]->getCValue();
    releaseFrames = sustainFrames + release * sampleRate;

	// calculate the length of the envelope in frames
    framesInEnvelope = (unsigned) (delay * sampleRate) + (unsigned) (attack * sampleRate) + (unsigned) (decay * sampleRate) + (unsigned) (sustain * sampleRate) + (unsigned) (release * sampleRate);

	// if we are pre-rendering the buffer for efficiency do so here
    if (storeBuffer) {
        if (envelope != NULL)
            free(envelope);
        envelope = (float*) malloc(sizeof(float) * framesInEnvelope);

        // delay
        unsigned framesFilled = 0;
        for (unsigned i = 0; i < (unsigned) (delay * sampleRate); i++, framesFilled++) {
            envelope[framesFilled] = 0.0;
        }
        // attack
        for (unsigned i = 0; i < (unsigned) (attack * sampleRate); i++, framesFilled++) {
            envelope[framesFilled] = (i / (attack * sampleRate)) * attackheight;
        }
        // decay
        for (unsigned i = 0; i < (unsigned) (decay * sampleRate); i++, framesFilled++) {
            envelope[framesFilled] = attackheight - ((i / (decay * sampleRate)) * (attackheight - sustainheight));
        }
        // sustain
        for (unsigned i = 0; i < (unsigned) (sustain * sampleRate); i++, framesFilled++) {
            envelope[framesFilled] = sustainheight;
        }
        // release
        for (unsigned i = 0; i < (unsigned) (release * sampleRate); i++, framesFilled++) {
            envelope[framesFilled]  = sustainheight - ((i / (release * sampleRate)) * (sustainheight));
        }
    }
}

inline float ADSRNode::getEnvelopeValue(unsigned fn) {
    unsigned framesFilled = 0;
    float ret = 0.0;
    if (fn < delayFrames) {
        ret = 0.0;
    }
    else if (fn < attackFrames) {
        ret = ((fn - delayFrames) / float(attackFrames - delayFrames)) * attackheight;
    }
    else if (fn < decayFrames) {
        ret = attackheight - (((fn - attackFrames) / float(decayFrames - attackFrames)) * (attackheight - sustainheight));
    }
    else if (fn < sustainFrames) {
        ret = sustainheight;
    }
    else if (fn < releaseFrames) {
        ret = sustainheight - (((fn - sustainFrames) / float(releaseFrames - sustainFrames)) * (sustainheight));
    }
    else {
        ret = 0.0;
    }
    return ret;
}


