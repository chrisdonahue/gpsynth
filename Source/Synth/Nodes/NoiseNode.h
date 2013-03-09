/*
  ==============================================================================

    NoiseNode.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef NOISENODE_H
#define NOISENODE_H

#include "../GPNode.h"
#include <list>

class NoiseNode: public GPNode {
    public:
        NoiseNode(unsigned buffersize, GPRandom* r);
        ~NoiseNode();
        NoiseNode* getCopy();

        double evaluate(double* t, double* v);
        std::string toString();
        void mutate(GPParams* e);

    private:
        GPRandom* rng;
        unsigned bufferSize;
        unsigned currentBufferIndex;
        double* buffer;
};

#endif
