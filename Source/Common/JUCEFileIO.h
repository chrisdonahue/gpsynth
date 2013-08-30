#ifndef JUCEFILEIO_H
#define JUCEFILEIO_H

#include "../../JuceLibraryCode/JuceHeader.h"
#include <assert.h>

// JUCE FILE IO
namespace JUCEFileIO {
    extern void get_wave_file_metadata(std::string path, unsigned long* num_frames, unsigned* bits_per_sample, double* length_seconds, double* sampling_frequency, double* nyquist_frequency);

    extern void loadWavFile(String path, unsigned chunkSize, unsigned numFrames, float* buffer);
    extern void saveWavFile(String path, String desc, String origin, float sampleRate, unsigned chunkSize, unsigned numFrames, float* data);
    extern void saveTextFile(String path, String text);
    extern void appendToTextFile(String path, String text);
    extern String readTextFromFile(String path);
}

#endif
