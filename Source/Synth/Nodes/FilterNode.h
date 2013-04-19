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
    FilterNode* getCopy();

    void evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer);
	void evaluateBlockPerformance(unsigned fn, float* t, unsigned nv, float* v, float* min, float* max, unsigned n, float* buffer);
    void toString(bool printRange, std::stringstream& ss);
    void fillFromParams();
    void updateMutatedParams();

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
