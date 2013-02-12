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

class GPInfo {
    public:
        // IDs
        int nextNetworkID;

        // special node values
        double *time;
        float *cps;

        // chance params
        float mutationChance;
        float simplifyChance;
        float specialChance;
        float harmonyChance;
        float functionChance;
        float crossChance;
};

#endif  // __GPCORE_H_E81233F7__
