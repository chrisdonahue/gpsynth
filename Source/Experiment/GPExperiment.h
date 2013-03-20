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
#include "../Synth/Nodes/ModOscilNode.h"
#include "../Synth/Nodes/FilterNode.h"
#include "../Synth/Nodes/NoiseNode.h"

#include "../Dependencies/kissfft/kiss_fft.h"
#include "../Dependencies/kissfft/kiss_fftr.h"
#include "../../JuceLibraryCode/JuceHeader.h"

#include <limits>

class GPExperiment {
    public:
        // CONSTUCTION
        GPExperiment(GPRandom* r, String target, GPParams* p, double* constants);
        ~GPExperiment();

        // EVOLUTION CONTROL
        GPNetwork* evolve();

    private:
        // EXPERIMENT PARAMETERS
        GPParams* params;
        float fitnessThreshold;
        int numGenerations;
        bool lowerFitnessIsBetter;
        double bestPossibleFitness;
        double penaltyFitness;

        // TARGET DATA CONTAINERS
        double targetSampleRate;
        unsigned numTargetFrames;
        float* targetFrames;
        kiss_fft_cpx* targetSpectrum;
        double* targetSpectrumMagnitudes;
        double* targetSpectrumPhases;
        double* weightMatrix;

        // EVALUATION DATA
        double* sampleTimes;
        double* specialValues;
        // TODO: replace specialValues with specialValuesByTime
        double* specialValuesByFrame;
        unsigned numSpecialValues;

        // EXPERIMENT STATE
        float minFitnessAchieved;
        int numEvaluatedGenerations;

        // SYNTH
        GPSynth* synth;

        // FILL EVALUATION BUFFERS
        void fillEvaluationBuffers(double* constantSpecialValues, double* variableSpecialValues, unsigned numConstantSpecialValues, unsigned numVariableSpecialValues);

        // WAV INTERFACE
        ScopedPointer<WavAudioFormat> wavFormat;
        unsigned wavFileBufferSize;
        void getWavFileInfo(String path, unsigned* numFrames, double* sampleRate);
        void loadWavFile(String path, unsigned n, float* buffer);
        // TODO: move targetBuffer stuff from loadTargetWavFile to fillEvaluationBuffers
        void saveWavFile(String path, String metadata, unsigned numFrames, double sampleRate, float* data);

        // FITNESS FUNCTION
        double suboptimize(GPNetwork* candidate, int64 numSamples, float* buffer);
        void renderIndividual(GPNetwork* candidate, int64 numSamples, float* buffer);
        void renderIndividualByBlock(GPNetwork* candidate, int64 numSamples, unsigned n, float* buffer);
        double compareToTarget(unsigned type, float* candidateFrames);

        // FOURIER TRANSFORM
        unsigned calculateFftBufferSize(unsigned numFrames, unsigned n);
        void FftReal(unsigned n, const kiss_fft_scalar* in, kiss_fft_cpx* out, double* magnitude, double* phase);
        void FftReal(unsigned numFrames, const float* input, unsigned n, kiss_fft_cpx* out, double* magnitude, double* phase);
};

#endif
