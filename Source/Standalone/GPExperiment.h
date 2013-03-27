/*
  ==============================================================================

    GPExperiment.h
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef GPEXPERIMENT_H
#define GPEXPERIMENT_H

#include "../Common/GPFunctions.h"
#include "../Common/GPParams.h"
#include "../Synth/GPSynth.h"
#include "../../JuceLibraryCode/JuceHeader.h"
#include "SimpleInteractiveGUI.h"
#include "../Synth/Nodes/ValueNode.h"
#include "../Synth/Nodes/FunctionNode.h"
#include "../Synth/Nodes/OscilNode.h"
//#include "../Nodes/DelayNode.h"
//#include "../Nodes/FilterNode.h"

class GPExperiment {
    public:
        // CONSTUCTION
        GPExperiment(String target, GPParams* p);
        ~GPExperiment();

        // EVOLUTION CONTROL
        GPNetwork* evolve();

    private:
        // EXPERIMENT PARAMETERS
        GPParams* params;
        float fitnessThreshold;
        int numGenerations;
        bool lowerFitnessIsBetter;
        double bestPossibleFitness;

        // EVALUATION DATA
        double* sampleTimes;
        double* specialValues;

        // EXPERIMENT STATE
        float minFitnessAchieved;
        int numEvaluatedGenerations;

        // SYNTH
        GPSynth* synth;

        // WAV INTERFACE
        ScopedPointer<WavAudioFormat> wavFormat;
        unsigned wavFileBufferSize;
        void saveWavFile(String path, String metadata, unsigned numFrames, float* data);

        // FITNESS FUNCTION
        float* evaluateIndividual(GPNetwork* candidate);
        double interactiveFitness(GPNetwork* candidate);
        double compareToTarget(float* candidateFrames);
};

#endif
