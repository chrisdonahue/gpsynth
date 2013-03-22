/*
  ==============================================================================

    FilterNode.cpp
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#include "FilterNode.h"

/*
    ==============
    PUBLIC METHODS
    ==============
*/

FilterNode::FilterNode(int t, int fpc, double sr, GPMutatableParam* cfmin, GPMutatableParam* cfmax, GPMutatableParam* bwqmin, GPMutatableParam* bwqmax, GPNode* signal, GPNode* center, GPNode* bandwidth) :
params()
{
    type = t;
    fadeParameterChanges = fpc;
    sampleRate = sr;

    mutatableParams.push_back(cfmin);
    mutatableParams.push_back(cfmin);
    mutatableParams.push_back(bwqmin);
    mutatableParams.push_back(bwqmax);

    descendants.push_back(signal);
    descendants.push_back(center);
    descendants.push_back(bandwidth);
    arity = 3;

    if (type == 0) {
        filter = new Dsp::SmoothedFilterDesign<Dsp::RBJ::Design::LowPass, 1> (fadeParameterChanges);
    }
    else if (type == 1) {
        filter = new Dsp::SmoothedFilterDesign<Dsp::RBJ::Design::HighPass, 1> (fadeParameterChanges);
    }
    else if (type == 2) {
        filter = new Dsp::SmoothedFilterDesign<Dsp::RBJ::Design::BandPass2, 1> (fadeParameterChanges);
    }
    else if (type == 3) {
        filter = new Dsp::SmoothedFilterDesign<Dsp::RBJ::Design::BandStop, 1> (fadeParameterChanges);
    }

    updateMutatedParams();
}

FilterNode::~FilterNode() {
    delete filter;
}

FilterNode* FilterNode::getCopy() {
    return new FilterNode(type, fadeParameterChanges, sampleRate, mutatableParams[0]->getCopy(), mutatableParams[1]->getCopy(), mutatableParams[2]->getCopy(), mutatableParams[3]->getCopy(), descendants[0] == NULL ? NULL : descendants[0]->getCopy(), descendants[1] == NULL ? NULL : descendants[1]->getCopy(), descendants[2] == NULL ? NULL : descendants[2]->getCopy());
}

void FilterNode::evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer) {
    if (fn = 0)
        filter->reset();

    descendants[0]->evaluateBlock(fn, t, nv, v, min, max, n, buffer);
    float* audioData[1];
    audioData[0] = buffer;

    float* cfbuffer = (float*) malloc(sizeof(float) * n);
    double cfmin;
    double cfmax;
    descendants[1]->evaluateBlock(fn, t, nv, v, &cfmin, &cfmax, n, cfbuffer);
    double cfscale = (centerFrequencyMax - centerFrequencyMin) / (cfmax - cfmin);

    float* bwqbuffer = (float*) malloc(sizeof(float) * n);
    double bwqmin;
    double bwqmax;
    descendants[2]->evaluateBlock(fn, t, nv, v, &bwqmin, &bwqmax, n, bwqbuffer);
    double bwqscale = (bandwidthQualityMax - bandwidthQualityMin) / (bwqmax - bwqmin);

    // TODO: calculate this for lopass/hipass
    if (type < 2) {
        *min = *min;
        *max = *max;
    }
    if (type >= 2) {
        *min = *min;
        *max = *max;
    }

    for (int i = 0; i < n; i++) {
        params[1] = (cfbuffer[i] * cfscale) + centerFrequency;
        params[2] = (bwqbuffer[i] * bwqscale) + bandwidthQuality;
        filter->process(1, audioData);
        audioData[0] = audioData[0] + 1;
    }

    free(bwqbuffer);
    free(cfbuffer);
}

std::string FilterNode::toString() {
    char buffer[1024];
    if (type == 0)
        snprintf(buffer, 1024, "(lowpass %.2lf %.2lf %s)", centerFrequency, bandwidthQuality, descendants[0]->toString().c_str());
    else if (type == 1)
        snprintf(buffer, 1024, "(highpass %.2lf %.2lf %s)", centerFrequency, bandwidthQuality, descendants[0]->toString().c_str());
    else if (type == 2)
        snprintf(buffer, 1024, "(bandpass %.2lf %.2lf %s)", centerFrequency, bandwidthQuality, descendants[0]->toString().c_str());
    else if (type == 3)
        snprintf(buffer, 1024, "(bandstop %.2lf %.2lf %s)", centerFrequency, bandwidthQuality, descendants[0]->toString().c_str());
    return std::string(buffer);
}

void FilterNode::updateMutatedParams() {
    // update mutated params
    centerFrequencyMin = mutatableParams[0]->getValue();
    centerFrequencyMax = mutatableParams[1]->getValue();
    bandwidthQualityMin = mutatableParams[2]->getValue();
    bandwidthQualityMax = mutatableParams[3]->getValue();

    // remake filter
    params[0] = sampleRate;
    centerFrequency = (centerFrequencyMin + centerFrequencyMax) / 2;
    bandwidthQuality = (bandwidthQualityMin + bandwidthQualityMax) / 2;
    params[1] = centerFrequency;
    params[2] = bandwidthQuality;
    filter->setParams(params);

    // call on descendants
    if (descendants[0] != NULL)
        descendants[0]->updateMutatedParams();
    if (descendants[1] != NULL)
        descendants[1]->updateMutatedParams();
    if (descendants[2] != NULL)
        descendants[2]->updateMutatedParams();
}
