#ifndef GPSYNTH_H
#define GPSYNTH_H

#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include "../Common/GPLogger.h"
#include "../Common/GPRandom.h"
#include "GPNetwork.h"

struct GPSynthParams {
    // synth evolution parameters
    unsigned population_size;
    unsigned max_initial_height;
    unsigned max_height;
    bool erc;
    bool backup_all_networks;
    unsigned backup_precision;

    // fitness landscape parameters
    double best_possible_fitness;
    bool lower_fitness_is_better;

    // synth genetic parameters
    // numeric mutation
    double nm_proportion;
    double nm_temperature;
    unsigned nm_selection_type;
    double nm_selection_percentile;
    // mutation
    double mu_proportion;
    unsigned mu_type;
    unsigned mu_selection_type;
    double mu_selection_percentile;
    // crossover
    double x_proportion;
    unsigned x_type;
    unsigned x_selection_type;
    double x_selection_percentile;
    // reproduction
    double re_proportion;
    unsigned re_selection_type;
    double re_selection_percentile;
    // random new individual
    double new_proportion;
    unsigned new_type;
};

class GPSynth {
public:
    // CONSTRUCTION
    GPSynth(GPLogger* logger, GPSynthParams* params, GPRandom* rng, std::vector<GPNode*>* primitives);
    ~GPSynth();

    // EVOLUTION CONTROL
    GPNetwork* getIndividual();
    void getIndividuals(std::vector<GPNetwork*>& networks);
	bool replaceIndividual(GPNetwork* old, GPNetwork* nu);
    int assignFitness(GPNetwork* net, double fitness);
    int prevGeneration();
    void endGeneration();
    void printGenerationDelim();
    void printGenerationSummary();
    void printEvolutionSummary();

    // PUBLIC EVOLUTION STATE
    int currentGenerationNumber;
    GPNetwork* generationChamp;
    GPNetwork* champ;

private:
    // CONSTRUCTION
    GPNode* fullRecursive(unsigned cd, unsigned d);
    GPNetwork* full(unsigned d);
    GPNode* growRecursive(unsigned cd, unsigned m);
    GPNetwork* grow(unsigned m);
    void initPopulation();

    // EVOLUTION CONTROL
    int nextGeneration();
    void calculateGenerationRanks();
    void calculateGenerationNormalizedFitnesses();

    // HELPERS
    void addNetworkToPopulation(GPNetwork* net);
    void clearGenerationState();
    GPNetwork* selectFromEvaluated(unsigned selectionType, unsigned parameter);

    // GENETIC OPERATIONS
    GPNetwork* crossover(unsigned crossoverType, GPNetwork* one, GPNetwork* two);
    void mutate(unsigned mutationType, GPNetwork* one);
    void numericallyMutate(GPNetwork* one);
    GPNetwork* newIndividual(unsigned new_type);

    // PRIVATE EVOLUTION STATE
    GPLogger* logger;
    GPSynthParams* params;
    GPRandom* rng;
    unsigned populationSize;
    int nextNetworkID;
    bool lowerFitnessIsBetter;
    double overallBestFitness;
    double generationBestFitness;
    double generationAverageFitness;
    bool currentGenerationAlive;

    // CONVERGENCE-VARYING
    double convergenceFactor;
    std::vector<double*> continuousConvergenceVaryingParams;
    std::vector<double*> continuousConvergenceVaryingTemperatures;

    // AVAILABLE CONTAINERS
    std::vector<GPNode*>* availablePrimitives;
    std::vector<GPNode*>* availableFunctions;
    std::vector<GPNode*>* availableTerminals;

    // NETWORK CONTAINERS
    std::vector<std::string*> allNetworks;
    std::set<GPNetwork*> unevaluated;
    std::set<GPNetwork*> evaluated;
    std::map<int, GPNetwork*> currentGeneration;

    // SELECTION CONTAINERS
    std::vector<double> rawFitnesses;
    std::vector<double> normalizedFitnesses;
    std::vector<GPNetwork*> rank;
};

// EXTERNAL COMPARATORS
extern bool compareFitnessesLower(GPNetwork* one, GPNetwork* two);
extern bool compareFitnessesHigher(GPNetwork* one, GPNetwork* two);

#endif
