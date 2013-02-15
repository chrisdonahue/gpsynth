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

struct GPParams {
    float mutationChance;
    float simplifyChance;
    float specialChance;
    float harmonyChance;
    float functionChance;
    float crossChance;

    int valueRange;
    int LFORange;
    int numPartials;
};

class GPNode {
    public:
        virtual double evaluate(double* t, float* f) = 0;
        virtual std::string toString() = 0;
        virtual GPNode* getCopy() = 0;
        virtual void mutate(GPParams* e) = 0;

        GPNode* parent;
        GPNode* left;
        GPNode* right;

        void traceLineage() {
            if (left != NULL) {
                left->parent = this;
                left->traceLineage();
            }
            if (right != NULL) {
                right->parent = this;
                right->traceLineage();
            }
        };
};

#endif
