/*
  ==============================================================================

    GPNetwork.h
    Created: 6 Feb 2013 11:05:02am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef GPNETWORK_H
#define GPNETWORK_H

#include <cmath>
#include <iostream>
#include <limits>
#include <string>
#include <string.h>
#include "GPNode.h"
#include "GPPrimitives.h"
#include "../Common/GPHelpers.h"
#include "../Common/JUCEFileIO.h"

class GPNetwork {
public:
    // CONSTRUCTION
    GPNetwork(GPNode* r, std::string o);
    GPNetwork(GPRandom* rng, std::string netstring);
    ~GPNetwork();
    GPNetwork* getCopy(std::string neworigin);

    // EXAMINATION
	void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer);
    std::string toString(unsigned precision);
    GPNode* getRoot();
    bool equals(GPNetwork* other, unsigned precision);
    GPNode* getRandomNetworkNode(GPRandom* r);
    std::vector<GPMutatableParam*>* getAllMutatableParams();

    // HELPERS
    void prepareToRender(float sr, unsigned blockSize, float maxTime);
    void doneRendering();
    void traceNetwork();
    void replaceSubtree(GPNode* one, GPNode* two);
    void ephemeralRandom(GPRandom* r);

    // PUBLIC STATE
    int ID;
    std::string origin;
    int height;
    double fitness;
    
    // interval
    float minimum;
    float maximum;

    // render state
    bool preparedToRender;

private:
    // PRIVATE STATE
    GPNode* root;
    GPNode* renderRoot;
    std::vector<GPNode*> allNodes;
    std::vector<GPMutatableParam*> allMutatableParams;
};

// S-EXPRESSION PARSING
// macro to "consume" a token when we observe it
#define consume (*currentIndex)++
extern GPNode* createSubtree(std::vector<std::string> tokens, unsigned* currentIndex, GPRandom* rng);
// macro for standard structure of vector of tokens/index
#define tokenizer tokens, currentIndex
// macro for calling createSubtree easily
#define subtreeArgs rng
extern GPMutatableParam* createMutatableParam(std::vector<std::string> tokens, unsigned* currentIndex, bool ismutatable, std::string type);

#endif
