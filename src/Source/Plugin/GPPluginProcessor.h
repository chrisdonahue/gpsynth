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
#include "../Editor/CabbageEditorWindow.h"

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


    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();

    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    //==============================================================================
    AudioProcessorEditor* createEditor();
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
    //==============================================================================


    void setMidiDebug(bool val) {
        showMIDI=val;
    }
    bool getMidiDebug() {
        return showMIDI;
    }
    //======== log information about GUI controls ===============

    void createGUI(String source);
    //midiBuffers
    MidiBuffer midiBuffer;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GPPluginAudioProcessor);
};

#endif  // __GPPLUGINPROCESSOR_H_7631AE8A__
