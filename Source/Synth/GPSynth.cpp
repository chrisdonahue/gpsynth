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
populationSize(pszie),
nextNetworkID(0), generationID(0), maxFitness(max), lowerIsBetter(lowerbetter),
nodeAddChance(addchance), nodeMutateChance(mutatechance), crossoverChance(crosschance),
allNetworks(), upForEvaluation(), evaluated(),
engine(seed), uni_real(0.0, 1.0)
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

    nodeParams->rng = new GPRandom(s);

    nodeParams->availableNodes = nodes;
    nodeParams->availableFunctions = functions;
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
    std::find finder(upForEvaluation.begin(), upForEvaluation.end(), net);
    if (finder == upForEvaluation.end()) {
        evaluated.push_back(std::pair<GPNetwork*, double>(evaluated, fitness));
        upForEvaluation.erase(finder);
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
        upForEvaluation.insert(allNetworks.pop_back());
    }
    generationID--;
    return generationID;
}

int GPSynth::nextGeneration() {
    std::vector<double> fitnessWeights();
    for (std::map<GPNetwork*, double>::iterator i = evaluated.begin(); i != evaluated.end(); i++) {
        if (i->second < 0) {
            std::cerr << "Negative fitness value detected when attempting to advance generation" << std::endl;
            return generationID;
        }
        fitnessWeights.push_back(i->second);
    }

    normalizeDistribution(&fitnessWeights);

    for (int i = 0; i < populationSize; i++) {
        GPNetwork* dad = evaluated[sampleFromDistribution(&fitnessWeights)].first;
        GPNetwork* one = dad->getCopy();
        GPNetwork* offspring = one;

        if (random() < nodeAddChance) {
            one->mutateAddNode();
        }
        if (random() < nodeMutationChance) {
            one->mutate(nodeParams);
        }
        if (random() < crossoverChance) {
            GPNetwork* mom = evaluated[sampleFromDistribution(&fitnessWeights)].first;
            while (dad == mom) {
                mom = evaluated[sampleFromDistribution(&fitnessWeights)].first;
            }
            GPNetwork* two = mom->getCopy();
            if (random() < nodeAddChance) {
                two>mutateAddNode(getRandomNode());
            }
            if (random() < nodeMutationChance) {
                two->mutate(nodeParams);
            }
            offspring = reproduce(one, two);
        }

        offspring.ID = nextNetworkID++;

        allNetworks.push_back(offspring);
        upForEvaluation.push_back(offspring);
    }

    evaluated.clear();
    generationID++;
    return generationID;
}
