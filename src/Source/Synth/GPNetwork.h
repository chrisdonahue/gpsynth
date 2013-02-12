/*
  ==============================================================================

    GPNetwork.h
    Created: 6 Feb 2013 11:05:02am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef __GPNETWORK_H_3A9005CA__
#define __GPNETWORK_H_3A9005CA__

#include <stdio>
#include "GPNode.h"

class GPNetwork {
    public:
        GPNetwork(int id, GPNode* r);
        ~GPNetwork();
        GPNetwork* getCopy();

        GPNode* getRoot();
        double evaluate(double* t, float* f);
        std::string toString();
        void replaceSubtree(GPNode* one, GPNode* two);
        void swap(GPNode* one, GPNode* two);

    private:
        int ID;
        std::string asText;
        GPNode* root;

};

#endif  // __GPNETWORK_H_3A9005CA__
