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
        GPSynth(unsigned psize, double max, GPNodeParams* p, double addchance, double subchance, double mutatechance, double crosschance, unsigned crosstype, unsigned selecttype);
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
        GPNetwork* generateInitialNetwork();
        void initPopulation();
        GPNode* getRandomNode();

        // EVOLUTION CONTROL
        GPNetwork* selectFromEvaluated();

        // SYNTH EVOLUTION STATE
        int populationSize;
        int nextNetworkID;
        int generationID;
        double maxFitness;
        int crossoverType;
        int selectionType;
        int currentIndividualNumber;

        // NETWORK MUTATION RATES
        double nodeAddChance;
        double nodeRemoveChance;
        double nodeMutateChance;
        double crossoverChance;

        // NETWORK CONTAINERS
        std::vector<GPNetwork*> allNetworks;
        std::vector<GPNetwork*> upForEvaluation;
        std::vector<GPNetwork*> evaluated;
        std::vector<double> fitnesses;
        std::vector<int> rank;
};

#endif
