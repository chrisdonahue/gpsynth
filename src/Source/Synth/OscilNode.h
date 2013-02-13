/*
  ==============================================================================

    OscilNode.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#include <stdio.h>
#include "GPNode.h"

class OscilNode: public GPNode {
    public:
        OscilNode(GPNode* l, GPNode* r);
        ~OscilNode();
        OscilNode* getCopy();

        double evaluate(double* t, float* f);
        std::string toString();
        void traceLineage();

    private:
        int partial;
        double w;
};
