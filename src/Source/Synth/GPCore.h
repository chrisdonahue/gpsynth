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
float add(GPNode& left, GPNode& right);
float subtract(GPNode& left, GPNode& right);
float multiply(GPNode& left, GPNode& right);
float modulo(GPNode& left, GPNode& right);
float divide(GPNode& left, GPNode& right);
float pow(GPNode& left, GPNode& right);
float sine(GPNode& left, GPNode& right);
float cosine(GPNode& left, GPNode& right);

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
