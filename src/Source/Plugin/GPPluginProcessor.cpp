/*
  ==============================================================================

    GPPluginProcessor.cpp
    Created: 6 Feb 2013 7:17:28pm
    Author:  cdonahue

  ==============================================================================
*/

#include "GPPluginProcessor.h"

#define MAX_BUFFER_SIZE 1024
//===========================================================
// STANDALONE - CONSTRUCTOR
//===========================================================
GPPluginAudioProcessor::GPPluginAudioProcessor()
{
    createGUI();
}

//===========================================================
// STANDALONE - DESTRUCTOR
//===========================================================
GPPluginAudioProcessor::~GPPluginAudioProcessor()
{
}

//===========================================================
// PARSE CSD FILE AND FILL GUI/GUI-LAYOUT VECTORs.
// NO JUCE WIDGETS GET CREATED IN THIS CLASS. ALL
// GUI OBJECTS ARE CREATED ON THE FLY IN THE GP PLUGIN
// EDITOR FROM INFORMATION HELD IN THE GUICONTROLS VECTOR
//===========================================================
void GPPluginAudioProcessor::createGUI()
{
}

//==============================================================================
//#ifdef GP_Build_Standalone
GPPluginAudioProcessor* JUCE_CALLTYPE createGPPluginFilter()
{
    return new GPPluginAudioProcessor();
}
/*
#else
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GPPluginAudioProcessor();
}
#endif
*/

//==============================================================================
const String GPPluginAudioProcessor::getName() const
{
    return pluginName;
}

bool GPPluginAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool GPPluginAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

int GPPluginAudioProcessor::getNumPrograms()
{
    return 0;
}

int GPPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GPPluginAudioProcessor::setCurrentProgram (int /*index*/)
{
}

const String GPPluginAudioProcessor::getProgramName (int /*index*/)
{
    return String::empty;
}

void GPPluginAudioProcessor::changeProgramName (int /*index*/, const String& /*newName*/)
{
}

//==============================================================================
void GPPluginAudioProcessor::prepareToPlay (double /*sampleRate*/, int /*samplesPerBlock*/)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}
//==============================================================================
void GPPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}
//==============================================================================
//this following callback only runs in plugin mode, and only when one of the
//host widgets is being used
void GPPluginAudioProcessor::timerCallback() {

}
//==============================================================================
void GPPluginAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{

}


//==============================================================================
// MIDI functions
//==============================================================================

//==============================================================================
bool GPPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

//==============================================================================
void GPPluginAudioProcessor::getStateInformation (MemoryBlock& /*destData*/)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GPPluginAudioProcessor::setStateInformation (const void* /*data*/, int /*sizeInBytes*/)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================


