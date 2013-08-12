#ifndef GPPARAMS_H
#define GPPARAMS_H

#include "GPRandom.h"

struct GPMatchingExperimentParams {
    // experiment parameters
    unsigned experimentNumber;
    unsigned suboptimizeType;
    unsigned fitnessFunctionType;
    unsigned numGenerations;
    double thresholdFitness;
    bool ephemeralRandomConstants;

    // auxiliary experiment parameters
    unsigned wavFileBufferSize;
    unsigned renderBlockSize;

    // fitness function weights
    double magnitudeWeight;
    double phaseWeight;
    double envelopeWeight;

    // time domain fitness parameters
    double envelopeFollowerAttack;
    double envelopeFollowerDecay;
    unsigned compareEnvelopeNumSkipFrames;

    // frequency domain fitness parameters
    char windowType[5];
    unsigned fftSize;
    unsigned fftOverlap;
    bool dBMagnitude;
    // moving average
    unsigned averageComparisonType;
    unsigned movingAveragePastRadius;
    unsigned movingAverageFutureRadius;
    double exponentialMovingAverageAlpha;
    bool compareToMaxDeviation;
    double penaltyComparisonExponent;
    bool weightFftFrames;
    double frameWeightExponent;
    // phase penalty
    double penalizeBadPhase;
    // magnitude penalty over/undershooting system
    double goodComparisonFactor;
    double badComparisonFactor;
    double baseComparisonFactor;
};

#endif
