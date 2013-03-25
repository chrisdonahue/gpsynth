/*
  ==============================================================================

    Main.cpp
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "GPExperiment.h"

//==============================================================================
class GeneticProgrammingSynthesizerExperiment  : public JUCEApplication
{
public:
    //==============================================================================
    GeneticProgrammingSynthesizerExperiment() {}

    const String getApplicationName()       {
        return String("GP Synthesis Expermient");
    }
    const String getApplicationVersion()    {
        return String("1.0.0");
    }
    bool moreThanOneInstanceAllowed()       {
        return true;
    }

    //==============================================================================
    void initialise (const String& commandLine)
    {
        juce::Thread::setCurrentThreadName("experiment");

        // This method is where you should put your application's initialisation code..
        StringArray args = getCommandLineParameterArray();

        // temp fields
        String target("");
        unsigned seed = time(NULL);
        std::vector<double> constants(0);

        params = (GPParams*) malloc(sizeof(GPParams));

        // auxillary params
        params->verbose = false;
        params->wavFileBufferSize = 256;
        params->renderBlockSize = 1024;
        params->fftSize = 256;
        
        // fitness function params
        params->fitnessFunctionType = 1;
        params->penalizeBadMagnitude = 2;
        params->penalizeBadPhase = 2;
        params->magnitudeWeight = 1.0;
        params->phaseWeight = 1.0;

        // experiment params
        params->experimentNumber = 0;
        params->numGenerations = 100;
        params->thresholdFitness = 0.0;
        params->ephemeralRandomConstants = true;
        params->suboptimize = false;

        // synth evolution params
        params->populationSize = 500;
        params->lowerFitnessIsBetter = true;
        params->bestPossibleFitness = 0;
        params->penaltyFitness = std::numeric_limits<float>::max();
        params->maxInitialDepth = 5;
        params->maxDepth = 12;

        // synth genetic params
        params->proportionOfPopulationForGreedySelection = 0.0;
        params->numericMutationSelectionType = 1;
        params->percentileOfPopulationToSelectFromForNumericMutation = 0.2;
        params->proportionOfPopulationToNumericallyMutate = 0.05;
        params->numericMutationTemperatureConstant = 0.02;
        params->crossoverSelectionType = 0;
        params->crossoverType = 0;
        params->proportionOfPopulationFromCrossover = 0.9;
        params->reproductionSelectionType = 0;

        // value node params
        params->valueNodeMinimum = -1.0;
        params->valueNodeMaximum = 1.0;

        // oscil node params
        params->oscilNodeMaxPartial = 10;

        // delay node
        params->delayNodeBufferMaxSeconds = 1.0;

        // filter node
        params->filterNodeCenterFrequencyMinimum = 20;
        params->filterNodeCenterFrequencyMaximum = 20000;
        params->filterNodeQualityMinimum = 0.1;
        params->filterNodeQualityMaximum = 10.0;
        params->filterNodeBandwidthMinimum = 0;
        params->filterNodeBandwidthMaximum = 10000;

        // adsr node
        params->ADSRNodeEnvelopeMin = 0.0;
        params->ADSRNodeEnvelopeMax = 1.0;

        // modulation node
        params->modulationNodeFrequencyRange = 10;
        params->numVariables = 0;

        for (String* i = args.begin(); i < args.end(); i++) {
          if (i->equalsIgnoreCase("--target")) {
            target = *(++i);
          }
          else if (i->equalsIgnoreCase("--seed")) {
            seed = (++i)->getIntValue();
          }
          else if (i->equalsIgnoreCase("--verbose")) {
            params->verbose = true;
          }
          else if (i->equalsIgnoreCase("--fft")) {
            params->fftSize = (++i)->getIntValue();
          }
          else if (i->equalsIgnoreCase("--exp")) {
            params->experimentNumber = (++i)->getIntValue();
          }
          else if (i->equalsIgnoreCase("--threshold"))  {
            params->thresholdFitness = (++i)->getDoubleValue();
          }
          else if (i->equalsIgnoreCase("--fitnesstype")) {
            params->fitnessFunctionType = (++i)->getIntValue();
          }
          else if (i->equalsIgnoreCase("--numgenerations"))  {
            params->numGenerations = (++i)->getIntValue();
          }
          else if (i->equalsIgnoreCase("--values")) {
            String* current = (++i);
            while ( (i != args.end())  && !((current + 1)->startsWith(String("--"))) ) {
                constants.push_back(current->getDoubleValue());
                current = ++i;
            }
            constants.push_back(current->getDoubleValue());
            params->numVariables = constants.size();
          }
          else if (i->equalsIgnoreCase("--popsize")) {
            params->populationSize = (++i)->getIntValue();
          }
          else if (i->equalsIgnoreCase("--mid")) {
            params->maxInitialDepth = (++i)->getIntValue();
          }
          else if (i->equalsIgnoreCase("--md")) {
            params->maxDepth = (++i)->getIntValue();
          }
          else if (i->equalsIgnoreCase("--ctype")) {
            params->crossoverType = (++i)->getIntValue();
          }
          else if (i->equalsIgnoreCase("--rselect")) {
            params->reproductionSelectionType = (++i)->getIntValue();
          }
          else if (i->equalsIgnoreCase("--cselect")) {
            params->crossoverSelectionType = (++i)->getIntValue();
          }
          else if (i->equalsIgnoreCase("--crosspercent")) {
            params->proportionOfPopulationFromCrossover = (++i)->getDoubleValue();
          }
        }

        // TODO: check all value ranges here
        if (target.equalsIgnoreCase("")) {
            std::cerr << "No target specified. Exiting application." << std::endl;
            quit();
        }

        experiment = new GPExperiment(new GPRandom(seed), target, params, constants.data());

        GPNetwork* champion = experiment->evolve();
        delete champion;
        quit();
    }

    void shutdown()
    {
        delete experiment;
        free(params);
    }

    //==============================================================================
    void systemRequestedQuit()
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const String& commandLine)
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

private:
    GPExperiment* experiment;
    GPParams* params;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (GeneticProgrammingSynthesizerExperiment)
