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

FilterNode::FilterNode(int t, int o, int fpc, double sr, GPMutatableParam* cf, GPMutatableParam* bwq, GPNode* l) :
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

    left = l;

    isBinary = false;
    isTerminal = false;
}

FilterNode::~FilterNode() {
    delete filter;
}

FilterNode* FilterNode::getCopy() {
    return new FilterNode(type, order, fadeParameterChanges, sampleRate, mutatableParams[0]->getCopy(), mutatableParams[1]->getCopy(), left == NULL ? NULL : left->getCopy());
}

double FilterNode::evaluate(double* t, double* v) {
    double* audioData[1];
    audioData[0] = new double[1];
    audioData[0][0] = left->evaluate(t, v);
    filter->process(1, audioData);
    return audioData[0][0];
}

void FilterNode::evaluateBlock(double* t, unsigned nv, double* v, unsigned n, float* buffer) {
    float* audioData[1];
    audioData[0] = buffer;
    left->evaluateBlock(t, nv, v, n, audioData[0]);
    filter->process(n, audioData);
}

std::string FilterNode::toString() {
    char buffer[1024];
    if (type == 0)
        snprintf(buffer, 1024, "(lowpass %.2lf %.2lf %s)", centerFrequency, quality, left->toString().c_str());
    else if (type == 1)
        snprintf(buffer, 1024, "(highpass %.2lf %.2lf %s)", centerFrequency, quality, left->toString().c_str());
    else if (type == 2)
        snprintf(buffer, 1024, "(bandpass %.2lf %.2lf %s)", centerFrequency, bandwidth, left->toString().c_str());
    else if (type == 3)
        snprintf(buffer, 1024, "(bandstop %.2lf %.2lf %s)", centerFrequency, bandwidth, left->toString().c_str());
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
    left->updateMutatedParams();
}
