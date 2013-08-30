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
    extern double compareSpectraWeighted(bool dBComparison, unsigned fftSize, unsigned overlap, unsigned numFrames, unsigned fftOutputBufferSize, kiss_fftr_cfg fftConfig, const float* candidateFrames, kiss_fft_scalar* candidateAmplitudeBuffer, kiss_fft_cpx* candidateSpectraBuffer, double* candidateMagnitudeBuffer, double* candidatePhaseBuffer, const float* window, double* targetSpectrumMagnitudes, double* targetSpectrumPhases, double* binUndershootingPenalty, double* binOvershootingPenalty, double* fftFrameWeight, double penalizeBadPhase, double magnitudeWeight, double phaseWeight);
    struct GPAudioComparisonContainer {
        unsigned num_frames;
        unsigned fft_output_buffer_size;
        // TODO:
    };

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
}

#endif
