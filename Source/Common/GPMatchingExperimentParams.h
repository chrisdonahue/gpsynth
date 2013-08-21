#ifndef GPMATCHINGEXPERIMENTPARAMS_H
#define GPMATCHINGEXPERIMENTPARAMS_H

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

#endif
