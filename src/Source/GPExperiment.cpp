/*
  ==============================================================================

    GPExperiment.cpp
    Created: 6 Feb 2013 7:19:38pm
    Author:  cdonahue

  ==============================================================================
*/

#include "GPExperiment.h"

GPSynth::GPSynth(int psize) :
allNetworks(), upForEvaluation(), evaluated()
{
    populationSize = 50;
    nextNetworkID = 0;
    generationID = 0;

    valueRange = 1;
    LFORange = 10;
    numPartials = 10;

    mutationChance = 0.5;
    simplifyChance = 0.5;
    specialChance = 0.5;
    harmonyChance = 0.5;
    functionChance = 0.5;
    crossChance = 0.5;

    maxFitness = 0;
}

GPSynth::~GPSynth() {
    delete &allNetworks;
    delete &upForEvaluation;
    delete &evaluated;
}

void GPSynth::initPopulation() {
    GPNetwork* newnet;
    for (int i = 0; i < populationSize; i++) {
        GPNode* osc;
        newnet = new GPNetwork(nextNetworkID++, osc);
        allNetworks.push_back(newnet);
        upForEvaluation.insert(std::pair<GPNetwork*, double>(newnet, -1.0));
    }
}

void GPSynth::prevGeneration() {
    if (generationID == 0) {
        std::cerr << "Attempted to revert to a previous generation during generation 0" << std::endl;
        return;
    }
    // clear upForEvaluation
    // delete past populationSize network
    for (int i = 0; i < populationSize; i++) {
        // push back networks to upForEvaluation
    }
    // clear last few in evaluated
    generationID--;
}

void GPSynth::nextGeneration() {
    for (std::map<GPNetwork*, double>::iterator i = upForEvaluation.begin(); i != upForEvaluation.end(); i++) {
        if (i->second < 0) {
            std::cerr << "Negative fitness value detected when attempting to advance generation" << std::endl;
            return;
        }
        // SET UP PROBABILITIES BASED ON FITNESS
    }

        /*
    for (std::map<GPNode*, double>::iterator i = upForEvaluation.begin(); i != upForEvaluation.end(); i++) {
        GPNode* dad = random sample based on probabilities
        GPNode* one = dad->getCopy();
        GPNode* offspring = one;

        if (mutation):
            mutate(one);

        if (crossover) 
            GPNode* mom = random sample based on probabilities
            while (dad == mom) {
                mom = random sample based on probabilities
            }
            two = mom->getCopy()
            if (mutation)
                mutate(two)
            offspring = reproduce(one, two);

        networks.push_back(offspring);
        upForEvaluation.push_back(offspring);
    }

        */
    evaluated.clear();
    generationID++;
}
