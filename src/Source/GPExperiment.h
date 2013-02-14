/*
  ==============================================================================

    GPExperiment.h
    Created: 6 Feb 2013 7:19:38pm
    Author:  cdonahue

  ==============================================================================
*/

#include <vector>
#include <map>

class GPSynth {
    public:
        GPSynth(int psize);

        // experiment probabilities
        float mutationChance;
        float simplifyChance;
        float specialChance;
        float harmonyChance;
        float functionChance;
        float crossChance;

    private:
        // experiment state
        int populationSize;
        int nextNetworkID;
        int generationID;
        double maxFitness;

        // experiment parameters
        int valueRange;
        int LFORange;
        int numPartials;

        // network containers
        std::vector<GPNetwork*> allNetworks;
        std::map<GPNetwork*, double> upForEvaluation;
        std::vector<GPNetwork*> evaluated;

}
