/*
  ==============================================================================

    SplineNode.cpp
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#include "SplineNode.h"

/*
    ==============
    PUBLIC METHODS
    ==============
*/

SplineNode::SplineNode(bool store, bool terminal, double sr, GPMutatableParam* del, GPMutatableParam* atk, GPMutatableParam* atkh, GPMutatableParam* dec, GPMutatableParam* sus, GPMutatableParam* sush, GPMutatableParam* rel, GPNode* signal) :
    sampleRate(sr)
{
    storeBuffer = store;
    terminalSpline = terminal;
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

    if (terminalSpline) {
        arity = 0;
    }
    else {
        descendants.push_back(signal);
        arity = 1;
    }

    fillFromParams();
}

SplineNode::~SplineNode() {
    if (storeBuffer) {
        free(envelope);
    }
}

SplineNode* SplineNode::getCopy() {
    if (terminalSpline) {
        return new SplineNode(storeBuffer, terminalSpline, sampleRate, mutatableParams[0]->getCopy(), mutatableParams[1]->getCopy(), mutatableParams[2]->getCopy(), mutatableParams[3]->getCopy(), mutatableParams[4]->getCopy(), mutatableParams[5]->getCopy(), mutatableParams[6]->getCopy(), NULL);
    }
    else {
        return new SplineNode(storeBuffer, terminalSpline, sampleRate, mutatableParams[0]->getCopy(), mutatableParams[1]->getCopy(), mutatableParams[2]->getCopy(), mutatableParams[3]->getCopy(), mutatableParams[4]->getCopy(), mutatableParams[5]->getCopy(), mutatableParams[6]->getCopy(), descendants[0] == NULL ? NULL : descendants[0]->getCopy());
    }
}

void SplineNode::evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer) {
    if (storeBuffer) {
        // if frame number is within the envelope
        if (fn < framesInEnvelope)
            releaseFinished = false;
        else
            releaseFinished = true;

        // if this is a terminal node
        if (terminalSpline) {
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
                // if Spline hasn't finished releasing but will within these n frames
                if (fn + n > framesInEnvelope) {
                    for (unsigned i = 0; fn + i < framesInEnvelope; i++) {
                        buffer[i] = buffer[i] * envelope[fn + i];
                    }
                    for (unsigned i = framesInEnvelope - fn; i < n; i++) {
                        buffer[i] = 0.0;
                    }
                    releaseFinished = true;
                }
                // else if Spline hasn't finished releasing and won't within n
                else {
                    for (unsigned i = 0; i < n; i++) {
                        buffer[i] = buffer[i] * envelope[fn + i];
                    }
                }
            }
        }
        // else if Spline has finished releasing for all n frames
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
        if (terminalSpline) {
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
                // if Spline hasn't finished releasing but will within these n frames
                if (fn + n > framesInEnvelope) {
                    for (unsigned i = 0; fn + i < framesInEnvelope; i++) {
                        buffer[i] = buffer[i] * getEnvelopeValue(fn + i);
                    }
                    for (unsigned i = framesInEnvelope - fn; i < n; i++) {
                        buffer[i] = 0.0;
                    }
                    releaseFinished = true;
                }
                // else if Spline hasn't finished releasing and won't within n
                else {
                    for (unsigned i = 0; i < n; i++) {
                        buffer[i] = buffer[i] * getEnvelopeValue(fn + i);
                    }
                }
            }
        }
        // else if Spline has finished releasing for all n frames
        if (releaseFinished) {
            for (unsigned i = 0; i < n; i++) {
                buffer[i] = 0.0;
            }
        }
    }
}

void SplineNode::evaluateBlockPerformance(unsigned fn, float* t, unsigned nv, float* v, float* min, float* max, unsigned n, float* buffer) {
    // if frame number is within the envelope
    if (fn < framesInEnvelope)
        releaseFinished = false;
    else
        releaseFinished = true;

    // if this is a terminal node
    if (terminalSpline) {
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
        descendants[0]->evaluateBlockPerformance(fn, t, nv, v, min, max, n, buffer);
        intervalMultiply(min, max, minimum, maximum, *min, *max);
        if (!releaseFinished) {
            // if Spline hasn't finished releasing but will within these n frames
            if (fn + n > framesInEnvelope) {
                for (unsigned i = 0; fn + i < framesInEnvelope; i++) {
                    buffer[i] = buffer[i] * envelope[fn + i];
                }
                for (unsigned i = framesInEnvelope - fn; i < n; i++) {
                    buffer[i] = 0.0;
                }
                releaseFinished = true;
            }
            // else if Spline hasn't finished releasing and won't within n
            else {
                for (unsigned i = 0; i < n; i++) {
                    buffer[i] = buffer[i] * envelope[fn + i];
                }
            }
        }
    }
    // else if Spline has finished releasing for all n frames
    if (releaseFinished) {
        for (unsigned i = 0; i < n; i++) {
            buffer[i] = 0.0;
        }
    }
}

inline float SplineNode::getEnvelopeValue(unsigned fn) {
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

void SplineNode::toString(bool printRange, std::stringstream& ss) {
    ss << "(Spline";
    for (unsigned i = 0; i < mutatableParams.size(); i++) {
      ss << " ";
      mutatableParams[i]->toString(printRange, ss);
    }
    if (!terminalSpline) {
        ss << " ";
        descendants[0]->toString(printRange, ss);
    }
    ss << ")";
}

void SplineNode::fillFromParams() {
    delay = mutatableParams[0]->getValue();
    delayFrames = delay * sampleRate;

    attack = mutatableParams[1]->getValue();
    attackFrames = delayFrames + attack * sampleRate;
    attackheight = mutatableParams[2]->getValue();

    decay = mutatableParams[3]->getValue();
    decayFrames = attackFrames + decay * sampleRate;

    sustain = mutatableParams[4]->getValue();
    sustainFrames = decayFrames + sustain * sampleRate;
    sustainheight = mutatableParams[5]->getValue();

    release = mutatableParams[6]->getValue();
    releaseFrames = sustainFrames + release * sampleRate;

    minimum = std::numeric_limits<double>::max();
    maximum = std::numeric_limits<double>::min();

    // calculate minimum
    if (mutatableParams[2]->getMin() < minimum)
	minimum = mutatableParams[2]->getMin();
    if (mutatableParams[2]->getMax() < minimum)
	minimum = mutatableParams[2]->getMax();
    if (mutatableParams[5]->getMin() < minimum)
	minimum = mutatableParams[5]->getMin();
    if (mutatableParams[5]->getMax() < minimum)
	minimum = mutatableParams[5]->getMax();
    if (0 < minimum)
	minimum = 0;

    // calculate maximum
    if (mutatableParams[2]->getMin() > maximum)
	maximum = mutatableParams[2]->getMin();
    if (mutatableParams[2]->getMax() > maximum)
	maximum = mutatableParams[2]->getMax();
    if (mutatableParams[5]->getMin() > maximum)
	maximum = mutatableParams[5]->getMin();
    if (mutatableParams[5]->getMax() > maximum)
	minimum = mutatableParams[5]->getMax();
    if (0 > maximum)
	maximum = 0;

    framesInEnvelope = (unsigned) (delay * sampleRate) + (unsigned) (attack * sampleRate) + (unsigned) (decay * sampleRate) + (unsigned) (sustain * sampleRate) + (unsigned) (release * sampleRate);

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

void SplineNode::updateMutatedParams() {
    fillFromParams();

    if (!terminalSpline)
        descendants[0]->updateMutatedParams();
}


void SplineNode::prepareToPlay() {
}

void SplineNode::getRange(float* min, float* max) {

}
