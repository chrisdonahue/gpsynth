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
        : parent(NULL), descendants(0),
          depth(-1),
          mutatableParams(0)
    {
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
    virtual GPNode* getCopy() = 0;
    virtual void prepareToPlay() = 0;
    virtual void evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, double* min, double* max, unsigned n, float* buffer) = 0;
	virtual void evaluateBlockPerformance(unsigned fn, float* t, unsigned nv, float* v, float* min, float* max, unsigned n, float* buffer) = 0;
    virtual void getRange(float* min, float* max) = 0;
    virtual void updateMutatedParams() = 0;
    virtual void toString(bool printRange, std::stringstream& ss) = 0;

    // LINEAGE POINTERS
    GPNode* parent;
    std::vector<GPNode*> descendants;

    // TREE STATE
    int depth;
    unsigned arity;

    // MUTATABLE PARAMS
    std::vector<GPMutatableParam*> mutatableParams;

    // PROPOGATE EPHEMERAL RANDOM CONSTANTS
    void ephemeralRandom(GPRandom* r) {
        for (unsigned i = 0; i < mutatableParams.size(); i++) {
            mutatableParams[i]->ephemeralRandom(r);
        }
        for (unsigned i = 0; i < arity; i++) {
            descendants[i]->ephemeralRandom(r);
        }
    };

    // INHERITED TRACE METHOD
    // this method ensures that I only have assign descendant pointers correctly during genetic operations
    void trace(std::vector<GPNode*>* allnodes, std::vector<GPMutatableParam*>* allmutatableparams, GPNode* p, unsigned* treeHeight, int currentDepth) {
        // assign parent
        parent = p;

        // assign depth
        depth = currentDepth;

        // add this node to the collection of all nodes
        allnodes->push_back(this);
        
        // add this nodes mutatable params to the collection of all mutatable params
        for (unsigned i = 0; i < mutatableParams.size(); i++) {
            if (mutatableParams[i]->isMutatable)
                allmutatableparams->push_back(mutatableParams[i]);
        }
        
        // update tree height if necessary
        if (currentDepth > *treeHeight) {
            *treeHeight = currentDepth;
        }

        // trace the rest of the tree
        for (unsigned i = 0; i < arity; i++) {
            descendants[i]->trace(allnodes, allmutatableparams, this, treeHeight, currentDepth + 1);
        }
    };
};

#endif
