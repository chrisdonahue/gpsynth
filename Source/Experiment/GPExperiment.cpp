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
    if (expnum == 0) {
        std::vector<GPNode*>* nodes = new std::vector<GPNode*>();
        nodes.push_back(new FunctionNode(multiply(), "*", NULL, NULL));
        nodes.push_back(new FunctionNode(add(), "+", NULL, NULL));
        nodes.push_back(new ValueNode(0, -1));
        nodes.push_back(new ValueNode(-1, 0));
        nodes.push_back(new ValueNode(-1, 1));
        nodes.push_back(new OscilNode(1, 0, NULL, NULL));

        std::vector<double>* nlikelihoods = new std::vector<double>();
        nlikelihoods.push_back(1);
        nlikelihoods.push_back(1);
        nlikelihoods.push_back(1);
        nlikelihoods.push_back(1);
        nlikelihoods.push_back(1);
        nlikelihoods.push_back(1);

        std::vector<GPFunction*>* functions = new std::vector<GPFunction*>();
        functions.push_back(add());
        functions.push_back(multiply());

        std::vector<double>* flikelihoods = new std::vector<double>();
        flikelihoods.push_back(1);
        flikelihoods.push_back(1);

        synth = new GPSynth(psize, s, 0.0, addchance, subchance, mutatechance, crosschance, crosstype, selecttype, nodes, nlikelihoods, functions, flikelihoods);
    }
    sampleRate = 44100.0;
    targetFrames = NULL;
    numTargetFrames = 0;
    specialValues = vals->data();
    loadWavFile(target);
}

GPExperiment::~GPExperiment() {
    free(targetFrames);
    delete wavFormat;
    delete nodes;
    delete nlikelihoods;
    delete functions;
    delete flikelihoods;
}

/*
    =================
    EVOLUTION CONTROL
    =================
*/

void GPExperiment::evolve() {
    evolve(numTargetFrames, targetFrames);
}

String GPExperiment::evolve(unsigned numFrames, float* targetData) {
    GPNetwork* champ;
    while (minFitnessAchieved > fitnessThreshold && currentGeneration < numGenerations) {
        GPNetwork* candidate = synth->getIndividual();
        float* candidateData = evaluateIndividual(candidate, numFrames);
        double fitness = compare(candidateData, targetData);
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
    ScopedPointer<AudioFormatReader> afr(wavFormat->createReaderFor(fis));

    numTargetFrames = afr.lengthInSamples();
    sampleRate = sampleRate();

    free(targetFrames);
    targetFrames = (float*) malloc(sizeof(gloat) * numTargetFrames);
    
    int64 numRemaining = numTargetFrames;
    int64 numCompleted = 0;
    while (numRemaining > 0) {
        unsigned numToRead = numRemaining > 200 ? 200 : numRemaining;
        afr.read(asb, 0, numToRead, numCompleted, true, false);
        float* chanData = asb.getSampleData(0);
        memcpy(chanData, targetFrames[numCompleted], numToRead);
        numRemaining -= numToRead;
    }
}

void GPExperiment::saveWavFile(String path, String metadata, unsigned numFrames, float* data) {
    File output(path);
    FileOutputStream *fos = output.createOutputStream();
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

float* GPExperiment::evaluateIndividual(GPNetwork* candidate, unsigned numFrams) {
    return NULL;
}

double GPExperiment::compare(float* dataone, float* datatwo) {
    return -1;
}
