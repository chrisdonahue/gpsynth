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
    // evaluate signal
    descendants[0]->evaluateBlock(fn, t, nv, v, min, max, n, buffer);
    float* audioData[1];
    audioData[0] = buffer;

    // evaluate center frequency signal
    float* cfbuffer = (float*) malloc(sizeof(float) * n);
    //float* cfbuffer = (float*) malloc(sizeof(float) * 1);
    double cfmin = std::numeric_limits<double>::min();
    double cfmax = std::numeric_limits<double>::max();
    descendants[1]->evaluateBlock(fn, t, nv, v, &cfmin, &cfmax, n, cfbuffer);
    //descendants[1]->evaluateBlock(fn + (n-1), t + (n-1), nv, v + (n-1), &cfmin, &cfmax, 1, cfbuffer);

    // create mapping from center frequency signal to center frequency multiplier
    double cfm = 0;
    double cfb = 0;
    continuousMapRange(cfmin, cfmax, centerFrequencyMultiplierMin, centerFrequencyMultiplierMax, &cfm, &cfb);

    // get pointer to variable we are using
    double* currentIndex = v + variableNum;

    // reset filter on init
    if (fn == 0) {
        filter->reset();
        params[2] = (*currentIndex) * (cfbuffer[0] * cfm + cfb);
        params[3] = bandwidthQuality;
        filter->setParams(params);
    }

    /*
    float* bwqbuffer = (float*) malloc(sizeof(float) * n);
    double bwqmin = std::numeric_limits<double>::min();
    double bwqmax = std::numeric_limits<double>::max();
    descendants[2]->evaluateBlock(fn, t, nv, v, &bwqmin, &bwqmax, n, bwqbuffer);
    double bwqscale = (bandwidthQualityMax - bandwidthQualityMin) / (bwqmax - bwqmin);
    */

    // TODO: calculate min/max for lopass/hipass


    // update filter params
    params[2] = (*currentIndex) * (cfbuffer[n-1] * cfm + cfb);
    params[3] = bandwidthQuality;

   /* 
    if (params[2] < 1.0 || params[2] >= params[0] / 2) {
	std::cout << "[" << cfmin << ", " << cfmax << "] -> [" << centerFrequencyMultiplierMin << ", " << centerFrequencyMultiplierMax << "]" << std::endl;
	std::stringstream ss;
	descendants[1]->toString(ss);
	std::cout << ss.str() << std::endl;
        std::cout << params[2] << ", " << cfbuffer[n-1] << ", " << cfm << ", "<< cfb << std::endl;
    }
    if (cfm > 0 && cfm <= 0.01) {
        std::cout << "WOW RANGE IS CLOSE" << std::endl;
	std::cout << "[" << cfmin << ", " << cfmax << "] -> [" << centerFrequencyMultiplierMin << ", " << centerFrequencyMultiplierMax << "]" << std::endl;
	std::stringstream ss;
	descendants[1]->toString(ss);
	std::cout << ss.str() << std::endl;
        std::cout << params[2] << ", " << cfbuffer[n-1] << ", " << cfm << ", "<< cfb << std::endl;
        std::cout << "------------" << std::endl;
    }
    */

    for (unsigned i = 0; i < n; i++) {
        if (cfbuffer[i] < -1.0 || cfbuffer[i] > 1.0) {
            std::cout << "SIN IS MESSED UP" << std::endl;
            std::cout << buffer[i] << std::endl;
        }
    }

    filter->setParams(params);
    filter->process(n, audioData);

    for (unsigned i = 0; i < n; i++) {
        if (buffer[i] < -1.0 || buffer[i] > 1.0) {
            std::cout << "[" << cfmin << ", " << cfmax << "] -> [" << centerFrequencyMultiplierMin << ", " << centerFrequencyMultiplierMax << "]" << std::endl;
            std::stringstream ss;
            descendants[1]->toString(true, ss);
            std::cout << ss.str() << std::endl;
            std::cout << params[2] << ", " << params[3] << ", " << cfbuffer[n-1] << ", " << cfm << ", "<< cfb << std::endl;
            std::cout << "FILTER IS MESSED UP" << std::endl;
            std::cout << buffer[i] << std::endl;
            exit(-1);
        }
    }

    //free(bwqbuffer);
    free(cfbuffer);
}

void FilterNode::evaluateBlockPerformance(unsigned fn, float* t, unsigned nv, float* v, float* min, float* max, unsigned n, float* buffer) {

}

void FilterNode::toString(bool printRange, std::stringstream& ss) {
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
    ss << "[";
    mutatableParams[0]->toString(printRange, ss);
    ss <<  ", ";
    mutatableParams[1]->toString(printRange, ss);
    ss << "] ";
    mutatableParams[2]->toString(printRange, ss);
    ss << " ";
    descendants[0]->toString(printRange, ss);
    ss << " ";
    descendants[1]->toString(printRange, ss);
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
    //params[2] = centerFrequencyMultiplier * 1.0;
    //params[3] = bandwidthQuality;
    //filter->setParams(params);
}

void FilterNode::updateMutatedParams() {
    fillFromParams();

    // call on descendants
    descendants[0]->updateMutatedParams();
    descendants[1]->updateMutatedParams();
    //descendants[2]->updateMutatedParams();
}
