/*
  ==============================================================================

    GPNetwork.h
    Created: 6 Feb 2013 11:05:02am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef GPNETWORK_H
#define GPNETWORK_H

#include <iostream>
#include "GPNode.h"

class GPNetwork {
    public:
        // CONSTRUCTION
        GPNetwork(GPNode* r);
        ~GPNetwork();
        GPNetwork* getCopy();

        // EXAMINATION
        double evaluate(double* t, double* v);
        int getDepth();
        std::string toString();
        GPNode* getRoot();
        bool equals(GPNetwork* other);

        // MUTATION
        GPNode* getRandomNetworkNode(GPRandom* r);
        void mutate(GPParams* p);
        void replaceSubtree(GPNode* one, GPNode* two);
        void swap(GPNode* one, GPNode* two);
        void traceNetwork();

        // PUBLIC STATE
        int ID;
        double fitness;

    private:
        // PRIVATE STATE
        int depth;
        std::string asText;
        GPNode* root;
        std::vector<GPNode*> allNodes;
};

#endif
