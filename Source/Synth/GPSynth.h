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
#include "Nodes/ValueNode.h"
#include "Nodes/OscilNode.h"
#include "GPFunctions.h"

class GPSynth {
    public:
        // CONSTRUCTION
        GPSynth(unsigned psize, double best, bool lowerbetter, unsigned mid, unsigned md, unsigned crosstype, unsigned selecttype, double crosspercent, double addchance, double subchance, double mutatechance, std::vector<GPNode*>* nodes, GPNodeParams* p);
        ~GPSynth();

        // EVOLUTION CONTROL
        int assignFitness(GPNetwork* net, double fitness);
        int prevGeneration();
        int nextGeneration();
        GPNetwork* getIndividual();

        // CROSSOVER
        GPNetwork* reproduce(GPNetwork* one, GPNetwork* two);

        // NODE MUTATION PARAMS
        GPNodeParams* nodeParams;

    private:
        // CONSTRUCTION
        GPNetwork* full(unsigned d);
        GPNetworK* grow(unsigned m);
        void initPopulation();

        // PRIVATE HELPERS
        GPNode* getRandomNode();
        void addNetworkToPopulation(GPNetwork* net);
        GPNetwork* selectFromEvaluated();

        // SYNTH EVOLUTION STATE
        GPRandom* rng;
        int nextNetworkID;
        int generationID;
        int currentIndividualNumber;

        // EVOLUTION PARAMS
        int populationSize;
        double bestPossibleFitness;
        bool lowerFitnessIsBetter;
        unsigned maxInitialDepth;
        unsigned maxDepth;
        unsigned crossoverType;
        unsigned reproductionSelectionType;
        unsigned crossoverSelectionType;
        double crossoverProportion;

        // NETWORK MUTATION RATES
        double nodeAddChance;
        double nodeRemoveChance;
        double nodeMutateChance;

        // AVAILABLE CONTAINERS
        std::vector<GPNode*>* availableNodes;
        std::vector<GPNode*> avaibleFunctions;
        std::vector<GPNode*> availableTerminals;

        // NETWORK CONTAINERS
        std::vector<GPNetwork*> allNetworks;
        std::vector<GPNetwork*> upForEvaluation;
        std::vector<GPNetwork*> evaluated;

        // SELECTION CONTAINERS
        std::vector<double> rawFitnesses;
        std::vector<double> normalizedFitnesses;
        std::vector<int> rank;
};

#endif
