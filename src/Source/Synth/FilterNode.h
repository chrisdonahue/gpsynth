/*
  ==============================================================================

    FilterNode.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#include <stdio.h>
#include "GPNode.h"

class FilterNode: public GPNode {
    public:
        FilterNode(int x, int y, GPNode* l, GPNode* r);
        ~FilterNode();
        FilterNode* getCopy();

        void setMemoryConstants(int x, int y);
        double evaluate(double* t, float* f);
        std::string toString();
        void traceLineage();

    private:
        int numX;
        int numY;
        int numFilled;
        double* xMem;
        double* yMem;
};
