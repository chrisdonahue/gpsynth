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

GPSynth::GPSynth(unsigned psize, bool lowerbetter, double best, unsigned mid, unsigned md, unsigned crosstype, unsigned reproduceselecttype, unsigned crossselecttype, double crosspercent, double addchance, double subchance, double mutatechance, std::vector<GPNode*>* nodes, GPNodeParams* p) :
nextNetworkID(0), generationID(0), currentIndividualNumber(0),
populationSize(psize), lowerFitnessIsBetter(lowerbetter), bestPossibleFitness(best), maxInitialDepth(mid), maxDepth(md), crossoverType(crosstype), reproductionSelectionType(reproduceselecttype), crossoverSelectionType(crossselecttype), crossoverProportion(crosspercent),
nodeAddChance(addchance), nodeRemoveChance(subchance), nodeMutateChance(mutatechance),
availableNodes(nodes), availableFunctions(), availableTerminals(),
allNetworks(), upForEvaluation(), evaluated(),
rawFitnesses(), normalizedFitnesses()
{
    for (int i = 0; i < nodes->size(); i++) {
        if (nodes->at(i)->isTerminal) {
            availableTerminals.push_back(nodes->at(i));
        }
        else {
            availableFunctions.push_back(nodes->at(i));
        }
    }
    nodeParams = p;
    rng = nodeParams->rng;
    std::cout << "Initializing population of size " << populationSize << " with best possible fitness of " << bestPossibleFitness << std::endl;
    initPopulation();
}

GPSynth::~GPSynth() {
    delete availableNodes;
    delete &availableFunctions;
    delete &availableTerminals;
    delete &allNetworks;
    delete &upForEvaluation;
    delete &evaluated;
    delete &rawFitnesses;
    delete &normalizedFitnesses;
}

GPNode* GPSynth::fullRecursive(unsigned cd, GPNode* p, unsigned d) {
    if (cd == d) {
        GPNode* term = availableTerminals[rng->random(availableTerminals.size())]->getCopy();
        term->parent = p;
        return term;
    }
    else {
        GPNode* ret = availableFunctions[rng->random(availableFunctions.size())]->getCopy();
        ret->parent = p;
        ret->left = fullRecursive(cd + 1, ret, d);
        if (ret->isBinary) {
            ret->right = fullRecursive(cd + 1, ret, d);
        }
        return ret;
    }
}

GPNetwork* GPSynth::full(unsigned d) {
    return new GPNetwork(fullRecursive(0, NULL, d));
}

GPNode* GPSynth::growRecursive(unsigned cd, GPNode* p, unsigned m) {
    if (cd == m) {
        GPNode* term = availableTerminals[rng->random(availableTerminals.size())]->getCopy();
        term->parent = p;
        return term;
    }
    else {
        GPNode* ret = availableNodes->at(rng->random(availableNodes->size()))->getCopy();
        ret->parent = p;
        if (ret->isTerminal) {
            return ret;
        }
        ret->left = fullRecursive(cd + 1, ret, m);
        if (ret->isBinary) {
            ret->right = fullRecursive(cd + 1, ret, m);
        }
        return ret;
    }
}

GPNetwork* GPSynth::grow(unsigned m) {
    return new GPNetwork(growRecursive(0, NULL, m));
}

void GPSynth::initPopulation() {
    // implementation of ramped half and half
    unsigned numPerPart = populationSize / (maxInitialDepth - 1);
    unsigned numFullPerPart = numPerPart / 2;
    unsigned numGrowPerPart = numFullPerPart + (numPerPart % 2);
    unsigned additionalLargest = populationSize % (maxInitialDepth - 1);
    unsigned additionalFull = additionalLargest / 2;
    unsigned additionalGrow = additionalFull + (additionalLargest % 2);

    std::cout << numPerPart << ", " << numFullPerPart << ", " << numGrowPerPart << ", " << additionalLargest << ", " << additionalFull << ", " << additionalGrow << std::endl;

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

GPNetwork* GPSynth::getIndividual() {
    if (currentIndividualNumber == populationSize) {
        currentIndividualNumber = 0;
        nextGeneration();
    }
    if (currentIndividualNumber == 0) {
        std::cout << "------------------------- START OF GENERATION " << generationID << " -------------------------" << std::endl;
    }
    GPNetwork* ret = upForEvaluation[currentIndividualNumber];
    std::cout << "Testing algorithm " << ret->ID << " with structure: " << ret->toString() << std::endl;
    return upForEvaluation[currentIndividualNumber];
}

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
        std::cerr << "Assigned fitness for a pointer not in upForEvaluation. Probably tried to assign fitness twice for the same network. This shouldn't happen" << std::endl;
        return -1;
    }
    return populationSize - evaluated.size();
}

int GPSynth::prevGeneration() {
    if (generationID == 0) {
        std::cerr << "Attempted to revert to a previous generation during generation 0" << std::endl;
        return generationID;
    }
    clearGenerationState();
    upForEvaluation.clear();
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
    double generationCumulativeFitness = 0;
    double generationBestFitness = lowerFitnessIsBetter ? INFINITY : 0;
    GPNetwork* champ = NULL;
    for (int i = 0; i < rawFitnesses.size(); i++) {
        double fitness = rawFitnesses[i];
        if (fitness < 0) {
            std::cerr << "Negative fitness value detected when attempting to advance generation" << std::endl;
            return generationID;
        }
        generationCumulativeFitness += fitness;
        if (lowerFitnessIsBetter && fitness < generationBestFitness) {
            generationBestFitness = fitness;
            champ = evaluated[i];
        }
        else if (!lowerFitnessIsBetter && fitness > generationBestFitness) {
            generationBestFitness = fitness;
            champ = evaluated[i];
        }
    }
    double generationAverageFitness = generationCumulativeFitness / populationSize;
    std::cout << "Generation " << generationID << " had average fitness " << generationAverageFitness << " and minimum fitness " << generationBestFitness << " with structure " << champ->toString() << std::endl;

    unsigned numToReproduce = (unsigned) ((1 - crossoverProportion) * populationSize);

    for (int i = 0; i < numToReproduce; i++) {
      GPNetwork* selected = selectFromEvaluated(reproductionSelectionType);
      GPNetwork* one = selected->getCopy();
      if (nodeParams->rng->random() < nodeMutateChance) {
        one->mutate(nodeParams);
      }
      addNetworkToPopulation(one);
    }
    while(upForEvaluation.size() < populationSize) {
      GPNetwork* dad = selectFromEvaluated(reproductionSelectionType);
      GPNetwork* mom = selectFromEvaluated(reproductionSelectionType);
      GPNetwork* one = dad->getCopy();
      GPNetwork* two = dad->getCopy();

      if (nodeParams->rng->random() < nodeMutateChance) {
        one->mutate(nodeParams);
      }
      if (nodeParams->rng->random() < nodeMutateChance) {
        two->mutate(nodeParams);
      }

      GPNetwork* offspring = reproduce(one, two);

      // standard GP
      if (offspring == NULL) {
        if (one->getDepth() > maxDepth) {
          delete one;
          one = dad->getCopy();
        }
        addNetworkToPopulation(one);
        if (upForEvaluation.size() < populationSize) {
          if (two->getDepth() > maxDepth) {
            delete two;
            two = mom->getCopy();
          }
          addNetworkToPopulation(mom);
        }
      }
      // some other type
      else {
        addNetworkToPopulation(offspring);
      }
    }

    clearGenerationState();
    generationID++;
    return generationID;
}

/*
   =======
   HELPERS
   =======
*/

void GPSynth::addNetworkToPopulation(GPNetwork* net) {
    net->ID = nextNetworkID++;
    allNetworks.push_back(net);
    upForEvaluation.push_back(net);
}

void GPSynth::clearGenerationState() {
  rawFitnesses.clear();
  normalizedFitnesses.clear();
  rank.clear();
  evaluated.clear();
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
                standardizedFitnesses->push_back(bestPossibleFitness - rawFitnesses[i]);
            }
        }

        // TODO: USE STANDARDIZED FITNESSES TO FILL RANK

        // ADJUST FITNESS
        std::vector<double>* adjustedFitnesses = new std::vector<double>();
        double sum = 0;
        double si = 0;
        for (int i = 0; i < standardizedFitnesses->size(); i++) {
            si = 1/(1 + standardizedFitnesses->at(i));
            sum += si;
            adjustedFitnesses->push_back(si);
        }

        // NORMALIZE FITNESS
        for (int i = 0; i < standardizedFitnesses->size(); i++) {
            normalizedFitnesses.push_back((standardizedFitnesses->at(i))/sum);
        }

        // DELETE INTERMEDIATE DATA
        if (!lowerFitnessIsBetter) {
            delete standardizedFitnesses;
        }
        delete adjustedFitnesses;
    }

    if (selectionType == 0) {
        // fitness proportionate selection (lower better)
        return evaluated[nodeParams->rng->sampleFromDistribution(&normalizedFitnesses)];
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
        GPNode* subtreeone = one->getRandomNetworkNode(nodeParams->rng);
        GPNode* subtreetwo = two->getRandomNetworkNode(nodeParams->rng);
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
        GPNetwork* newnet = new GPNetwork(newroot);
        return newnet;
    }
    // experimental array crossover
    else if (crossoverType == 4) {
        return NULL;
    }
    return NULL;
}
