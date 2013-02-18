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
    nodeParams = (GPNodeParams*) malloc(sizeof(GPNodeParams));

    populationSize = 50;
    nextNetworkID = 0;
    generationID = 0;

    nodeParams->numVariables = 1;

    nodeParams->valueRange = 1;
    nodeParams->LFORange = 10;
    nodeParams->numPartials = 10;

    nodeParams->simplifyChance = 0.5;
    nodeParams->specialChance = 0.5;
    nodeParams->harmonyChance = 0.5;
    nodeParams->functionChance = 0.5;

    nodeAddChance = 0.5;
    nodeMutateChance = 0.5;
    crossoverChance = 0.5;

    maxFitness = 0;
}

GPSynth::~GPSynth() {
    delete &allNetworks;
    delete &upForEvaluation;
    delete &evaluated;
    free(nodeParams);
}

GPNetwork* GPSynth::generateInitialNetwork() {
    GPNode* init = getRandomNode();
    newnet = new GPNetwork(nextNetworkID++, init);
}

GPNode* GPSynth::getRandomNode() {

}

void GPSynth::initPopulation() {
    GPNetwork* newnet;
    for (int i = 0; i < populationSize; i++) {
        newnet = generateInitialNetwork();
        allNetworks.push_back(newnet);
        upForEvaluation.push_back(newnet);
        //upForEvaluation.insert(std::pair<GPNetwork*, double>(newnet, -1.0));
    }
}

void GPSynth::prevGeneration() {
    if (generationID == 0) {
        std::cerr << "Attempted to revert to a previous generation during generation 0" << std::endl;
        return;
    }
    upForEvaluation.clear();
    evaluated.clear();
    for (int i = 0; i < populationSize; i++) {
        upForEvaluation.insert(allNetworks.pop_back());
    }
    generationID--;
}

void GPSynth::normalizeDistribution(std::vector<double>* weights) {
    double sum = 0;
    for (std::vector<double>::iterator i = weights->begin(); i != weights->end(); i++) {
        sum += *i;
    }
    for (std::vector<double>::iterator i = weights->begin(); i != weights->end(); i++) {
        *i = (*i) / sum;
    }
}

int GPSynth::sampleFromDistribution(std::vector<double>* weights) {
    // sample from normalized distribution
    return -1;
}

void GPSynth::nextGeneration() {
    std::vector<double> fitnessWeights();
    for (std::map<GPNetwork*, double>::iterator i = evaluated.begin(); i != evaluated.end(); i++) {
        if (i->second < 0) {
            std::cerr << "Negative fitness value detected when attempting to advance generation" << std::endl;
            return;
        }
        fitnessWeights.push_back(i->second);
        // SET UP PROBABILITIES BASED ON FITNESS
    }

    normalizeDistribution(fitnessWeights);

    /*
    for (std::map<GPNode*, double>::iterator i = evaluated.begin(); i != upForEvaluation.end(); i++) {
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
