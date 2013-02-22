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
        GPExperiment(String target, unsigned expnum, unsigned psize, unsigned s, double addchance, double subchance, double mutatechance, double crosschance, double threshold, unsigned numgens, unsigned selecttype, unsigned crosstype, std::vector<double>* vals);
        ~GPExperiment();

        // EVOLUTION CONTROL
        String evolve();

    private:
        // EXPERIMENT PARAMETERS
        float fitnessThreshold;
        int numGenerations;

        // EXPERIMENT STATE
        float minFitnessAchieved;
        int currentGeneration;

        // TARGET DATA CONTAINERS
        double sampleRate;
        int64 numTargetFrames;
        float* targetFrames;
        double* specialValues;

        // SYNTH
        GPSynth* synth;
        GPNodeParams* nodeParams;

        // WAV INTERFACE
        ScopedPointer<WavAudioFormat> wavFormat;
        void loadWavFile(String path);
        void saveWavFile(String path, String metadata, unsigned numFrames, float* data);

        // FITNESS FUNCTION
        float* evaluateIndividual(GPNetwork* candidate);
        double compareToTarget(float* candidateFrames);
};

#endif
