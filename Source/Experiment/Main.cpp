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
        unsigned expnum = 0;
        unsigned popsize = 50;
        unsigned seed = time(NULL);
        double addchance = 0.5;
        double subchance = 0.5;
        double mutatechance = 0.5;
        double crosschance = 0.5;
        double threshold = 1.0;
        unsigned numGenerations = 50;
        unsigned selecttype = 0;
        unsigned crosstype = 0;
        std::vector<double>* values = new std::vector<double>();

        for (String* i = args.begin(); i < args.end(); i++) {
          if (i->equalsIgnoreCase("--target")) {
            target = *(++i);
          }
          else if (i->equalsIgnoreCase("--exp")) {
            expnum = (++i)->getIntValue();
          }
          else if (i->equalsIgnoreCase("--popsize")) {
            popsize = (++i)->getIntValue();
          }
          else if (i->equalsIgnoreCase("--seed")) {
            seed = (++i)->getIntValue();
          }
          else if (i->equalsIgnoreCase("--addchance"))  {
            addchance = (++i)->getDoubleValue();
          }
          else if (i->equalsIgnoreCase("--removechance"))  {
            subchance = (++i)->getDoubleValue();
          }
          else if (i->equalsIgnoreCase("--mutatechance"))  {
            mutatechance = (++i)->getDoubleValue();
          }
          else if (i->equalsIgnoreCase("--crosschance"))  {
            crosschance = (++i)->getDoubleValue();
          }
          else if (i->equalsIgnoreCase("--threshold"))  {
            threshold = (++i)->getDoubleValue();
          }
          else if (i->equalsIgnoreCase("--numgenerations"))  {
            numGenerations = (++i)->getIntValue();
          }
          else if (i->equalsIgnoreCase("--selection")) {
            selecttype = (++i)->getIntValue();
          }
          else if (i->equalsIgnoreCase("--cross")) {
            crosstype = (++i)->getIntValue();
          }
          else if (i->equalsIgnoreCase("--values")) {
            String* current = (++i);
            while ( (i != args.end())  && !(current->startsWith(String("--"))) ) {
                values->push_back(current->getDoubleValue());
                current = ++i;
            }
          }
        }

        // check all value ranges here
        if (target.equalsIgnoreCase("")) {
            std::cerr << "No target specified. Exiting application." << std::endl;
            quit();
        }

        juce::Thread::setCurrentThreadName("experiment");
        experiment = new GPExperiment(target, expnum, popsize, seed, addchance, subchance, mutatechance, crosschance, threshold, numGenerations, selecttype, crosstype, values);
        experiment->evolve();
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
