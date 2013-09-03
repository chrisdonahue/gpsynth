#include "GPAudioUtil.h"

/*
	==============
	fft processing
	==============
*/

void GPAudioUtil::calculate_fft_buffer_size(unsigned num_frames, unsigned n, unsigned o, unsigned* fft_num_bins, unsigned* fft_num_frames, unsigned* fft_output_buffer_length) {
    unsigned num_fft_calls = 0;
    unsigned shift = n - o;
    for (unsigned i = 0; i < num_frames;) {
        num_fft_calls++;
        i += shift;
    }
    *fft_num_bins = ((n/2) + 1);
    *fft_num_frames = num_fft_calls;
    *fft_output_buffer_length = num_fft_calls * (*fft_num_bins);
}

void GPAudioUtil::window(const char* type, unsigned n, float* window_buffer) {
    if (strcmp(type, "hann") == 0) {
        // 0.5 * (1 - cos(2*pi*n)/(N-1))
        double insideCosineValue = 0.0;
        double increment = (2 * M_PI)/(n - 1);
        for (unsigned i = 0; i < n; i++) {
            window_buffer[i] = 0.5 * (1 - cos(insideCosineValue));
            insideCosineValue += increment;
        }
    }
    else if (strcmp(type, "rect") == 0) {
        for (unsigned i = 0; i < n; i++) {
            window_buffer[i] = 1.0;
        }
    }
}

void GPAudioUtil::apply_window(unsigned n, kiss_fft_scalar* buffer, const float* window) {
    for (unsigned i = 0; i < n; i++) {
        buffer[i] *= window[i];
    }
}

void GPAudioUtil::fft_real(kiss_fftr_cfg cfg, unsigned num_frames, const float* input, unsigned n, unsigned overlap, const float* window, kiss_fft_scalar* in_buffer, kiss_fft_cpx* out_buffer, double* magnitude, double* phase) {
    unsigned fft_output_size = (n/2 + 1);
    unsigned shift = n - overlap;
    int64 num_completed = 0;
    int64 num_remaining = num_frames;
    int64 num_fft_output_used = 0;
    while (num_remaining > 0) {
        // fill the input buffer
        unsigned num_to_transform = num_remaining > n ? n : num_remaining;
        for (size_t i = 0; i < num_to_transform; i++) {
            in_buffer[i] = input[num_completed + i];
        }
        num_completed += shift;
        num_remaining -= shift;
        // 0 out rest of input buffer if we're out of frames
        for (size_t i = num_to_transform; i < n; i++) {
            in_buffer[i] = 0;
        }

        // apply window
        GPAudioUtil::apply_window(n, in_buffer, window);

        // perform fft
        kiss_fftr(cfg, in_buffer, out_buffer + num_fft_output_used);

        // analyze output
        //printf("FREQ\t\tREAL\tIMAG\tMAG\tPHASE\n");
        for (size_t bin = num_fft_output_used; bin < num_fft_output_used + fft_output_size; bin++) {
            magnitude[bin] = sqrt(out_buffer[bin].r * out_buffer[bin].r + out_buffer[bin].i * out_buffer[bin].i);
            if (out_buffer[bin].r == 0 && out_buffer[bin].i == 0) {
                phase[bin] = 0;
            }
            else {
                phase[bin] = atan(out_buffer[bin].i / out_buffer[bin].r);
            }
            //printf("%.1lf\t\t%.2lf\t%.2lf\t%.2lf\t%.2lf\n", (44100.0 / n) * bin, out_buffer[bin].r, out_buffer[bin].i, magnitude[bin], phase[bin]);
            //std::cout << "BIN: " << bin << ", REAL: " << out_buffer[bin].r << ", IMAGINARY:" << out_buffer[bin].i << ", MAG: " << magnitude[bin] << ", PHASE: " << phase[bin] << std::endl;
        }
        num_fft_output_used += fft_output_size;
    }
}

/*
	=================
	waveform analysis
	=================
*/

void GPAudioUtil::find_moving_average(unsigned type, unsigned n, const double* data, double* moving_average_buffer, double* data_average, double* max_deviation_below, double* max_deviation_above, unsigned past_radius, unsigned future_radius, double alpha) {
    // NON-MOVING AVERAGE
    if (type == 0) {
        double sum = 0;
        double max = std::numeric_limits<double>::min();
        double min = std::numeric_limits<double>::max();
        // EXCLUDE DC OFFSET
        for (unsigned i = 0; i < n; i++) {
            double magnitude = data[i];
            sum += magnitude;
            if (magnitude > max)
                max = magnitude;
            if (magnitude < min)
                min = magnitude;
        }
        double average = sum / ((double) n);
        for (unsigned i = 0; i < n; i++) {
            moving_average_buffer[i] = average;
        }
        *max_deviation_above = max - average;
        *max_deviation_below = average - min;
        //*frameaverage = average;
        return;
    }
    
    // CREATE TEMPORARY BUFFER FOR WEIGHTS
    unsigned weightArraySize = (past_radius + future_radius) + 1;
    float* weights = (float*) malloc(sizeof(float) * weightArraySize);

    // ASSIGN WEIGHTS BY TYPE
    // CONSTANT WEIGHT
    if (type == 1) {
        for (unsigned i = 0; i < weightArraySize; i++) {
            weights[i] = 1.0;
        }
    }
    // EXPONENTIAL WEIGHT
    else if (type == 2) {
        for (unsigned i = 0; i < past_radius; i++) {
            unsigned numAlpha = past_radius - i + 1;
            weights[i] = pow(alpha, numAlpha);
        }
        weights[past_radius] = alpha;
        for (unsigned i = past_radius + 1; i < weightArraySize; i++) {
            unsigned numAlpha = i - past_radius + 1;
            weights[i] = pow(alpha, numAlpha);
        }
    }
    
    // PRINT WEIGHT ARRAY
    /*
    for (unsigned i = 0; i < weightArraySize; i++) {
        std::cerr << weights[i] << std::endl;
    }
    */

    // CALCULATE MOVING AVERAGE BASED ON WEIGHTS
    int leftrad = past_radius;
    int rightrad = future_radius;
    for (int i = 0; i < (int) n; i++) {
        int lowerIndex = i - leftrad < 0 ? 0 : i - leftrad;
        int upperIndex = i + rightrad + 1 > n ? n : i + rightrad + 1;
        int weightIndex = i - leftrad < 0 ? leftrad - i: 0;
        double sum = 0.0;
        double weightsum = 0.0;
        //std::cerr << i << ": (" << lowerIndex << ", " << upperIndex << ", " << weightIndex << ")" << std::endl;
        for (int j = lowerIndex, k = weightIndex; j < upperIndex; j++, k++) {
            sum += data[j] * weights[k];
            weightsum += weights[k];
        }
        moving_average_buffer[i] = (sum / weightsum);
    }
    free(weights);

    // CALCULATE MIN/MAX DEVIATION INEFFICIENTLY
    double maxdeva = std::numeric_limits<double>::min();
    double maxdevb = std::numeric_limits<double>::min();
    double maxrata = std::numeric_limits<double>::min();
    double maxratb = std::numeric_limits<double>::min();
    double sum = 0.0;
    for (unsigned i = 0; i < n; i++) {
        double pointValue = data[i];
        sum += pointValue;
        double pointAverage = moving_average_buffer[i];
        // if value is below average
        if (pointValue < pointAverage) {
            if (pointAverage - pointValue > maxdevb)
                maxdevb = pointAverage - pointValue;

            // absolute value over moving average
            double absValue = (pointAverage - pointValue) + pointAverage;
            if (pointAverage/absValue > maxratb)
                maxratb = pointAverage/absValue;
        }
        else {
            if (pointValue - pointAverage > maxdeva)
                maxdeva = pointValue - pointAverage;
            if (pointAverage/pointValue > maxrata)
                maxrata = pointAverage/pointValue;
        }
    }
    *max_deviation_above = maxdeva;
    *max_deviation_below = maxdevb;
}



/*
	==================
	domain enumeration
	==================
*/

void GPAudioUtil::fill_time_domain_buffer(unsigned num_samples, double sr, float* buffer) {
    for (unsigned frame = 0; frame < num_samples; frame++) {
        buffer[frame] = float(frame)/sr;
    }
}

void GPAudioUtil::fill_frequency_domain_buffer(unsigned fft_size, double sr, float* buffer) {
    for (unsigned i = 0; i < (fft_size/2) + 1; i++) {
        buffer[i] = (sr / fft_size) * i;
    }
}

/*
	=============
	graph helpers
	=============
*/

std::string GPAudioUtil::float_buffers_to_graph_string(std::string options, std::string x_label, std::string y_label, bool index_as_x_axis, unsigned n, const float* x, const float* y, const float* z) {
	std::stringstream ss;
	ss << options;
	ss << std::endl;
	ss << x_label;
	ss << std::endl;
	ss << y_label;
	ss << std::endl;
	for (unsigned i = 0; i < n; i++) {
		if (index_as_x_axis) {
			ss << i;
		}
		else {
			ss << x[i];
		}
		if (y != NULL) {
			ss << "\t";
			ss << y[i];
		}
		if (z != NULL) {
			ss << "\t";
			ss << z[i];
		}
		ss << std::endl;
	}
	return ss.str();
}

std::string GPAudioUtil::double_buffers_to_graph_string(std::string options, std::string x_label, std::string y_label, bool index_as_x_axis, unsigned n, const double* x, const double* y, const double* z) {
	std::stringstream ss;
	ss << options;
	ss << std::endl;
	ss << x_label;
	ss << std::endl;
	ss << y_label;
	ss << std::endl;
	for (unsigned i = 0; i < n; i++) {
		if (index_as_x_axis) {
			ss << i;
		}
		else {
			ss << x[i];
		}
		if (y != NULL) {
			ss << "\t";
			ss << y[i];
		}
		if (z != NULL) {
			ss << "\t";
			ss << z[i];
		}
		ss << std::endl;
	}
	return ss.str();
}

/*
	======
	LEGACY
	======
*/

void GPAudioUtil::applyEnvelope(unsigned n, float* buffer, const float* envelope) {
	for (unsigned i = 0; i < n; i++) {
		buffer[i] *= envelope[i];
	}
}

// FROM: http://musicdsp.org/showArchiveComment.php?ArchiveID=136 
void GPAudioUtil::followEnvelope(unsigned n, float* buffer, float* envelope, double attack_in_ms, double release_in_ms, double samplerate) {
    double attack_coef = exp(log(0.01)/( attack_in_ms * samplerate * 0.001));
    double release_coef = exp(log(0.01)/( release_in_ms * samplerate * 0.001));
    
    double currentValue;
    envelope[0] = buffer[0];
    double currentEnvelope = envelope[0];
    for (unsigned i = 1; i < n; i++) {
        currentValue = fabs(buffer[i]);
        if (currentValue > currentEnvelope) {
            currentEnvelope = attack_coef * (currentEnvelope - currentValue) + currentValue;
        }
        else {
            currentEnvelope = release_coef * (currentEnvelope - currentValue) + currentValue;
        }
        envelope[i] = currentEnvelope;
    }
}

void GPAudioUtil::findEnvelope(bool ignoreZeroes, unsigned n, float* wav, float* env) {
    // MAKE AMPLITUDE ENVELOPE OF TARGET
    // x/y pairs for absolute waveform bound
    std::vector<unsigned> x;
    x.resize(0, 0);
    std::vector<float> y;
    y.resize(0, 0);

    // set initial value
    x.push_back(0);
    y.push_back(fabs(wav[0]));

    // find waveform minima/maxima
    float prevSlope = (wav[1] - wav[0]);
    float currSlope = 0;
    float slopeProduct = 0;
    for (unsigned i = 1; i < n - 2; i++) {
        currSlope = (wav[i + 1] - wav[i]);

        // if one slope is 0 we're at one edge of a plateau or silence
        float slopeProduct = currSlope * prevSlope;

        if (!ignoreZeroes) {
            if (slopeProduct == 0) {
                x.push_back(i);
                y.push_back(fabs(wav[i]));
            }
            // else if slope has changed we found a minimum or maximum
            else if (slopeProduct < 0 && prevSlope > 0) {
                x.push_back(i);
                y.push_back(fabs(wav[i]));
            }
        }
        else {
            //std::cout << i << ", " << n << std::endl;
            //std::cout << slopeProduct << std::endl;
            //std::cout << prevSlope << std::endl;
            if (slopeProduct < 0 && prevSlope > 0) {
                x.push_back(i);
                y.push_back(fabs(wav[i]));
            }
        }

        prevSlope = currSlope;
    }

    // set final value
    x.push_back(n - 1);
    y.push_back(fabs(wav[n - 1]));

    // fill env buffer
    for (unsigned i = 0; i < x.size() - 1; i++) {
        // calculate slope between points
        unsigned currFrameNumber = x[i];
        float currEnvValue = y[i];
        unsigned nextFrameNumber = x[i+1];
        float nextEnvValue = y[i+1];
        float slope = (nextEnvValue - currEnvValue)/(nextFrameNumber - currFrameNumber);

        // fill buffer from slope
        unsigned assignEnvelopeSample = currFrameNumber;
        while (assignEnvelopeSample < nextFrameNumber) {
            env[assignEnvelopeSample] = ((assignEnvelopeSample - currFrameNumber) * slope) + currEnvValue;
            assignEnvelopeSample++;
        }
    }
    env[n - 1] = wav[n - 1];
}

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
        GPAudioUtil::apply_window(n, in, window);

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
