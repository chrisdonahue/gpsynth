/*
  ==============================================================================

    FunctionNode.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef FUNCTIONNODE_H
#define FUNCTIONNODE_H

#include "../GPNode.h"

class FunctionNode: public GPNode {
public:
	// overrides
    FunctionNode* getCopy();
	void setRenderInfo(float sr, unsigned blockSize, float maxTime);
	void prepareToPlay();
    void evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer);
	void updateMutatedParams();
    void toString(bool printRange, std::stringstream& ss);

    // subclass override for interval calculation
    virtual float operate(float a, float b) = 0;
    virtual void operateInterval(float minone, float maxone, float mintwo, float maxtwo, float* lower, float* upper) = 0;

private:
    std::string symbol;
};

#endif
