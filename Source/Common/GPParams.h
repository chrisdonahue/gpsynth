#ifndef GPPARAMS_H
#define GPPARAMS_H

#include "GPRandom.h"
#include "GPFunctions.h"

struct GPParams {
    // experiment parameters
    unsigned experimentNumber;
    unsigned fitnessFunctionType;
    unsigned numGenerations;
    double thresholdFitness;
    bool ephemeralRandomConstants;

    // auxillary parameters
    bool verbose;
    bool saveGenerationChampions;
    unsigned printPrecision;
    unsigned savePrecision;
    unsigned wavFileBufferSize;
    unsigned renderBlockSize;

    // fitness function weights
    double magnitudeWeight;
    double phaseWeight;
    double envelopeWeight;

    // time domain fitness parameters
    bool saveTargetEnvelope;
    double envelopeFollowerAttack;
    double envelopeFollowerDecay;
    unsigned compareEnvelopeNumSkipFrames;

    // frequency domain fitness parameters
    char windowType[5];
    unsigned fftSize;
    unsigned fftOverlap;
    bool dBMagnitude;
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
    bool lowerFitnessIsBetter;
    double bestPossibleFitness;
    double penaltyFitness;
    unsigned maxInitialDepth;
    unsigned maxDepth;

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
    double valueNodeMinimum;
    double valueNodeMaximum;

    // oscil node
    unsigned oscilNodeMaxPartial;
    double oscilNodeMinIndexOfModulation;
    double oscilNodeMaxIndexOfModulation;

    // modulation node
    unsigned modulationNodeFrequencyRange;

    // delay node
    unsigned delayNodeMaxBufferSize;
    double delayNodeBufferMaxSeconds;

    // filter node
    double filterNodeCenterFrequencyMinimum;
    double filterNodeCenterFrequencyMaximumProportionOfNyquist;
    double filterNodeQualityMinimum;
    double filterNodeQualityMaximum;
    double filterNodeBandwidthMinimum;
    double filterNodeBandwidthMaximum;

    // adsr node
    double ADSRNodeEnvelopeMin;
    double ADSRNodeEnvelopeMax;

    // multiple nodes
    unsigned numVariables;
};

#endif
