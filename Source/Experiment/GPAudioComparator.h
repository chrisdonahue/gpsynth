#ifndef GPAUDIOCOMPARATOR_H
#define GPAUDIOCOMPARATOR_H

#include "JuceHeader.h"
#include "../Common/GPAudioUtil.h"

// params for log stream
struct GPAudioComparatorParams {
};

class GPAudioComparator {
    public:
        // constructors
        GPAudioComparator(GPLogger* logger, GPAudioComparatorParams* params);
        ~GPAudioComparator();

        // accessors
        unsigned long get_target_num_frames();
        double get_target_sampling_frequency();
        double get_target_length_seconds();
        float get_target_last_sample_time();

        // comparators
        double compare_amplitude(float* candidate_frames);
        double compare_spectra(float* candidate_frames);
        double compare_spectra_weighted(float* candidate_frames);

        // others
        void save_target_backup(std::string path);

    private:
        // GP object refs
        GPLogger* logger;

        // params
        GPAudioComparatorParams* params;

        // metadata
        unsigned long target_num_frames;
        unsigned target_bits_per_sample;
        double target_length_seconds;
        double target_sampling_frequency;
        double target_nyquist_frequency;
        
        // time domain
        float* target_frames;

        // freq domain
        unsigned fft_output_buffer_size;
        float* analysis_window;
        kiss_fft_cpx* target_spectra;
        double* target_magnitude;
        double* target_phase;
        double* bin_overshooting_penalty;
        double* bin_undershooting_penalty;
        
        // temporary FFT buffers
        kiss_fftr_cfg fft_config;
        kiss_fft_scalar* fft_amplitude_buffer;
        kiss_fft_cpx* fft_spectra_buffer;
        double* fft_magnitude_buffer;
        double* fft_phase_buffer;
};

#endif
