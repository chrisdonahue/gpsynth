/*
  ==============================================================================

    GPExperiment.h
    Created: 6 Feb 2013 7:19:38pm
    Author:  cdonahue

  ==============================================================================
*/

#include <vector>
#include <map>
#include <stdio.h>
#include <iostream>
#include "Synth/GPNetwork.h"

class GPSynth {
    public:
        GPSynth(int psize);
        ~GPSynth();

        void initPopulation();
        void prevGeneration();
        void nextGeneration();

        // experiment probabilities
        float mutationChance;
        float simplifyChance;
        float specialChance;
        float harmonyChance;
        float functionChance;
        float crossChance;

        // experiment parameters
        int valueRange;
        int LFORange;
        int numPartials;

    private:
        // experiment state
        int populationSize;
        int nextNetworkID;
        int generationID;
        double maxFitness;

        // network containers
        std::vector<GPNetwork*> allNetworks;
        std::map<GPNetwork*, double> upForEvaluation;
        std::vector<GPNetwork*> evaluated;
};
