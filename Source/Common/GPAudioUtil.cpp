/*
  ==============================================================================

    GPAudioUtil.cpp
    Author:  cdonahue

  ==============================================================================
*/

#include "GPAudioUtil.h"

/*
	===================
	WINDOWING FUNCTIONS
	===================
*/

void GPAudioUtil::window(const char* type, unsigned n, float* windowBuffer) {
    if (strcmp(type, "hann") == 0) {
        // 0.5 * (1 - cos(2*pi*n)/(N-1))
        double insideCosineValue = 0.0;
        double increment = (2 * M_PI)/(n - 1);
        for (unsigned i = 0; i < n; i++) {
            windowBuffer[i] = 0.5 * (1 - cos(insideCosineValue));
            insideCosineValue += increment;
        }
    }
    else if (strcmp(type, "rect") == 0) {
        for (unsigned i = 0; i < n; i++) {
            windowBuffer[i] = 1.0;
        }
    }
}


void GPAudioUtil::applyWindow(unsigned n, kiss_fft_scalar* buffer, const float* window) {
    for (unsigned i = 0; i < n; i++) {
        buffer[i] *= window[i];
    }
}

void GPAudioUtil::applyEnvelope(unsigned n, float* buffer, const float* envelope) {
	for (unsigned i = 0; i < n; i++) {
		buffer[i] *= envelope[i];
	}
}

/*
	===
	FFT
	===
*/

unsigned GPAudioUtil::calculateFftBufferSize(unsigned numFrames, unsigned n, unsigned o) {
    unsigned numFftCalls = 0;
    unsigned shift = n - o;
    for (unsigned i = 0; i < numFrames;) {
        numFftCalls++;
        i += shift;
    }
    return numFftCalls * ((n/2) + 1);
}

void GPAudioUtil::FftReal(kiss_fftr_cfg cfg, unsigned numFrames, const float* input, unsigned n, unsigned overlap, const float* window, kiss_fft_scalar* in, kiss_fft_cpx* out, bool dB, double* magnitude, double* phase) {
    float dBref = DBREF;
    unsigned fftOutputSize = (n/2 + 1);
    unsigned shift = n - overlap;
    int64 numCompleted = 0;
    int64 numRemaining = numFrames;
    int64 numFftOutputUsed = 0;
    while (numRemaining > 0) {
        // fill the input buffer
        unsigned numToTransform = numRemaining > n ? n : numRemaining;
        for (size_t i = 0; i < numToTransform; i++) {
            in[i] = input[numCompleted + i];
        }
        numCompleted += shift;
        numRemaining -= shift;
        // 0 out rest of input buffer if we're out of frames
        for (size_t i = numToTransform; i < n; i++) {
            in[i] = 0;
        }

        // apply window
        applyWindow(n, in, window);

        // perform fft
        kiss_fftr(cfg, in, out + numFftOutputUsed);

        // analyze output
        //printf("FREQ\t\tREAL\tIMAG\tMAG\tPHASE\n");
        if (dB) {
            for (size_t bin = numFftOutputUsed; bin < numFftOutputUsed + fftOutputSize; bin++) {
                magnitude[bin] = 10 * log10(out[bin].r * out[bin].r + out[bin].i * out[bin].i) - dBref;
                if (out[bin].r == 0 && out[bin].i == 0) {
                    phase[bin] = 0;
                }
                else {
                    phase[bin] = atan(out[bin].i / out[bin].r);
                }
            }
        }
        else {
            for (size_t bin = numFftOutputUsed; bin < numFftOutputUsed + fftOutputSize; bin++) {
                magnitude[bin] = sqrt(out[bin].r * out[bin].r + out[bin].i * out[bin].i);
                if (out[bin].r == 0 && out[bin].i == 0) {
                    phase[bin] = 0;
                }
                else {
                    phase[bin] = atan(out[bin].i / out[bin].r);
                }
            //    printf("%.1lf\t\t%.2lf\t%.2lf\t%.2lf\t%.2lf\n", (44100.0 / n) * bin, out[bin].r, out[bin].i, magnitude[bin], phase[bin]);
                //std::cout << "BIN: " << bin << ", REAL: " << out[bin].r << ", IMAGINARY:" << out[bin].i << ", MAG: " << magnitude[bin] << ", PHASE: " << phase[bin] << std::endl;
            }
        }
        numFftOutputUsed += fftOutputSize;
    }
}

/*
	==========
	COMPARISON
	==========
*/

double GPAudioUtil::compareAmplitudes(unsigned numSamples, const float* samplesOne, const float* samplesTwo) {
    double sum = 0;
    for (unsigned frameNum = 0; frameNum < numSamples; frameNum++) {
        double error = fabs(samplesTwo[frameNum] - samplesOne[frameNum]);
        sum += error;
    }
    return sum;
}

double GPAudioUtil::compareAmplitudesWeighted(unsigned numSamples, const float* samplesOne, const float* samplesTwo, float weight) {
    double sum = 0;
    for (unsigned frameNum = 0; frameNum < numSamples; frameNum++) {
        double error = fabs(samplesTwo[frameNum] - samplesOne[frameNum]);
        error = pow(error, weight);
        sum += error;
    }
    return sum;
}

double GPAudioUtil::compareSpectraWeighted(bool dBComparison, unsigned fftSize, unsigned overlap, unsigned numFrames, unsigned fftOutputBufferSize, kiss_fftr_cfg fftConfig, const float* candidateFrames, kiss_fft_scalar* candidateAmplitudeBuffer, kiss_fft_cpx* candidateSpectraBuffer, double* candidateMagnitudeBuffer, double* candidatePhaseBuffer, const float* window, double* targetSpectrumMagnitudes, double* targetSpectrumPhases, double* binUndershootingPenalty, double* binOvershootingPenalty, double* fftFrameWeight, double penalizeBadPhase, double magnitudeWeight, double phaseWeight) {
    FftReal(fftConfig, numFrames, candidateFrames, fftSize, overlap, window, candidateAmplitudeBuffer, candidateSpectraBuffer, dBComparison, candidateMagnitudeBuffer, candidatePhaseBuffer);

    double magnitudeError = 0;
    double phaseError = 0;
    unsigned numBins = (fftSize/2) + 1;
    unsigned numFftFrames = fftOutputBufferSize / numBins;
    for (unsigned i = 0; i < numFftFrames; i++) {
        double frameMagnitudeError = 0;
        double framePhaseError = 0;
        unsigned frameIndex = (i * numBins);
        for (unsigned j = 1; j < numBins; j++) {
            unsigned binIndex = frameIndex + j;
            if (candidateMagnitudeBuffer[binIndex] < targetSpectrumMagnitudes[binIndex]) {
                frameMagnitudeError += pow(targetSpectrumMagnitudes[binIndex] - candidateMagnitudeBuffer[binIndex], binUndershootingPenalty[binIndex]);
            }
            else {
                frameMagnitudeError += pow(candidateMagnitudeBuffer[binIndex] - targetSpectrumMagnitudes[binIndex], binOvershootingPenalty[binIndex]);
            }
            framePhaseError += pow(fabs(candidatePhaseBuffer[binIndex] - targetSpectrumPhases[binIndex]), penalizeBadPhase);
        }
        magnitudeError += fftFrameWeight[i] * frameMagnitudeError;
        //std::cerr << i << ": " << fftFrameWeight[i] << ", " << frameMagnitudeError << ", " << magnitudeError << std::endl;
        phaseError += framePhaseError;
    }
    return (magnitudeWeight * magnitudeError + phaseWeight * phaseError);
}
