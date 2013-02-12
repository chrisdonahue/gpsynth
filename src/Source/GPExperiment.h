/*
  ==============================================================================

    GPExperiment.h
    Created: 6 Feb 2013 7:19:38pm
    Author:  cdonahue

  ==============================================================================
*/

#ifndef __GPEXPERIMENT_H_93920796__
#define __GPEXPERIMENT_H_93920796__

#include <vector>
#include <map>

class GPSynth {
    // experiment state
    int populationSize;
    int nextNetworkID;
    int generationID;

    // experiment parameters
    int valueRange;
    int LFORange;
    int numPartials;

    // network containers
    std::vector<GPNetwork*> allNetworks;
    std::vector<GPNetwork*> upForEvaluation;
    std::map<GPNetwork*, double> evaluated;

    // experiment probabilities
    float mutationChance;
    float simplifyChance;
    float specialChance;
    float harmonyChance;
    float functionChance;
    float crossChance;
}

#endif  // __GPEXPERIMENT_H_93920796__
