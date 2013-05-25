#ifndef GPPARAMS_H
#define GPPARAMS_H

#include "GPRandom.h"

struct GPParams {
	// synth and experiment shared params
    bool verbose;
    unsigned savePrecision;
    unsigned printPrecision;
    bool lowerFitnessIsBetter;
    double bestPossibleFitness;

    // experiment parameters
    unsigned experimentNumber;
    unsigned suboptimizeType;
    unsigned fitnessFunctionType;
    unsigned numGenerations;
    double thresholdFitness;
    bool ephemeralRandomConstants;

    // auxiliary experiment parameters
    bool saveGenerationChampions;
    bool saveTargetEnvelope;
    bool saveTargetSpectrum;
    bool backupTarget;
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

    // synth evolution parameters
    unsigned populationSize;
    bool backupAllNetworks;
    unsigned backupPrecision;
    unsigned maxInitialHeight;
    unsigned maxHeight;

    // synth genetic parameters
    double proportionOfPopulationForGreedySelection;
    // numeric mutation
    unsigned numericMutationSelectionType;
    double proportionOfPopulationFromNumericMutation;
    double percentileOfPopulationToSelectFromForNumericMutation;
    double numericMutationTemperatureConstant;
    // mutation
    unsigned mutationSelectionType;
    unsigned mutationType;
    double proportionOfPopulationFromMutation;
    double percentileOfPopulationToSelectFromForMutation;
    // crossover
    unsigned crossoverSelectionType;
    unsigned crossoverType;
    double proportionOfPopulationFromCrossover;
    // reproduction
    unsigned reproductionSelectionType;
    double proportionOfPopulationFromReproduction;

    // value node
    float valueNodeMinimum;
    float valueNodeMaximum;

    // oscil node
    unsigned oscilNodeMaxPartial;
    float oscilNodeMinIndexOfModulation;
    float oscilNodeMaxIndexOfModulation;

    // modulation node
    unsigned LFONodeFrequencyRange;

    // delay node
    unsigned delayNodeMaxBufferSize;
    float delayNodeBufferMaxSeconds;

    // filter node
    float filterNodeCenterFrequencyMinimum;
    float filterNodeCenterFrequencyMaximumProportionOfNyquist;
    float filterNodeQualityMinimum;
    float filterNodeQualityMaximum;
    float filterNodeBandwidthMinimum;
    float filterNodeBandwidthMaximum;

    // adsr node
    float ADSRNodeEnvelopeMin;
    float ADSRNodeEnvelopeMax;
};

#endif
