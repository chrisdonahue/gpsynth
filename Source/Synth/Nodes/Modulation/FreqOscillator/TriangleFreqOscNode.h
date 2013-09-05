#ifndef TRIANGLEFREQOSCNODE_H
#define TRIANGLEFREQOSCNODE_H

#include "../WaveTableFreqNode.h"

class TriangleFreqOscNode: public WaveTableFreqNode {
public:
    TriangleFreqOscNode(GPMutatableParam* partial, GPMutatableParam* phase, GPNode* freq);
    ~TriangleFreqOscNode();

	// GPNode overrides
    TriangleFreqOscNode* getCopy();
    void setRenderInfo(float sr, unsigned blockSize, unsigned maxFrameNumber, float maxTime);
	
	// WaveTable overrides
	void makeAddAllWaveTables(double sampleRate, unsigned overSamp, unsigned constantRatioLimit, double baseFreq, double topFreq);
    void defineHarmonics(int len, int numHarmonics, double* ar, double* ai);

    // optional overrides
	void updateMutatedParams();

private:
};

#endif
