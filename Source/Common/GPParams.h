#ifndef GPPARAMS_H
#define GPPARAMS_H

#include "GPRandom.h"
#include "GPFunctions.h"

struct GPParams {
    // auxillary parameters
    bool verbose;
    unsigned wavFileBufferSize;
    unsigned renderBlockSize;
    unsigned fftSize;

    // fitness function parameters
    unsigned fitnessFunctionType;
    unsigned penalizeBadMagnitude;
    unsigned penalizeBadPhase;
    double magnitudeWeight;
    double phaseWeight;

    // experiment parameters
    unsigned experimentNumber;
    unsigned numGenerations;
    double thresholdFitness;
    bool ephemeralRandomConstants;
    bool suboptimize;

    // synth evolution parameters
    unsigned populationSize;
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

    // modulation node
    unsigned modulationNodeFrequencyRange;

    // delay node
    unsigned delayNodeMaxBufferSize;
    double delayNodeBufferMaxSeconds;

    // filter node
    double filterNodeCenterFrequencyMinimum;
    double filterNodeCenterFrequencyMaximum;
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
