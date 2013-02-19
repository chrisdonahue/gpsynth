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
#include <stdio.h>
#include <iostream>
#include "Synth/GPNetwork.h"
#include <stdlib.h>

class GPSynth {
    public:
        GPSynth(int psize, int seed);
        ~GPSynth();

        void prevGeneration();
        void nextGeneration();

        GPNodeParams* nodeParams;

    private:
        void initPopulation();
        GPNetwork* generateInitialNetwork();
        GPNode* getRandomNode();

        // RNG
        GPRandom* rng;

        // experiment state
        int populationSize;
        int nextNetworkID;
        int generationID;
        double maxFitness;

        // mutation rates
        float nodeAddChance;
        float nodeMutationChance;
        float crossoverChance;

        // network containers
        std::vector<GPNetwork*> allNetworks;
        std::vector<GPNetwork*> upForEvaluation;
        std::vector<std::pair<GPNetwork*, double> > evaluated;
};

#endif
