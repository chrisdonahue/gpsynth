/*
  ==============================================================================

    GPNetwork.h
    Created: 6 Feb 2013 11:05:02am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef __GPNETWORK_H_3A9005CA__
#define __GPNETWORK_H_3A9005CA__

#include "GPCore.h"
#include "GPNode.h"

class GPNetwork {
    public:
        GPNetwork(GPInfo* i, GPNode* r);
        ~GPNetwork();
        GPNetwork* getCopy();

        GPNode* getRoot();
        double evaluate(double* t, float* f);
        std::string toString();
        void mutate();
        GPNetwork* reproduce(GPNetwork* partner);

    private:
        int networkID;
        std::string asText;
        GPNode* root;
        GPInfo* info;

        void swap(GPNode* one, GPNode* two);
};

#endif  // __GPNETWORK_H_3A9005CA__
