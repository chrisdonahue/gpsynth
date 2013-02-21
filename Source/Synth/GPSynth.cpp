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

GPSynth::GPSynth(unsigned psize, unsigned s, double max, double addchance, double subchance, double mutatechance, double crosschance, unsigned crosstype, unsigned selecttype, std::vector<GPNode*>* nodes, std::vector<double>* nlikelihoods, std::vector<GPFunction*>* functions, std::vector<double>* flikelihoods) :
populationSize(psize),
nextNetworkID(0), generationID(0), maxFitness(max),
crossoverType(crosstype), selectionType(selecttype),
nodeAddChance(addchance), nodeRemoveChance(subchance), nodeMutateChance(mutatechance), crossoverChance(crosschance),
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
    nodeParams->nodeLikelihoods = nlikelihoods;
    nodeParams->availableFunctions = functions;
    nodeParams->functionLikelihoods = flikelihoods;

    initPopulation();
}

GPSynth::~GPSynth() {
    delete &allNetworks;
    delete &upForEvaluation;
    delete &evaluated;
    free(nodeParams);
}

GPNode* GPSynth::getRandomNode() {
    return nodeParams->availableNodes->at(nodeParams->rng->sampleFromDistribution(nodeParams->nodeLikelihoods));
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
    currentIndividualNumber = 0;
}

/*
   =================
   EVOLUTION CONTROL
   =================
*/

int GPSynth::assignFitness(GPNetwork* net, double fitness) {
    bool badPointer = true;
    for (int i = 0; i < upForEvaluation.size(); i++) {
        if (net == upForEvaluation[i]) {
            evaluated.push_back(std::make_pair(net, fitness));
            upForEvaluation.erase(upForEvaluation.begin() + i);
            currentIndividualNumber++;
            badPointer = false;
        }
    }
    if (badPointer) {
        std::cerr << "Assigned fitness for a pointer not in upForEvaluation. This shouldn't happen." << std::endl;
        return -1;
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
    currentIndividualNumber = 0;
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

    rng.normalizeDistribution(&fitnessWeights);

    for (int i = 0; i < populationSize; i++) {
        GPNetwork* dad = evaluated[rng.sampleFromDistribution(&fitnessWeights)].first;
        GPNetwork* one = dad->getCopy();
        GPNetwork* offspring = one;

        if (rng.random() < nodeAddChance) {
            one->mutateAddNode(nodeParams, getRandomNode());
        }
        if (rng.random() < nodeRemoveChance) {
            one->mutateRemoveNode(nodeParams);
        }
        if (rng.random() < nodeMutateChance) {
            one->mutate(nodeParams);
        }
        if (rng.random() < crossoverChance) {
            GPNetwork* mom = evaluated[rng.sampleFromDistribution(&fitnessWeights)].first;
            while (dad == mom) {
                mom = evaluated[rng.sampleFromDistribution(&fitnessWeights)].first;
            }
            GPNetwork* two = mom->getCopy();
            if (rng.random() < nodeAddChance) {
                two->mutateAddNode(nodeParams, getRandomNode());
            }
            if (rng.random() < nodeRemoveChance) {
                two->mutateRemoveNode(nodeParams);
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
    if (currentIndividualNumber == populationSize) {
        currentIndividualNumber = 0;
        nextGeneration();
    }
    return upForEvaluation[currentIndividualNumber];
}

GPNetwork* GPSynth::selectFromEvaluated() {
    //http://en.wikipedia.org/wiki/Selection_%28genetic_algorithm%29
    if (selectionType == 0) {
        // ranking linear selection (lower better)
        return NULL;
    }
    else if (selectionType == 1) {
        // ranking linear selection (higher better)
        return NULL;
    }
    else if (selectionType == 2) {
        // ranking curved selection (lower better)
        return NULL;
    }
    else if (selectionType == 3) {
        // ranking curved selection (higher better)
        return NULL;
    }
    else if (selectionType == 4) {
        // tournament selection (lower better)
        return NULL;
    }
    else if (selectionType == 5) {
        // tournament selection (higher better)
        return NULL;
    }
    else if (selectionType == 6) {
        // fitness proportionate selection (lower better)
        return NULL;
    }
    else if (selectionType == 7) {
        // fitness proportionate selection (higher better)
        return NULL;
    }
    else if (selectionType == 8) {
        // stochastic universal sampling selection (higher better)
        return NULL;
    }
    return NULL;
}

/*
    =========
    CROSSOVER
    =========
*/

GPNetwork* GPSynth::reproduce(GPNetwork* one, GPNetwork* two) {
    if (crossoverType == 0) {
        GPNode* newroot = new FunctionNode(multiply, "*", one->getRoot(), two->getRoot());
        GPNetwork* newnet = new GPNetwork(-1, newroot);
        return newnet; 
    }
    else if (crossoverType == 1) {
        return one;
    }
    else if (crossoverType == 2) {
        return two;
    }
    // standard GP crossover
    else if (crossoverType == 3) {
        return NULL;
    }
    // array crossover
    else if (crossoverType == 4) {
        return NULL;
    }
    else {
        return NULL;
    }
}
