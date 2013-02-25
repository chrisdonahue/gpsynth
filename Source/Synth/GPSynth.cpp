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

GPSynth::GPSynth(unsigned psize, double max, GPNodeParams* p, double addchance, double subchance, double mutatechance, double crosschance, unsigned crosstype, unsigned selecttype) :
populationSize(psize),
nextNetworkID(0), generationID(0), maxFitness(max),
crossoverType(crosstype), selectionType(selecttype),
nodeAddChance(addchance), nodeRemoveChance(subchance), nodeMutateChance(mutatechance), crossoverChance(crosschance),
allNetworks(), upForEvaluation(), evaluated(), fitnesses()
{
    nodeParams = p;
    std::cout << "Initializing population of size " << populationSize << " with best fitness " << max << std::endl;
    printGenerationDelim();
    currentIndividualNumber = 0;
    initPopulation();
}

GPSynth::~GPSynth() {
    delete &allNetworks;
    delete &upForEvaluation;
    delete &evaluated;
    free(nodeParams);
}

GPNode* GPSynth::getRandomNode() {
    return (nodeParams->availableNodes->at(nodeParams->rng->sampleFromDistribution(nodeParams->nodeLikelihoods)))->getCopy();
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
    bool badPointer = true;
    for (int i = 0; i < upForEvaluation.size(); i++) {
        if (net == upForEvaluation[i]) {
            evaluated.push_back(net);
            fitnesses.push_back(fitness);
            upForEvaluation.at(i) = NULL;
            std::cout << "Network " << net->ID << " was assigned fitness " << fitness << std::endl;
            currentIndividualNumber++;
            badPointer = false;
            break;
        }
    }
    if (badPointer) {
        std::cerr << "Assigned fitness for a pointer not in upForEvaluation. This shouldn't happen." << std::endl;
        return -1;
    }
    return evaluated.size() - populationSize;
}

int GPSynth::prevGeneration() {
    if (generationID == 0) {
        std::cerr << "Attempted to revert to a previous generation during generation 0" << std::endl;
        return generationID;
    }
    upForEvaluation.clear();
    evaluated.clear();
    fitnesses.clear();
    currentIndividualNumber = 0;
    for (int i = 0; i < populationSize; i++) {
        upForEvaluation.push_back(allNetworks.back());
        allNetworks.erase(allNetworks.end() - 1);
    }
    generationID--;
    return generationID;
}

int GPSynth::nextGeneration() {
    assert (evaluated.size() == fitnesses.size());
    upForEvaluation.clear();
    if (evaluated.size() != populationSize) {
        std::cerr << "Attempted to advance generation before evaluating all networks in the generation." << std::endl;
    }
    for (std::vector<double>::iterator i = fitnesses.begin(); i != fitnesses.end(); i++) {
        if (*i < 0) {
            std::cerr << "Negative fitness value detected when attempting to advance generation" << std::endl;
            return generationID;
        }
    }

    for (int i = 0; i < populationSize; i++) {
        GPNetwork* dad = selectFromEvaluated();
        GPNetwork* one = dad->getCopy();
        GPNetwork* offspring = one;

        if (nodeParams->rng->random() < nodeAddChance) {
            one->mutateAddNode(nodeParams, getRandomNode());
        }
        if (nodeParams->rng->random() < nodeRemoveChance) {
            one->mutateRemoveNode(nodeParams);
        }
        if (nodeParams->rng->random() < nodeMutateChance) {
            one->mutate(nodeParams);
        }
        if (nodeParams->rng->random() < crossoverChance) {
            GPNetwork* mom = selectFromEvaluated();
            while (dad == mom) {
                mom = selectFromEvaluated();
            }
            GPNetwork* two = mom->getCopy();
            if (nodeParams->rng->random() < nodeAddChance) {
                two->mutateAddNode(nodeParams, getRandomNode());
            }
            if (nodeParams->rng->random() < nodeRemoveChance) {
                two->mutateRemoveNode(nodeParams);
            }
            if (nodeParams->rng->random() < nodeMutateChance) {
                two->mutate(nodeParams);
            }
            offspring = reproduce(one, two);
        }

        offspring->ID = nextNetworkID++;

        allNetworks.push_back(offspring);
        upForEvaluation.push_back(offspring);
    }

    evaluated.clear();
    fitnesses.clear();
    rank.clear();
    generationID++;
    printGenerationDelim();
    return generationID;
}

void GPSynth::printGenerationDelim() {
    std::cout << "------------------------- START OF GENERATION " << generationID << " -------------------------" << std::endl;
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
    else if (selectionType == 9) {
        // fitness-unaware random selection
        return evaluated[(int) (nodeParams->rng->random() * evaluated.size())];
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
