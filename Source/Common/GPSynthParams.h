#ifndef GPPARAMS_H
#define GPPARAMS_H

#include "GPRandom.h"

struct GPParams {
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
};

#endif
