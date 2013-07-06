/*
  ==============================================================================

    SquareOscNode.h
    Author:  cdonahue

  ==============================================================================
*/

#ifndef SQUAREOSCNODE_H
#define SQUAREOSCNODE_H

#include "../WaveTableNode.h"

class SquareOscNode: public WaveTableNode {
public:
    SquareOscNode(GPMutatableParam* vn, GPMutatableParam* partial, GPMutatableParam* phase);
    ~SquareOscNode();

	// GPNode overrides
    SquareOscNode* getCopy();
    void setRenderInfo(float sr, unsigned blockSize, unsigned maxFrameNumber, float maxTime);
	
	// WaveTable overrides
	void makeAddAllWaveTables(double sampleRate, unsigned overSamp, unsigned constantRatioLimit, double baseFreq, double topFreq);
    void defineHarmonics(int len, int numHarmonics, double* ar, double* ai);

    // optional overrides
	void updateMutatedParams();

private:
};

#endif
