/*
  ==============================================================================

    GPExperiment.cpp
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#include "GPExperiment.h"

/*
    ============
    CONSTRUCTION
    ============
*/

GPExperiment::GPExperiment(String target, unsigned expnum, unsigned psize, unsigned s, double addchance, double subchance, double mutatechance, double crosschance, double threshold, unsigned numgens, unsigned selecttype, unsigned crosstype, std::vector<double>* vals) :
wavFormat(new WavAudioFormat())
{
    // EXPERIMENT PARAMETERS
    fitnessThreshold = threshold;
    numGenerations = numgens;

    // EXPERIMENT STATE
    minFitnessAchieved = INFINITY;
    numEvaluatedGenerations = 0;

    // TARGET DATA CONTAINERS
    sampleRate = 44100.0;
    targetFrames = NULL;
    numTargetFrames = 0;
    specialValues = vals->data();
    loadWavFile(target);

    // SYNTH
    nodeParams = (GPNodeParams*) malloc(sizeof(GPNodeParams));

    nodeParams->partialChance = 0.5;
    nodeParams->numPartials = 3;
    nodeParams->valueMin = -1;
    nodeParams->valueMax = 1;
    nodeParams->LFORange = 10;
    nodeParams->numVariables = vals->size();
    nodeParams->rng = new GPRandom(s);
    std::vector<GPNode*>* nodes = new std::vector<GPNode*>();
    std::vector<double>* nlikelihoods = new std::vector<double>();
    std::vector<GPFunction*>* functions = new std::vector<GPFunction*>();
    std::vector<double>* flikelihoods = new std::vector<double>();

    if (expnum == 0) {
        nodes->push_back(new FunctionNode(add, "*", NULL, NULL));
        nodes->push_back(new FunctionNode(multiply, "+", NULL, NULL));
        nodes->push_back(new ValueNode(0, -1));
        nodes->push_back(new ValueNode(-1, 0));
        nodes->push_back(new ValueNode(-1, 1));
        nodes->push_back(new OscilNode(1, 1, NULL, NULL));

        nlikelihoods->push_back(1);
        nlikelihoods->push_back(1);
        nlikelihoods->push_back(1);
        nlikelihoods->push_back(1);
        nlikelihoods->push_back(1);
        nlikelihoods->push_back(1);
      
        functions->push_back(add);
        functions->push_back(multiply);

        flikelihoods->push_back(1);
        flikelihoods->push_back(1);
    }
    
    nodeParams->availableNodes = nodes;
    nodeParams->rng->normalizeDistribution(nlikelihoods);
    nodeParams->nodeLikelihoods = nlikelihoods;
    nodeParams->availableFunctions = functions;
    nodeParams->rng->normalizeDistribution(flikelihoods);
    nodeParams->functionLikelihoods = flikelihoods;

    synth = new GPSynth(psize, 0, nodeParams, addchance, subchance, mutatechance, crosschance, crosstype, selecttype);
}

GPExperiment::~GPExperiment() {
    free(targetFrames);
    delete nodeParams->rng;
    delete nodeParams->availableNodes;
    delete nodeParams->nodeLikelihoods;
    delete nodeParams->availableFunctions;
    delete nodeParams->functionLikelihoods;
    free(nodeParams);
}

/*
    =================
    EVOLUTION CONTROL
    =================
*/

String GPExperiment::evolve() {
    GPNetwork* champ;
    int numMinimum = 0;
    int numEvaluated = 0;
    double generationCumulativeFitness = 0;
    double generationAverageFitness = 0;
    double generationMinimumFitness = INFINITY;

    while (minFitnessAchieved > fitnessThreshold && numEvaluatedGenerations < numGenerations) {
        GPNetwork* candidate = synth->getIndividual();
        std::cout << "Testing network " << candidate->ID << " with structure: " << candidate->toString() << std::endl;

        float* candidateData = evaluateIndividual(candidate);
        double fitness = compareToTarget(candidateData);
        generationCumulativeFitness += fitness;
        numEvaluated++;

        if (fitness < generationMinimumFitness) {
            generationMinimumFitness = fitness;
        }

        if (fitness < minFitnessAchieved) {
            minFitnessAchieved = fitness;
            champ = candidate;
            char buffer[100];
            snprintf(buffer, 100, "New Minimum (%d).wav", ++numMinimum);
            //saveWavFile(String(buffer), String(candidate->toString().c_str()), numTargetFrames, candidateData);
        }

        int numUnevaluatedThisGeneration = synth->assignFitness(candidate, fitness);

        if (numUnevaluatedThisGeneration == 0) {
            double generationAverageFitness = generationCumulativeFitness / numEvaluated;
            std::cout << "Generation " << numEvaluatedGenerations << " had an average fitness of " << generationAverageFitness << " and a minimum of " << generationMinimumFitness << std::endl;
            numEvaluated = 0;
            generationCumulativeFitness = 0;
            generationMinimumFitness = INFINITY;
            numEvaluatedGenerations++;
        }

        free(candidateData);
    }
    if (champ != NULL)
        return String(champ->toString().c_str());
}

/*
    =============
    WAV INTERFACE
    =============
*/

void GPExperiment::loadWavFile(String path) {
    // TODO: check if path exists
    File input(path);
    FileInputStream* fis = input.createInputStream();
    AudioSampleBuffer asb(1, 200);
    ScopedPointer<AudioFormatReader> afr(wavFormat->createReaderFor(fis, true));

    numTargetFrames = afr->lengthInSamples;
    sampleRate = afr->sampleRate;

    std::cout << "Target file has " << numTargetFrames << " frames at a sample rate of " <<  sampleRate << std::endl;

    free(targetFrames);
    targetFrames = (float*) malloc(sizeof(float) * numTargetFrames);
    
    int64 numRemaining = numTargetFrames;
    int64 numCompleted = 0;
    float* chanData = asb.getSampleData(0);
    while (numRemaining > 0) {
        int numToRead = numRemaining > 200 ? 200 : numRemaining;
        afr->read(&asb, 0, numToRead, numCompleted, true, false);
        memcpy(targetFrames + numCompleted, chanData, numToRead);
        numRemaining -= numToRead;
        numCompleted += numToRead;
    }
    assert (numCompleted == numTargetFrames && numRemaining == 0);
}

void GPExperiment::saveWavFile(String path, String metadata, unsigned numFrames, float* data) {
    File output(path);
    FileOutputStream* fos = output.createOutputStream();
    StringPairArray metaData = WavAudioFormat::createBWAVMetadata(metadata, "", "", Time::getCurrentTime(), 0, "");
    AudioSampleBuffer asb(1, 200);
    ScopedPointer<AudioFormatWriter> afw(wavFormat->createWriterFor(fos, sampleRate, 1, 32, metaData, 0));

    int64 numRemaining = numFrames;
    int64 numCompleted = 0;
    float* chanData = asb.getSampleData(0);
    while (numRemaining > 0) {
        int numToWrite = numRemaining > 200 ? 200 : numRemaining;
        for (int samp = 0; samp < numToWrite; samp++, numCompleted++) {
            chanData[samp] = data[numCompleted];
        }
        afw->writeFromAudioSampleBuffer(asb, 0, numToWrite);
        numRemaining -= numToWrite;
    }
    assert (numCompleted == numFrames && numRemaining == 0);
}

/*
    ================
    FITNESS FUNCTION
    ================
*/

float* GPExperiment::evaluateIndividual(GPNetwork* candidate) {
    float* ret = (float*) malloc(sizeof(float) * numTargetFrames);
    double time;
    for (int frameNum = 0; frameNum < numTargetFrames; frameNum++) {
        time = frameNum/sampleRate;
        ret[frameNum] = candidate->evaluate(&time, specialValues);
    }
    return ret;
}

double GPExperiment::compareToTarget(float* candidateFrames) {
    double sum = 0;
    for (int frameNum = 0; frameNum < numTargetFrames; frameNum++) {
        sum += pow(targetFrames[frameNum] - candidateFrames[frameNum], 2);
    }
    return sqrt(sum);
}
