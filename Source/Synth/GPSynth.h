/*
  ==============================================================================

    GPSynth.h
    Created: 6 Feb 2013 7:19:38pm
    Author:  cdonahue

  ==============================================================================
*/

#ifndef GPSYNTH_H
#define GPSYNTH_H

#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include "GPNetwork.h"
#include "Nodes/FunctionNode.h"

class GPSynth {
public:
    // CONSTRUCTION
    GPSynth(GPRandom* r, GPParams* p, std::vector<GPNode*>* nodes);
    ~GPSynth();

    // EVOLUTION CONTROL
    GPNetwork* getIndividual();
    std::vector<GPNetwork*>* getIndividuals(unsigned n);
    int assignFitness(GPNetwork* net, double fitness);
    int prevGeneration();
    void printGenerationDelim();
    void printGenerationSummary();

    // NODE MUTATION PARAMS
    GPParams* params;

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
    GPNetwork* crossover(GPNetwork* one, GPNetwork* two);
    void numericallyMutate(GPNetwork* one);
    void mutate(GPNetwork* one);

    // SYNTH EVOLUTION STATE
    GPRandom* rng;
    int nextNetworkID;
    int generationID;

    // EVOLUTION PARAMS
    unsigned populationSize;
    double generationAverageFitness;
    double generationBestFitness;
    bool lowerFitnessIsBetter;
    double bestPossibleFitness;
    unsigned maxInitialHeight;
    unsigned maxHeight;
    unsigned mutationType;
    unsigned crossoverType;

    // CONVERGENCE-VARYING
    double convergenceFactor;
    std::vector<double*> continuousConvergenceVaryingParams;
    std::vector<double*> continuousCongergenceVaryingTemperatures;

    // AVAILABLE CONTAINERS
    std::vector<GPNode*>* availableNodes;
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
