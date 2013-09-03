#ifndef JUCEFILEIO_H
#define JUCEFILEIO_H

#include "../../JuceLibraryCode/JuceHeader.h"
#include <assert.h>

// JUCE FILE IO
namespace JUCEFileIO {
    // wav interface
    extern void get_wav_file_metadata(std::string path, unsigned long* num_frames, unsigned* bits_per_sample, double* length_seconds, double* sampling_frequency, double* nyquist_frequency);
    extern void load_wav_file(std::string path, unsigned chunk_size, unsigned num_frames, float* buffer);
    extern void save_wav_file(std::string path, std::string desc, std::string origin, double sample_rate, unsigned chunk_size, unsigned num_frames, float* data);
    
    // text interface
    extern void save_text_file(std::string path, std::string text);
    extern void append_to_text_file(std::string path, std::string text);
    extern std::string read_text_from_file(std::string path);
}

#endif
