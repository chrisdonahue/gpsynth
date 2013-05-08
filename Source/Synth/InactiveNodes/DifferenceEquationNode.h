/*
  ==============================================================================

    DifferenceEquationNode.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef DIFFERENCEEQUATION_H
#define DIFFERENCEEQUATION_H

#include "../GPNode.h"

class DifferenceEquationNode: public GPNode {
public:
    DifferenceEquationNode(unsigned m, unsigned n, GPRandom* r, bool erc, GPMutatableParam* z, GPMutatableParam* p, GPNode* signal);
    DifferenceEquationNode(std::vector<GPMutatableParam*>& xcoef, std::vector<GPMutatableParam*>& ycoef, GPNode* signal);
    ~DifferenceEquationNode();

	// overrides
    DifferenceEquationNode* getCopy();
	void prepareToPlay();
    void evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer);
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
	void setRenderInfo(float sr, unsigned blockSize, float maxTime);
	void updateMutatedParams();
    void toString(std::stringstream& ss);

private:
    // difference equation zeroes
    unsigned numXCoefficients;
    std::vector<GPMutatableParam*> xcoefficients;
    float* xcoefs;
    float* x;

    // difference equation poles
    unsigned numYCoefficients;
    std::vector<GPMutatableParam*> ycoefficients;
    float* ycoefs;
    float* y;
};

#endif
