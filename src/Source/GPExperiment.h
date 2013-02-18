/*
  ==============================================================================

    GPExperiment.h
    Created: 6 Feb 2013 7:19:38pm
    Author:  cdonahue

  ==============================================================================
*/

#ifndef GPEXPERIMENT_H
#define GPEXPERIMENT_H

#include <vector>
#include <map>
#include <stdio.h>
#include <iostream>
#include "Synth/GPNetwork.h"
#include <stdlib.h>

class GPSynth {
    public:
        GPSynth(int psize);
        ~GPSynth();

        void prevGeneration();
        void nextGeneration();

        GPNodeParams* nodeParams;

    private:
        void initPopulation();
        GPNetwork* generateInitialNetwork();
        GPNode* getRandomNode();

        // experiment state
        int populationSize;
        int nextNetworkID;
        int generationID;
        double maxFitness;

        float nodeAddChance;
        float nodeMutationChance;
        float crossoverChance;

        // network containers
        std::vector<GPNetwork*> allNetworks;
        std::vector<GPNetwork*> upForEvaluation;
        std::vector<std::pair<GPNetwork*, double> > evaluated;
};

#endif
