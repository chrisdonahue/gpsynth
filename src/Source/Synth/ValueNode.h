/*
  ==============================================================================

    ValueNode.h
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#include <stdio.h>
#include "GPNode.h"

class ValueNode: public GPNode {
    public:
        ValueNode(double* v, bool t=false, bool f=false);
        ~ValueNode();
        ValueNode* getCopy();

        double evaluate(double* t, float* f);
        std::string toString();

    private:
        double* value;
        bool isTime;
        bool isFreq;
};
