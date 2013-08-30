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
    target_spectra = (kiss_fft_cpx*) malloc(sizeof(kiss_fft_cpx) * fft_output_buffer_size);
    target_magnitude = (double*) malloc(sizeof(double) * fft_output_buffer_size);
    target_phase = (double*) malloc(sizeof(double) * fft_output_buffer_size);
    bin_overshoot_p = (double*) malloc(sizeof(double) * fft_output_buffer_size);
    bin_undershoot_p = (double*) malloc(sizeof(double) * fft_output_buffer_size);

    // allocate candidate FFT buffers for speedy comparison
    fft_config = kiss_fftr_alloc(n, 0/*is_inverse_fft*/, NULL, NULL);
    fft_amplitude_buffer = (kiss_fft_scalar*) malloc(sizeof(kiss_fft_scalar) * n);
    fft_spectra_buffer = (kiss_fft_cpx*) malloc(sizeof(kiss_fft_cpx) * fft_output_buffer_size);
    fft_magnitude_buffer = (double*) malloc(sizeof(double) * fft_output_buffer_size);
    fft_phase_buffer = (double*) malloc(sizeof(double) * fft_output_buffer_size);

    // take FFT of target data
    GPAudioUtil::FftReal(fft_config, target_num_frames, n, overlap, analysis_window, fft_amplitude_buffer, target_spectra, target_magnitude, target_phase);
}

GPAudioComparator::~GPAudioComparator() {
    free(target_frames);
    free(analysis_window);
    free(target_spectra);
    free(target_magnitude);
    free(target_phase);
    free(bin_overshoot_p);
    free(bin_undershoot_p);
    free(fft_config);
    free(fft_amplitude_buffer);
    free(fft_spectra_buffer);
    free(fft_magnitude_buffer);
    free(fft_phase_buffer);
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

void save_target_spectrum(std::string path) {
    return;
}
