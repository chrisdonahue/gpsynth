/*
  ==============================================================================

    SinOsc.h
    Author:  cdonahue

  ==============================================================================
*/

#ifndef SINOSCNODE_H
#define SINOSCNODE_H

#include "../../../GPNode.h"

class SinOsc: public GPNode {
public:
    SinOsc(GPMutatableParam* vn, GPMutatableParam* partial, GPMutatableParam* phase);
    ~SinOsc();

	// GPNode overrides
    SinOsc* getCopy();
    void setRenderInfo(float sr, unsigned blockSize, unsigned maxFrameNumber, float maxTime);
	
	// WaveTable overrides
	void makeAddAllWaveTables(double sampleRate, unsigned overSamp, unsigned constantRatioLimit, double baseFreq, double topFreq);
    void defineHarmonics(int len, int numHarmonics, double* ar, double* ai);

    // optional overrides
	void updateMutatedParams();

private:
};

#endif
