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
#include <algorithm>

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
        virtual bool equals(GPNode* other) = 0;

        // HERITAGE POINTERS
        GPNode* parent;
        GPNode* left;
        GPNode* right;

        // N-ARY INDICATOR FOR MUTATION
        bool isBinary;
        bool isTerminal;

        // INHERITED TRACE METHOD
        void traceSubtree(std::vector<GPNode*>* allnodes, GPNode* p, int* rootHeight, int currentDepth) {
            parent = p;
            allnodes->push_back(this);
            if (currentDepth > *rootHeight) {
                *rootHeight = currentDepth;
            }
            if (!isTerminal) {
                left->traceSubtree(allnodes, this, rootHeight, currentDepth + 1);
                if (isBinary) {
                    right->traceSubtree(allnodes, this, rootHeight, currentDepth + 1);
                }
            }
        };
};

/*
    Structure passed to a network for mutation at the node level.
*/
struct GPNodeParams {
    // VALUENODE
    double valueMin;
    double valueMax;

    // FUNCTION NODE
    std::vector<GPFunction*>* availableGPFunctions;

    // OSCILNODE
    float partialChance;
    int numPartials;

    // MODULATION NODE
    int LFORange;

    // MULTIPLE
    GPRandom* rng;
    int numVariables;
};

#endif
