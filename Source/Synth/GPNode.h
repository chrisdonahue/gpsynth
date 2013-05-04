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
        : parent(NULL), descendants(0), descendantBuffers(0),
          depth(-1),
          mutatableParams(0)
    {
    }
    virtual ~GPNode() {
        // if we never set render info...
        if (descendantBuffers.size() > 0) {
            for (unsigned i = 0; i < arity - 1; i++) {
                free(descendantBuffers[i]);
            }
        }
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
	virtual void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) = 0;
    virtual void updateMutatedParams() = 0;
    virtual void toString(bool printRange, std::stringstream& ss) = 0;

    // LINEAGE POINTERS
    GPNode* parent;
    std::vector<GPNode*> descendants;
    std::vector<float*> descendantBuffers;

    // TREE STATE
    int depth;
    unsigned arity;

    // MUTATABLE PARAMS
    std::vector<GPMutatableParam*> mutatableParams;
    float minimum;
    float maximum;

    // OVERRIDABLE FUNCTIONS
    virtual void setRenderInfo(float sr, unsigned blockSize, float maxTime) {
        // if descendant buffers have ever been allocated before free them
        for (unsigned i = 0; i < descendantBuffers.size(); i++) {
            free(descendantBuffers[i]);
        }

        // allocate some new buffers in case blockSize changed
        descendantBuffers.resize(arity == 0 ? 0 : arity - 1, NULL);
        for (unsigned i = 0; i < descendantBuffers.size(); i++) {
            descendantBuffers[i] = (float*) malloc(sizeof(float) * blockSize);
        }

        // recursively execute
        for (unsigned i = 0; i < arity; i++) {
            descendants[i]->setRenderInfo(sr, blockSize, maxTime);
        }
    }

    // gets the range of output values for this node
    virtual void getRange(float* min, float* max) {
        *min = minimum;
        *max = maximum;
    }

    // NON-OVERRIDABLE FUNCTIONS
    // propogate ephemeral random constants
    void ephemeralRandom(GPRandom* r) {
        for (unsigned i = 0; i < mutatableParams.size(); i++) {
            mutatableParams[i]->ephemeralRandom(r);
        }
        for (unsigned i = 0; i < arity; i++) {
            descendants[i]->ephemeralRandom(r);
        }
    }

    // this trace method ensures that I only have assign descendant pointers correctly during genetic operations
    void trace(std::vector<GPNode*>* allnodes, std::vector<GPMutatableParam*>* allmutatableparams, GPNode* p, int* treeHeight, int currentDepth) {
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

    std::string toString(bool printRange, unsigned precision) {
        std::stringstream ss;
        ss.precision(precision);
        toString(printRange, ss);
        return ss.str();
    }
};

#endif
