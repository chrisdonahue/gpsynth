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
        GPExperiment(String target, unsigned s, unsigned expnum, double threshold, unsigned numgens, std::vector<double>* vals, unsigned psize, unsigned mid, unsigned md, unsigned crosstype, unsigned selecttype, double crosspercent, double addchance, double subchance, double mutatechance);
        ~GPExperiment();

        // EVOLUTION CONTROL
        GPNetwork* evolve();

    private:
        // EXPERIMENT PARAMETERS
        float fitnessThreshold;
        int numGenerations;
        bool lowerFitnessIsBetter;
        double bestPossibleFitness;

        // TARGET DATA CONTAINERS
        double sampleRate;
        int64 numTargetFrames;
        float* targetFrames;
        double* specialValues;

        // EXPERIMENT STATE
        float minFitnessAchieved;
        int numEvaluatedGenerations;

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
