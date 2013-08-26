#ifndef TRIANGLEOSCNODE_H
#define TRIANGLEOSCNODE_H

#include "../WaveTableNode.h"

class TriangleOscNode: public WaveTableNode {
public:
    TriangleOscNode(GPMutatableParam* vn, GPMutatableParam* partial, GPMutatableParam* phase);
    ~TriangleOscNode();

	// GPNode overrides
    TriangleOscNode* getCopy();
    void setRenderInfo(float sr, unsigned blockSize, unsigned maxFrameNumber, float maxTime);
	
	// WaveTable overrides
	void makeAddAllWaveTables(double sampleRate, unsigned overSamp, unsigned constantRatioLimit, double baseFreq, double topFreq);
    void defineHarmonics(int len, int numHarmonics, double* ar, double* ai);

    // optional overrides
	void updateMutatedParams();

private:
};

#endif
