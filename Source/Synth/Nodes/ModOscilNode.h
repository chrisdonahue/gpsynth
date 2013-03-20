/*
  ==============================================================================

    ModOscilNode.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef MODOSCILNODE_H
#define MODOSCILNODE_H

#include <cmath>
#include "../GPNode.h"

class ModOscilNode: public GPNode {
    public:
        ModOscilNode(GPNode* zero, GPNode* one);
        ~ModOscilNode();
        ModOscilNode* getCopy();

        void evaluateBlock(double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer);
        std::string toString();
        void updateMutatedParams();

    private:
        //int partial;
        double w;
};

#endif
