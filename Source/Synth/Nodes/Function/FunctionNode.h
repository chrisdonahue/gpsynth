/*
  ==============================================================================

    FunctionNode.h
    Author:  cdonahue

  ==============================================================================
*/

#ifndef FUNCTIONNODE_H
#define FUNCTIONNODE_H

#include "../../GPNode.h"

class FunctionNode: public GPNode {
public:
	// overrides
    void toString(std::stringstream& ss);

    // subclass overrides
    virtual FunctionNode* getCopy() = 0;
	virtual void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) = 0;
    
    // function symbol
    std::string symbol;
private:
};

#endif
