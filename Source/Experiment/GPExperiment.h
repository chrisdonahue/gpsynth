/*
  ==============================================================================

    GPExperiment.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef GPEXPERIMENT_H
#define GPEXPERIMENT_H

#include "../Common/GPFunctions.h"
#include "../Common/GPParams.h"

#include "../Synth/GPSynth.h"
#include "../Synth/Nodes/ConstantNode.h"
#include "../Synth/Nodes/TimeNode.h"
#include "../Synth/Nodes/VariableNode.h"
#include "../Synth/Nodes/FunctionNode.h"
#include "../Synth/Nodes/OscilNode.h"
//#include "../Synth/Nodes/DelayNode.h"
//#include "../Synth/Nodes/FilterNode.h"
//#include "../Synth/Nodes/NoiseNode.h"

#include "../Dependencies/kissfft/kiss_fft.h"
#include "../Dependencies/kissfft/kiss_fftr.h"
#include "../../JuceLibraryCode/JuceHeader.h"

#include <limits>

class GPExperiment {
    public:
        // CONSTUCTION
        GPExperiment(String target, GPParams* p);
        ~GPExperiment();

        // EVOLUTION CONTROL
        GPNetwork* evolve();

    private:
        // EXPERIMENT PARAMETERS
        GPParams* params;
        float fitnessThreshold;
        int numGenerations;
        bool lowerFitnessIsBetter;
        double silenceFitness;
        double bestPossibleFitness;
        double worstFitness;

        // TARGET DATA CONTAINERS
        double sampleRate;
        int64 numTargetFrames;
        float* targetFrames;
        kiss_fft_cpx* targetSpectrum;
        double* targetSpectrumMagnitudes;
        double* targetSpectrumPhases;

        // EVALUATION DATA
        double* sampleTimes;
        double* specialValues;

        // EXPERIMENT STATE
        float minFitnessAchieved;
        int numEvaluatedGenerations;

        // SYNTH
        GPSynth* synth;

        // WAV INTERFACE
        ScopedPointer<WavAudioFormat> wavFormat;
        unsigned wavFileBufferSize;
        void loadTargetWavFile(String path);
        void saveWavFile(String path, String metadata, unsigned numFrames, float* data);

        // FITNESS FUNCTION
        float* renderIndividual(GPNetwork* candidate);
        double interactiveFitness(GPNetwork* candidate);
        double compareToTarget(float* candidateFrames);
        void FftReal(unsigned n, const kiss_fft_scalar* in, kiss_fft_cpx* out, double* magnitude, double* phase);
};

#endif
