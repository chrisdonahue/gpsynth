/*
  ==============================================================================

    GPExperiment.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef GPEXPERIMENT_H
#define GPEXPERIMENT_H

#include "../Synth/GPSynth.h"
#include "../../JuceLibraryCode/JuceHeader.h"

class GPExperiment {
    public:
        // CONSTUCTION
        GPExperiment(String target, unsigned psize, unsigned s, double addchance, double mutatechance, double crosschance, double threshold, unsigned numGenerations, unsigned selecttype, unsigned crosstype, std::vector<double>* vals);
        ~GPExperiment();

        // EVOLUTION CONTROL
        void evolve();

    private:
        // EXPERIMENT PARAMETERS
        float fitnessThreshold;
        int numGenerations;
        int numMinimum;

        // EXPERIMENT STATE
        GPSynth* synth;
        float minFitnessAchieved;
        int currentGeneration;

        // TARGET DATA CONTAINERS
        double sampleRate;
        unsigned numTargetFrames;
        float* targetFrames;
        double* specialValues;

        // EVOLUTION CONTOL
        String evolve(unsigned numFrames, float* targetData);

        // WAV INTERFACE
        void loadWavFile(String path);
        void saveWavFile(String path, String metadata, unsigned numFrames, float* data);

        // FITNESS FUNCTION
        float* evaluateIndividual(GPNetwork* candidate, unsigned numFrames);
        double compare(float* dataone, float* datatwo);
};

#endif
