/*
  ==============================================================================

    SplineTerminalNode.cpp
    Author:  cdonahue

  ==============================================================================
*/

#include "SplineTerminalNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

// points should always have something in it
SplineTerminalNode::SplineTerminalNode(GPRandom* r, bool ephemeralRandom, GPMutatableParam* splinetype, GPMutatableParam* numpoints, std::vector<GPMutatableParam*>& points)
{
    assert(!(splinetype->isMutatable));
    assert(!(numpoints->isMutatable));
    if (ephemeralRandom)
        assert(points.size() == 2);

    rng = r;
    splineType = splinetype->getDValue();
    numPoints = numpoints->getDValue();

    mutatableParams.push_back(splinetype);
    mutatableParams.push_back(numpoints);

    // randomize the initial points (determines the range if ephemeralRandom == true)
    if (ephemeralRandom) {
        // use points array to determine range of mutatable params
        minimum = points[0]->getCMin();
        maximum = points[0]->getCMax();
        maxSegmentLength = points[1]->getCMax();

        // create the initial value for the spline function
        GPMutatableParam* initialValue = new GPMutatableParam("splinepoint", true, minimum, maximum, maximum);
        initialValue->ephemeralRandom(rng);
        mutatableParams.push_back(initialValue);

        // create the array of spline points
        for (int i = 0; i < numPoints; i++) {
            GPMutatableParam* newSplineLength = new GPMutatableParam("splinelength", true, 0.0f, 0.0f, maxSegmentLength);
            GPMutatableParam* newSplinePoint = new GPMutatableParam("splinepoint", true, minimum, minimum, maximum);
            newSplineLength->ephemeralRandom(rng);
            newSplinePoint->ephemeralRandom(rng);
            mutatableParams.push_back(newSplineLength);
            mutatableParams.push_back(newSplinePoint);
        }
    }
    else {
        for (unsigned i = 0; i < points.size(); i++) {
            mutatableParams.push_back(points[i]);
        }
    }

    arity = 1;
    descendants.push_back(signal);
}

SplineTerminalNode::~SplineTerminalNode() {
    doneRendering();
}

/*
    =========
    OVERRIDES
    =========
*/

SplineTerminalNode* SplineTerminalNode::getCopy() {
    // make copies of spline points
    std::vector<GPMutatableParam*> paramCopies(numPoints * 2 + 1);
    for (unsigned i = 2; i < mutatableParams.size(); i++) {
        paramCopies[i] = mutatableParams[i]->getCopy();
    }

    return new SplineTerminalNode(rng, false, mutatableParams[0]->getCopy(), mutatableParams[1]->getCopy(), paramCopies);
}

void SplineTerminalNode::setRenderInfo(float sr, unsigned blockSize, unsigned maxNumFrames, float maxTime) {
    doneRendering();
    sampleRate = sr;
    GPNode::setRenderInfo(sr, blockSize, maxNumFrames, maxTime);
}

void SplineTerminalNode::doneRendering() {
    if (preparedToRender) {
        sampleRate = 0;
        free(envelope);
    }
    GPNode::doneRendering();
}

void SplineTerminalNode::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {
    // if frame number is within the envelope
    if (firstFrameNumber < framesInEnvelope)
        releaseFinished = false;
    else
        releaseFinished = true;

    // if this is a terminal node
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
    // else if Spline has finished releasing for all n frames
    else {
        for (unsigned i = 0; i < numSamples; i++) {
            buffer[i] = 0.0;
        }
    }
}

void SplineTerminalNode::updateMutatedParams() {
    GPNode::updateMutatedParams();

    // get minimum value for spline envelope
    float minSplineHeight = mutatableParams[2]->getCMin();
    for (int i = 1; i < numPoints; i++) {
        int heightIndex = (i * 2) + 2;
        if (mutatableParams[heightIndex]->getCMin() < minSplineHeight)
            minSplineHeight = mutatableParams[heightIndex]->getCMin();
    }
    
    // get maximum value for attack or sustain
    float maxSplineHeight = mutatableParams[0]->getCMax();
    for (int i = 1; i < numPoints; i++) {
        int heightIndex = (i * 2) + 2;
        if (mutatableParams[heightIndex]->getCMax() > maxSplineHeight)
            maxSplineHeight = mutatableParams[heightIndex]->getCMax();
    }
    
    // update min/max of terminal Spline 
    minimum = minSplineHeight;
    maximum = maxSplineHeight;
    
    fillFromParams();
}

void SplineTerminalNode::toString(std::stringstream& ss) {
    ss << "(spline";
    for (unsigned i = 0; i < mutatableParams.size(); i++) {
        ss << " ";
        mutatableParams[i]->toString(ss);
    }
    ss << ")";
}

/*
    ==============
    CLASS SPECIFIC
    ==============
*/

void SplineTerminalNode::fillFromParams() {
    if (splineType == 0) {
        unsigned currentFrame = 0;
        unsigned usedPoints = 0;
        float currentLevel = mutatableParams[2]->getCValue();
        while (currentFrame < maxNumFrames && usedPoints < numPoints) {
            float transitionLength = mutatableParams[(usedPoints * 2) + 2 + 1]->getCValue();
            float nextLevel = mutatableParams[(usedPoints * 2) + 2 + 2]->getCValue();
            unsigned currentTransitionFrame = 0;
            unsigned numTransitionFrames = (unsigned) transitionLength * sampleRate;
            float slope = (nextLevel - currentLevel) / float(numTransitionFrames);
            while (currentTransitionFrame < numTransitionFrames && currentFrame < maxNumFrames) {
                envelope[currentFrame] = currentLevel + currentTransitionFrame * slope;
                currentFrame++;
                currentTransitionFrame++;
            }
            usedPoints++;
            currentLevel = nextLevel;
        }
        while (currentFrame < maxNumFrames) {
            envelope[currentFrame] = currentLevel;
            currentFrame++;
        }
    }
    else {
        unsigned currentFrame = 0;
        float currentLevel = mutatableParams[2]->getCValue();
        while (currentFrame < maxNumFrames) {
            envelope[currentFrame] = currentLevel;
            currentFrame++;
        }
    }
}
