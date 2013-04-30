/*
  ==============================================================================

    FilterNode.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef FILTERNODE_H
#define FILTERNODE_H

#include "../GPNode.h"
#include "../../Dependencies/YorkFilters/filterdesign.h"

class FilterNode: public GPNode {
public:
    FilterNode(unsigned t, unsigned o, unsigned fpc, double sr, int vn, GPMutatableParam* cfmultmin, GPMutatableParam* cfmultmax, GPMutatableParam* bwq, GPNode* signal, GPNode* center, GPNode* bandwidth);
    ~FilterNode();

	// overrides
    FilterNode* getCopy();
	void prepareToPlay();
    void evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer);
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
	void getRangeTemp(float* min, float* max);
	void updateMutatedParams();
    void toString(bool printRange, std::stringstream& ss);

private:
    unsigned type;
    unsigned order;
    unsigned fadeParameterChanges;
    double sampleRate;
    double nyquist;

    int variableNum;

    double centerFrequencyMultiplierMin;
    double centerFrequencyMultiplierMax;
    double bandwidthQuality;
    double bandwidthQualityMinimum;
    double bandwidthQualityMaximum;

    double maxGain;
};

#endif
