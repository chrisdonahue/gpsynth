#include "JUCEFileIO.h"

/*
    =============
    wav interface
    =============
*/

void JUCEFileIO::get_wav_file_metadata(std::string path, unsigned long* num_frames, unsigned* bits_per_sample, double* length_seconds, double* sampling_frequency, double* nyquist_frequency) {
    File input(String(path));
    if (!(input.existsAsFile())) {
        std::cerr << "Invalid input file: " << path << std::endl;
    }
    assert(input.existsAsFile());
    FileInputStream* fis = input.createInputStream();
    ScopedPointer<WavAudioFormat> wavFormat(new WavAudioFormat());
    ScopedPointer<AudioFormatReader> afr(wavFormat->createReaderFor(fis, true));

    // get info on target from JUCE
    *num_frames = afr->lengthInSamples;
    *sampling_frequency = afr->sampleRate;
    *bits_per_sample = afr->bitsPerSample;

    // calculate more info on target
    *length_seconds = (*num_frames) / (*sampling_frequency);
    *nyquist_frequency = (*sampling_frequency) / 2;
}

void JUCEFileIO::load_wav_file(std::string path, unsigned chunk_size, unsigned num_frames, float* buffer) {
    File input(path);
    if (!(input.existsAsFile())) {
        std::cerr << "Invalid input file: " << path << std::endl;
    }
    assert(input.existsAsFile());
    FileInputStream* fis = input.createInputStream();
    ScopedPointer<WavAudioFormat> wavFormat(new WavAudioFormat());
    ScopedPointer<AudioFormatReader> afr(wavFormat->createReaderFor(fis, true));

    // get waveform of target
    AudioSampleBuffer asb(1, num_frames);
    afr->read(&asb, 0, num_frames, 0, false, true);
    float* chanData = asb.getSampleData(0);
    memcpy(buffer, chanData, sizeof(float) * num_frames);
}

void JUCEFileIO::save_wav_file(std::string path, std::string desc, std::string origin, double sample_rate, unsigned chunk_size, unsigned num_frames, float* data) {
    File output(path);
    if (output.existsAsFile()) {
        output.deleteFile();
    }
    output.create();
    FileOutputStream* fos = output.createOutputStream();

    StringPairArray metaData(true);
    metaData = WavAudioFormat::createBWAVMetadata(desc, ProjectInfo::versionString, "", Time::getCurrentTime(), int64(sample_rate), "JUCE");

    ScopedPointer<WavAudioFormat> wavFormat(new WavAudioFormat());
    ScopedPointer<AudioFormatWriter> afw(wavFormat->createWriterFor(fos, sample_rate, 1, 32, metaData, 0));

    int64 numRemaining = num_frames;
    int64 numCompleted = 0;
    AudioSampleBuffer asb(1, chunk_size);
    float* chanData = asb.getSampleData(0);
    while (numRemaining > 0) {
        int numToWrite = numRemaining > chunk_size ? chunk_size : numRemaining;
        for (int samp = 0; samp < numToWrite; samp++, numCompleted++) {
            chanData[samp] = data[numCompleted];
        }
        afw->writeFromAudioSampleBuffer(asb, 0, numToWrite);
        numRemaining -= numToWrite;
    }
    assert (numCompleted == num_frames && numRemaining == 0);
}

/*
    ==============
    text interface
    ==============
*/

void JUCEFileIO::save_text_file(std::string path, std::string text) {
    File output(path);
    if (output.existsAsFile()) {
        output.deleteFile();
    }
    output.create();
    output.replaceWithText(text);
}

void JUCEFileIO::append_to_text_file(std::string path, std::string text) {
    File output(path);
    if (!output.existsAsFile()) {
		output.create();
    }
    output.appendText(text);
}

std::string JUCEFileIO::read_text_from_file(std::string path) {
    File input(path);
    if (!input.existsAsFile()) {
		return "";
    }
	return input.loadFileAsString().toStdString();
}
