#ifndef GPAUDIOUTIL_H
#define GPAUDIOUTIL_H

#define DBREF -54.0

#include <sstream>

#include "JuceHeader.h"
#include "../Dependencies/kissfft/kiss_fft.h"
#include "../Dependencies/kissfft/kiss_fftr.h"

namespace GPAudioUtil {
	// fft processing
	extern void calculate_fft_buffer_size(unsigned num_frames, unsigned n, unsigned o, unsigned* fft_num_bins, unsigned* fft_num_frames, unsigned* fft_output_buffer_length);
	extern void window(const char* type, unsigned n, float* window_buffer);
	extern void apply_window(unsigned n, kiss_fft_scalar* buffer, const float* window);
    extern void fft_real(kiss_fftr_cfg cfg, unsigned num_frames, const float* input, unsigned n, unsigned overlap, const float* window, kiss_fft_scalar* in_buffer, kiss_fft_cpx* out_buffer, double* magnitude, double* phase);

	// waveform analysis
    extern void find_moving_average(unsigned type, unsigned n, const double* data, double* moving_average_buffer, double* data_average, double* max_deviation_below, double* max_deviation_above, unsigned past_radius, unsigned future_radius, double alpha);

	// domain enumeration
    extern void fill_time_domain_buffer(unsigned num_samples, double sr, float* buffer);
    extern void fill_frequency_domain_buffer(unsigned fft_size, double sr, float* buffer);

	// graph helpers
	extern std::string float_buffers_to_graph_string(std::string options, std::string x_label, std::string y_label, bool index_as_x_axis, unsigned n, const float* x, const float* y, const float* z);
	extern std::string double_buffers_to_graph_string(std::string options, std::string x_label, std::string y_label, bool index_as_x_axis, unsigned n, const double* x, const double* y, const double* z);

	// LEGACY
	extern void applyEnvelope(unsigned n, float* buffer, const float* envelope);
    void followEnvelope(unsigned n, float* buffer, float* envelope, double attack_in_ms, double release_in_ms, double samplerate);
    void findEnvelope(bool ignoreZeroes, unsigned n, float* wav, float* env);
	extern double compareAmplitudes(unsigned numSamples, const float* samplesOne, const float* samplesTwo);
    extern double compareAmplitudesWeighted(unsigned numSamples, const float* samplesOne, const float* samplesTwo, float weight);
    extern void FftReal(kiss_fftr_cfg cfg, unsigned numFrames, const float* input, unsigned n, unsigned overlap, const float* window, kiss_fft_scalar* in, kiss_fft_cpx* out, bool dB, double* magnitude, double* phase);
}

#endif
