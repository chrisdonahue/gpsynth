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

FilterNode::FilterNode(int t, int o, int fpc, double sr, GPMutatableParam* cf, GPMutatableParam* bwq, GPNode* signal, GPNode* center, GPNode* bandwidth) :
params()
{
    type = t;
    order = o;
    fadeParameterChanges = fpc;
    sampleRate = sr;
    centerFrequency = cf->getValue();
    if (type < 2)
        quality = bwq->getValue();
    else
        bandwidth = bwq->getValue();

    mutatableParams.push_back(cf);
    mutatableParams.push_back(bwq);

    if (type == 0) {
        filter = new Dsp::SmoothedFilterDesign<Dsp::RBJ::Design::LowPass, 1> (fadeParameterChanges);
        params[0] = sampleRate;
        params[1] = centerFrequency;
        params[2] = quality;
    }
    else if (type == 1) {
        filter = new Dsp::SmoothedFilterDesign<Dsp::RBJ::Design::HighPass, 1> (fadeParameterChanges);
        params[0] = sampleRate;
        params[1] = centerFrequency;
        params[2] = quality;
    }
    else if (type == 2) {
        filter = new Dsp::SmoothedFilterDesign<Dsp::RBJ::Design::BandPass2, 1> (fadeParameterChanges);
        params[0] = sampleRate;
        params[1] = centerFrequency;
        params[2] = bandwidth;
    }
    else if (type == 3) {
        filter = new Dsp::SmoothedFilterDesign<Dsp::RBJ::Design::BandStop, 1> (fadeParameterChanges);
        params[0] = sampleRate;
        params[1] = centerFrequency;
        params[2] = bandwidth;
    }
    filter->setParams(params);

    descendants.push_back(signal);
    descendants.push_back(center);
    descendants.push_back(bandwidth);
    arity = 3;
}

FilterNode::~FilterNode() {
    delete filter;
}

FilterNode* FilterNode::getCopy() {
    return new FilterNode(type, order, fadeParameterChanges, sampleRate, mutatableParams[0]->getCopy(), mutatableParams[1]->getCopy(), descendants[0] == NULL ? NULL : descendants[0]->getCopy(), descendants[1] == NULL ? NULL : descendants[1]->getCopy(), descendants[2] == NULL ? NULL : descendants[2]->getCopy());
}

void FilterNode::evaluateBlock(double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer) {
    // TODO: is this possible to calculate?...
    float* audioData[1];
    audioData[0] = buffer;
    descendants[0]->evaluateBlock(t, nv, v, min, max, n, audioData[0]);
    // TODO: calculate this
    *min = std::numeric_limits<double>::infinity() * -1; 
    *max = std::numeric_limits<double>::infinity();
    filter->process(n, audioData);
}

std::string FilterNode::toString() {
    char buffer[1024];
    if (type == 0)
        snprintf(buffer, 1024, "(lowpass %.2lf %.2lf %s)", centerFrequency, quality, descendants[0]->toString().c_str());
    else if (type == 1)
        snprintf(buffer, 1024, "(highpass %.2lf %.2lf %s)", centerFrequency, quality, descendants[0]->toString().c_str());
    else if (type == 2)
        snprintf(buffer, 1024, "(bandpass %.2lf %.2lf %s)", centerFrequency, bandwidth, descendants[0]->toString().c_str());
    else if (type == 3)
        snprintf(buffer, 1024, "(bandstop %.2lf %.2lf %s)", centerFrequency, bandwidth, descendants[0]->toString().c_str());
    return std::string(buffer);
}

void FilterNode::updateMutatedParams() {
    centerFrequency = mutatableParams[0]->getValue();
    params[1] = centerFrequency;
    if (type < 2) {
        quality = mutatableParams[1]->getValue();
        params[2] = quality;
    }
    else {
        bandwidth = mutatableParams[1]->getValue();
        params[1] = bandwidth;
    }
    filter->setParams(params);
    descendants[0]->updateMutatedParams();
}
