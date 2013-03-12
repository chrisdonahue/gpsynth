#ifndef GPPARAMS_H
#define GPPARAMS_H

#include "GPRandom.h"
#include "GPFunctions.h"

struct GPParams {
    // auxillary params
    unsigned wavFileBufferSize;
    unsigned evaluationBlockSize;
    unsigned fftSize;
    GPRandom* rng;

    // experiment parameters
    bool verbose;
    unsigned experimentNumber;
    std::vector<double>* variableValues;
    unsigned numGenerations;
    double thresholdFitness;
    unsigned fitnessFunctionType;

    // synth evolution parameters
    unsigned populationSize;
    bool lowerFitnessIsBetter;
    double bestPossibleFitness;
    double silenceFitness;
    unsigned maxInitialDepth;
    unsigned maxDepth;

    // synth genetic parameters
    double mutationDuringInitializationChance;
    double proportionOfPopulationFromCrossover;
    unsigned reproductionSelectionType;
    unsigned crossoverType;
    unsigned crossoverSelectionType;
    double proportionOfPopulationForGreedySelection;
    double mutationDuringCrossoverChance;

    // value node
    double valueNodeMinimum;
    double valueNodeMaximum;

    // oscil node
    double oscilNodePartialChance;
    unsigned oscilNodeNumPartials;

    // modulation node
    unsigned modulationNodeFrequencyRange;

    // delay node
    unsigned delayNodeMaxBufferSize;
    double delayNodeBufferMaxSeconds;

    // filter node
    unsigned filterNodeMaxBufferSize;
    double filterNodeBufferMaxSeconds;

    // noise node
    unsigned noiseNodeMaxBufferSize;
    double noiseNodeBufferMaxSeconds;

    // multiple nodes
    unsigned numVariables;
    double mutateRight;
};

#endif
