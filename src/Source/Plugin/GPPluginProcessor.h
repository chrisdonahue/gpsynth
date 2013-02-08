/*
  ==============================================================================

    GPPluginProcessor.h
    Created: 6 Feb 2013 7:17:28pm
    Author:  cdonahue

  ==============================================================================
*/

#ifndef __GPPLUGINPROCESSOR_H_7631AE8A__
#define __GPPLUGINPROCESSOR_H_7631AE8A__

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Synth/GPNetwork.h"
#include "../Synth/GPNode.h"
#include "../Synth/GPCore.h"
#include "../Synth/ValueNode.h"
#include "../Synth/FunctionNode.h"

//==============================================================================
// GPPluginAudioProcessor definition
//==============================================================================
class GPPluginAudioProcessor  : public AudioProcessor,
    public ChangeBroadcaster,
    public Timer,
    public ActionBroadcaster
{
    //==============================================================================
    String pluginName;
    void timerCallback();
    void changeListenerCallback(ChangeBroadcaster *source);

public:
    //==============================================================================
    GPPluginAudioProcessor();
    ~GPPluginAudioProcessor();

    bool silenceInProducesSilenceOut() const;
    int getNumParameters();
    float getParameter (int index);
    void setParameter (int index, float newValue);
    const String getParameterName (int index);
    const String getParameterText (int index);
    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;
    AudioProcessorEditor* createEditor();

    //===== GP CODE =====
    GPInfo* info;
    GPNetwork* net;
    float* time;
    float* cps;


    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();

    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    //==============================================================================
    bool hasEditor() const;

    //==============================================================================
    const String getName() const;


    bool acceptsMidi() const;
    bool producesMidi() const;

    //==============================================================================
    int getNumPrograms();
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);

    //======== log information about GUI controls ===============

    void createGUI();
    //midiBuffers
    MidiBuffer midiBuffer;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GPPluginAudioProcessor);
};

#endif  // __GPPLUGINPROCESSOR_H_7631AE8A__
