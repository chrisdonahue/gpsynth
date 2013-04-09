#ifndef GPPARAMS_H
#define GPPARAMS_H

#include "GPRandom.h"
#include "GPFunctions.h"

struct GPParams {
    // auxillary parameters
    bool verbose;
    bool saveGenerationChampions;
    unsigned printPrecision;
    unsigned backupPrecision;
    unsigned savePrecision;
    unsigned wavFileBufferSize;
    unsigned renderBlockSize;
    unsigned fftSize;

    // fitness function parameters
    unsigned fitnessFunctionType;
    unsigned penalizeBadMagnitude;
    unsigned penalizeBadPhase;
    double magnitudeWeight;
    double phaseWeight;
    double goodComparisonFactor;
    double badComparisonFactor;
    double baseComparisonFactor;

    // experiment parameters
    unsigned experimentNumber;
    unsigned envelopeIterations;
    unsigned numGenerations;
    double thresholdFitness;
    bool ephemeralRandomConstants;

    // synth evolution parameters
    bool backupAllNetworks;
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
