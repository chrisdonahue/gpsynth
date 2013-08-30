#include "GPAudioComparator.h"

/*
    constructors
*/

GPAudioComparator::GPAudioComparator(GPAudioComparatorParams* params, std::string target_file_path)
{
    // fetch metadata
    GPAudioUtil::get_wav_file_metadata(target_file_path, &target_num_frames, &target_bits_per_sample, &target_length_seconds, &target_sampling_frequency, &target_nyquist_frequency);

    // fetch wav file frames
    target_frames = (float*) malloc(sizeof(float) * target_num_frames);
    GPAudioUtil::load_wav_file(target_file_path, params->wav_file_buffer_size, target_num_frames, target_frames);

    // set up FFT info
    unsigned n = params->fft_size;
    assert((n > 0) && ((n & (n - 1)) == 0));
    unsigned overlap = params->fft_overlap;
    assert(overlap < n);
    fft_output_buffer_size = GPAudioUtil::calculateFftBufferSize(target_num_frames, n, overlap);

    // allocate and create window
    analysis_window = (float*) malloc(sizeof(float) * n);
    GPAudioUtil::window(params->fft_window, n, analysis_window);

    // allocate FFT buffers
    kiss_fft_scalar* target_amplitude_buffer = (kiss_fft_scalar*) malloc(sizeof(kiss_fft_scalar) * n);
    target_spectra = (kiss_fft_cpx*) malloc(sizeof(kiss_fft_cpx) * fft_output_buffer_size);
    target_phase = (double*) malloc(sizeof(double) * fft_output_buffer_size);

}

GPAudioComparator::~GPAudioComparator() {
    free(target_frames);
    free(analysis_window);
}

/*
    accessors
*/

unsigned long GPAudioComparator::get_target_num_frames() {
    return target_num_frames;
}

unsigned GPAudioComparator::get_target_bits_per_sample() {
    return target_bits_per_sample;
}

double GPAudioComparator::get_target_length_seconds() {
    return target_length_seconds;
}

double GPAudioComparator::get_target_sampling_frequency() {
    return target_sampling_frequency;
}

double GPAudioComparator::get_target_nyquist_frequency() {
    return target_nyquist_frequency;
}

float GPAudioComparator::get_target_last_sample_time() {
    return (float) (float(target_num_frames - 1)/target_sampling_frequency);
}

/*
    comparators
*/

double GPAudioComparator::compare_amplitude(float* candidate_frames) {
    return 0.0f;
}

double GPAudioComparator::compare_spectra(float* candidate_frames) {
    return 0.0f;
}

double GPAudioComparator::compare_spectra_weighted(float* candidate_frames) {
    return 0.0f;
}

/*
    others
*/

void save_target_backup(std::string path) {
    return;
}
