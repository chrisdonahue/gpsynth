/*
  ==============================================================================

    TimeNode.h
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef TIMENODE_H
#define TIMENODE_H

#include "../GPNode.h"

class TimeNode: public GPNode {
    public:
        TimeNode();
        ~TimeNode();
        TimeNode* getCopy();

        void evaluateBlock(double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer);
        std::string toString();
        void updateMutatedParams();

    private:
};

#endif
