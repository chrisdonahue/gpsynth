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
#include <stdlib.h>

class GPSynth {
    public:
        // CONSTRUCTION
        GPSynth(unsigned psize, unsigned s, double max, double addchance, double mutatechance, double crosschance, unsigned crosstype, unsigned selecttype, std::vector<GPNode*>* nodes, std::vector<double>* nlikelihoods, std::vector<GPFunction*>* functions, std::vector<double>* flikelihoods);
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
        float nodeAddChance;
        float nodeMutateChance;
        float crossoverChance;

        // NETWORK CONTAINERS
        std::vector<GPNetwork*> allNetworks;
        std::vector<GPNetwork*> upForEvaluation;
        std::vector<std::pair<GPNetwork*, double> > evaluated;
};

#endif
