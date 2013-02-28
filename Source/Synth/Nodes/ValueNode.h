/*
  ==============================================================================

    ValueNode.h
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef VALUENODE_H
#define VALUENODE_H

#include "../GPNode.h"

class ValueNode: public GPNode {
    public:
        ValueNode(double v, int p);
        ~ValueNode();
        ValueNode* getCopy();

        double evaluate(double* t, double* v);
        std::string toString();
        bool equals(GPNode* other);
        void mutate(GPParams* e);

    private:
        double value;
        bool isTime;
        bool isVariable;
        bool isConstant;
        int variableNum;
};

#endif
