/*
  ==============================================================================

    GPSynth.cpp
    Created: 6 Feb 2013 7:19:38pm
    Author:  cdonahue

  ==============================================================================
*/

#include "GPSynth.h"

/*
   ============
   CONSTRUCTION
   ============
*/

GPSynth::GPSynth(int psize, unsigned s, double max, bool lowerbetter, double addchance, double mutatechance, double crosschance, std::vector<GPNode*>* nodes, std::vector<double>* nlikelihoods, std::vector<GPFunction*>* functions, std::vector<double>* flikelihoods) :
populationSize(psize),
nextNetworkID(0), generationID(0), maxFitness(max), lowerIsBetter(lowerbetter),
nodeAddChance(addchance), nodeMutateChance(mutatechance), crossoverChance(crosschance),
allNetworks(), upForEvaluation(), evaluated(),
rng(s)
{
    nodeParams = (GPNodeParams*) malloc(sizeof(GPNodeParams));

    nodeParams->numVariables = 1;

    nodeParams->valueRange = 1;
    nodeParams->LFORange = 10;
    nodeParams->numPartials = 10;

    nodeParams->simplifyChance = 0.5;
    nodeParams->specialChance = 0.5;
    nodeParams->harmonyChance = 0.5;
    nodeParams->functionChance = 0.5;

    nodeParams->rng = &rng;

    nodeParams->availableNodes = nodes;
    nodeParams->availableFunctions = functions;
}

GPSynth::~GPSynth() {
    delete &allNetworks;
    delete &upForEvaluation;
    delete &evaluated;
    free(nodeParams);
}

GPNode* GPSynth::getRandomNode() {
    return NULL;
}

GPNetwork* GPSynth::generateInitialNetwork() {
    GPNode* init = getRandomNode();
    return new GPNetwork(nextNetworkID++, init);
}

void GPSynth::initPopulation() {
    GPNetwork* newnet;
    for (int i = 0; i < populationSize; i++) {
        newnet = generateInitialNetwork();
        allNetworks.push_back(newnet);
        upForEvaluation.push_back(newnet);
    }
}

/*
   =================
   EVOLUTION CONTROL
   =================
*/

int GPSynth::assignFitness(GPNetwork* net, double fitness) {
    for (int i = 0; i < upForEvaluation.size(); i++) {
        if (net == upForEvaluation[i]) {
            evaluated.push_back(std::make_pair(net, fitness));
            upForEvaluation.erase(upForEvaluation.begin() + i);
        }
    }
    return generationID;
}

int GPSynth::prevGeneration() {
    if (generationID == 0) {
        std::cerr << "Attempted to revert to a previous generation during generation 0" << std::endl;
        return generationID;
    }
    upForEvaluation.clear();
    evaluated.clear();
    for (int i = 0; i < populationSize; i++) {
        upForEvaluation.push_back(allNetworks.back());
        allNetworks.erase(allNetworks.end() - 1);
    }
    generationID--;
    return generationID;
}

int GPSynth::nextGeneration() {
    std::vector<double> fitnessWeights;
    for (std::vector<std::pair<GPNetwork*, double> >::iterator i = evaluated.begin(); i != evaluated.end(); i++) {
        if (i->second < 0) {
            std::cerr << "Negative fitness value detected when attempting to advance generation" << std::endl;
            return generationID;
        }
        fitnessWeights.push_back(i->second);
    }

    rng.normalizeDistribution(lowerIsBetter, &fitnessWeights);

    for (int i = 0; i < populationSize; i++) {
        GPNetwork* dad = evaluated[rng.sampleFromDistribution(lowerIsBetter, &fitnessWeights)].first;
        GPNetwork* one = dad->getCopy();
        GPNetwork* offspring = one;

        if (rng.random() < nodeAddChance) {
            one->mutateAddNode(nodeParams, getRandomNode());
        }
        if (rng.random() < nodeMutateChance) {
            one->mutate(nodeParams);
        }
        if (rng.random() < crossoverChance) {
            GPNetwork* mom = evaluated[rng.sampleFromDistribution(lowerIsBetter, &fitnessWeights)].first;
            while (dad == mom) {
                mom = evaluated[rng.sampleFromDistribution(lowerIsBetter, &fitnessWeights)].first;
            }
            GPNetwork* two = mom->getCopy();
            if (rng.random() < nodeAddChance) {
                two->mutateAddNode(nodeParams, getRandomNode());
            }
            if (rng.random() < nodeMutateChance) {
                two->mutate(nodeParams);
            }
            offspring = reproduce(one, two);
        }

        offspring->ID = nextNetworkID++;

        allNetworks.push_back(offspring);
        upForEvaluation.push_back(offspring);
    }

    evaluated.clear();
    generationID++;
    return generationID;
}

GPNetwork* GPSynth::getIndividual() {
    return NULL;
}

/*
    =========
    CROSSOVER
    =========
*/

GPNetwork* GPSynth::reproduce(GPNetwork* one, GPNetwork* two) {
    return NULL;
}
