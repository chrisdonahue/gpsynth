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

GPExperiment::GPExperiment(String target, GPParams* p) :
wavFormat(new WavAudioFormat())
{
    // EXPERIMENT PARAMETERS
    params = p;
    specialValues = p->variableValues->data();
    numGenerations = p->numGenerations;
    fitnessThreshold = p->thresholdFitness;

    // TARGET DATA CONTAINERS
    sampleRate = 44100.0;
    targetFrames = NULL;
    numTargetFrames = 0;
    loadWavFile(target);

    // EXPERIMENT STATE
    minFitnessAchieved = INFINITY;
    numEvaluatedGenerations = 0;

    // SYNTH
    std::vector<GPNode*>* nodes = new std::vector<GPNode*>();
    std::vector<GPFunction>* unaryFunctions = new std::vector<GPFunction>();
    std::vector<GPFunction>* binaryFunctions = new std::vector<GPFunction>();

    if (params->experimentNumber == 0) {
        GPNode* ansroot = new FunctionNode(multiply, new OscilNode(1, 1, NULL, NULL), new OscilNode(1, 2, NULL, NULL));
        GPNetwork* answer = new GPNetwork(ansroot);
        answer->traceNetwork();
        sampleRate = 44100.0;
        numTargetFrames = 88200;
        targetFrames = evaluateIndividual(answer);
        saveWavFile("./Answer.wav", String(answer->toString().c_str()), numTargetFrames, targetFrames);
        delete answer;

        p->lowerFitnessIsBetter = true;
        p->bestPossibleFitness = 0;

        //nodes->push_back(new FunctionNode(add, "+", NULL, NULL));
        nodes->push_back(new FunctionNode(multiply, NULL, NULL));
        //nodes->push_back(new ValueNode(0, -1));
        //nodes->push_back(new ValueNode(-1, 0));
        //nodes->push_back(new ValueNode(-1, 1));
        nodes->push_back(new OscilNode(1, 1, NULL, NULL));
        nodes->push_back(new OscilNode(1, 2, NULL, NULL));
        //nodes->push_back(new OscilNode(1, 3, NULL, NULL));

        binaryFunctions->push_back(add);
        binaryFunctions->push_back(multiply);
    }

    p->availableUnaryFunctions = unaryFunctions;
    p->availableBinaryFunctions = binaryFunctions;

    synth = new GPSynth(p, nodes);
}

GPExperiment::~GPExperiment() {
    free(targetFrames);
    delete params->availableUnaryFunctions;
    delete params->availableBinaryFunctions;
    delete synth;
}

/*
    =================
    EVOLUTION CONTROL
    =================
*/

GPNetwork* GPExperiment::evolve() {
    GPNetwork* champ;
    int numMinimum = 0;
    int numEvaluated = 0;
    double generationCumulativeFitness = 0;
    double generationAverageFitness = 0;
    double generationMinimumFitness = INFINITY;

    while (minFitnessAchieved > fitnessThreshold && numEvaluatedGenerations < numGenerations) {
        GPNetwork* candidate = synth->getIndividual();

        float* candidateData = evaluateIndividual(candidate);
        double fitness = compareToTarget(candidateData);
        generationCumulativeFitness += fitness;
        numEvaluated++;

        //TODO: handle lowerFitnessIsBetter

        if (fitness < generationMinimumFitness) {
            generationMinimumFitness = fitness;
        }

        if (fitness < minFitnessAchieved) {
            minFitnessAchieved = fitness;
            champ = candidate;
            //char buffer[100];
            //snprintf(buffer, 100, "New Minimum (%d).wav", ++numMinimum);
            //saveWavFile(String(buffer), String(candidate->toString().c_str()), numTargetFrames, candidateData);
            if (fitness == bestPossibleFitness) {
                saveWavFile("./DiscoveredChamp.wav", String(champ->toString().c_str()), numTargetFrames, candidateData);
            }
        }

        int numUnevaluatedThisGeneration = synth->assignFitness(candidate, fitness);

        if (numUnevaluatedThisGeneration == 0) {
            synth->printGenerationSummary();
            generationMinimumFitness = INFINITY;
            numEvaluatedGenerations++;
        }

        free(candidateData);
    }
    std::cout << "-------------------------------- SUMMARY ---------------------------------" << std::endl;

    if (minFitnessAchieved <= fitnessThreshold) {
        std::cout << "Evolution found a synthesis algorithm at or below the specified fitness threshold" << std::endl;
    }
    std::cout << "Evolution ran for " << numEvaluatedGenerations << " generations" << std::endl;
    if (champ != NULL) {
        std::cout << "The best synthesis algorithm found was " << champ->toString() << " and had a fitness of " << minFitnessAchieved << std::endl;
    }
    return champ;
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
    if (output.existsAsFile()) {
        output.deleteFile();
        output.create();
    }
    FileOutputStream* fos = output.createOutputStream();
    //StringPairArray metaData = WavAudioFormat::createBWAVMetadata(metadata, "", "", Time::getCurrentTime(), 0, "");
    AudioSampleBuffer asb(1, 200);
    //ScopedPointer<AudioFormatWriter> afw(wavFormat->createWriterFor(fos, sampleRate, 1, 32, metaData, 0));
    ScopedPointer<AudioFormatWriter> afw(wavFormat->createWriterFor(fos, sampleRate, 1, 32, StringPairArray(), 0));

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
