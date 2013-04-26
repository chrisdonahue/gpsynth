/*
  ==============================================================================

    OscilNode.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef OSCILNODE_H
#define OSCILNODE_H

#include "../GPNode.h"

class OscilNode: public GPNode {
public:
    OscilNode(bool terminal, GPMutatableParam* p, int vn, GPMutatableParam* i, GPNode* mod);
    ~OscilNode();

	// overrides
    OscilNode* getCopy();
	void prepareToPlay();
    void evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer);
	void evaluateBlockPerformance(unsigned fn, float* t, unsigned nv, float* v, float* min, float* max, unsigned n, float* buffer);
	void getRange(float* min, float* max);
	void updateMutatedParams();
    void toString(bool printRange, std::stringstream& ss);

private:
    bool terminalOscil;
    int partial;
    int variableNum;
    double w;
    double index;
};

#endif
