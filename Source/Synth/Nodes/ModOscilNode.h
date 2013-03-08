/*
  ==============================================================================

    ModOscilNode.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef MODOSCILNODE_H
#define MODOSCILNODE_H

#include <cmath>
#include "../GPNode.h"

class ModOscilNode: public GPNode {
    public:
        ModOscilNode(int p, GPNode* l, GPNode* r);
        ~ModOscilNode();
        ModOscilNode* getCopy();

        double evaluate(double* t, double* v);
        std::string toString();
        void mutate(GPParams* e);

    private:
        int partial;
        double w;
};

#endif
