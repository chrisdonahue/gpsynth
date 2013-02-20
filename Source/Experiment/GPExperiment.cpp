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

GPExperiment::GPExperiment(String target, unsigned expnum, unsigned psize, unsigned s, double addchance, double mutatechance, double crosschance, double threshold, unsigned numGenerations, unsigned selecttype, unsigned crosstype, std::vector<double>* vals) {
    if (expnum == 0) {
        std::vector<GPNode*>* nodes = new std::vector<GPNode*>();
        std::vector<double>* nlikelihoods = new std::vector<double>();
        std::vector<GPFunction>* functions = new std::vector<GPFunction>();
        std::vector<double>* flikelihoods = new std::vector<double>();
        synth = new GPSynth(psize, s, 0.0, addchance, mutatechance, crosschance, crosstypep, selecttype, nodes, nlikelihoods, functions, flikelihoods);
    }
    samplerate = 44100.0;
    targetFrames = NULL;
    numTargetFrames = 0;
    specialValues = vals->data();
    loadWavFile(target);
}

GPExperiment::~GPExperiment() {

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
        float** candidateData = evaluateIndividual(candidate, numFrames);
        double fitness = compare(candidateData, targetData);
        if (fitness < minFitnessAchieved) {
            minFitnessAchieved = fitness;
            champ = candidate;
            saveWavFile(String("New Minimum.wav"), String(candidate->toString().c_str()), candidateData);
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

float* GPExperiment::loadWavFile(String path) {
    return NULL;
}

void GPExperiment::saveWavFile(String path, String metadata, unsigned numFrames, float* data) {
    ScopedPointer<WavAudioFormat> wavFormat(new WavAudioFormat());
    File output(path);
    FileOutputStream *fos = output.createOutputStream();
    StringPairArray metaData = WavAudioFormat::createBWAVMetadata(metadata, "", "", Time::getCurrentTime(), 0, "");
    AudioSampleBuffer asb(1, 200);
    ScopedPointer<AudioFormatWriter> afw(wavFormat->createWriterFor(fos, sampleRate, 1, 32, metaData, 0));

    unsigned numRemaining = numFrames;
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
