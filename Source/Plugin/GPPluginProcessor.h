/*
  ===================================

  Evolutionary Synthesis Audio Plugin

  ===================================
*/

#ifndef __PLUGINPROCESSOR_H_526ED7A9__
#define __PLUGINPROCESSOR_H_526ED7A9__

#define POPULATIONSIZE 10
#define NUMVOICES 1
#define MAXNOTELEN 1.0f
#define SAVEPRECISION 15

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Synth/GPSynth.h"
#include "../Common/GPParser.h"

class GPVoice;

class GeneticProgrammingSynthesizerAudioProcessor  : public AudioProcessor,
													public Timer
{
public:
	// Construction
    GeneticProgrammingSynthesizerAudioProcessor();
    ~GeneticProgrammingSynthesizerAudioProcessor();

	// audio engine overrides
    void prepareToPlay (double sampleRate, int samplesPerBlock);
	void donePlaying ();
    void releaseResources();
    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);
    void reset();

	// custom plugin methods
	void updateSampleTimes();
	void changeNumVariables(unsigned numvariables);
	void setAlgorithm();
	void fillFromGeneration();
	void deleteGenerationState();
	void nextGeneration();
	void saveCurrentNetwork();
    void timerCallback();
	void debugPrint(String dbgmsg);

	// audio interface
    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;
    bool silenceInProducesSilenceOut() const;
    double getTailLengthSeconds() const;

	// editor overrides
    bool hasEditor() const                  { return true; }
    AudioProcessorEditor* createEditor();

	// parameters interface
    const String getName() const            { return JucePlugin_Name; }
    int getNumParameters();
    float getParameter (int index);
    void setParameter (int index, float newValue);
    const String getParameterName (int index);
    const String getParameterText (int index);

    enum Parameters
    {
		algorithmParam,
		algorithmFitnessParam,
		gainParam,
        totalNumParams
    };

	unsigned algorithm;
	float algorithmFitness;
    float gain;
	double* fitnesses;

	// program change interface
    int getNumPrograms()                                                { return 0; }
    int getCurrentProgram()                                             { return 0; }
    void setCurrentProgram (int /*index*/)                              { }
    const String getProgramName (int /*index*/)                         { return String::empty; }
    void changeProgramName (int /*index*/, const String& /*newName*/)   { }

	// MIDI plugin interface
    bool acceptsMidi() const;
    bool producesMidi() const;

	// save settings interface
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);

    //==============================================================================
    // These properties are public so that our editor component can access them
    // A bit of a hacky way to do it, but it's only a demo! Obviously in your own
    // code you'll do this much more neatly..

    // this is kept up to date with the midi messages that arrive, and the UI component
    // registers with it so it can represent the incoming messages
    MidiKeyboardState keyboardState;

    // this keeps a copy of the last set of time info that was acquired during an audio
    // callback - the UI component will read this and display it.
    AudioPlayHead::CurrentPositionInfo lastPosInfo;

    // these are used to persist the UI's size - the values are stored along with the
    // filter's other parameters, and the UI component will update them when it gets
    // resized.
    int lastUIWidth, lastUIHeight;

private:
	// render info
	float samplerate;
	unsigned numsamplesperblock;
	
	// num variables
	unsigned numvariables;

	// max note length 
	float maxnoteleninseconds;
	unsigned maxnumframes;
	float** allvoicessampletimes;

    // genetic algorithm
	unsigned seed;
	GPRandom rng;
	GPParams* params;
	std::vector<GPNode*>* currentPrimitives;
	GPSynth* gpsynth;
	
	// networks and population
	GPNetwork* currentAlgorithm;
	std::vector<GPNetwork*> currentGeneration;

	// synthesiser voices
	bool generationActive;
	bool algorithmPrepared;
	unsigned numSynthVoicesPerAlgorithm;
	GPNetwork** currentCopies;
	std::vector<GPNetwork**> currentGenerationCopies;

	// flags for timer on high-memory operations
	bool nextGenerationPressed;
	unsigned algorithmLastTimer;

	// synthesiser
	Synthesiser synth;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GeneticProgrammingSynthesizerAudioProcessor)
};

#endif  // __PLUGINPROCESSOR_H_526ED7A9__
