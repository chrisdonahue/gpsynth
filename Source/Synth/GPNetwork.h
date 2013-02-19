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
#include <stdio.h>
#include "GPNode.h"

class GPNetwork {
    public:
        GPNetwork(int id, GPNode* r);
        ~GPNetwork();
        GPNetwork* getCopy();
        GPNode* getRoot();

        double evaluate(double* t, double* v);
        std::string toString();

        void mutateAddNode(GPNodeParams* p);
        void mutate(GPNodeParams* p);
        void replaceSubtree(GPNode* one, GPNode* two);
        void swap(GPNode* one, GPNode* two);

        int ID;

    private:
        std::string asText;
        GPNode* root;
        std::vector<GPNode*> allNodes;

        void traceNetwork();
};

#endif
