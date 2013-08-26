#ifndef SPLINEENVELOPENODE_H
#define SPLINEENVELOPENODE_H

#include "../../GPNode.h"

class SplineEnvelopeNode: public GPNode {
public:
    SplineEnvelopeNode(GPMutatableParam* splinetype, GPMutatableParam* numpoints, std::vector<GPMutatableParam*>* pointsOrParams, GPNode* signal);
    ~SplineEnvelopeNode();

	// overrides
    SplineEnvelopeNode* getCopy();
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);

    // optional overrides
    void ephemeralRandom(GPRandom* rng);
	void setRenderInfo(float sr, unsigned blockSize, unsigned maxNumFrames, float maxTime);
    void doneRendering();
	void updateMutatedParams();

	// class specific
    void fillFromParams();

private:
    int splineType;
    int numSegments;
    bool isPrimitive;

    float sampleRate;

    unsigned envelopeSize;
    float* envelope;
};

#endif
