#ifndef SQUAREFREQOSCNODE_H
#define SQUAREFREQOSCNODE_H

#include "../WaveTableFreqNode.h"

class SquareFreqOscNode: public WaveTableFreqNode {
public:
    SquareFreqOscNode(GPMutatableParam* partial, GPMutatableParam* phase, GPNode* freq);
    ~SquareFreqOscNode();

	// GPNode overrides
    SquareFreqOscNode* getCopy();
    void setRenderInfo(float sr, unsigned blockSize, unsigned maxFrameNumber, float maxTime);
	
	// WaveTable overrides
	void makeAddAllWaveTables(double sampleRate, unsigned overSamp, unsigned constantRatioLimit, double baseFreq, double topFreq);
    void defineHarmonics(int len, int numHarmonics, double* ar, double* ai);

    // optional overrides
	void updateMutatedParams();

private:
};

#endif
