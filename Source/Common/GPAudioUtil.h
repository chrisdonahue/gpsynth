/*
  ==============================================================================

    GPAudioUtil.h
    Author:  cdonahue

  ==============================================================================
*/

#ifndef GPAUDIOUTIL_H
#define GPAUDIOUTIL_H

#define DBREF -54.0

#include "JuceHeader.h"
#include "../Dependencies/kissfft/kiss_fft.h"
#include "../Dependencies/kissfft/kiss_fftr.h"

namespace GPAudioUtil {
	// WINDOWING FUNCTIONS
	extern void window(const char* type, unsigned n, float* windowBuffer);
	extern void applyWindow(unsigned n, kiss_fft_scalar* buffer, const float* window);
	extern void applyEnvelope(unsigned n, float* buffer, const float* envelope);

	// FFT
	extern unsigned calculateFftBufferSize(unsigned numFrames, unsigned n, unsigned o);
    extern void FftReal(kiss_fftr_cfg cfg, unsigned numFrames, const float* input, unsigned n, unsigned overlap, const float* window, kiss_fft_scalar* in, kiss_fft_cpx* out, bool dB, double* magnitude, double* phase);

	// COMPARISON
	extern double compareAmplitudes(unsigned numSamples, const float* samplesOne, const float* samplesTwo);
    extern double compareAmplitudesWeighted(unsigned numSamples, const float* samplesOne, const float* samplesTwo, float weight);
    extern double compareSpectraWeighted(bool dBComparison, unsigned fftSize, unsigned overlap, unsigned numFrames, unsigned fftOutputBufferSize, kiss_fftr_cfg fftConfig, const float* candidateFrames, kiss_fft_scalar* candidateAmplitudeBuffer, kiss_fft_cpx* candidateSpectraBuffer, double* candidateMagnitudeBuffer, double* candidatePhaseBuffer, const float* window, double* targetSpectrumMagnitudes, double* targetSpectrumPhases, double* binUndershootingPenalty, double* binOvershootingPenalty, double* fftFrameWeight, double penalizeBadPhase, double magnitudeWeight, double phaseWeight);

	// waveform analysis
    void find_moving_average(unsigned type, unsigned n, const double* data, double* moving_average_buffer, double* data_average, double* max_deviation_below, double* max_deviation_above, unsigned past_radius, unsigned future_radius, double alpha);
    void followEnvelope(unsigned n, float* buffer, float* envelope, double attack_in_ms, double release_in_ms, double samplerate);
    void findEnvelope(bool ignoreZeroes, unsigned n, float* wav, float* env);

	// domain enumeration
    void fill_time_domain_buffer(unsigned numSamples, double sr, float* buffer);
    void fill_frequency_domain_buffer(unsigned fftSize, double sr, float* buffer);

	// graph helpers
	std::string float_buffers_to_graph_string(std::string options, std::string x_label, std::string y_label, bool index_as_x_axis, unsigned n, const float* x, const float* y, const float* z);
	std::string double_buffers_to_graph_string(std::string options, std::string x_label, std::string y_label, bool index_as_x_axis, unsigned n, const double* x, const double* y, const double* z);
}

#endif
