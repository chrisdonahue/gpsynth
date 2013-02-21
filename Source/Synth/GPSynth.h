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
#include "GPNetwork.h"
#include "Nodes/FunctionNode.h"
#include "Nodes/ValueNode.h"
#include "Nodes/OscilNode.h"
#include "GPFunctions.h"
#include <stdlib.h>

class GPSynth {
    public:
        // CONSTRUCTION
        GPSynth(unsigned psize, unsigned s, double max, GPNodeParams* p, double addchance, double subchance, double mutatechance, double crosschance, unsigned crosstype, unsigned selecttype);
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

        // RNG
        GPRandom rng;

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
        std::vector<std::pair<GPNetwork*, double> > evaluated;
};

#endif
