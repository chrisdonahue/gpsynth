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
#include "../Common/GPSynthParams.h"
#include "GPNetwork.h"

class GPSynth {
public:
    // CONSTRUCTION
    GPSynth(GPParams* p, GPRandom* r, std::vector<GPNode*>* nodes);
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

    // PRIVATE EVOLUTION STATE
    GPParams* params;
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
