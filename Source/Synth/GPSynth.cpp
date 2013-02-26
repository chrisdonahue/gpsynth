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

GPSynth::GPSynth(unsigned psize, double best, bool lowerbetter, GPNodeParams* p, double addchance, double subchance, double mutatechance, double crosschance, unsigned mid, unsigned md, unsigned crosstype, unsigned selecttype) :
populationSize(psize),
nextNetworkID(0), generationID(0),
bestPossibleFitness(best), lowerFitnessIsBetter(lower), currentIndividualNumber(0),
maxInitialDepth(mid), maxDepth(md), crossoverType(crosstype), selectionType(selecttype),
nodeAddChance(addchance), nodeRemoveChance(subchance), nodeMutateChance(mutatechance), crossoverChance(crosschance),
allNetworks(), upForEvaluation(), evaluated(),
rawFitnesses(), normalizedFitnesses()
{
    nodeParams = p;
    std::cout << "Initializing population of size " << populationSize << " with best possible fitness of " << max << std::endl;
    initPopulation(maxInitialDepth);
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

GPNetwork* GPSynth::full(unsigned d) {
    return NULL;
}

GPNetwork* GPSynth::grow(unsigned m) {
    return NULL;
}

void GPSynth::addNetworkToPopulation(GPNetwork* net) {
    net->ID = nextNetworkID++;
    allNetworks.push_back(net);
    upForEvaluation.push_back(net);
    net->traceNetwork();
}

void GPSynth::initPopulation() {
    // implementation of ramped half and half
    unsigned numPerPart = (unsigned) (populationSize / maxInitialDepth);
    unsigned numFullPerPart = (unsigned) (numPerPart / 2);
    unsigned numGrowPerPart = numFullPerPart + (numPerPart % 2);
    unsigned additionalLargest = populationSize % maxInitialDepth;
    unsigned additionalFull = (unsigned) (additionalLargest / 2);
    unsigned additionalGrow = additionalFull + (additionalLargest % 2);

    for (int i = 0; i < maxInitialDepth - 1; i++) {
        for (int j = 0; j < numFullPerPart; j++) {
            addNetworkToPopulation(full(i + 2));
        }
        for (int j = 0; j < numGrowPerPart; j++) {
            addNetworkToPopulation(grow(i + 2));
        }
    }
    for (int j = 0; j < additionalFull; j++) {
        addNetworkToPopulation(full(maxInitialDepth));
    }
    for (int j = 0; j < additionalGrow; j++) {
        addNetworkToPopulation(grow(maxInitialDepth));
    }
    assert(allNetworks.size() == populationSize);
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
            rawFitnesses.push_back(fitness);
            upForEvaluation.at(i) = NULL;
            std::cout << "Algorithm " << net->ID << " was assigned fitness " << fitness << std::endl;
            currentIndividualNumber++;
            badPointer = false;
            break;
        }
    }
    if (badPointer) {
        std::cerr << "Assigned fitness for a pointer not in upForEvaluation. This shouldn't happen" << std::endl;
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
    rawFitnesses.clear();
    currentIndividualNumber = 0;
    // TODO: fix this for assigned fitnesses and NAN and whatnot
    for (int i = 0; i < populationSize; i++) {
        upForEvaluation.push_back(allNetworks.back());
        allNetworks.erase(allNetworks.end() - 1);
    }
    generationID--;
    return generationID;
}

int GPSynth::nextGeneration() {
    assert (evaluated.size() == rawFitnesses.size());
    upForEvaluation.clear();
    if (evaluated.size() != populationSize) {
        std::cerr << "Attempted to advance generation before evaluating all algorithms in the generation" << std::endl;
    }
    for (std::vector<double>::iterator i = rawFitnesses.begin(); i != rawFitnesses.end(); i++) {
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
        addNetworkToPopulation(offspring);
    }

    evaluated.clear();
    rawFitnesses.clear();
    rank.clear();
    generationID++;
    return generationID;
}

GPNetwork* GPSynth::getIndividual() {
    if (currentIndividualNumber == populationSize) {
        currentIndividualNumber = 0;
        nextGeneration();
    }
    if (currentIndividualNumber == 0) {
        std::cout << "------------------------- START OF GENERATION " << generationID << " -------------------------" << std::endl;
    }
    return upForEvaluation[currentIndividualNumber];
}

GPNetwork* GPSynth::selectFromEvaluated(unsigned selectionType) {
    //http://en.wikipedia.org/wiki/Selection_%28genetic_algorithm%29
    if (normalizedFitnesses.size() == 0) {
        // STANDARDIZE FITNESS
        std::vector<double>* standardizedFitnesses;
        if (lowerFitnessIsBetter) {
            standardizedFitnesses = &rawFitnesses;
        }
        else {
            standardizedFitnesses = new std::vector<double>();
            for (int i = 0; i < rawFitnesses.size(); i++) {
                standardizedFitnesses.push_back(bestPossibleFitness - rawFitnesses[i]);
            }
        }

        // TODO: USE STANDARDIZED FITNESSES TO FILL RANK

        // ADJUST FITNESS
        std::vector<double>* adjustedFitnesses = new std::vector<double>();
        double sum = 0;
        double si = 0;
        for (int i = 0; i < standardizedFitnesses.size(); i++) {
            si = 1/(1+standardizedFitnesses[i]);
            sum += si;
            adjustedFitnesses.push_back(si);
        }

        // NORMALIZE FITNESS
        for (int i = 0; i < standardizedFitnesses.size(); i++) {
            normalizedFitnesses.push_back(standardizedFitnesses[i]/sum);
        }

        // DELETE INTERMEDIATE DATA
        if (!lowerFitnessIsBetter) {
            delete standardizedFitnesses;
        }
        delete adjustedFitnesses;
    }

    if (selectionType == 0) {
        // fitness proportionate selection (lower better)
        return evaluated[nodeParams->rng->sampleFromDistribution(normalizedFitnesses)];
    }
    else if (selectionType == 1) {
        // ranking linear selection
        return NULL;
    }
    else if (selectionType == 2) {
        // ranking curved selection
        return NULL;
    }
    else if (selectionType == 3) {
        // tournament selection
        return NULL;
    }
    else if (selectionType == 4) {
        // stochastic universal sampling selection
        return NULL;
    }
    else if (selectionType == 5) {
        // greedy over selection
        return NULL;
    }
    else if (selectionType == 6) {
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
        // standard GP crossover
        GPNode* subtreeone = one->getRandomNetworkNode();
        GPNode* subtreetwo = two->getRandomNetworkNode();
        one->replaceSubtree(subtreeone, subtreetwo);
        two->replaceSubtree(subtreetwo, subtreeone);

        return NULL;
    }
    else if (crossoverType == 1) {
        return one;
    }
    else if (crossoverType == 2) {
        return two;
    }
    else if (crossoverType == 3) {
        // AM crossover
        GPNode* newroot = new FunctionNode(multiply, "*", one->getRoot(), two->getRoot());
        GPNetwork* newnet = new GPNetwork(-1, newroot);
        return newnet;
    }
    // experimental array crossover
    else if (crossoverType == 4) {
        return NULL;
    }
    return NULL;
}
