#ifndef GPPARAMS_H
#define GPPARAMS_H

#include "GPRandom.h"
#include "GPFunctions.h"

struct GPParams {
    // auxillary params
    bool verbose;
    unsigned wavFileBufferSize;
    unsigned renderBlockSize;
    unsigned fftSize;

    // experiment parameters
    unsigned experimentNumber;
    unsigned fitnessFunctionType;
    unsigned numGenerations;
    double thresholdFitness;

    // synth evolution parameters
    unsigned populationSize;
    bool lowerFitnessIsBetter;
    double bestPossibleFitness;
    double penaltyFitness;
    unsigned maxInitialDepth;
    unsigned maxDepth;

    // synth genetic parameters
    double proportionOfPopulationFromCrossover;
    double proportionOfPopulationForGreedySelection;
    unsigned reproductionSelectionType;
    unsigned crossoverType;
    unsigned crossoverSelectionType;

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

    // multiple nodes
    unsigned numVariables;
};

#endif
