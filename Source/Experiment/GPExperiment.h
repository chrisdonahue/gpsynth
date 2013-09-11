#ifndef GPEXPERIMENT_H
#define GPEXPERIMENT_H

// GP object includes
#include "../Common/GPLogger.h"
#include "../Common/GPHelpers.h"
#include "../Common/GPAudioUtil.h"
#include "../Synth/GPNode.h"
#include "../Synth/GPSynth.h"
#include "GPAudioComparator.h"

// JUCE include
#include "JuceHeader.h"
#include "../Common/JUCEFileIO.h"

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
    bool log_save_gen_summary_file;
    bool log_save_gen_champ_audio;
    bool log_save_overall_champ_audio;
    bool log_save_target_spectrum;
    bool log_save_target_copy;

    // auxiliary experiment parameters
    unsigned aux_wav_file_buffer_size;
    unsigned aux_render_block_size;

    // experiment parameters
    unsigned exp_ff_type;
    unsigned exp_suboptimize_ff_type;
    unsigned exp_suboptimize_type;
    unsigned exp_generations;
    double exp_threshold;
};

class GPExperiment {
public:
    // construction
    GPExperiment(GPLogger* logger, GPMatchingExperimentParams* params, unsigned seed, std::string beagle_cfg_file_path, GPSynth* synth, GPAudioComparator* comparator, std::string output_dir_path, std::vector<float>& constants);
    GPExperiment(GPLogger* logger);
    ~GPExperiment();

    // evolution control
    GPNetwork* evolve();
    double beagleComparisonCallback(unsigned type, float* candidateFramesBuffer);

    // sanity test
    int sanityTest(GPRandom* rng);

private:
    bool is_sanity_test;

    // GP objects
    GPLogger* logger;
    GPSynth* synth;
    GPAudioComparator* comparator;

    // experiment params
    GPMatchingExperimentParams* params;
    unsigned seed;
    std::string seed_string;
    std::string beagle_cfg_file_path;
    std::string output_dir_path;

    // suboptimization state
    GPNetwork* suboptimize_network;
    std::vector<GPMutatableParam*> suboptimize_best_params;
    double suboptimize_min_fitness;

    // target metadata
    float target_sampling_frequency;
    float target_nyquist_frequency;
    unsigned target_num_frames;
    float target_last_sample_start_time;

    // evaluation data
    float* target_sample_times;
    unsigned numConstantValues;
    float* constantValues;
    unsigned numVariableValues;
    float* variableValues;

    // experiment state
    double minFitnessAchieved;
    int numEvaluatedGenerations;

    // FITNESS FUNCTION
    double suboptimizeAndCompareToTarget(unsigned suboptimizeType, GPNetwork* candidate, float* buffer);
    void renderIndividualByBlockPerformance(GPNetwork* candidate, unsigned renderblocksize, unsigned numconstantvalues, float* constantvalues, int64 numsamples, float* sampletimes, float* buffer);
    double compareToTarget(unsigned type, float* candidateFrames);
};

#endif
