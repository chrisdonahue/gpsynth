/*
  ==============================================================================

    FilterNode.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef FILTERNODE_H
#define FILTERNODE_H

#include "../GPNode.h"
#include <list>

class FilterNode: public GPNode {
    public:
        FilterNode(int x, int y, GPNode* l, GPNode* r);
        ~FilterNode();
        FilterNode* getCopy();

        void setMemoryConstants(int x, int y);
        double evaluate(double* t, double* v);
        std::string toString();
        void mutate(GPParams* e);

    private:
        int numFilled;

        int numX;
        std::list<double> xMem;
        double* xCoefficients;

        int numY;
        std::list<double> yMem;
        double* yCoefficients;
};

#endif
