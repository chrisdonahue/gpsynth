#ifndef WAVETABLENODE_H
#define WAVETABLENODE_H

#include "../../GPNode.h"
#include "../../../Dependencies/WaveTableOsc/WaveTableOsc.h"

class WaveTableNode: public GPNode {
public:
    // overrides
    void setRenderInfo(float sr, unsigned blockSize, unsigned maxFrameNumber, float maxTime);
    void doneRendering();
    void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
	void updateMutatedParams();

	// WaveTable helpers
	void fft(int N, double* ar, double* ai);
	float makeAddWaveTable(int len, double* ar, double* ai, double scale, double topFreq);

	// GPNode subclass overrides
    virtual WaveTableNode* getCopy() = 0;

	// WaveTable subclass overrides
	virtual void makeAddAllWaveTables(double sampleRate, unsigned overSamp, unsigned constantRatioLimit, double baseFreq, double topFreq) = 0;
    virtual void defineHarmonics(int len, int numHarmonics, double* ar, double* ai) = 0;

    // member variables
    int variableNum;
    float sampleRate;
    float partial;
    float phase;
    WaveTableOsc* osc;
private:
};

#endif
