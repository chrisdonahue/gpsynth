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
#include "../GPNode.h"

class ValueNode: public GPNode {
    public:
        ValueNode(double* v, int p);
        ~ValueNode();
        ValueNode* getCopy();

        double evaluate(double* t, double* v);
        std::string toString();
        void mutate(GPNodeParams* e);

    private:
        double* value;
        bool isTime;
        int variableNum;
};

#endif
