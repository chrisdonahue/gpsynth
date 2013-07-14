/*
  ==============================================================================

    SplineEnvelopeNode.cpp
    Author:  cdonahue

  ==============================================================================
*/

#include "SplineEnvelopeNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

// points should always have something in it
SplineEnvelopeNode::SplineEnvelopeNode(GPMutatableParam* splinetype, GPMutatableParam* numsegments, std::vector<GPMutatableParam*>& pointsOrParams, GPNode* signal)
{
    assert(splinetype->isUnmutatable());
    isPrimitive = (pointsOrParams.size() == 2 && numsegments->isMutatable());
    assert(isPrimitive || numsegments->isUnmutatable());

    splineType = splinetype->getDValue();

    mutatableParams.push_back(splinetype);
    mutatableParams.push_back(numsegments);

    if (isPrimitive) {
    }
    else {
        numSegments = numsegments->getDValue();
        assert(pointsOrParams.size() == (numSegments * 2) + 1);
    }

    for (unsigned i = 0; i < pointsOrParams.size(); i++) {
        mutatableParams.push_back(pointsOrParams[i]);
    }

    arity = 1;
    descendants.push_back(signal);
}

SplineEnvelopeNode::~SplineEnvelopeNode() {
    doneRendering();
}

/*
    =========
    OVERRIDES
    =========
*/

SplineEnvelopeNode* SplineEnvelopeNode::getCopy() {
    // make copies of spline points
    std::vector<GPMutatableParam*> paramCopies(mutatableParams.size() - 2);
    for (unsigned i = 2; i < mutatableParams.size(); i++) {
        paramCopies[i] = mutatableParams[i]->getCopy();
    }

    return new SplineEnvelopeNode(mutatableParams[0]->getCopy(), mutatableParams[1]->getCopy(), paramCopies, descendants[0] == NULL ? NULL : descendants[0]->getCopy());
}

void SplineEnvelopeNode::ephemeralRandom(GPRandom* rng) {
    // if this is a primitive spline then generate its points
    if (isPrimitive) {
        // randomize the number of points and lock it
        mutatableParams[1]->ephemeralRandom(rng);
        numSegments = mutatableParams[1]->getDValue();
        mutatableParams[1]->setUnmutatable();

        // get copies
        GPMutatableParam* ampRangeCopy = mutatableParams[2]->getCopy();
        ampRangeCopy->setType("spline_amp");
        GPMutatableParam* segmentLengthRangeCopy = mutatableParams[3]->getCopy();
        segmentLengthRangeCopy->setType("spline_segment_length");

        // remove and delete range specifiers
        delete mutatableParams[2];
        delete mutatableParams[3];
        mutatableParams.resize(2);

        // create the segments
        for (int i = 0; i < numSegments; i++) {
            GPMutatableParam* newSplineAmp = ampRangeCopy->getCopy();
            GPMutatableParam* newSplineSegmentLength = segmentLengthRangeCopy->getCopy();
            mutatableParams.push_back(newSplineAmp);
            mutatableParams.push_back(newSplineSegmentLength);
        }

        // create the final value
        GPMutatableParam* newSplineAmpFinal = ampRangeCopy->getCopy();
        newSplineAmpFinal->setType("spline_amp_final");
        mutatableParams.push_back(newSplineAmpFinal);

        // delete copies
        delete ampRangeCopy;
        delete segmentLengthRangeCopy;

        // set as no longer primitive
        isPrimitive = false;
    }

    GPNode::ephemeralRandom(rng);
}

void SplineEnvelopeNode::setRenderInfo(float sr, unsigned blockSize, unsigned maxNumFrames, float maxTime) {
    assert(!isPrimitive);
    doneRendering();
    sampleRate = sr;
    envelopeSize = maxNumFrames;
    envelope = (float*) malloc(sizeof(float) * envelopeSize);
    GPNode::setRenderInfo(sr, blockSize, maxNumFrames, maxTime);
}

void SplineEnvelopeNode::doneRendering() {
    if (preparedToRender) {
        sampleRate = 0;
        free(envelope);
    }
    GPNode::doneRendering();
}

void SplineEnvelopeNode::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {
    descendants[0]->evaluateBlockPerformance(firstFrameNumber, numSamples, sampleTimes, numConstantVariables, constantVariables, buffer);
    // copy envelope into buffer
    for (unsigned bi = 0, ei = firstFrameNumber; bi < numSamples; bi++, ei++) {
        buffer[bi] = buffer[bi] * envelope[ei];
    }
}

void SplineEnvelopeNode::updateMutatedParams() {
    GPNode::updateMutatedParams();

    assert(!isPrimitive);

    // get minimum and maximum value for spline envelope
    float minSplineHeight = mutatableParams[2]->getCMin();
    float maxSplineHeight = mutatableParams[2]->getCMax();
    for (int i = 4; i < mutatableParams.size(); i += 2) {
        if (mutatableParams[i]->getCMin() < minSplineHeight)
            minSplineHeight = mutatableParams[i]->getCMin();
        if (mutatableParams[i]->getCMax() > maxSplineHeight)
            maxSplineHeight = mutatableParams[i]->getCMax();
    }
    
    // update min/max of envelope spline
    intervalMultiply(&minimum, &maximum, minSplineHeight, maxSplineHeight, descendants[0]->minimum, descendants[0]->maximum);
    
    fillFromParams();
}

void SplineEnvelopeNode::toString(std::stringstream& ss) {
    ss << "(spline*";
    for (unsigned i = 0; i < mutatableParams.size(); i++) {
        ss << " ";
        mutatableParams[i]->toString(ss);
    }
    ss << " ";
    descendants[0]->toString(ss);
    ss << ")";
}

/*
    ==============
    CLASS SPECIFIC
    ==============
*/

void SplineEnvelopeNode::fillFromParams() {
    if (splineType == 0) {
        unsigned currentFrame = 0;
        unsigned usedPoints = 0;
        float currentLevel = mutatableParams[2]->getCValue();
        while (currentFrame < envelopeSize && usedPoints < numSegments) {
            float transitionLength = mutatableParams[(usedPoints * 2) + 2 + 1]->getCValue();
            float nextLevel = mutatableParams[(usedPoints * 2) + 2 + 2]->getCValue();
            unsigned currentTransitionFrame = 0;
            unsigned numTransitionFrames = (unsigned) (transitionLength * sampleRate);
            // may be infinite if numTransitionFrames is 0 but will never be used in that case
            float slope = (nextLevel - currentLevel) / float(numTransitionFrames);
            while (currentTransitionFrame < numTransitionFrames && currentFrame < envelopeSize) {
                envelope[currentFrame] = currentLevel + currentTransitionFrame * slope;
                currentFrame++;
                currentTransitionFrame++;
            }
            usedPoints++;
            currentLevel = nextLevel;
        }
        while (currentFrame < envelopeSize) {
            envelope[currentFrame] = currentLevel;
            currentFrame++;
        }
    }
    else {
        unsigned currentFrame = 0;
        float currentLevel = mutatableParams[2]->getCValue();
        while (currentFrame < envelopeSize) {
            envelope[currentFrame] = currentLevel;
            currentFrame++;
        }
    }
}
