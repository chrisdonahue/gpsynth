#include "GPSynth.h"

/*
   ============
   CONSTRUCTION
   ============
*/

GPSynth::GPSynth(GPLogger* logger, GPSynthParams* params, GPRandom* rng, std::vector<GPNode*>* primitives) :
    logger(logger),
    params(params), rng(rng), availablePrimitives(primitives),
    allNetworks(), unevaluated(), evaluated(), currentGeneration(),
    rawFitnesses(), normalizedFitnesses(), rank()
{
    // init public evolution sate
    currentGenerationNumber = 0;
    generationChamp = NULL;
    champ = NULL;

    // init private evolution state
    populationSize = params->population_size;
    nextNetworkID = 0;
    lowerFitnessIsBetter = params->lower_fitness_is_better;
    overallBestFitness = lowerFitnessIsBetter ? std::numeric_limits<double>::max() : 0;
    currentGenerationAlive = false;

    // categorize primitives
    availableFunctions = new std::vector<GPNode*>();
    availableTerminals = new std::vector<GPNode*>();
    for (unsigned i = 0; i < availablePrimitives->size(); i++) {
        if (availablePrimitives->at(i)->arity == 0) {
            availableTerminals->push_back(availablePrimitives->at(i));
        }
        else {
            availableFunctions->push_back(availablePrimitives->at(i));
        }
    }
    assert(availableFunctions->size() > 0 && availableTerminals->size() > 0);

    // init generation containers
    clearGenerationState();

    // create initial population
    initPopulation();

    // declare the start of generation 0
    printGenerationDelim();
}

GPSynth::~GPSynth() {
    clearGenerationState();
    for (unsigned i = 0; i < availablePrimitives->size(); i++) {
        delete availablePrimitives->at(i);
    }
	// TODO: maybe delete this elsewhere and pass by reference or something
    delete availablePrimitives;
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
            ret = availablePrimitives->at(rng->random(availablePrimitives->size()))->getCopy();
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
    logger->log << "Initializing population of size " << populationSize << std::flush;

	// if maxInitialHeight is 0 we just want a single terminal
    GPNetwork* newnet;
    unsigned maxInitialHeight = params->max_initial_height;
	if (maxInitialHeight == 0) {
		for (unsigned i = 0; i < populationSize; i++) {
			newnet = full(0);
			addNetworkToPopulation(newnet);
		}
		return;
	}

    // implementation of ramped half and half
    unsigned numPerPart = maxInitialHeight <= 1 ? 0 : populationSize / (maxInitialHeight - 1);
    unsigned numFullPerPart = numPerPart / 2;
    unsigned numGrowPerPart = numFullPerPart + (numPerPart % 2);
    unsigned additionalLargest = maxInitialHeight <= 1 ? populationSize : populationSize % (maxInitialHeight - 1);
    unsigned additionalFull = additionalLargest / 2;
    unsigned additionalGrow = additionalFull + (additionalLargest % 2);

    // TODO: test for equality before adding to population
    for (unsigned i = 0; i < maxInitialHeight - 1; i++) {
        for (unsigned j = 0; j < numFullPerPart; j++) {
            newnet = full(i + 2);
            if (params->erc)
                newnet->ephemeralRandom(rng);
            addNetworkToPopulation(newnet);
        }
        for (unsigned j = 0; j < numGrowPerPart; j++) {
            newnet = grow(i + 2);
            if (params->erc)
                newnet->ephemeralRandom(rng);
            addNetworkToPopulation(newnet);
        }
    }
    for (unsigned j = 0; j < additionalFull; j++) {
        newnet = full(maxInitialHeight);
        if (params->erc)
            newnet->ephemeralRandom(rng);
        addNetworkToPopulation(newnet);
    }
    for (unsigned j = 0; j < additionalGrow; j++) {
        newnet = grow(maxInitialHeight);
        if (params->erc)
            newnet->ephemeralRandom(rng);
        addNetworkToPopulation(newnet);
    }
    assert(unevaluated.size() == populationSize);

    currentGenerationAlive = true;
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
    logger->verbose << "Testing algorithm " << ret->ID << " made by " << ret->origin << " with height " << ret->height << " and structure " << logger->net_to_string_print(ret) << std::flush;
    return ret;
}

void GPSynth::getIndividuals(std::vector<GPNetwork*>& networks) {
    // if no more networks remain advance population
    if (unevaluated.empty()) {
        nextGeneration();
    }

	// if too many networks were asked for print and return
    if (unevaluated.size() < networks.size()) {
        logger->error << "Requested multiple individuals out of population that did not have enough remaining" << std::flush;
        return;
    }

	// fill 'er up! (you should assignFitness on all of these before asking for another block
	// otherwise you will probably get the same networks)
	unsigned i = 0;
	for (std::set<GPNetwork*>::iterator iter = unevaluated.begin(); i < networks.size(); i++, iter++) {
		networks[i] = *(iter);
	}

	// sort synth voices by ID
	std::sort(networks.begin(), networks.end(), compareNetworksByID);
}

bool GPSynth::replaceIndividual(GPNetwork* old, GPNetwork* nu) {
	// check if old is already evaluated
	if (evaluated.find(old) != evaluated.end()) {
        logger->error << "Tried replacing population individual that had already been evaluated" << std::flush;
		return false;
	}

	// check if old is still in population
	unsigned oldGenerationID = old->ID % populationSize;
	if (currentGeneration[oldGenerationID] != old) {
		logger->error << "Tried replacing individual that is not in the current generation" << std::flush;
		return false;
	}

	// check to make sure nu is the right height
	nu->traceNetwork();
	if (nu->height > params->max_height) {
		logger->error << "Tried replacing individual that is too tall for the population" << std::flush;
		return false;
	}

	// otherwise lets go ahead and insert it into our population
	nu->ID = old->ID;

	// replace the old network in the current generation
	currentGeneration[oldGenerationID] = nu;

	// replace the old in evaluated/unevaluated
	unevaluated.erase(old);
	evaluated.erase(old);
    if (nu->fitness != -1) {
        logger->log << "Replacement individual " << nu->ID << " made by " << nu->origin << " with height " << nu->height << " and structure " << logger->net_to_string_print(nu) << " was already evaluated and replaced algorithm " << old->ID << " ." << std::flush;
        
        assignFitness(nu, nu->fitness);
    }
    else {
        unevaluated.insert(nu);
    }

	// replace the old network in the string backups
    if (params->backup_all_networks) {
		allNetworks[oldGenerationID] = std::string(nu->toString(params->backup_precision));
	}

	// delete the old
	delete old;

	// replacement successful
	return true;
}

int GPSynth::assignFitness(GPNetwork* net, double fitness) {
    // assign network fitness and move it to evaluated
    net->fitness = fitness;
    unevaluated.erase(net);
    evaluated.insert(net);
    rawFitnesses[net->ID % populationSize] = fitness;

    // print if verbose
    logger->verbose << "Algorithm " << net->ID << " was assigned fitness " << fitness << std::flush;

    // calculate num remaining and print summary if generation is done
    int numStillNeedingEvaluation = populationSize - evaluated.size();
    if (numStillNeedingEvaluation == 0) {
        endGeneration();
        printGenerationSummary();
    }
    return numStillNeedingEvaluation;
}

int GPSynth::prevGeneration() {
	// check to see we're not at generation 1
    if (currentGenerationNumber == 0) {
        logger->error << "Attempted to revert to a previous generation during generation 0" << std::flush;
        return currentGenerationNumber;
    }

	// check to see if we are backing up
	if (!params->backup_all_networks) {
        logger->error << "Attempted to revert to a previous generation but no networks backed up" << std::flush;
        return currentGenerationNumber;
	}

	// clear out current generation state
	clearGenerationState();

	// recreate the old networks
	std::vector<GPNetwork*> lastGeneration(0);
	for (unsigned i = 0; i < populationSize; i++) {
		// sort by ID in fillFromGeneration()
		std::string oldnetstring = allNetworks[((currentGenerationNumber - 1) * populationSize) + i];
		GPNetwork* oldnetwork = new GPNetwork(rng, oldnetstring);
		lastGeneration.push_back(oldnetwork);
	}
	
	// delete old strings
	// we have to remove 2 * popsize because we're supposed to be at the end state of 2 gens ago
	for (unsigned i = 0; i < populationSize * 2; i++) {
		allNetworks.pop_back();
	}
	//appendToTextFile("./debug.txt", "ALL NETWORKS AFTER DELETE: " + String(allNetworks.size()) + "\n");

	// add old networks back to population
	nextNetworkID -=  2 * populationSize;
	for (unsigned i = 0; i < populationSize; i++) {
		addNetworkToPopulation(lastGeneration[i]);
	}
	//appendToTextFile("./debug.txt", "ALL NETWORKS AFTER ADD: " + String(allNetworks.size()) + "\n");

	// decrement generation number and return it
    currentGenerationNumber--;
    return currentGenerationNumber;
}

void GPSynth::printGenerationDelim() {
    logger->log << "------------------------- START OF GENERATION " << currentGenerationNumber << " -------------------------" << std::flush;
}

void GPSynth::endGeneration() {
    assert(currentGenerationAlive);

    // parse assigned fitnesses
    unsigned generationBestNetworkID = 0;
    double generationCumulativeFitness = 0;
    for (std::set<GPNetwork*>::iterator i = evaluated.begin(); i != evaluated.end(); i++) {
        // find fitness
        GPNetwork* evaluatedNet = (*i);
        unsigned generationID = evaluatedNet->ID % populationSize;
        double fitness = rawFitnesses[generationID];

        // report if fitness negative
        if (fitness < 0) {
            logger->error << "Negative fitness value detected when summarizing generation" << std::flush;
        }

        // determine if we have a new generation champion
        bool newGenChamp = lowerFitnessIsBetter ? fitness < generationBestFitness : fitness > generationBestFitness;
        if (newGenChamp) {
            generationBestNetworkID = generationID;
            generationBestFitness = fitness;
        }

        // update cumulative fitness
        generationCumulativeFitness += fitness;
    }

    // calculate average fitness
    generationAverageFitness = generationCumulativeFitness / evaluated.size();

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
    
    currentGenerationAlive = false;
}

void GPSynth::printGenerationSummary() {
    // print generation summary
    logger->log << "Generation " << currentGenerationNumber << " had average fitness " << generationAverageFitness << " and best fitness " << generationBestFitness << " attained by algorithm " << generationChamp->ID << " made by " << generationChamp->origin << " with height " << generationChamp->height << " and structure " << logger->net_to_string_print(generationChamp) << std::flush;
}

void GPSynth::printEvolutionSummary() {
    float numEvaluatedGenerations = (float) currentGenerationNumber + (evaluated.size() / populationSize);

    logger->log << "-------------------------------- SUMMARY --------------------------------" << std::flush;
    logger->log << "Evolution ran for " << numEvaluatedGenerations << " generations" << std::flush;
    if (champ != NULL) {
        logger->log << "The best synthesis algorithm found was number " << champ->ID << " made by " << champ->origin << " with height " << champ->height << ", fitness " << champ->fitness << " and structure " << logger->net_to_string_print(champ) << std::flush;
    }
}

void GPSynth::getCurrentGeneration(std::vector<GPNetwork*>& networks) {
    for (int i = 0; i < populationSize; i++) {
        networks.push_back(currentGeneration[i]);
    }
}

int GPSynth::nextGeneration() {
    assert(!currentGenerationAlive);
    assert(evaluated.size() == rawFitnesses.size());
    assert(evaluated.size() == populationSize);
    assert(unevaluated.size() == 0);

    // CREATE TEMP CONTAINER FOR NEXT GENERATION
    std::vector<GPNetwork*>* nextGeneration = new std::vector<GPNetwork*>();

    // CALCULATE NUMBER OF INDIVIDUALS BY EACH MUTATION TYPE
    double proportionSum = 0;
    proportionSum += params->nm_proportion + params->mu_proportion + params->x_proportion + params->re_proportion + params->new_proportion;

    unsigned numToNumericMutate = (unsigned) ((params->nm_proportion) * populationSize);
    unsigned numToMutate = (unsigned) ((params->mu_proportion/proportionSum) * populationSize);
    unsigned numToCrossover = (unsigned) ((params->x_proportion/proportionSum) * populationSize);
    unsigned numToReproduce = (unsigned) ((params->re_proportion/proportionSum) * populationSize);
    unsigned numToCreate = (unsigned) ((params->new_proportion/proportionSum) * populationSize);

    logger->debug << numToCreate << std::flush;

    assert(numToNumericMutate + numToMutate + numToCrossover + numToReproduce <= populationSize);
    numToCrossover += populationSize - (numToNumericMutate + numToMutate + numToCrossover + numToReproduce + numToCreate);

    //std::cout << populationSize << ", " << numToNumericMutate << ", " << numToMutate << ", " << numToCrossover << ", " << numToReproduce << std::endl;

    // NUMERIC MUTATION
    unsigned numForPossibleNumericMutation = params->nm_selection_percentile * populationSize;
    for (unsigned i = 0; i < numToNumericMutate; i++) {
        GPNetwork* selected = selectFromEvaluated(params->nm_selection_type, numForPossibleNumericMutation);
        GPNetwork* one = selected->getCopy("numeric mutation");
        one->traceNetwork();
        numericallyMutate(one);
        nextGeneration->push_back(one);
        //selected->fitness = newfitness;
        //rawFitnesses[selected->ID % populationSize] = newfitness;
    }

    // MUTATION
    unsigned numForPossibleMutation = params->mu_selection_percentile * populationSize;
    for (unsigned i = 0; i < numToMutate; i++) {
        GPNetwork* selected = selectFromEvaluated(params->mu_selection_type, numForPossibleMutation);
        GPNetwork* one = selected->getCopy("mutation");
        one->traceNetwork();
        mutate(params->mu_type, one);
        nextGeneration->push_back(one);
        //selected->fitness = newfitness;
    }

    // CROSSOVER
    unsigned numForPossibleCrossover = params->x_selection_percentile * populationSize;
    for (unsigned i = 0; i < numToCrossover;) {
        GPNetwork* dad = selectFromEvaluated(params->x_selection_type, numForPossibleCrossover);
        GPNetwork* mom = selectFromEvaluated(params->x_selection_type, numForPossibleCrossover);
        GPNetwork* one = dad->getCopy("crossover");
        int oneID = dad->ID;
        double oneFitness = one->fitness;
        one->traceNetwork();
        GPNetwork* two = mom->getCopy("crossover");
        int twoID = mom->ID;
        double twoFitness = two->fitness;
        two->traceNetwork();

        GPNetwork* offspring = crossover(params->x_type, one, two);

        // standard GP with two offspring
        if (offspring == NULL) {
            one->traceNetwork();
			assert(one->height >= 0);
            if ((unsigned) one->height > params->max_height) {
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
                if ((unsigned) two->height > params->max_height) {
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
    unsigned numForPossibleReproduction = params->re_selection_type;
    for (unsigned i = 0; i < numToReproduce; i++) {
        GPNetwork* selected = selectFromEvaluated(params->re_selection_type, numForPossibleReproduction);
        int oldID = selected->ID;
        double oldFitness = selected->fitness;
        GPNetwork* one = selected->getCopy("reproduction");
        one->ID = oldID;
        one->fitness = oldFitness;
        nextGeneration->push_back(one);
    }

    // CREATE
    for (unsigned i = 0; i < numToCreate; i++) {
        GPNetwork* one = newIndividual(params->new_type);
        one->traceNetwork();
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

    currentGenerationAlive = true;

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
            standardizedFitnesses->push_back(params->best_possible_fitness - rawFitnesses[i]);
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
    assert(net->height >= 0 && (unsigned) net->height <= params->max_height);
    if (params->backup_all_networks)
      allNetworks.push_back(std::string(net->toString(params->backup_precision)));
    currentGeneration.insert(std::make_pair(net->ID % populationSize, net));
    if (net->fitness != -1) {
        logger->verbose << "Testing algorithm " << net->ID << " made by " << net->origin << " with height " << net->height << " and structure " << logger->net_to_string_print(net) << " which was algorithm " << oldID << " from the previous generation." << std::flush;
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
        assert(params->max_height - forReplacement->depth >= 0);

        // grow new replacement subtree
        GPNode* replacement = growRecursive(0, params->max_height - forReplacement->depth);
        if (params->erc)
            replacement->ephemeralRandom(rng);

        // replace and delete old
        one->replaceSubtree(forReplacement, replacement);
        delete forReplacement;
    }
    else if (mutationType == 1) {
        // grow new node
        GPNetwork* two = grow(params->max_initial_height);
        crossover(params->x_type, one, two);
        delete two;
    }
}

void GPSynth::numericallyMutate(GPNetwork* one) {
    //std::cout << "BEFORE NUMERIC MUTATION " << one->toString(3) << std::endl;
    double bestProportion = generationBestFitness / generationAverageFitness;
    double temperatureConstant = params->nm_temperature;

    //std::cout << bestProportion << ", " << temperatureConstant << std::endl;

    std::vector<GPMutatableParam*>* network_params = one->getAllMutatableParams();
    for (unsigned i = 0; i < network_params->size(); i++) {
        GPMutatableParam* p = network_params->at(i);
        if (p->isContinuous()) {
            double value = p->getCValue();
            double min = p->getCMin();
            double max = p->getCMax();
            double temperatureFactor = bestProportion * (max - min) * temperatureConstant;
            double rand = rng->random();
            double mutationAmount = (rand * temperatureFactor * 2) - temperatureFactor;
            //std::cout << "CONTINUOUS VALUE: " << value << ", MUTATION AMOUNT: " << mutationAmount << ", MIN: " << min << ", MAX: " << max << std::endl;
            p->setCData(min, value + mutationAmount, max);
        }
        else {
            int value = p->getDValue();
            int min = p->getDMin();
            int max = p->getDMax();
            double temperatureFactor = bestProportion * (max - min) * temperatureConstant;
            double rand = rng->random();
            double mutationAmount = (rand * temperatureFactor * 2) - temperatureFactor;
            //std::cout << "DISCRETE VALUE: " << value << ", MUTATION AMOUNT: " << mutationAmount << ", MIN: " << min << ", MAX: " << max << std::endl;
            p->setDData(min, (int) (value + mutationAmount), max);
        }
    }
    //std::cout << "AFTER NUMERIC MUTATION " << one->toString(3) << std::endl;
}

GPNetwork* GPSynth::newIndividual(unsigned new_type) {
    if (new_type == 0) {
        GPNetwork* newnet = grow(params->max_initial_height);
        if (params->erc)
            newnet->ephemeralRandom(rng);
        return newnet;
    }
    else if (new_type == 1) {
        GPNetwork* newnet = grow(params->max_height);
        if (params->erc)
            newnet->ephemeralRandom(rng);
        return newnet;
    }
    else if (new_type == 2) {
        GPNetwork* newnet = full(params->max_height);
        if (params->erc)
            newnet->ephemeralRandom(rng);
        return newnet;
    }
    else if (new_type == 3) {
        GPNetwork* newnet = full(params->max_height);
        if (params->erc)
            newnet->ephemeralRandom(rng);
        return newnet;
    }
    return NULL;
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
