/*
  ==============================================================================

    JUCEFileIO.cpp
    Created: 6 Feb 2013 7:19:11pm
    Author:  cdonahue

  ==============================================================================
*/

#include "JUCEFileIO.h"

/*
    ============
    JUCE FILE IO
    ============
*/

void getWavFileInfo(String path, unsigned* numFrames, double* sampleRate) {
    File input(path);
    if (!(input.existsAsFile())) {
        std::cerr << "Invalid input file: " << path << std::endl;
    }
    assert(input.existsAsFile());
    FileInputStream* fis = input.createInputStream();
    ScopedPointer<WavAudioFormat> wavFormat(new WavAudioFormat());
    ScopedPointer<AudioFormatReader> afr(wavFormat->createReaderFor(fis, true));

    // get info on target
    *numFrames = afr->lengthInSamples;
    *sampleRate = afr->sampleRate;
}

// TODO: get chunkSize working again
void loadWavFile(String path, unsigned chunkSize, unsigned numFrames, float* buffer) {
    File input(path);
    if (!(input.existsAsFile())) {
        std::cerr << "Invalid input file: " << path << std::endl;
    }
    assert(input.existsAsFile());
    FileInputStream* fis = input.createInputStream();
    ScopedPointer<WavAudioFormat> wavFormat(new WavAudioFormat());
    ScopedPointer<AudioFormatReader> afr(wavFormat->createReaderFor(fis, true));

    // get waveform of target
    AudioSampleBuffer asb(1, numFrames);
    afr->read(&asb, 0, numFrames, 0, false, true);
    float* chanData = asb.getSampleData(0);
    memcpy(buffer, chanData, sizeof(float) * numFrames);
}

void saveWavFile(String path, String desc, String origin, double sampleRate, unsigned chunkSize, unsigned numFrames, float* data) {
    File output(path);
    if (output.existsAsFile()) {
        output.deleteFile();
    }
    output.create();
    FileOutputStream* fos = output.createOutputStream();

    StringPairArray metaData(true);
    metaData = WavAudioFormat::createBWAVMetadata(desc, ProjectInfo::versionString, "", Time::getCurrentTime(), int64(sampleRate), "JUCE");

    ScopedPointer<WavAudioFormat> wavFormat(new WavAudioFormat());
    ScopedPointer<AudioFormatWriter> afw(wavFormat->createWriterFor(fos, sampleRate, 1, 32, metaData, 0));

    int64 numRemaining = numFrames;
    int64 numCompleted = 0;
    AudioSampleBuffer asb(1, chunkSize);
    float* chanData = asb.getSampleData(0);
    while (numRemaining > 0) {
        int numToWrite = numRemaining > chunkSize ? chunkSize : numRemaining;
        for (int samp = 0; samp < numToWrite; samp++, numCompleted++) {
            chanData[samp] = data[numCompleted];
        }
        afw->writeFromAudioSampleBuffer(asb, 0, numToWrite);
        numRemaining -= numToWrite;
    }
    assert (numCompleted == numFrames && numRemaining == 0);
}

void saveTextFile(String path, String text) {
    File output(path);
    if (output.existsAsFile()) {
        output.deleteFile();
    }
    output.create();
    output.replaceWithText(text);
}
