#ifndef SAWFREQOSCNODE_H
#define SAWFREQOSCNODE_H

#include "../WaveTableFreqNode.h"

class SawFreqOscNode: public WaveTableFreqNode {
public:
    SawFreqOscNode(GPMutatableParam* phase, GPNode* freq);
    ~SawFreqOscNode();

	// GPNode overrides
    SawFreqOscNode* getCopy();
    void setRenderInfo(float sr, unsigned blockSize, unsigned maxFrameNumber, float maxTime);
	
	// WaveTable overrides
	void makeAddAllWaveTables(double sampleRate, unsigned overSamp, unsigned constantRatioLimit, double baseFreq, double topFreq);
    void defineHarmonics(int len, int numHarmonics, double* ar, double* ai);

private:
};

#endif
