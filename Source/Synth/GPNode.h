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
#include "../Common/GPRandom.h"

typedef double (GPFunction)(double, double);

struct GPNodeParams {
    float simplifyChance;
    float specialChance;
    float harmonyChance;
    float functionChance;

    int valueRange;
    int LFORange;
    int numPartials;

    int numVariables;

    GPRandom* rng;

    std::vector<GPNode*>* availableNodes;
    std::vector<double>* nodeLikelihoods;
    std::vector<GPFunction*>* availableFunctions;
    std::vector<double>* functionLikelihoods;
};

class GPNode {
    public:
        virtual double evaluate(double* t, double* v) = 0;
        virtual std::string toString() = 0;
        virtual GPNode* getCopy() = 0;
        virtual void mutate(GPNodeParams* e) = 0;

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
