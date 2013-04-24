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
        return String("GP Synthesis Experiment");
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
        String saveFilesTo("./");
        unsigned seed = time(NULL);
        std::vector<double> constants(0);
        bool printExperimentInfo = false;

        // init params
        params = (GPParams*) malloc(sizeof(GPParams));

        // experiment params
        params->experimentNumber = 0;
        params->fitnessFunctionType = 1;
        params->numGenerations = 100;
        params->thresholdFitness = 0.0;
        params->ephemeralRandomConstants = true;

        // auxillary params
        params->verbose = false;
        params->saveGenerationChampions = false;
        params->backupTarget = false;
        params->printPrecision = 3;
        params->savePrecision = 4;
        params->wavFileBufferSize = 256;
        params->renderBlockSize = 256;

        // fitness function weights
        params->magnitudeWeight = 1.0;
        params->phaseWeight = 0.1;
        params->envelopeWeight = 0.5;

        // time domain fitness parameters
        params->saveTargetEnvelope = false;
        params->envelopeFollowerAttack = 1;
        params->envelopeFollowerDecay = 300;
        params->compareEnvelopeNumSkipFrames = 15;

        // frequency domain fitness parametersi
        strncpy(params->windowType, "rect", 5);
        params->fftSize = 256;
        params->fftOverlap = 0;
        params->dBMagnitude = false;
        // moving average
        params->averageComparisonType = 1;
        params->movingAveragePastRadius = 50;
        params->movingAverageFutureRadius = 50;
        params->exponentialMovingAverageAlpha = 0.2;
        params->saveTargetSpectrum = false;
        params->compareToMaxDeviation = true;
        params->logPenaltyComparison = false;
        params->weightFftFrames = false;
        params->frameWeightExponent = 0.5;
        // phase penalty
        params->penalizeBadPhase = 2;
        // magnitude penalty
        params->goodComparisonFactor = 0.1;
        params->badComparisonFactor = 1.1;
        params->baseComparisonFactor = 0.9;

        // synth evolution params
        params->populationSize = 500;
        params->backupAllNetworks = false;
        params->backupPrecision = 100;
        //params->lowerFitnessIsBetter = true; should be done in experiment
        //params->bestPossibleFitness = 0; should be done in experiment
        //params->penaltyFitness = std::numeric_limits<float>::max(); experiment
        params->maxInitialDepth = 5;
        params->maxDepth = 12;

        // synth genetic params
        params->proportionOfPopulationForGreedySelection = 0.0;
        // numeric mutation
        params->numericMutationSelectionType = 1;
        params->proportionOfPopulationFromNumericMutation = 0.05;
        params->percentileOfPopulationToSelectFromForNumericMutation = 0.05;
        params->numericMutationTemperatureConstant = 0.9;
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
        params->oscilNodeMaxPartial = 30;
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
            // temp fields
            if (i->equalsIgnoreCase("--target")) {
                target = *(++i);
            }
            else if (i->equalsIgnoreCase("--path")) {
                saveFilesTo = *(++i);
            }
            else if (i->equalsIgnoreCase("--seed")) {
                seed = (++i)->getIntValue();
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
            else if (i->equalsIgnoreCase("--expinfo")) {
                printExperimentInfo = true;
            }

            // experiment params
            else if (i->equalsIgnoreCase("--experiment")) {
                params->experimentNumber = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--fitnesstype")) {
                params->fitnessFunctionType = (++i)->getIntValue();
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
            else if (i->equalsIgnoreCase("--noerc")) {
                params->ephemeralRandomConstants = false;
            }

            // auxillary params
            else if (i->equalsIgnoreCase("--verbose")) {
                params->verbose = true;
            }
            else if (i->equalsIgnoreCase("--succinct")) {
                params->verbose = false;
            }
            else if (i->equalsIgnoreCase("--savegenchamps")) {
                params->saveGenerationChampions = true;
            }
            else if (i->equalsIgnoreCase("--nosavegenchamps")) {
                params->saveGenerationChampions = false;
            }
            else if (i->equalsIgnoreCase("--backuptarget")) {
                params->backupTarget = true;
            }
            else if (i->equalsIgnoreCase("--nobackuptarget")) {
                params->backupTarget = false;
            }
            else if (i->equalsIgnoreCase("--printprecision")) {
                params->printPrecision = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--saveprecision")) {
                params->savePrecision = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--loadwavblock")) {
                params->wavFileBufferSize = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--render")) {
                params->renderBlockSize = (++i)->getIntValue();
            }

            // fitness function weights
            else if (i->equalsIgnoreCase("--ffmagweight"))  {
                params->magnitudeWeight = (++i)->getDoubleValue();
            }
            else if (i->equalsIgnoreCase("--ffphaseweight"))  {
                params->phaseWeight = (++i)->getDoubleValue();
            }
            else if (i->equalsIgnoreCase("--ffenvweight"))  {
                params->envelopeWeight = (++i)->getDoubleValue();
            }

            // time domain fitness params
            else if (i->equalsIgnoreCase("--savetargetenv")) {
                params->saveTargetEnvelope = true;
            }
            else if (i->equalsIgnoreCase("--nosavetargetenv")) {
                params->saveTargetEnvelope = false;
            }
            else if (i->equalsIgnoreCase("--envatk"))  {
                params->envelopeFollowerAttack = (++i)->getDoubleValue();
            }
            else if (i->equalsIgnoreCase("--envdcy"))  {
                params->envelopeFollowerDecay = (++i)->getDoubleValue();
            }
            else if (i->equalsIgnoreCase("--envskip")) {
                params->compareEnvelopeNumSkipFrames = (++i)->getIntValue();
            }

            // frequency domain fitness params
            else if (i->equalsIgnoreCase("--window"))  {
                String* window = ++i;
                if (window->equalsIgnoreCase("rect")) {
                    strncpy(params->windowType, "rect", 5);
                }
                else if (window->equalsIgnoreCase("hann")) {
                    strncpy(params->windowType, "hann", 5);
                }
            }
            else if (i->equalsIgnoreCase("--fft")) {
                params->fftSize = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--fftoverlap")) {
                params->fftOverlap = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--dbmag")) {
                params->dBMagnitude = true;
            }
            else if (i->equalsIgnoreCase("--ampmag")) {
                params->dBMagnitude = false;
            }
            else if (i->equalsIgnoreCase("--averagetype")) {
                params->averageComparisonType = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--averagepastradius")) {
                params->movingAveragePastRadius = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--averagefutureradius")) {
                params->movingAverageFutureRadius = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--alpha"))  {
                params->exponentialMovingAverageAlpha = (++i)->getDoubleValue();
            }
            else if (i->equalsIgnoreCase("--savetargetspectrum")) {
                params->saveTargetSpectrum = true;
            }
            else if (i->equalsIgnoreCase("--nosavetargetspectrum")) {
                params->saveTargetSpectrum = false;
            }
            else if (i->equalsIgnoreCase("--comparedev")) {
                params->compareToMaxDeviation = true;
            }
            else if (i->equalsIgnoreCase("--compareavg")) {
                params->compareToMaxDeviation = false;
            }
            else if (i->equalsIgnoreCase("--logpenalty")) {
                params->logPenaltyComparison = true;
            }
            else if (i->equalsIgnoreCase("--linpenalty")) {
                params->logPenaltyComparison = false;
            }
            else if (i->equalsIgnoreCase("--weightframes")) {
                params->weightFftFrames = true;
            }
            else if (i->equalsIgnoreCase("--noweightframes")) {
                params->weightFftFrames = false;
            }
            else if (i->equalsIgnoreCase("--frameweightexp"))  {
                params->frameWeightExponent = (++i)->getDoubleValue();
            }
            else if (i->equalsIgnoreCase("--phasepenalty"))  {
                params->penalizeBadPhase = (++i)->getDoubleValue();
            }
            else if (i->equalsIgnoreCase("--goodmagcomp"))  {
                params->goodComparisonFactor = (++i)->getDoubleValue();
            }
            else if (i->equalsIgnoreCase("--badmagcomp"))  {
                params->badComparisonFactor = (++i)->getDoubleValue();
            }
            else if (i->equalsIgnoreCase("--basemagcomp"))  {
                params->baseComparisonFactor = (++i)->getDoubleValue();
            }

            // synth evolution parameters
            else if (i->equalsIgnoreCase("--popsize")) {
                params->populationSize = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--backup")) {
                params->backupAllNetworks = true;
            }
            else if (i->equalsIgnoreCase("--nobackup")) {
                params->backupAllNetworks = false;
            }
            else if (i->equalsIgnoreCase("--backupprecision")) {
                params->backupPrecision = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--bestfitness"))  {
                params->bestPossibleFitness = (++i)->getDoubleValue();
            }
            else if (i->equalsIgnoreCase("--mid")) {
                params->maxInitialDepth = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--md")) {
                params->maxDepth = (++i)->getIntValue();
            }

            // synth genetic params
            else if (i->equalsIgnoreCase("--greedy"))  {
                params->proportionOfPopulationForGreedySelection = (++i)->getDoubleValue();
            }
            // numeric mutation
            else if (i->equalsIgnoreCase("--nmselect")) {
                params->numericMutationSelectionType = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--numericmutation")) {
                params->proportionOfPopulationFromNumericMutation = (++i)->getDoubleValue();
            }
            else if (i->equalsIgnoreCase("--nmselectparam")) {
                params->percentileOfPopulationToSelectFromForNumericMutation = (++i)->getDoubleValue();
            }
            else if (i->equalsIgnoreCase("--nmtemperature")) {
                params->numericMutationTemperatureConstant = (++i)->getDoubleValue();
            }
            // mutation
            else if (i->equalsIgnoreCase("--mselect")) {
                params->mutationSelectionType = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--mtype")) {
                params->mutationType = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--mutation")) {
                params->proportionOfPopulationFromMutation = (++i)->getDoubleValue();
            }
            else if (i->equalsIgnoreCase("--mselectparam")) {
                params->percentileOfPopulationToSelectFromForMutation = (++i)->getDoubleValue();
            }
            // crossover
            else if (i->equalsIgnoreCase("--cselect")) {
                params->crossoverSelectionType = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--ctype")) {
                params->crossoverType = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--crossover")) {
                params->proportionOfPopulationFromCrossover = (++i)->getDoubleValue();
            }
            // reproduction
            else if (i->equalsIgnoreCase("--rselect")) {
                params->reproductionSelectionType = (++i)->getIntValue();
            }
            else if (i->equalsIgnoreCase("--reproduction")) {
                params->proportionOfPopulationFromReproduction = (++i)->getDoubleValue();
            }
        }

        // TODO: check all value ranges here
        if (target.equalsIgnoreCase("")) {
            std::cerr << "No target specified. Exiting application." << std::endl;
            quit();
        }

        if (printExperimentInfo) {
            std::cerr << commandLine << std::endl;
            printImportantExperimentInfo();
        }

        requestedQuit = false;
        experiment = new GPExperiment(new GPRandom(seed), seed, target, saveFilesTo, params, constants.data(), &requestedQuit);

        // TODO start a new thread to do this
        GPNetwork* champion = experiment->evolve();
        delete champion;
        quit();
    }

    void printImportantExperimentInfo() {
        // PRINT COMP INFORMATION TO ERROR LOG
        time_t now = time(0);
        struct tm tstruct;
        char buff[200];
        tstruct = *localtime(&now);
        strftime(buff, sizeof(buff), "Date run: %m.%d.%Y\nTime run: %H.%M.%S\n", &tstruct);

        // PRINT TIME/DATE
        std::cerr << buff;

        // PRINT GITHUB COMMIT ID
        FILE *gitid = popen("git rev-parse HEAD", "r");
        char gitbuffer[200];
        while (fgets(gitbuffer, sizeof(gitbuffer) - 1, gitid) != NULL) {
            std::cerr << "Git commit ID: " << gitbuffer;
        }
        pclose(gitid);

        // PRINT HOST INFO
        FILE *hostname = popen("hostname", "r");
        char hostbuffer[200];
        while (fgets(hostbuffer, sizeof(hostbuffer) - 1, hostname) != NULL) {
            std::cerr << "Host name: " << hostbuffer;
        }
        pclose(hostname);

        // PRINT CPU INFO
        FILE *lscpu = popen("lscpu", "r");
        char lscpubuffer[1024];
        while (fgets(lscpubuffer, sizeof(lscpubuffer) - 1, lscpu) != NULL) {
            std::cerr << lscpubuffer;
        }
        pclose(lscpu);

        // PRINT MEMORY INFO
        FILE *meminfo = popen("grep \"Mem\" /proc/meminfo", "r");
        char meminfobuffer[200];
        while (fgets(meminfobuffer, sizeof(meminfobuffer) - 1, meminfo) != NULL) {
            std::cerr << meminfobuffer;
        }
        pclose(meminfo);
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
        std::cout << "ACTUALLY REQUESTED QUIT..." << std::endl;
        requestedQuit = true;
        quit();
    }

    void anotherInstanceStarted (const String& commandLine)
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

private:
    bool requestedQuit;
    GPExperiment* experiment;
    GPParams* params;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (GeneticProgrammingSynthesizerExperiment)
