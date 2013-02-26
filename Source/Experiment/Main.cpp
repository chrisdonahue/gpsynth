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
        unsigned s = time(NULL);
        unsigned expnum = 0;
        double threshold = 0.0;
        unsigned numgens = 50;
        std::vector<double>* vals = new std::vector<double>();
        unsigned psize = 500;
        unsigned mid = 6;
        unsigned md = 17;
        unsigned crosstype = 0;
        unsigned rselect = 0;
        unsigned cselect = 0;
        double crosspercent = 0.9;
        double addchance = 0.0;
        double subchance = 0.0;
        double mutatechance = 0.01;

        for (String* i = args.begin(); i < args.end(); i++) {
          if (i->equalsIgnoreCase("--target")) {
            target = *(++i);
          }
          else if (i->equalsIgnoreCase("--seed")) {
            s = (++i)->getIntValue();
          }
          else if (i->equalsIgnoreCase("--exp")) {
            expnum = (++i)->getIntValue();
          }
          else if (i->equalsIgnoreCase("--threshold"))  {
            threshold = (++i)->getDoubleValue();
          }
          else if (i->equalsIgnoreCase("--numgenerations"))  {
            numgens = (++i)->getIntValue();
          }
          else if (i->equalsIgnoreCase("--values")) {
            String* current = (++i);
            while ( (i != args.end())  && !((current + 1)->startsWith(String("--"))) ) {
                vals->push_back(current->getDoubleValue());
                current = ++i;
            }
          }
          else if (i->equalsIgnoreCase("--popsize")) {
            psize = (++i)->getIntValue();
          }
          else if (i->equalsIgnoreCase("--mid")) {
            mid = (++i)->getIntValue();
          }
          else if (i->equalsIgnoreCase("--md")) {
            md = (++i)->getIntValue();
          }
          else if (i->equalsIgnoreCase("--ctype")) {
            crosstype = (++i)->getIntValue();
          }
          else if (i->equalsIgnoreCase("--rselect")) {
            rselect = (++i)->getIntValue();
          }
          else if (i->equalsIgnoreCase("--cselect")) {
            cselect = (++i)->getIntValue();
          }
          else if (i->equalsIgnoreCase("--crosspercent")) {
            crosspercent = (++i)->getDoubleValue();
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
        }

        // TODO: check all value ranges here
        if (target.equalsIgnoreCase("")) {
            std::cerr << "No target specified. Exiting application." << std::endl;
            quit();
        }

        juce::Thread::setCurrentThreadName("experiment");
        experiment = new GPExperiment(target, s, expnum, threshold, numgens, vals, psize, mid, md, crosstype, rselect, cselect, crosspercent, addchance, subchance, mutatechance);
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
