/*
  ==============================================================================

    GPExperiment.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef GPEXPERIMENT_H
#define GPEXPERIMENT_H

// Common GPSynth includes
#include "../Common/GPParams.h"
#include "../Common/GPHelpers.h"
#include "../Common/GPAudioUtil.h"
#include "../Common/JUCEFileIO.h"

// GPSynth includes
#include "../Synth/GPNode.h"
#include "../Synth/GPPrimitives.h"
#include "../Synth/GPSynth.h"

// JUCE include
#include "JuceHeader.h"

// BEAGLE includes
#include <numeric>
#include <stdexcept>
#include "beagle/GA.hpp"
//#include "PACC/Math/Vector.hpp"
#include "AudioComparisonEvalOp.hpp"

// other includes
#include "../Dependencies/kissfft/kiss_fftr.h"
#include <limits>
#include <fstream>

class GPExperiment {
public:
    // CONSTUCTION
    GPExperiment(GPParams* p, GPRandom* rng, unsigned s, String target, String path, unsigned numconstants, float* constants, bool* rq);
    ~GPExperiment();

    // EVOLUTION CONTROL
    GPNetwork* evolve();
    double beagleComparisonCallback(unsigned type, GPNetwork* candidate, float* candidateFramesBuffer);

private:
    // EXPERIMENT PARAMETERS
    GPParams* params;
    unsigned seed;
    String targetPath;
    String savePath;
    float fitnessThreshold;
    int numGenerations;
    bool lowerFitnessIsBetter;

    // TARGET DATA CONTAINERS
    // metadata
    float targetSampleRate;
    float targetNyquist;
    unsigned numTargetFrames;
    float targetLengthSeconds;
    // time domain
    float* targetFrames;
    float* targetEnvelope;
    // freq domain
    unsigned fftOutputBufferSize;
    kiss_fft_cpx* targetSpectra;
    double* targetMagnitude;
    double* targetPhase;
    // fitness function analysis
    float* analysisWindow;
    double* binOvershootingPenalty;
    double* binUndershootingPenalty;
    double* fftFrameWeight;
    
    // COMPARISON BUFFERS
    kiss_fftr_cfg fftConfig;
    kiss_fft_scalar* candidateAmplitudeBuffer;
    kiss_fft_cpx* candidateSpectraBuffer;
    double* candidateMagnitudeBuffer;
    double* candidatePhaseBuffer;

    // EVALUATION DATA
    float* targetSampleTimes;
    unsigned numConstantValues;
    float* constantValues;
    unsigned numVariableValues;
    float* variableValues;

    // EXPERIMENT STATE
    double minFitnessAchieved;
    int numEvaluatedGenerations;
    bool* requestedQuit;

    // SYNTH
    GPSynth* synth;

    // FILL EVALUATION BUFFERS
    void fillEvaluationBuffers(unsigned numconstantvalues, float* constantvalues, unsigned numvariablevalues, float* variablevalues);

    // FITNESS FUNCTION
    double suboptimizeAndCompareToTarget(unsigned suboptimizeType, GPNetwork* candidate, float* buffer);
    void renderIndividualByBlockPerformance(GPNetwork* candidate, unsigned renderblocksize, unsigned numconstantvalues, float* constantvalues, int64 numsamples, float* sampletimes, float* buffer);
    double compareToTarget(unsigned type, float* candidateFrames);

    // WAVEFORM OPERATIONS
    void findMovingAverage(unsigned type, unsigned n, const double* buffer, double* movingaverage, unsigned pastRadius, unsigned futureRadius, double alpha, double* frameaverage, double* maxdeviationabove, double* maxdeviationbelow, double* maxratioabove, double* minratiobelow);
    void followEnvelope(unsigned n, float* buffer, float* envelope, double attack_in_ms, double release_in_ms, double samplerate);
    void findEnvelope(bool ignoreZeroes, unsigned n, float* wav, float* env);

    // GRAPH HELPERS
    void fillTimeAxisBuffer(unsigned numSamples, float sr, float* buffer);
    void fillFrequencyAxisBuffer(unsigned fftSize, double sr, float* buffer);
    String floatBuffersToGraphText(String options, String xlab, String ylab, bool indexAsX, unsigned n, const float* x, const float* y, const float* z);
    String doubleBuffersToGraphText(String options, String xlab, String ylab, String zlab, bool indexAsX, unsigned n, const double* x, const double* y, const double* z);

    // SANITY TEST CODE
    void sanityTest(GPRandom* rng);
};

#endif
