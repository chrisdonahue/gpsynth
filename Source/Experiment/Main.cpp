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
        // This method is where you should put your application's initialisation code..
        StringArray args = getCommandLineParameterArray();

        String target("");
        unsigned seed = time(NULL);
        GPParams* params = (GPParams*) malloc(sizeof(GPParams));
        params->wavFileBufferSize = 256;
        params->evaluationBlockSize = 1024;
        params->fftSize = 16;
        params->verbose = false;
        params->experimentNumber = 0;
        params->numGenerations = 100;
        params->thresholdFitness = 0.0;
        params->fitnessFunctionType = 0;
        params->populationSize = 500;
        params->maxInitialDepth = 6;
        params->maxDepth = 17;
        params->mutationDuringInitializationChance = 0.5;
        params->proportionOfPopulationFromCrossover = 0.9;
        params->reproductionSelectionType = 0;
        params->crossoverType = 0;
        params->crossoverSelectionType = 0;
        params->proportionOfPopulationForGreedySelection = 0.0;
        params->mutationDuringCrossoverChance = 0.1;
        params->variableValues = new std::vector<double>();
        params->modulationNodeFrequencyRange = 10;
        params->oscilNodePartialChance = 0.5;
        params->valueNodeMinimum = -1.0;
        params->valueNodeMaximum = 1.0;
        params->delayNodeBufferMaxSeconds = 0.1;
        params->filterNodeBufferMaxSeconds = 0.1;

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
                params->variableValues->push_back(current->getDoubleValue());
                current = ++i;
            }
            params->variableValues->push_back(current->getDoubleValue());
            params->numVariables = params->variableValues->size();
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
          else if (i->equalsIgnoreCase("--mutatechance"))  {
            params->mutationDuringCrossoverChance = (++i)->getDoubleValue();
          }
        }
        params->rng = new GPRandom(seed);

        // TODO: check all value ranges here
        if (target.equalsIgnoreCase("")) {
            std::cerr << "No target specified. Exiting application." << std::endl;
            quit();
        }

        juce::Thread::setCurrentThreadName("experiment");
        experiment = new GPExperiment(target, params);
        GPNetwork* champion = experiment->evolve();
        shutdown();
        quit();
    }

    void shutdown()
    {
        experiment = 0;// = nullptr;
    }

    //==============================================================================
    void systemRequestedQuit()
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        shutdown();
        quit();
    }

    void anotherInstanceStarted (const String& commandLine)
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

private:
    ScopedPointer<GPExperiment> experiment;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (GeneticProgrammingSynthesizerExperiment)
