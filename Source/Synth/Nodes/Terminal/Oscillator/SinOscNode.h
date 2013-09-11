#ifndef SINOSCNODE_H
#define SINOSCNODE_H

#include "../WaveTableNode.h"

class SinOscNode: public WaveTableNode {
public:
    SinOscNode(GPMutatableParam* vn, GPMutatableParam* partial, GPMutatableParam* phase);
    ~SinOscNode();

	// GPNode overrides
    SinOscNode* getCopy();
    void setRenderInfo(float sr, unsigned blockSize, unsigned maxFrameNumber, float maxTime);
	
	// WaveTable overrides
	void makeAddAllWaveTables(double sampleRate, unsigned overSamp, unsigned constantRatioLimit, double baseFreq, double topFreq);
    void defineHarmonics(int len, int numHarmonics, double* ar, double* ai);

    // optional overrides
	void updateMutatedParams();

private:
};

#endif
