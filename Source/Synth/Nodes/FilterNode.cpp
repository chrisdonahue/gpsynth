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

FilterNode::FilterNode(unsigned t, unsigned o, unsigned fpc, double sr, int vn, GPMutatableParam* cfmultmin, GPMutatableParam* cfmultmax, GPMutatableParam* bwq, GPNode* signal, GPNode* center, GPNode* bandwidth) :
    params()
{
    type = t;
    order = o;
    fadeParameterChanges = fpc;
    sampleRate = sr;
    nyquist = sampleRate/2;

    variableNum = vn;

    mutatableParams.push_back(cfmultmin);
    mutatableParams.push_back(cfmultmax);
    mutatableParams.push_back(bwq);

    descendants.push_back(signal);
    descendants.push_back(center);
    //descendants.push_back(bandwidth);
    //arity = 3;
    arity = 2;

    if (type == 0) {
        filter = new Dsp::SmoothedFilterDesign<Dsp::Butterworth::Design::LowPass <3>, 1, Dsp::DirectFormII> (fadeParameterChanges);
    }
    else if (type == 1) {
        filter = new Dsp::SmoothedFilterDesign<Dsp::Butterworth::Design::HighPass <3>, 1, Dsp::DirectFormII> (fadeParameterChanges);
    }
    else if (type == 2) {
        filter = new Dsp::SmoothedFilterDesign<Dsp::Butterworth::Design::BandPass <3>, 1, Dsp::DirectFormII> (fadeParameterChanges);
    }
    else if (type == 3) {
        filter = new Dsp::SmoothedFilterDesign<Dsp::Butterworth::Design::BandStop <3>, 1, Dsp::DirectFormII> (fadeParameterChanges);
    }
    params[0] = sampleRate;
    params[1] = order;

    fillFromParams();
}

FilterNode::~FilterNode() {
    delete filter;
}

FilterNode* FilterNode::getCopy() {
    //return new FilterNode(type, order, fadeParameterChanges, sampleRate, variableNum, mutatableParams[0]->getCopy(), mutatableParams[1]->getCopy(), mutatableParams[2]->getCopy(), descendants[0] == NULL ? NULL : descendants[0]->getCopy(), descendants[1] == NULL ? NULL : descendants[1]->getCopy(), descendants[2] == NULL ? NULL : descendants[2]->getCopy());
    return new FilterNode(type, order, fadeParameterChanges, sampleRate, variableNum, mutatableParams[0]->getCopy(), mutatableParams[1]->getCopy(), mutatableParams[2]->getCopy(), descendants[0] == NULL ? NULL : descendants[0]->getCopy(), descendants[1] == NULL ? NULL : descendants[1]->getCopy(), NULL);
}

void FilterNode::evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer) {
    if (fn = 0)
        filter->reset();

    descendants[0]->evaluateBlock(fn, t, nv, v, min, max, n, buffer);
    float* audioData[1];
    audioData[0] = buffer;

    float* cfbuffer = (float*) malloc(sizeof(float) * n);
    double cfmin = std::numeric_limits<double>::min();
    double cfmax = std::numeric_limits<double>::max();
    descendants[1]->evaluateBlock(fn, t, nv, v, &cfmin, &cfmax, n, cfbuffer);
    double cfscale = (centerFrequencyMultiplierMax - centerFrequencyMultiplierMin) / (cfmax - cfmin);

    /*
    float* bwqbuffer = (float*) malloc(sizeof(float) * n);
    double bwqmin = std::numeric_limits<double>::min();
    double bwqmax = std::numeric_limits<double>::max();
    descendants[2]->evaluateBlock(fn, t, nv, v, &bwqmin, &bwqmax, n, bwqbuffer);
    double bwqscale = (bandwidthQualityMax - bandwidthQualityMin) / (bwqmax - bwqmin);
    */

    // TODO: calculate min/max for lopass/hipass

    double* currentIndex = v + variableNum;
    /*
    for (unsigned i = 0; i < n; i++) {
        params[2] = (*currentIndex) * ((cfbuffer[i] * cfscale) + centerFrequencyMultiplier);
        
        if (i == 0)
            std::cout << "CF: " << params[2] << ", BW: " << bandwidthQuality << ", v0: " << *currentIndex << ", BUF: " << cfbuffer[i] << ", SCALE: " << cfscale << ", CFM: " << centerFrequencyMultiplier << std::endl;
        
        //params[3] = (params[1]/nyquist) * (bwqbuffer[i] * bandwidthQuality);
        params[3] = bandwidthQuality;
        filter->setParams(params);
        filter->process(1, audioData);
        audioData[0] = audioData[0] + 1;
        currentIndex += nv;
    }
    */

    params[2] = (*currentIndex) * ((cfbuffer[0] * cfscale) + centerFrequencyMultiplier);
    params[3] = bandwidthQuality;
    filter->setParams(params);
    filter->process(n, audioData);

    //free(bwqbuffer);
    free(cfbuffer);
}

void FilterNode::toString(std::stringstream& ss) {
    if (type == 0) {
        ss << "(lowpass ";
    }
    else if (type == 1) {
        ss << "(highpass ";
    }
    else if (type == 2) {
        ss << "(bandpass ";
    }
    else if (type == 3) {
        ss << "(bandstop ";
    }
    ss << centerFrequencyMultiplier << " " << bandwidthQuality << " ";
    descendants[0]->toString(ss);
    ss << " ";
    descendants[1]->toString(ss);
    //ss << " ";
    //descendants[2]->toString(ss);
    ss << ")";
}

void FilterNode::fillFromParams() {
    // update mutated params
    centerFrequencyMultiplierMin = mutatableParams[0]->getValue();
    centerFrequencyMultiplierMax = mutatableParams[1]->getValue();
    if (centerFrequencyMultiplierMax < centerFrequencyMultiplierMin) {
        centerFrequencyMultiplierMin = centerFrequencyMultiplierMax;
        centerFrequencyMultiplierMax = centerFrequencyMultiplierMin;
    }
    bandwidthQuality = mutatableParams[2]->getValue();

    // remake filter
    centerFrequencyMultiplier = (centerFrequencyMultiplierMin + centerFrequencyMultiplierMax) / 2;
    params[2] = centerFrequencyMultiplier * 1.0;
    params[3] = bandwidthQuality;
    filter->setParams(params);
}

void FilterNode::updateMutatedParams() {
    fillFromParams();

    // call on descendants
    descendants[0]->updateMutatedParams();
    descendants[1]->updateMutatedParams();
    //descendants[2]->updateMutatedParams();
}
