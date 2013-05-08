/*
  ==============================================================================

    NoiseNode.cpp
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#include "NoiseNode.h"

/*
    ========================
    CONSTRUCTION/DESTRUCTION
    ========================
*/

NoiseNode::NoiseNode(GPRandom* r) {
    rng = r;
    
    arity = 0;
    
    minimum = -1;
    maximum = 1;
}

NoiseNode::~NoiseNode() {
}

/*
    =========
    OVERRIDES
    =========
*/

NoiseNode* NoiseNode::getCopy() {
    return new NoiseNode(rng);
}

void NoiseNode::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {
	firstFrameNumber;
	sampleTimes;
    numConstantVariables;
    constantVariables;

    if (firstFrameNumber == 0) {
        g_fScale = 2.0f / 0xffffffff;
        g_x1 = 0x67452301;
        g_x2 = 0xefcdab89;
    }

    whitenoise(buffer, numSamples, 1.0);
}

void NoiseNode::toString(std::stringstream& ss) {
    ss << "(whitenoise)";
}

/*
    ==============
    CLASS SPECIFIC
    ==============
*/

/*
    http://musicdsp.org/showArchiveComment.php?ArchiveID=216
*/
void NoiseNode::whitenoise(
    float* _fpDstBuffer, // Pointer to buffer
    unsigned int _uiBufferSize, // Size of buffer
    float _fLevel ) // Noiselevel (0.0 ... 1.0)
{
    _fLevel *= g_fScale;

    while( _uiBufferSize-- )
    {
        g_x1 ^= g_x2;
        *_fpDstBuffer++ = g_x2 * _fLevel;
        g_x2 += g_x1;
    }
}
