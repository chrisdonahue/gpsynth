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
        GPNetwork(int id, GPNode* r);
        ~GPNetwork();
        GPNetwork* getCopy();

        // EXAMINATION
        double evaluate(double* t, double* v);
        std::string toString();
        GPNode* getRoot();

        // MUTATION
        void mutateAddNode(GPNodeParams* p, GPNode* newnode);
        void mutate(GPNodeParams* p);
        void replaceSubtree(GPNode* one, GPNode* two);
        void swap(GPNode* one, GPNode* two);
        void traceNetwork();

        // PUBLIC STATE
        int ID;

    private:
        // PRIVATE STATE
        std::string asText;
        GPNode* root;
        std::vector<GPNode*> allNodes;
};

#endif
