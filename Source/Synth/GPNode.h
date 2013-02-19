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

/*
    Convenient naming convention for GP-safe functions
*/
typedef double (GPFunction)(double, double);

/*
    Structure passed to a network for mutation at the node level.
*/
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
        // PURE VIRTUAL METHODS THAT ALL SUBCLASSES WILL IMPLEMENT
        virtual double evaluate(double* t, double* v) = 0;
        virtual std::string toString() = 0;
        virtual GPNode* getCopy() = 0;
        virtual void mutate(GPNodeParams* e) = 0;

        // HERITAGE POINTERS
        GPNode* parent;
        GPNode* left;
        GPNode* right;

        // INHERITED TRACE METHOD FOR RESTORING PARENT LINKS
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
