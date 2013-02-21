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
#include <vector>

/*
    Convenient naming convention for GP-safe functions
*/
typedef double (GPFunction)(double, double);

struct GPNodeParams;

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

        // N-ARY INDICATOR FOR MUTATION
        bool isBinary;
        bool isTerminal;

        // INHERITED TRACE METHOD FOR RESTORING PARENT LINKS
        void traceLineage(std::vector<GPNode*>* allnodes) {
            allnodes->push_back(this);
            if (left != NULL) {
                left->parent = this;
                left->traceLineage(allnodes);
            }
            if (right != NULL) {
                right->parent = this;
                right->traceLineage(allnodes);
            }
        };
};

/*
    Structure passed to a network for mutation at the node level.
*/
struct GPNodeParams {
    // OSCILNODE
    float partialChance;
    int numPartials;

    // VALUENODE
    double valueMin;
    double valueMax;

    // MODULATION NODE
    int LFORange;

    // MULTIPLE
    int numVariables;

    GPRandom* rng;

    std::vector<GPNode*>* availableNodes;
    std::vector<double>* nodeLikelihoods;
    std::vector<GPFunction*>* availableFunctions;
    std::vector<double>* functionLikelihoods;
};

#endif
