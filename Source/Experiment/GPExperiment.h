

#include "../Synth/GPSynth.h"
#include "../../JuceLibraryCode/JuceHeader.h"

class GPExperiment {
    public:
        // CONSTUCTION
        GPExperiment(String target, int psize, unsigned s, bool lowerbetter, double addchance, double mutatechance, double crosschance, double threshold, int numGenerations);
        ~GPExperiment();

        // EVOLUTION CONTROL
        void evolve();

    private:
        // EXPERIMENT PARAMETERS
        float fitnessThreshold;
        int numGenerations;
        int numMinimum;

        // EXPERIMENT STATE
        GPSynth* synth;
        float minFitnessAchieved;
        int currentGeneration;

        // TARGET DATA CONTAINERS
        unsigned numTargetFrames;
        float** targetFrames;

        // EVOLUTION CONTOL
        String evolve(unsigned numFrames, float** targetData);

        // WAV INTERFACE
        float** loadWavFile(String path);
        unsigned numFramesInWavFile(String path);
        void saveWavFile(String path, String metadata, float** data);

        // FITNESS FUNCTION
        float** evaluateIndividual(GPNetwork* candidate, unsigned numFrames);
        double compare(float** dataone, float** datatwo);
};
