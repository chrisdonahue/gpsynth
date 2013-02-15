/*
  ==============================================================================

    OscilNode.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef OSCILNODE_H
#define OSCILNODE_H

#include <stdio.h>
#include <cmath>
#include "GPNode.h"

class OscilNode: public GPNode {
    public:
        OscilNode(int p, GPNode* l, GPNode* r);
        ~OscilNode();
        OscilNode* getCopy();

        double evaluate(double* t, double* v);
        std::string toString();
        void mutate(GPParams* e);

    private:
        int partial;
        double w;
};

#endif
