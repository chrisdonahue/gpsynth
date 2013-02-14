/*
  ==============================================================================

    DelayNode.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#include <stdio.h>
#include "GPNode.h"
#include <list>

class DelayNode: public GPNode {
    public:
        DelayNode(int n, GPNode* l, GPNode* r);
        ~DelayNode();
        DelayNode* getCopy();

        void setMemoryConstants(int x, int y);
        double evaluate(double* t, float* f);
        std::string toString();
        void traceLineage();

    private:
        int numFilled;

        int numSamples;
        std::list<double> delayLine;
};
