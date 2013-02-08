/*
  ==============================================================================

    GPCore.h
    Created: 6 Feb 2013 11:05:44am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef __GPCORE_H_E81233F7__
#define __GPCORE_H_E81233F7__

#include <string>
#include <vector>
#include <set>
#include <cmath>
#include "GPNode.h"

class GPInfo {
    public:
        // IDs
        int nextNetworkID;

        // special node values
        float *time;
        float *cps;

        // chance params
        float mutationChance;
        float simplifyChance;
        float specialChance;
        float harmonyChance;
        float functionChance;
        float crossChance;
};

//available network functions
float add(GPNode* left, GPNode* right) {
    return left->evaluate() + right->evaluate();
}
float subtract(GPNode* left, GPNode* right) {
    return left->evaluate() - right->evaluate();
}
float multiply(GPNode* left, GPNode* right) {
    return left->evaluate() * right->evaluate();
}
float modulo(GPNode* left, GPNode* right) {
    int mod = int(right->evaluate());
    if (mod > 0) {
        return fmod(left->evaluate(), mod);
    }
    else {
        return left->evaluate();
    }
}
float divide(GPNode* left, GPNode* right) {
    float denom = right->evaluate();
    if (denom != 0) {
        return float(left->evaluate()) / denom;
    }
    else {
        return 0.0;
    }
}
float pow(GPNode* left, GPNode* right) {
    float l = left->evaluate();
    float r = right->evaluate();
    if (l < 0) {
        r = int(r);
    }
    return pow(l, r);
}
float sine(GPNode* left, GPNode* right) {
    return sin(left->evaluate());
}
float cosine(GPNode* left, GPNode* right) {
    return cos(left->evaluate());
}

/*
// parameters
int populationSize;
int valueRange;
int LFORange;
int numPartials;
 
// IDs
int nextGenerationID;

// network containers
std::vector<GPNetwork&> networks;
std::vector<GPNetwork&> currentlyEvaluating;
std::set<GPNetwork&> evaluated;
std::set<GPNetwork&> selected;
*/

#endif  // __GPCORE_H_E81233F7__
