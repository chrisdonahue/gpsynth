/*
  ==============================================================================

    Main.cpp
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "GPStandaloneDialog.h"

//==============================================================================
class InteractiveGeneticProgrammingSynthesis  : public JUCEApplication
{
public:
    //==============================================================================
    InteractiveGeneticProgrammingSynthesis() {}

    const String getApplicationName()       {
        return String("Interactive Genetic Programming Synthesis");
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

        // temp fields
        unsigned seed = time(NULL);
        std::vector<double> constants(0);

        params = (GPParams*) malloc(sizeof(GPParams));

        // auxillary params
        params->verbose = false;
        params->printPrecision = 3;
        params->backupPrecision = 100;
        params->savePrecision = 4;
        params->wavFileBufferSize = 256;
        params->renderBlockSize = 256;
        params->fftSize = 256;

        // fitness function params
        params->fitnessFunctionType = 1;
        //params->penalizeBadMagnitude = 2;
        params->penalizeBadPhase = 2;
        params->magnitudeWeight = 1.0;
        params->phaseWeight = 0.1;
        params->goodComparisonFactor = 0.1;
        params->badComparisonFactor = 1.1;
        params->baseComparisonFactor = 0.9;

        // experiment params
        params->experimentNumber = 0;
        params->envelopeIterations = 0;
        params->numGenerations = 100;
        params->thresholdFitness = 0.0;
        params->ephemeralRandomConstants = true;

        // synth evolution params
        params->backupAllNetworks = false;
        params->populationSize = 500;
        params->lowerFitnessIsBetter = true;
        params->bestPossibleFitness = 0;
        params->maxInitialDepth = 5;
        params->maxDepth = 12;

        // synth genetic params
        params->proportionOfPopulationForGreedySelection = 0.0;
        // numeric mutation
        params->numericMutationSelectionType = 1;
        params->proportionOfPopulationFromNumericMutation = 0.05;
        params->percentileOfPopulationToSelectFromForNumericMutation = 0.05;
        params->numericMutationTemperatureConstant = 0.02;
        // mutation
        params->mutationSelectionType = 1;
        params->mutationType = 0;
        params->proportionOfPopulationFromMutation = 0.05;
        params->percentileOfPopulationToSelectFromForMutation = 0.05;
        // crossover
        params->crossoverSelectionType = 0;
        params->crossoverType = 0;
        params->proportionOfPopulationFromCrossover = 0.9;
        // reproduction
        params->reproductionSelectionType = 0;
        params->proportionOfPopulationFromReproduction = 0.05;

        // value node params
        params->valueNodeMinimum = -1.0;
        params->valueNodeMaximum = 1.0;

        // oscil node params
        params->oscilNodeMaxPartial = 10;
        params->oscilNodeMinIndexOfModulation = 0.0;
        params->oscilNodeMaxIndexOfModulation = 2.0;

        // delay node
        params->delayNodeBufferMaxSeconds = 1.0;

        // filter node
        params->filterNodeCenterFrequencyMinimum = 20;
        params->filterNodeCenterFrequencyMaximumProportionOfNyquist = 0.9999;
        params->filterNodeQualityMinimum = 0.1;
        params->filterNodeQualityMaximum = 10.0;
        params->filterNodeBandwidthMinimum = 10;
        params->filterNodeBandwidthMaximum = 2000;

        // adsr node
        params->ADSRNodeEnvelopeMin = 0.0;
        params->ADSRNodeEnvelopeMax = 1.0;

        // modulation node
        params->modulationNodeFrequencyRange = 10;
        params->numVariables = 0;

        for (String* i = args.begin(); i < args.end(); i++) {
            if (i->equalsIgnoreCase("--seed")) {
                seed = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--verbose")) {
                params->verbose = true;
            }
            else if (i->equalsIgnoreCase("--precision")) {
                params->printPrecision = (++i)->getIntValue();
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
            else if (i->equalsIgnoreCase("--fft")) {
                params->fftSize = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--render")) {
                params->renderBlockSize = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--fitnesstype")) {
                params->fitnessFunctionType = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--experiment")) {
                params->experimentNumber = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--enviter")) {
                params->envelopeIterations = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--generations"))  {
                params->numGenerations = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--threshold"))  {
                params->thresholdFitness = (++i)->getDoubleValue();
            }
            else if (i->equalsIgnoreCase("--erc")) {
                params->ephemeralRandomConstants = true;
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
            else if (i->equalsIgnoreCase("--numericmutation")) {
                params->proportionOfPopulationFromNumericMutation = (++i)->getDoubleValue();
            }
            else if (i->equalsIgnoreCase("--nmselect")) {
                params->numericMutationSelectionType = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--nmselectparam")) {
                params->percentileOfPopulationToSelectFromForNumericMutation = (++i)->getDoubleValue();
            }
            else if (i->equalsIgnoreCase("--nmtemperature")) {
                params->numericMutationTemperatureConstant = (++i)->getDoubleValue();
            }
            else if (i->equalsIgnoreCase("--mutation")) {
                params->proportionOfPopulationFromMutation = (++i)->getDoubleValue();
            }
            else if (i->equalsIgnoreCase("--mselect")) {
                params->mutationSelectionType = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--mtype")) {
                params->mutationType = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--mselectparam")) {
                params->percentileOfPopulationToSelectFromForMutation = (++i)->getDoubleValue();
            }
            else if (i->equalsIgnoreCase("--crossover")) {
                params->proportionOfPopulationFromCrossover = (++i)->getDoubleValue();
            }
            else if (i->equalsIgnoreCase("--cselect")) {
                params->crossoverSelectionType = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--ctype")) {
                params->crossoverType = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--reproduction")) {
                params->proportionOfPopulationFromReproduction = (++i)->getDoubleValue();
            }
            else if (i->equalsIgnoreCase("--rselect")) {
                params->reproductionSelectionType = (++i)->getIntValue();
            }
        }

	// TODO: check all value ranges here

        pluginWindow = new StandaloneFilterWindow (String("Interactive GP Synthesis"), Colours::black, params);
        //filterWindow->setTitleBarButtonsRequired (DocumentWindow::allButtons, false);
        pluginWindow->setVisible (true);
        //filterWindow->setResizable (true, true);
        juce::Thread::setCurrentThreadName("interactivegp");
    }

    void shutdown()
    {
        pluginWindow = 0;// = nullptr;
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
    ScopedPointer<StandaloneFilterWindow> pluginWindow;
    ScopedPointer<GPParams> params;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (InteractiveGeneticProgrammingSynthesis)
