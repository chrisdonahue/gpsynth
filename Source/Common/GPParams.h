#ifndef GPPARAMS_H
#define GPPARAMS_H

#include "GPRandom.h"

struct GPParams {
    // auxillary params
    unsigned waveFileBufferSize;
    GPRandom* rng;

    // experiment parameters
    unsigned experimentNumber;
    std::vector<double>* variableValues;
    unsigned numGenerations;
    double thresholdFitness;
    unsigned fitnessFunctionType;

    // synth evolution parameters
    unsigned populationSize;
    bool lowerFitnessIsBetter;
    unsigned bestPossibleFitness;
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

    // function node
    std::vector<GPFunction*>* availableGPFunctions;

    // oscil node
    double oscilNodePartialChance;
    unsigned oscilNodeNumPartials;

    // modulation node
    unsigned modulationNodeFrequencyRange;

    // delay node
    double delayNodeBufferMaxSeconds;

    // filter node
    double filterNodeBufferMaxSeconds;

    // multiple nodes
    unsigned numVariables;
};

#endif
