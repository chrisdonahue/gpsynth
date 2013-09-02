#include "GPAudioComparator.h"

/*
    constructors
*/

GPAudioComparator::GPAudioComparator(GPAudioComparatorParams* params, std::string target_file_path)
{
    // fetch metadata
    GPAudioUtil::get_wav_file_metadata(target_file_path, &target_num_frames, &target_bits_per_sample, &target_length_seconds, &target_sampling_frequency, &target_nyquist_frequency);
    target_last_sample_time = (float) (float(target_num_frames - 1)/target_sampling_frequency);

    // fetch wav file frames
    target_frames = (float*) malloc(sizeof(float) * target_num_frames);
    GPAudioUtil::load_wav_file(target_file_path, params->wav_file_buffer_size, target_num_frames, target_frames);

    // set up FFT info
    unsigned n = params->fft_size;
    assert((n > 0) && ((n & (n - 1)) == 0));
    unsigned overlap = params->fft_overlap;
    assert(overlap < n);
    fft_output_buffer_size = GPAudioUtil::calculateFftBufferSize(target_num_frames, n, overlap);
    unsigned num_bins = (n/2) + 1;
    unsigned num_fft_frames = fft_output_buffer_size / num_bins;

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

	// allocate buffers for moving average calculation
    double* time_axis = (double*) malloc(sizeof(double) * (numBins - 1));
    double* moving_average = (double*) malloc(sizeof(double) * (numBins - 1));
    float* float_freq_axis = (float*) malloc(sizeof(float) * numBins);
    GPAudioUtil::fill_frequency_domain_buffer(n, target_sample_rate, float_freq_axis);
    double* freq_axis = (double*) malloc(sizeof(double) * (numBins));
    for (unsigned i = 0; i < numBins; i++) {
        freq_axis[i] = (double) float_freq_axis[i];
    }
    free(float_freq_axis);

    // calculate penalties for each frame
    double base = params->ff_mag_base_comparison;
    double good = params->ff_mag_good_comparison;
    double bad = params->ff_mag_bad_comparison;
    double frame_average_sum = 0.0;
    for (unsigned i = 0; i < num_fft_frames; i++) {
        // find moving average
        unsigned data_offset = (i * num_bins) + 1;
        double frame_average, max_deviation_below, max_deviation_above;
        GPAudioUtil::find_moving_average(params->ff_moving_average_type, numBins - 1, target_magnitude + dataOffset, moving_average, &frame_average, &max_deviation_below, &max_deviation_above, params->ff_moving_average_past_radius, params->ff_moving_average_future_radius, params->ff_moving_average_exponential_alpha);
        //fftFrameWeight[i] = frameAverage;
        frame_average_sum += frame_average;

        // compare each bin EXCEPT DC OFFSET to the moving average magnitude
        for (unsigned j = 1; j < num_bins; j++) {
            unsigned bin_index = (i * num_bins) + j;
            double bin_magnitude = target_magnitude[bin_index];
            double bin_average = moving_average[j - 1];
            
            // calculate bin comparison metric data
            double numerator, denominator, proportion;
            numerator = fabs(bin_magnitude - bin_average);
            if (binMagnitude > binAverage) {
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

        // save spectrum files if requested
        if (params->log_save_target_spectrum) {
            std::stringstream ss;
            ss << output_dir_path << "target_info/" << i << ".";
            double time = i * (n - overlap);
            time = double(time)/targetSampleRate;
            for (unsigned j = 0; j < numBins - 1; j++) {
                timeAxis[j] = time;
            }
            saveTextFile(String(ss.str() + "magnitude.txt"), doubleBuffersToGraphText(String(overlap), String(""), String(""), String(""), false, numBins - 1, timeAxis, freqAxis + 1, targetMagnitude + dataOffset));
            saveTextFile(String(ss.str() + "mac.txt"), doubleBuffersToGraphText(String(""), String(""), String(""), String(""), false, numBins - 1, freqAxis + 1, mac, NULL));
            saveTextFile(String(ss.str() + "undershoot.txt"), doubleBuffersToGraphText(String(""), String(""), String(""), String(""), false, numBins - 1, freqAxis + 1, targetMagnitude + dataOffset, binUndershootingPenalty + dataOffset));
            saveTextFile(String(ss.str() + "overshoot.txt"), doubleBuffersToGraphText(String(""), String(""), String(""), String(""), false, numBins - 1, freqAxis + 1, targetMagnitude + dataOffset, binOvershootingPenalty + dataOffset));
        }
    }

	// free moving average calculation buffers
	free(freq_axis);
	free(moving_average);
	free(time_axis);
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

float GPAudioComparator::get_target_last_sample_start_time() {
    return target_last_sample_start_time;
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
