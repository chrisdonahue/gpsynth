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

GPSynth::GPSynth(GPParams* p, GPRandom* r, std::vector<GPNode*>* nodes) :
    params(p), rng(r), availableNodes(nodes),
    allNetworks(), unevaluated(), evaluated(), currentGeneration(),
    rawFitnesses(), normalizedFitnesses(), rank()
{
    // init public evolution sate
    currentGenerationNumber = 0;
    generationChamp = NULL;
    champ = NULL;

    // init private evolution state
    populationSize = params->populationSize;
    nextNetworkID = 0;
    lowerFitnessIsBetter = params->lowerFitnessIsBetter;
    overallBestFitness = lowerFitnessIsBetter ? std::numeric_limits<double>::max() : 0;

    // categorize primitives
    availableFunctions = new std::vector<GPNode*>();
    availableTerminals = new std::vector<GPNode*>();
    for (unsigned i = 0; i < availableNodes->size(); i++) {
        if (availableNodes->at(i)->arity == 0) {
            availableTerminals->push_back(nodes->at(i));
        }
        else {
            availableFunctions->push_back(nodes->at(i));
        }
    }
    assert(availableFunctions->size() > 0 && availableTerminals->size() > 0);

    // init generation containers
    clearGenerationState();

    // create initial population
    std::cerr << "Initializing population of size " << populationSize << " with best possible fitness of " << params->bestPossibleFitness << std::endl;
    initPopulation();

    // declare the start of generation 0
    printGenerationDelim();
}

GPSynth::~GPSynth() {
    clearGenerationState();
    for (unsigned i = 0; i < allNetworks.size(); i++) {
        delete allNetworks[i];
    }
    for (unsigned i = 0; i < availableNodes->size(); i++) {
        delete availableNodes->at(i);
    }
    delete availableNodes;
    delete availableFunctions;
    delete availableTerminals;
    delete generationChamp;
    delete champ;
}

GPNode* GPSynth::fullRecursive(unsigned cd, unsigned d) {
    if (cd == d) {
        GPNode* term = availableTerminals->at(rng->random(availableTerminals->size()))->getCopy();
        return term;
    }
    else {
        GPNode* ret = availableFunctions->at(rng->random(availableFunctions->size()))->getCopy();
        for (unsigned i = 0; i < ret->arity; i++) {
            ret->descendants[i] = fullRecursive(cd + 1, d);
        }
        return ret;
    }
}

GPNetwork* GPSynth::full(unsigned d) {
    return new GPNetwork(fullRecursive(0, d), "full");
}

GPNode* GPSynth::growRecursive(unsigned cd, unsigned m) {
    if (cd == m) {
        GPNode* term = availableTerminals->at(rng->random(availableTerminals->size()))->getCopy();
        return term;
    }
    else {
        GPNode* ret;
        if (cd == 0) {
            ret = availableFunctions->at(rng->random(availableFunctions->size()))->getCopy();
        }
        else {
            ret = availableNodes->at(rng->random(availableNodes->size()))->getCopy();
        }
        if (ret->arity == 0) {
            return ret;
        }
        for (unsigned i = 0; i < ret->arity; i++) {
            ret->descendants[i] = growRecursive(cd + 1, m);
        }
        return ret;
    }
}

GPNetwork* GPSynth::grow(unsigned m) {
    return new GPNetwork(growRecursive(0, m), "grow");
}

void GPSynth::initPopulation() {
    // implementation of ramped half and half
    unsigned maxInitialHeight = params->maxInitialHeight;
    unsigned numPerPart = maxInitialHeight <= 1 ? 0 : populationSize / (maxInitialHeight - 1);
    unsigned numFullPerPart = numPerPart / 2;
    unsigned numGrowPerPart = numFullPerPart + (numPerPart % 2);
    unsigned additionalLargest = maxInitialHeight <= 1 ? populationSize : populationSize % (maxInitialHeight - 1);
    unsigned additionalFull = additionalLargest / 2;
    unsigned additionalGrow = additionalFull + (additionalLargest % 2);

    // TODO: test for equality before adding to population
    GPNetwork* newnet;
    for (unsigned i = 0; i < maxInitialHeight - 1; i++) {
        for (unsigned j = 0; j < numFullPerPart; j++) {
            newnet = full(i + 2);
            if (params->ephemeralRandomConstants)
                newnet->ephemeralRandom(rng);
            addNetworkToPopulation(newnet);
        }
        for (unsigned j = 0; j < numGrowPerPart; j++) {
            newnet = grow(i + 2);
            if (params->ephemeralRandomConstants)
                newnet->ephemeralRandom(rng);
            addNetworkToPopulation(newnet);
        }
    }
    for (unsigned j = 0; j < additionalFull; j++) {
        newnet = full(maxInitialHeight);
        if (params->ephemeralRandomConstants)
            newnet->ephemeralRandom(rng);
        addNetworkToPopulation(newnet);
    }
    for (unsigned j = 0; j < additionalGrow; j++) {
        newnet = grow(maxInitialHeight);
        if (params->ephemeralRandomConstants)
            newnet->ephemeralRandom(rng);
        addNetworkToPopulation(newnet);
    }
    assert(unevaluated.size() == populationSize);
}

/*
   =================
   EVOLUTION CONTROL
   =================
*/

GPNetwork* GPSynth::getIndividual() {
    // if no more networks remain advance population
    if (unevaluated.empty()) {
        nextGeneration();
    }

    // logic to deal with reproduced algorithms for efficiency
    GPNetwork* ret = *(unevaluated.begin());
    if (params->verbose)
        std::cout << "Testing algorithm " << ret->ID << " made by " << ret->origin << " with height " << ret->height << " and structure " << ret->toString(params->printPrecision) << std::endl;
    return ret;
}

std::vector<GPNetwork*>* GPSynth::getIndividuals(unsigned n) {
    if (unevaluated.size() < n) {
        std::cout << "Requested multiple individuals out of population that did not have enough remaining" << std::endl;
        return NULL;
    }
    return NULL;
}

int GPSynth::assignFitness(GPNetwork* net, double fitness) {
    // assign network fitness and move it to evaluated
    net->fitness = fitness;
    unevaluated.erase(net);
    evaluated.insert(net);
    rawFitnesses[net->ID % populationSize] = fitness;

    // print if verbose
    if (params->verbose) {
        std::cout << "Algorithm " << net->ID << " was assigned fitness " << fitness << std::endl;
    }

    // calculate num remaining and print summary if generation is done
    int numStillNeedingEvaluation = populationSize - evaluated.size();
    if (numStillNeedingEvaluation == 0) {
        printGenerationSummary();
    }
    return numStillNeedingEvaluation;
}

int GPSynth::prevGeneration() {
    if (currentGenerationNumber == 0) {
        std::cerr << "Attempted to revert to a previous generation during generation 0" << std::endl;
        return currentGenerationNumber;
    }
    //clearGenerationState();
    /*
        // TODO: implement this, requires GPNetwork(std::string)
    */
    currentGenerationNumber--;
    return currentGenerationNumber;
}

void GPSynth::printGenerationDelim() {
    std::cout << "------------------------- START OF GENERATION " << currentGenerationNumber << " -------------------------" << std::endl;
}

void GPSynth::printGenerationSummary() {
    assert(evaluated.size() == rawFitnesses.size());
    assert(evaluated.size() == populationSize);

    // parse assigned fitnesses
    unsigned generationBestNetworkID = 0;
    double generationCumulativeFitness = 0;
    for (unsigned i = 0; i < rawFitnesses.size(); i++) {
        // grab and check that fitness is positive
        double fitness = rawFitnesses[i];
        if (fitness < 0) {
            std::cerr << "Negative fitness value detected when summarizing generation" << std::endl;
            return;
        }

        // determine if we have a new generation champion
        bool newGenChamp = lowerFitnessIsBetter ? fitness < generationBestFitness : fitness > generationBestFitness;
        if (newGenChamp) {
            generationBestNetworkID = i;
            generationBestFitness = fitness;
        }

        // update cumulative fitness
        generationCumulativeFitness += fitness;
    }

    // calculate average fitness
    generationAverageFitness = generationCumulativeFitness / populationSize;

    // update generation champ
    GPNetwork* best = currentGeneration[generationBestNetworkID];
    delete generationChamp;
    generationChamp = best->getCopy(best->origin);
    generationChamp->ID = best->ID;
    generationChamp->fitness = generationBestFitness;
    generationChamp->traceNetwork();

    // update overall champ
    bool newChamp = lowerFitnessIsBetter ? generationBestFitness < overallBestFitness : generationBestFitness > overallBestFitness;
	if (newChamp) {
		delete champ;
		champ = best->getCopy(best->origin);
		champ->ID = best->ID;
		champ->fitness = generationBestFitness;
		champ->traceNetwork();
	}

    // print generation summary
    std::cerr << "Generation " << currentGenerationNumber << " had average fitness " << generationAverageFitness << " and best fitness " << generationBestFitness << " attained by algorithm " << generationChamp->ID << " made by " << generationChamp->origin << " with height " << generationChamp->height << " and structure " << generationChamp->toString(params->savePrecision) << std::endl;
}

int GPSynth::nextGeneration() {
    assert(evaluated.size() == rawFitnesses.size() && evaluated.size() == populationSize && unevaluated.size() == 0);

    // CREATE TEMP CONTAINER FOR NEXT GENERATION
    std::vector<GPNetwork*>* nextGeneration = new std::vector<GPNetwork*>();

    // CALCULATE NUMBER OF INDIVIDUALS BY EACH MUTATION TYPE
    double proportionSum = 0;
    proportionSum += params->proportionOfPopulationFromNumericMutation + params->proportionOfPopulationFromMutation + params->proportionOfPopulationFromCrossover + params->proportionOfPopulationFromReproduction;

    unsigned numToNumericMutate = (unsigned) ((params->proportionOfPopulationFromNumericMutation/proportionSum) * populationSize);
    unsigned numToMutate = (unsigned) ((params->proportionOfPopulationFromMutation/proportionSum) * populationSize);
    unsigned numToCrossover = (unsigned) ((params->proportionOfPopulationFromCrossover/proportionSum) * populationSize);
    unsigned numToReproduce = (unsigned) ((params->proportionOfPopulationFromReproduction/proportionSum) * populationSize);

    assert(numToNumericMutate + numToMutate + numToCrossover + numToReproduce <= populationSize);
    numToCrossover += populationSize - (numToNumericMutate + numToMutate + numToCrossover + numToReproduce);

    //std::cout << populationSize << ", " << numToNumericMutate << ", " << numToMutate << ", " << numToCrossover << ", " << numToReproduce << std::endl;

    // NUMERIC MUTATION
    unsigned numForPossibleNumericMutation = params->percentileOfPopulationToSelectFromForNumericMutation * populationSize;
    for (unsigned i = 0; i < numToNumericMutate; i++) {
        GPNetwork* selected = selectFromEvaluated(params->numericMutationSelectionType, numForPossibleNumericMutation);
        GPNetwork* one = selected->getCopy("numeric mutation");
        numericallyMutate(one);
        nextGeneration->push_back(one);
        //selected->fitness = newfitness;
        //rawFitnesses[selected->ID % populationSize] = newfitness;
    }

    // MUTATION
    unsigned numForPossibleMutation = params->percentileOfPopulationToSelectFromForMutation * populationSize;
    for (unsigned i = 0; i < numToMutate; i++) {
        GPNetwork* selected = selectFromEvaluated(params->mutationSelectionType, numForPossibleMutation);
        GPNetwork* one = selected->getCopy("mutation");
        one->traceNetwork();
        mutate(params->mutationType, one);
        nextGeneration->push_back(one);
        //selected->fitness = newfitness;
    }

    // CROSSOVER
    for (unsigned i = 0; i < numToCrossover;) {
        GPNetwork* dad = selectFromEvaluated(params->crossoverSelectionType, 0);
        GPNetwork* mom = selectFromEvaluated(params->crossoverSelectionType, 0);
        GPNetwork* one = dad->getCopy("crossover");
        int oneID = dad->ID;
        double oneFitness = one->fitness;
        one->traceNetwork();
        GPNetwork* two = mom->getCopy("crossover");
        int twoID = mom->ID;
        double twoFitness = two->fitness;
        two->traceNetwork();

        GPNetwork* offspring = crossover(params->crossoverType, one, two);

        // standard GP with two offspring
        if (offspring == NULL) {
            one->traceNetwork();
			assert(one->height >= 0);
            if ((unsigned) one->height > params->maxHeight) {
                delete one;
                one = dad->getCopy("reproduction during crossover");
                one->ID = oneID;
                one->fitness = oneFitness;
            }
            nextGeneration->push_back(one);
            i++;
            if (i < numToCrossover) {
                two->traceNetwork();
				assert(two->height >= 0);
                if ((unsigned) two->height > params->maxHeight) {
                    delete two;
                    two = mom->getCopy("reproduction during crossover");
                    two->ID = twoID;
                    two->fitness = twoFitness;
                }
                nextGeneration->push_back(two);
                i++;
            }
            else {
                delete two;
            }
        }
        // some other type with one offspring
        else {
            nextGeneration->push_back(offspring);
        }
    }

    // REPRODUCTION
    for (unsigned i = 0; i < numToReproduce; i++) {
        GPNetwork* selected = selectFromEvaluated(params->reproductionSelectionType, 0);
        int oldID = selected->ID;
        double oldFitness = selected->fitness;
        GPNetwork* one = selected->getCopy("reproduction");
        one->ID = oldID;
        one->fitness = oldFitness;
        nextGeneration->push_back(one);
    }

    // DELETE STATE FROM LAST GENERATIOn
    clearGenerationState();

    // INCREMENT THE GENERATION COUNT
    currentGenerationNumber++;

    // PRINT START OF GENERATION DELIMITER
    printGenerationDelim();

    // POPULATE STATE WITH NEXT GENERATION
    assert(nextGeneration->size() == populationSize);
    for (unsigned i = 0; i < populationSize; i++) {
        addNetworkToPopulation(nextGeneration->at(i));
    }
    delete nextGeneration;

    return currentGenerationNumber;
}

void GPSynth::calculateGenerationRanks() {
    for (unsigned i = 0; i < populationSize; i++) {
        rank[i] = currentGeneration[i];
    }
    if (lowerFitnessIsBetter)
        std::sort(rank.begin(), rank.end(), compareFitnessesLower);
    else
        std::sort(rank.begin(), rank.end(), compareFitnessesHigher);
}

void GPSynth::calculateGenerationNormalizedFitnesses() {
    // STANDARDIZE FITNESS
    std::vector<double>* standardizedFitnesses;
    if (lowerFitnessIsBetter) {
        standardizedFitnesses = &rawFitnesses;
    }
    else {
        standardizedFitnesses = new std::vector<double>();
        for (unsigned i = 0; i < rawFitnesses.size(); i++) {
            standardizedFitnesses->push_back(params->bestPossibleFitness - rawFitnesses[i]);
        }
    }

    // ADJUST FITNESS
    std::vector<double>* adjustedFitnesses = new std::vector<double>();
    double sum = 0;
    double si = 0;
    for (unsigned i = 0; i < standardizedFitnesses->size(); i++) {
        si = 1/(1 + standardizedFitnesses->at(i));
        sum += si;
        adjustedFitnesses->push_back(si);
    }

    // NORMALIZE FITNESS
    for (unsigned i = 0; i < adjustedFitnesses->size(); i++) {
        normalizedFitnesses[i] = (adjustedFitnesses->at(i)/sum);
    }

    // DELETE INTERMEDIATE DATA
    if (!lowerFitnessIsBetter) {
        delete standardizedFitnesses;
    }
    delete adjustedFitnesses;
}

/*
   =======
   HELPERS
   =======
*/

void GPSynth::addNetworkToPopulation(GPNetwork* net) {
    int oldID = -1;
    if (net->fitness != -1) {
        oldID = net->ID;
    }
    net->ID = nextNetworkID++;
    net->traceNetwork();
    assert(net->height >= 0 && (unsigned) net->height <= params->maxHeight);
    if (params->backupAllNetworks)
      allNetworks.push_back(new std::string(net->toString(params->backupPrecision)));
    currentGeneration.insert(std::make_pair(net->ID % populationSize, net));
    if (net->fitness != -1) {
        if (params->verbose)
            std::cout << "Testing algorithm " << net->ID << " made by " << net->origin << " with height " << net->height << " and structure " << net->toString(params->printPrecision) << " which was algorithm " << oldID << " from the previous generation." << std::endl;
        assignFitness(net, net->fitness);
    }
    else {
        unevaluated.insert(net);
    }
}

void GPSynth::clearGenerationState() {
    // delete all networks from last generation
    for (std::set<GPNetwork*>::iterator i = unevaluated.begin(); i != unevaluated.end(); i++) {
        delete (*i);
    }
    for (std::set<GPNetwork*>::iterator i = evaluated.begin(); i != evaluated.end(); i++) {
        delete (*i);
    }
    evaluated.clear();
    unevaluated.clear();
    currentGeneration.clear();
    generationBestFitness = lowerFitnessIsBetter ? std::numeric_limits<double>::max() : 0;
    generationAverageFitness = lowerFitnessIsBetter ? std::numeric_limits<double>::max() : 0;

    // clear out selection containers
    rawFitnesses.clear();
    rawFitnesses.resize(populationSize, -1.0);
    normalizedFitnesses.clear();
    normalizedFitnesses.resize(populationSize, -1.0);
    rank.clear();
    rank.resize(populationSize, NULL);
}

GPNetwork* GPSynth::selectFromEvaluated(unsigned selectionType, unsigned parameter) {
    //http://en.wikipedia.org/wiki/Selection_%28genetic_algorithm%29
    assert(rawFitnesses.size() == populationSize && normalizedFitnesses.size() == populationSize);
    if (selectionType == 0 && normalizedFitnesses[0] == -1) {
        calculateGenerationNormalizedFitnesses();
    }
    else if (selectionType == 1 && rank[0] == NULL) {
        calculateGenerationRanks();
    }

    if (selectionType == 0) {
        // fitness proportionate selection
        return currentGeneration[rng->sampleFromDistribution(&normalizedFitnesses)];
    }
    else if (selectionType == 1) {
        // ranking random selection from top parameter elements
        assert(parameter <= populationSize);
        if (parameter == 0) {
            return rank[0];
        }
        else {
            return rank[rng->random(parameter)];
        }
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
        return currentGeneration[(int) (rng->random() * evaluated.size())];
    }
    return NULL;
}

/*
    =========
    CROSSOVER
    =========
*/

GPNetwork* GPSynth::crossover(unsigned crossoverType, GPNetwork* one, GPNetwork* two) {
    if (crossoverType == 0) {
        // standard GP crossover
        GPNode* subtreeone = one->getRandomNetworkNode(rng);
        GPNode* subtreeonecopy = subtreeone->getCopy();
        GPNode* subtreetwo = two->getRandomNetworkNode(rng);
        GPNode* subtreetwocopy = subtreetwo->getCopy();
        one->replaceSubtree(subtreeone, subtreetwocopy);
        two->replaceSubtree(subtreetwo, subtreeonecopy);
        delete subtreeone;
        delete subtreetwo;

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
        /*
        GPNode* newroot = new FunctionNode(multiply, one->getRoot(), two->getRoot());
        GPNetwork* newnet = new GPNetwork(newroot, "AM crossover");
        return newnet;
        */
        return NULL;
    }
    // experimental array crossover
    else if (crossoverType == 4) {
        return NULL;
    }
    return NULL;
}

void GPSynth::mutate(unsigned mutationType, GPNetwork* one) {
    if (mutationType == 0) {
        // pick old subtree
        GPNode* forReplacement = one->getRandomNetworkNode(rng);
        assert(params->maxHeight - forReplacement->depth >= 0);

        // grow new replacement subtree
        GPNode* replacement = growRecursive(0, params->maxHeight - forReplacement->depth);
        if (params->ephemeralRandomConstants)
            replacement->ephemeralRandom(rng);

        // replace and delete old
        one->replaceSubtree(forReplacement, replacement);
        delete forReplacement;
    }
}

void GPSynth::numericallyMutate(GPNetwork* one) {
    //std::cout << "BEFORE NUMERIC MUTATION " << one->toString() << std::endl;
    double bestProportion = generationBestFitness / generationAverageFitness;
    double temperatureConstant = params->numericMutationTemperatureConstant;

    //std::cout << bestProportion << ", " << temperatureConstant << std::endl;

    std::vector<GPMutatableParam*>* params = one->getAllMutatableParams();
    for (unsigned i = 0; i < params->size(); i++) {
        GPMutatableParam* p = params->at(i);
        if (p->isContinuous) {
            double value = p->getCValue();
            double min = p->getCMin();
            double max = p->getCMax();
            double temperatureFactor = bestProportion * (max - min) * temperatureConstant;
            double rand = rng->random();
            double mutationAmount = (rand * temperatureFactor * 2) - temperatureFactor;
            //std::cout << "CONTINUOUS VALUE: " << value << ", MUTATION AMOUNT: " << mutationAmount << ", MIN: " << min << ", MAX: " << max << std::endl;
            p->setCValue(value + mutationAmount);
        }
        else {
            int value = p->getDValue();
            int min = p->getDMin();
            int max = p->getDMax();
            double temperatureFactor = bestProportion * (max - min) * temperatureConstant;
            double rand = rng->random();
            double mutationAmount = (rand * temperatureFactor * 2) - temperatureFactor;
            //std::cout << "DISCRETE VALUE: " << value << ", MUTATION AMOUNT: " << mutationAmount << ", MIN: " << min << ", MAX: " << max << std::endl;
            p->setDValue((int) (value + mutationAmount));
        }
    }
    //std::cout << "AFTER NUMERIC MUTATION " << one->toString() << std::endl;
}

/*
    ====================
    EXTERNAL COMPARATORS
    ====================
*/

bool compareFitnessesLower(GPNetwork* one, GPNetwork* two) {
    return one->fitness < two->fitness;
}

bool compareFitnessesHigher(GPNetwork* one, GPNetwork* two) {
    return two->fitness < one->fitness;
}
