/*
  ==============================================================================

    SquareOscNode.cpp
    Author:  cdonahue

  ==============================================================================
*/

#include "SquareOscNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

SquareOscNode::SquareOscNode(GPMutatableParam* vn, GPMutatableParam* partial, GPMutatableParam* phase) {
    assert(!(vn->isMutatable) && !(vn->isContinuous));
    variableNum = vn->getDValue();

    mutatableParams.push_back(vn);
    mutatableParams.push_back(partial);
    mutatableParams.push_back(phase);

    arity = 0;
    
    minimum = -1;
    maximum = 1;

    symbol = "squareosc";
}

SquareOscNode::~SquareOscNode() {
}

/*
    ================
    GPNODE OVERRIDES
    ================
*/

SquareOscNode* SquareOscNode::getCopy() {
    return new SquareOscNode(mutatableParams[0]->getCopy(), mutatableParams[1]->getCopy(), mutatableParams[2]->getCopy());
}

void SquareOscNode::setRenderInfo(float sr, unsigned blockSize, unsigned maxFrameNumber, float maxTime) {
	WaveTableNode::setRenderInfo(sr, blockSize, maxFrameNumber, maxTime);
}

void SquareOscNode::updateMutatedParams() {
    GPNode::updateMutatedParams();

	// update angular frequency constant
    partial = mutatableParams[1]->getValue();
    phase = mutatableParams[2]->getValue();
	
    // minimum/maximum constant and declared in constructor
}

/*
    ===================
    WAVETABLE OVERRIDES
    ===================
*/

void SquareOscNode::makeAddAllWaveTables(double sampleRate, unsigned overSamp, unsigned constantRatioLimit, double baseFrequency, double topFrequency) {
    // calc number of harmonics where the highest harmonic baseFreq and lowest alias an octave higher would meet
    double baseFreq = baseFrequency;
    int maxHarms = sampleRate / (3.0 * baseFreq) + 0.5;

    // round up to nearest power of two
    unsigned int v = maxHarms;
    v--;            // so we don't go up if already a power of 2
    v |= v >> 1;    // roll the highest bit into all lower bits...
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;            // and increment to power of 2
    int tableLen = v * 2 * overSamp;  // double for the sample rate, then oversampling

    double ar[tableLen], ai[tableLen];   // for ifft

	double topFreq = topFrequency;
    //double topFreq = baseFreq * 2.0 / sampleRate;
    double scale = 0.0;
    for (; maxHarms >= 1; maxHarms >>= 1) {
        defineHarmonics(tableLen, maxHarms, ar, ai);
        scale = WaveTableNode::makeAddWaveTable(tableLen, ar, ai, scale, topFreq);
        topFreq *= 2;
        if (tableLen > constantRatioLimit) // variable table size (constant oversampling but with minimum table size)
            tableLen >>= 1;
    }
}

void SquareOscNode::defineHarmonics(int len, int numHarmonics, double* ar, double* ai) {
    if (numHarmonics > (len >> 1))
        numHarmonics = (len >> 1);
    
    // clear
    for (int idx = 0; idx < len; idx++) {
        ai[idx] = 0;
        ar[idx] = 0;
    }

    // sawtooth
    for (int idx = 1, jdx = len - 1; idx <= numHarmonics; idx++, jdx--) {
        double temp = -1.0 / idx;
        ar[idx] = -temp;
        ar[jdx] = temp;
    }
}
