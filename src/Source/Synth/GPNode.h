/*
  ==============================================================================

    GPNode.h
    Created: 6 Feb 2013 11:06:00am
    Author:  cdonahue

  ==============================================================================
*/

#include <string>

// forward declarator for eventual mutate function
class GPExperiment;

class GPNode {
    public:
        virtual double evaluate(double* t, float* f) = 0;
        virtual std::string toString() = 0;
        virtual GPNode* getCopy() = 0;
        //virtual void mutate(GPExperiment* e) = 0;

    protected:
        GPNode* parent;
        GPNode* left;
        GPNode* right;
};
