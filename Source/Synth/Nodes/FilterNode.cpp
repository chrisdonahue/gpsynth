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

FilterNode::FilterNode(int t, int o, int fpc, double sr, double cf, double bw, double q, GPNode* l) :
params()
{
    type = t;
    order = o;
    fadeParameterChanges = fpc;
    sampleRate = sr;
    centerFrequency = cf;
    bandwidth = bw;
    quality = q;

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
    right = NULL;
    parent = NULL;
    isBinary = false;
    isTerminal = false;
}

FilterNode::~FilterNode() {
    delete left;
    delete right;
    delete filter;
}

FilterNode* FilterNode::getCopy() {
    return new FilterNode(type, order, fadeParameterChanges, sampleRate, centerFrequency, bandwidth, quality, left->getCopy());
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
        snprintf(buffer, 1024, "(lowpass %lf %lf %s)", centerFrequency, quality, left->toString().c_str());
    else if (type == 1)
        snprintf(buffer, 1024, "(highpass %lf %lf %s)", centerFrequency, quality, left->toString().c_str());
    else if (type == 2)
        snprintf(buffer, 1024, "(bandpass %lf %lf %s)", centerFrequency, bandwidth, left->toString().c_str());
    else if (type == 3)
        snprintf(buffer, 1024, "(bandstop %lf %lf %s)", centerFrequency, bandwidth, left->toString().c_str());
    return std::string(buffer);
}

void FilterNode::mutate(GPParams* e) {

}
