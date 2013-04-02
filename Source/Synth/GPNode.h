/*
  ==============================================================================

    GPNode.h
    Created: 6 Feb 2013 11:06:00am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef GPNODE_H
#define GPNODE_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <algorithm>
#include <limits>
#include "../Common/GPParams.h"
#include "../Common/GPMutatableParam.h"
#include "../Common/GPHelpers.h"
#include <sstream>

class GPNode {
public:
    GPNode()
        : parent(NULL), descendants(0), arity(0),
          mutatableParams(0) {
    }
    virtual ~GPNode() {
        for (unsigned i = 0; i < arity; i++) {
            delete descendants[i];
        }
        for (unsigned i = 0; i < mutatableParams.size(); i++) {
            delete mutatableParams[i];
        }
    }

    // PURE VIRTUAL METHODS THAT ALL SUBCLASSES WILL IMPLEMENT
    virtual void evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer) = 0;
    virtual void toString(std::stringstream& ss) = 0;
    virtual GPNode* getCopy() = 0;
    virtual void updateMutatedParams() = 0;

    // TREE STATE
    unsigned depth;

    // HERITAGE POINTERS
    GPNode* parent;
    std::vector<GPNode*> descendants;

    // INDICATORS ABOUT TYPE
    unsigned arity;
    bool isTerminal;

    // MUTATABLE PARAMS
    std::vector<GPMutatableParam*> mutatableParams;

    void ephemeralRandom(GPRandom* r) {
        for (unsigned i = 0; i < mutatableParams.size(); i++) {
            mutatableParams[i]->ephemeralRandom(r);
        }
        for (unsigned i = 0; i < arity; i++) {
            descendants[i]->ephemeralRandom(r);
        }
    }

    // INHERITED TRACE METHOD
    void traceSubtree(std::vector<GPNode*>* allnodes, std::vector<GPMutatableParam*>* allmutatableparams, GPNode* p, unsigned* rootHeight, unsigned currentDepth) {
        parent = p;
        depth = currentDepth;
        allnodes->push_back(this);
        for (unsigned i = 0; i < mutatableParams.size(); i++) {
            if (mutatableParams[i]->isMutatable)
                allmutatableparams->push_back(mutatableParams[i]);
        }
        if (currentDepth > *rootHeight) {
            *rootHeight = currentDepth;
        }
        for (unsigned i = 0; i < arity; i++) {
            descendants[i]->traceSubtree(allnodes, allmutatableparams, this, rootHeight, currentDepth + 1);
        }
    };
};

#endif
