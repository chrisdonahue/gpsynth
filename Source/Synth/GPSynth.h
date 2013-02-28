/*
  ==============================================================================

    GPSynth.h
    Created: 6 Feb 2013 7:19:38pm
    Author:  cdonahue

  ==============================================================================
*/

#ifndef GPSYNTH_H
#define GPSYNTH_H

#include <vector>
#include <map>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include "GPNetwork.h"
#include "Nodes/FunctionNode.h"

class GPSynth {
    public:
        // CONSTRUCTION
        GPSynth(GPParams* p, std::vector<GPNode*>* nodes);
        ~GPSynth();

        // EVOLUTION CONTROL
        GPNetwork* getIndividual();
        int assignFitness(GPNetwork* net, double fitness);
        int prevGeneration();
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

        // HELPERS
        void addNetworkToPopulation(GPNetwork* net);
        void clearGenerationState();
        GPNetwork* selectFromEvaluated(unsigned selectionType);
        
        // CROSSOVER
        GPNetwork* reproduce(GPNetwork* one, GPNetwork* two);

        // SYNTH EVOLUTION STATE
        GPRandom* rng;
        int nextNetworkID;
        int generationID;
        int currentIndividualNumber;

        // EVOLUTION PARAMS
        int populationSize;
        bool lowerFitnessIsBetter;
        double bestPossibleFitness;
        unsigned maxInitialDepth;
        unsigned maxDepth;

        // GENETIC PARAMS
        double mutationDuringInitializationChance;
        double proportionOfPopulationFromCrossover;
        unsigned reproductionSelectionType;
        unsigned crossoverType;
        unsigned crossoverSelectionType;
        double mutationDuringCrossoverChance;

        // AVAILABLE CONTAINERS
        std::vector<GPNode*>* availableNodes;
        std::vector<GPNode*>* availableFunctions;
        std::vector<GPNode*>* availableTerminals;

        // NETWORK CONTAINERS
        std::vector<std::string*> allNetworks;
        std::vector<GPNetwork*> upForEvaluation;
        std::vector<GPNetwork*> evaluated;

        // SELECTION CONTAINERS
        std::vector<double> rawFitnesses;
        std::vector<double> normalizedFitnesses;
        std::vector<int> rank;
};

#endif
