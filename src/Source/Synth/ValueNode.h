/*
  ==============================================================================

    ValueNode.h
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef VALUENODE_H
#define VALUENODE_H

#include <stdio.h>
#include "GPNode.h"

class ValueNode: public GPNode {
    public:
        ValueNode(double* v, bool t=false, bool f=false);
        ~ValueNode();
        ValueNode* getCopy();

        double evaluate(double* t, float* f);
        std::string toString();
        void mutate(GPExperiment* e);

    private:
        double* value;
        bool isTime;
        bool isFreq;
};

#endif
