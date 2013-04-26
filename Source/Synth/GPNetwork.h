/*
  ==============================================================================

    GPNetwork.h
    Created: 6 Feb 2013 11:05:02am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef GPNETWORK_H
#define GPNETWORK_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <string>
#include "GPNode.h"
#include "GPPrimitives.h"

class GPNetwork {
public:
    // CONSTRUCTION
    GPNetwork(GPNode* r, std::string o);
    GPNetwork(GPParams* p, double sr, std::string netstring);
    ~GPNetwork();
    GPNetwork* getCopy();

    // EXAMINATION
    void evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, unsigned n, float* buffer);
	void evaluateBlockPerformance(unsigned fn, float* t, unsigned nv, float* v, unsigned n, float* buffer);
    std::string toString(bool printRange, unsigned precision);
    GPNode* getRoot();
    bool equals(GPNetwork* other);
    GPNode* getRandomNetworkNode(GPRandom* r);
    std::vector<GPMutatableParam*>* getAllMutatableParams();

    // HELPERS
    void mutate(GPParams* p);
    void replaceSubtree(GPNode* one, GPNode* two);
    void traceNetwork();
    void ephemeralRandom(GPRandom* r);
    void updateMutatedParams();

    // PUBLIC STATE
    int ID;
    std::string origin;
    int height;
    double fitness;
    
    // interval
    float minimum;
    float maximum;

private:
    // PRIVATE STATE
    GPNode* root;
    std::vector<GPNode*> allNodes;
    std::vector<GPMutatableParam*> allMutatableParams;
};

extern GPNode* createSubtree(GPParams* p, double sr, char* tokenized);
extern GPMutatableParam* createMutatableParam(char* tokenized);

#endif
