#ifndef GPEXPERIMENT_H
#define GPEXPERIMENT_H

// Common GPSynth includes
#include "../Common/GPLogger.h"
#include "../Common/GPHelpers.h"
#include "../Common/GPAudioUtil.h"
#include "../Common/JUCEFileIO.h"

// GPSynth includes
#include "../Synth/GPNode.h"
#include "../Synth/GPSynth.h"

// JUCE include
#include "JuceHeader.h"

// BEAGLE includes
#include <numeric>
#include <stdexcept>
#include "beagle/Exception.hpp"
#include "beagle/GA.hpp"
#include "AudioComparisonEvalOp.hpp"
#include "AudioComparisonParams.hpp"

// other includes
#include "../Dependencies/kissfft/kiss_fftr.h"
#include <limits>

struct GPMatchingExperimentParams {
    // logging parameters
    bool log_save_gen_champ_audio;
    bool log_save_overall_champ_audio;
    bool log_save_target_spectrum;
    bool log_save_target_copy;

    // auxiliary experiment parameters
    unsigned aux_wav_file_buffer_size;
    unsigned aux_render_block_size;

    // experiment parameters
    unsigned exp_suboptimize_type;
    unsigned exp_generations;
    double exp_threshold;

    // fitness function
    unsigned ff_type;
    double ff_spectrum_mag_weight;
    double ff_spectrum_phase_weight;
    char ff_fft_window[5];
    unsigned ff_fft_size;
    unsigned ff_fft_overlap;
    unsigned ff_moving_average_type;
    unsigned ff_moving_average_past_radius;
    unsigned ff_moving_average_future_radius;
    double ff_moving_average_exponential_alpha;
    bool ff_weight_frames;
    double ff_weight_frames_exponent;
    double ff_phase_comparison_exponent;
    double ff_mag_base_comparison;
    double ff_mag_good_comparison;
    double ff_mag_bad_comparison;
};

class GPExperiment {
public:
    // CONSTRUCTION
    GPExperiment(GPLogger* logger, GPMatchingExperimentParams* params, GPSynth* synth, std::string target_file_path, std::string output_dir_path, std::vector<float>& constants);
    GPExperiment(GPLogger* logger);
    ~GPExperiment();

    // EVOLUTION CONTROL
    GPNetwork* evolve();
    double beagleComparisonCallback(unsigned type, GPNetwork* candidate, float* candidateFramesBuffer);

    // SANITY TEST CODE
    int sanityTest(GPRandom* rng);

private:
    bool is_sanity_test;

    // EXPERIMENT PARAMETERS
    GPLogger* logger;
    GPSynth* synth;
    GPMatchingExperimentParams* params;
    unsigned seed;
    String targetPath;
    String savePath;
    float fitnessThreshold;
    int numGenerations;

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
};

#endif
