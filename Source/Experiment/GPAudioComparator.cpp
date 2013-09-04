#include "GPAudioComparator.h"

/*
    constructors
*/

GPAudioComparator::GPAudioComparator(GPLogger* logger, GPAudioComparatorParams* params, std::string target_file_path) :
    logger(logger),
    params(params)
{
    // fetch metadata
    JUCEFileIO::get_wav_file_metadata(target_file_path, &target_num_frames, &target_bits_per_sample, &target_length_seconds, &target_sampling_frequency, &target_nyquist_frequency);
    target_last_sample_start_time = (float) (float(target_num_frames - 1)/target_sampling_frequency);

    // fetch wav file frames
    target_frames = (float*) malloc(sizeof(float) * target_num_frames);
    JUCEFileIO::load_wav_file(target_file_path, params->aux_wav_file_buffer_size, target_num_frames, target_frames);

    // set up FFT info
    unsigned n = params->fft_size;
    assert((n > 0) && ((n & (n - 1)) == 0));
    unsigned overlap = params->fft_overlap;
    assert(overlap < n);
    GPAudioUtil::calculate_fft_buffer_size(target_num_frames, n, overlap, &fft_num_bins, &fft_num_frames, &fft_output_buffer_length);

    // allocate and create window
    analysis_window = (float*) malloc(sizeof(float) * n);
    GPAudioUtil::window(params->fft_window, n, analysis_window);

    // allocate FFT buffers
    target_spectra = (kiss_fft_cpx*) malloc(sizeof(kiss_fft_cpx) * fft_output_buffer_length);
    target_magnitude = (double*) malloc(sizeof(double) * fft_output_buffer_length);
    target_magnitude_moving_average = (double*) malloc(sizeof(double) * fft_output_buffer_length);
    target_phase = (double*) malloc(sizeof(double) * fft_output_buffer_length);
    bin_overshoot_p = (double*) malloc(sizeof(double) * fft_output_buffer_length);
    bin_undershoot_p = (double*) malloc(sizeof(double) * fft_output_buffer_length);

    // allocate candidate FFT buffers for speedy comparison
    fft_config = kiss_fftr_alloc(n, 0/*is_inverse_fft*/, NULL, NULL);
    fft_amplitude_buffer = (kiss_fft_scalar*) malloc(sizeof(kiss_fft_scalar) * n);
    fft_spectra_buffer = (kiss_fft_cpx*) malloc(sizeof(kiss_fft_cpx) * fft_output_buffer_length);
    fft_magnitude_buffer = (double*) malloc(sizeof(double) * fft_output_buffer_length);
    fft_phase_buffer = (double*) malloc(sizeof(double) * fft_output_buffer_length);

    // take FFT of target data
    GPAudioUtil::fft_real(fft_config, target_num_frames, target_frames, n, overlap, analysis_window, fft_amplitude_buffer, target_spectra, target_magnitude, target_phase);

    // calculate penalties for each frame
    double base = params->mag_comparison_p;
    double good = params->mag_good_comparison_additional_p;
    double bad = params->mag_bad_comparison_additional_p;
    double frame_average_sum = 0.0;
    for (unsigned i = 0; i < fft_num_frames; i++) {
        // find moving average
        unsigned data_offset = (i * fft_num_bins) + 1;
        double* moving_average = target_magnitude_moving_average + data_offset;
        double frame_average, max_deviation_below, max_deviation_above;
        GPAudioUtil::find_moving_average(params->moving_average_type, fft_num_bins - 1, target_magnitude + data_offset, moving_average, &frame_average, &max_deviation_below, &max_deviation_above, params->moving_average_past_radius, params->moving_average_future_radius, params->moving_average_exponential_alpha);
        //fftFrameWeight[i] = frameAverage;
        frame_average_sum += frame_average;

        // compare each bin EXCEPT DC OFFSET to the moving average magnitude
        for (unsigned j = 1; j < fft_num_bins; j++) {
            unsigned bin_index = (i * fft_num_bins) + j;
            double bin_magnitude = target_magnitude[bin_index];
            double bin_average = moving_average[j - 1];
            
            // calculate bin comparison metric data
            double numerator, denominator, proportion;
            numerator = fabs(bin_magnitude - bin_average);
            if (bin_magnitude > bin_average) {
                denominator = max_deviation_above;
            }
            else {
                denominator = max_deviation_below;
            }

            // calculate bin proportion for penalty assignments
            //proportion = pow(numerator / denominator, params->penaltyComparisonExponent);
            proportion = numerator / denominator;

            // check to make sure proportions are correct
            assert(proportion <= 1);

            // if we are above the mean penalize undershooting more
            if (bin_magnitude > bin_average) {
                bin_undershoot_p[bin_index] = (proportion * bad) + base;
                bin_overshoot_p[bin_index] = (proportion * good) + base;
            }

            // if we are below the mean penalize overshooting more
            else {
                bin_undershoot_p[bin_index] = (proportion * good) + base;
                bin_overshoot_p[bin_index] = (proportion * bad) + base;
            }
        }
    }
}

GPAudioComparator::~GPAudioComparator() {
    free(target_frames);
    free(analysis_window);
    free(target_spectra);
    free(target_magnitude);
    free(target_magnitude_moving_average);
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

float GPAudioComparator::get_target_last_sample_start_time() {
    return target_last_sample_start_time;
}

/*
    comparators
*/

double GPAudioComparator::compare_amplitude(float* candidate_frames) {
    double error = 0;
    for (unsigned long i = 0; i < target_num_frames; i++) {
        error += fabs(target_frames[i] - candidate_frames[i]);
    }
    return error;
}

double GPAudioComparator::compare_amplitude_weighted(float* candidate_frames) {
    double error = 0;
    double amp_comparison_p = params->amp_comparison_p;
    for (unsigned long i = 0; i < target_num_frames; i++) {
        error += pow(fabs(target_frames[i] - candidate_frames[i]), amp_comparison_p);
    }
    return error;
}

double GPAudioComparator::compare_spectra(float* candidate_frames) {
    GPAudioUtil::fft_real(fft_config, target_num_frames, candidate_frames, params->fft_size, params->fft_overlap, analysis_window, fft_amplitude_buffer, fft_spectra_buffer, fft_magnitude_buffer, fft_phase_buffer);

    double magnitude_error = 0;
    double phase_error = 0;
    for (unsigned i = 0; i < fft_num_frames; i++) {
        double frame_magnitude_error = 0;
        double frame_phase_error = 0;
        unsigned frame_index = (i * fft_num_bins);
        for (unsigned j = 1; j < fft_num_bins; j++) {
            unsigned bin_index = frame_index + j;
            frame_magnitude_error += pow(fabs(fft_magnitude_buffer[bin_index] - target_magnitude[bin_index]), params->mag_comparison_p);
            frame_phase_error += pow(fabs(fft_phase_buffer[bin_index] - target_phase[bin_index]), params->phase_comparison_p);
        }
        magnitude_error += frame_magnitude_error;
        phase_error += frame_phase_error;
    }
    
    return (magnitude_error * params->mag_weight_multiplier) + (phase_error * params->phase_weight_multiplier);
}

double GPAudioComparator::compare_spectra_weighted(float* candidate_frames) {
    GPAudioUtil::fft_real(fft_config, target_num_frames, candidate_frames, params->fft_size, params->fft_overlap, analysis_window, fft_amplitude_buffer, fft_spectra_buffer, fft_magnitude_buffer, fft_phase_buffer);

    double magnitude_error = 0;
    double phase_error = 0;
    for (unsigned i = 0; i < fft_num_frames; i++) {
        double frame_magnitude_error = 0;
        double frame_phase_error = 0;
        unsigned frame_index = (i * fft_num_bins);
        for (unsigned j = 1; j < fft_num_bins; j++) {
            unsigned bin_index = frame_index + j;
            if (fft_magnitude_buffer[bin_index] < target_magnitude[bin_index]) {
                frame_magnitude_error += pow(target_magnitude[bin_index] - fft_magnitude_buffer[bin_index], bin_undershoot_p[bin_index]);
            }
            else {
                frame_magnitude_error += pow(fft_magnitude_buffer[bin_index] - target_magnitude[bin_index], bin_overshoot_p[bin_index]);
            }
            frame_phase_error += pow(fabs(fft_phase_buffer[bin_index] - target_phase[bin_index]), params->phase_comparison_p);
        }
        magnitude_error += frame_magnitude_error;
        phase_error += frame_phase_error;
    }
    
    return (magnitude_error * params->mag_weight_multiplier) + (phase_error * params->phase_weight_multiplier);
}

/*
    others
*/

void GPAudioComparator::save_target_backup(std::string path) {
    return;
}

void GPAudioComparator::save_target_spectrum(std::string path) {
    unsigned n = params->fft_size;
    unsigned overlap = params->fft_overlap;
    double* time_axis = (double*) malloc(sizeof(double) * (fft_num_bins - 1));
    float* float_freq_axis = (float*) malloc(sizeof(float) * fft_num_bins);
    GPAudioUtil::fill_frequency_domain_buffer(n, target_sampling_frequency, float_freq_axis);
    double* freq_axis = (double*) malloc(sizeof(double) * (fft_num_bins));
    for (unsigned i = 0; i < fft_num_bins; i++) {
        freq_axis[i] = (double) float_freq_axis[i];
    }
    free(float_freq_axis);
    for (unsigned i = 0; i < fft_num_frames; i++) {
        unsigned data_offset = (i * fft_num_bins) + 1;
        // save spectrum files if requested
        std::stringstream ss;
        ss << path << "target_info/" << i << ".";
        double time = i * (params->fft_size - params->fft_overlap);
        time = double(time)/target_sampling_frequency;
        for (unsigned j = 0; j < fft_num_bins - 1; j++) {
            time_axis[j] = time;
        }
        std::stringstream overlapstream;
        overlapstream << overlap;
        JUCEFileIO::save_text_file(ss.str() + "magnitude.txt", GPAudioUtil::double_buffers_to_graph_string(overlapstream.str(), "", "", false, fft_num_bins - 1, time_axis, freq_axis + 1, target_magnitude + data_offset));
        JUCEFileIO::save_text_file(ss.str() + "mac.txt", GPAudioUtil::double_buffers_to_graph_string("", "", "", false, fft_num_bins - 1, freq_axis + 1, target_magnitude_moving_average + data_offset, NULL));
        JUCEFileIO::save_text_file(ss.str() + "undershoot.txt", GPAudioUtil::double_buffers_to_graph_string("", "", "", false, fft_num_bins - 1, freq_axis + 1, target_magnitude + data_offset, bin_undershoot_p + data_offset));
        JUCEFileIO::save_text_file(ss.str() + "overshoot.txt", GPAudioUtil::double_buffers_to_graph_string("", "", "", false, fft_num_bins - 1, freq_axis + 1, target_magnitude + data_offset, bin_overshoot_p + data_offset));
    }
    free(time_axis);
	free(freq_axis);
}
