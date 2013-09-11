#ifndef SAWOSCNODE_H
#define SAWOSCNODE_H

#include "../WaveTableNode.h"

class SawOscNode: public WaveTableNode {
public:
    SawOscNode(GPMutatableParam* vn, GPMutatableParam* partial, GPMutatableParam* phase);
    ~SawOscNode();

	// GPNode overrides
    SawOscNode* getCopy();
    void setRenderInfo(float sr, unsigned blockSize, unsigned maxFrameNumber, float maxTime);
	
	// WaveTable overrides
	void makeAddAllWaveTables(double sampleRate, unsigned overSamp, unsigned constantRatioLimit, double baseFreq, double topFreq);
    void defineHarmonics(int len, int numHarmonics, double* ar, double* ai);

    // optional overrides
	void updateMutatedParams();

private:
};

#endif
