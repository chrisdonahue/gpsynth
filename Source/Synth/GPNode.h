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
          mutatableParams(0), minimum((-1) * std::numeric_limits<float>::infinity()), maximum(std::numeric_limits<float>::infinity()),
          renderInfoSet(false), preparedToRender(false)
    {
    }
    virtual ~GPNode() {
        // if we prepared this subtree for rendering...
        if (preparedToRender) {
            unsigned numDescendantBuffers = arity == 0 ? 0 : arity - 1;
            for (unsigned i = 0; i < numDescendantBuffers; i++) {
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
	virtual void evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) = 0;
    virtual void toString(std::stringstream& ss) = 0;

    // LINEAGE POINTERS
    GPNode* parent;
    std::vector<GPNode*> descendants;
    std::vector<float*> descendantBuffers;

    // TREE STATE
    int depth;
    unsigned arity;

    // MUTATABLE PARAM RELATED
    std::vector<GPMutatableParam*> mutatableParams;
    float minimum;
    float maximum;

    // READY TO RENDER
    bool renderInfoSet;
    bool preparedToRender;

    // OVERRIDABLE FUNCTIONS
    virtual void prepareToPlay() {
    }

    virtual void setRenderInfo(float sr, unsigned blockSize, unsigned maxFrameNumber, float maxTime) {
        // clear out old render info if it exists
        doneRendering();

        // allocate some new buffers in case blockSize changed
        unsigned numDescendantBuffers = arity == 0 ? 0 : arity - 1;
        descendantBuffers.resize(numDescendantBuffers, NULL);
        for (unsigned i = 0; i < numDescendantBuffers; i++) {
            descendantBuffers[i] = (float*) malloc(sizeof(float) * blockSize);
        }

        // recursively execute
        for (unsigned i = 0; i < arity; i++) {
            descendants[i]->setRenderInfo(sr, blockSize, maxFrameNumber, maxTime);
        }

        renderInfoSet = true;
    }

    virtual void doneRendering() {
        if (renderInfoSet) {
            unsigned numDescendantBuffers = arity == 0 ? 0 : arity - 1;
            for (unsigned i = 0; i < numDescendantBuffers; i++) {
                free(descendantBuffers[i]);
            }

            for (unsigned i = 0; i < arity; i++) {
                descendants[i]->doneRendering();
            }
            
            renderInfoSet = false;
            preparedToRender = false;
        }
    }

    virtual void updateMutatedParams() {
        assert(renderInfoSet == true);
        for (unsigned i = 0; i < arity; i++) {
            descendants[i]->updateMutatedParams();
        }
        preparedToRender = true;
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

    std::string toString(unsigned precision) {
        std::stringstream ss;
        ss.precision(precision);
        toString(ss);
        return ss.str();
    }
};

#endif
