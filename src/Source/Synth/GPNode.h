/*
  ==============================================================================

    GPNode.h
    Created: 6 Feb 2013 11:06:00am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef GPNODE_H
#define GPNODE_H

#include <string>

class GPNode {
    public:
        virtual double evaluate(double* t, float* f) = 0;
        virtual std::string toString() = 0;
        virtual GPNode* getCopy() = 0;
        //void traceLineage();
        //virtual void mutate(GPExperiment* e) = 0;

        GPNode* parent;
        GPNode* left;
        GPNode* right;
};

#endif
