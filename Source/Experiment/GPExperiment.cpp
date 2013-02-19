#include "GPExperiment.h"

/*
    ============
    CONSTRUCTION
    ============
*/

GPExperiment::GPExperiment(String target, int psize, unsigned s, bool lowerbetter, double addchance, double mutatechance, double crosschance, double threshold, int numGenerations) {
    numTargetFrames = numFramesInWavFile(target);
    targetFrames = loadWavFile(target);
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

String GPExperiment::evolve(unsigned numFrames, float** targetData) {
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

float** GPExperiment::loadWavFile(String path) {
    return NULL;
}

unsigned GPExperiment::numFramesInWavFile(String path) {
    return 0;
}

void GPExperiment::saveWavFile(String path, String metadata, float** data) {
    return;
}

/*
    ================
    FITNESS FUNCTION
    ================
*/

float** GPExperiment::evaluateIndividual(GPNetwork* candidate, unsigned numFrams) {
    return NULL;
}

double GPExperiment::compare(float** dataone, float** datatwo) {
    return -1;
}
