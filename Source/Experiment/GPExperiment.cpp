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

GPExperiment::GPExperiment(String target, unsigned expnum, unsigned psize, unsigned s, double addchance, double subchance, double mutatechance, double crosschance, double threshold, unsigned numGenerations, unsigned selecttype, unsigned crosstype, std::vector<double>* vals) :
wavFormat(new WavAudioFormat())
{
    nodeParams = (GPNodeParams*) malloc(sizeof(GPNodeParams));

    nodeParams->numVariables = vals->size();

    nodeParams->valueRange = 1;
    nodeParams->LFORange = 10;
    nodeParams->numPartials = 10;

    nodeParams->simplifyChance = 0.5;
    nodeParams->specialChance = 0.5;
    nodeParams->harmonyChance = 0.5;
    nodeParams->functionChance = 0.5;

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
        nodes->push_back(new OscilNode(1, 0, NULL, NULL));

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

        synth = new GPSynth(psize, s, 0, nodeParams, addchance, subchance, mutatechance, crosschance, crosstype, selecttype);
    }
    
    nodeParams->availableNodes = nodes;
    nodeParams->nodeLikelihoods = nlikelihoods;
    nodeParams->availableFunctions = functions;
    nodeParams->functionLikelihoods = flikelihoods;
    
    sampleRate = 44100.0;
    targetFrames = NULL;
    numTargetFrames = 0;
    specialValues = vals->data();
    loadWavFile(target);
}

GPExperiment::~GPExperiment() {
    free(targetFrames);
    delete wavFormat;
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
    while (minFitnessAchieved > fitnessThreshold && currentGeneration < numGenerations) {
        GPNetwork* candidate = synth->getIndividual();
        float* candidateData = evaluateIndividual(candidate);
        double fitness = compareToTarget(candidateData);
        if (fitness < minFitnessAchieved) {
            minFitnessAchieved = fitness;
            champ = candidate;
            saveWavFile(String("New Minimum.wav"), String(candidate->toString().c_str()), numTargetFrames, candidateData);
        }
        currentGeneration = synth->assignFitness(candidate, fitness);
    }
    return String(champ->toString().c_str());
}

/*
    =============
    WAV INTERFACE
    =============
*/

void GPExperiment::loadWavFile(String path) {
    File input(path);
    FileInputStream* fis = input.createInputStream();
    AudioSampleBuffer asb(1, 200);
    ScopedPointer<AudioFormatReader> afr(wavFormat->createReaderFor(fis, true));

    numTargetFrames = afr->lengthInSamples;
    sampleRate = afr->sampleRate;

    free(targetFrames);
    targetFrames = (float*) malloc(sizeof(float) * numTargetFrames);
    
    int64 numRemaining = numTargetFrames;
    int64 numCompleted = 0;
    while (numRemaining > 0) {
        int numToRead = numRemaining > 200 ? 200 : numRemaining;
        afr->read(&asb, 0, numToRead, numCompleted, true, false);
        float* chanData = asb.getSampleData(0);
        memcpy(chanData, targetFrames + numCompleted, numToRead);
        numRemaining -= numToRead;
    }
}

void GPExperiment::saveWavFile(String path, String metadata, unsigned numFrames, float* data) {
    File output(path);
    FileOutputStream* fos = output.createOutputStream();
    StringPairArray metaData = WavAudioFormat::createBWAVMetadata(metadata, "", "", Time::getCurrentTime(), 0, "");
    AudioSampleBuffer asb(1, 200);
    ScopedPointer<AudioFormatWriter> afw(wavFormat->createWriterFor(fos, sampleRate, 1, 32, metaData, 0));

    int64 numRemaining = numFrames;
    int64 numComplete = 0;
    while (numRemaining > 0) {
        float* chanData = asb.getSampleData(0);
        if (numRemaining > 200) {
            for (int samp = 0; samp < 200; samp++, numComplete++) {
                chanData[samp] = data[numComplete];
            }
            afw->writeFromAudioSampleBuffer(asb, 0, 200);
            numRemaining -= 200;
        }
        else {
            for (int samp = 0; numComplete < numFrames; numComplete++) {
                chanData[samp] = data[numComplete];
            }
            afw->writeFromAudioSampleBuffer(asb, 0, numRemaining);
            numRemaining -= numRemaining;
        }
    }
}

/*
    ================
    FITNESS FUNCTION
    ================
*/

float* GPExperiment::evaluateIndividual(GPNetwork* candidate) {
    return NULL;
}

double GPExperiment::compareToTarget(float* candidateFrames) {
    double sum = 0;
    for (int frameNum = 0; frameNum < numTargetFrames; frameNum++) {
        sum += pow(targetFrames[frameNum] - candidateFrames[frameNum], 2);
    }
    return sqrt(sum);
}
