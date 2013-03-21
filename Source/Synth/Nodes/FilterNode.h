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
#include "../../Dependencies/DSPFilters/Dsp.h" 

class FilterNode: public GPNode {
    public:
        FilterNode(int t, int o, int fpc, double sr, GPMutatableParam* cf, GPMutatableParam* bwq, GPNode* signal, GPNode* center, GPNode* bandwidth);
        ~FilterNode();
        FilterNode* getCopy();

        void evaluateBlock(double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer);
        std::string toString();
        void updateMutatedParams();

    private:
        int type;
        int order;
        int fadeParameterChanges;
        double sampleRate;
        double centerFrequency;
        double bandwidth;
        double gainDb;
        double quality;
        Dsp::Filter* filter;
        Dsp::Params params;
};

#endif
