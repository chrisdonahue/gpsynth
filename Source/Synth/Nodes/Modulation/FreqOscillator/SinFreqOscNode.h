#ifndef SINFREQOSCNODE_H
#define SINFREQOSCNODE_H

#include "../WaveTableFreqNode.h"

class SinFreqOscNode: public WaveTableFreqNode {
public:
    SinFreqOscNode(GPMutatableParam* phase, GPNode* freq);
    ~SinFreqOscNode();

	// GPNode overrides
    SinFreqOscNode* getCopy();
    void setRenderInfo(float sr, unsigned blockSize, unsigned maxFrameNumber, float maxTime);
	
	// WaveTable overrides
	void makeAddAllWaveTables(double sampleRate, unsigned overSamp, unsigned constantRatioLimit, double baseFreq, double topFreq);
    void defineHarmonics(int len, int numHarmonics, double* ar, double* ai);

private:
};

#endif
