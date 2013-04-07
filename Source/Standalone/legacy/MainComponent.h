/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef __MAINCOMPONENT_H_9C53AA1C__
#define __MAINCOMPONENT_H_9C53AA1C__

#include "../JuceLibraryCode/JuceHeader.h"
#include "Plugin/GPPluginProcessor.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&);
    void resized();

private:
    ScopedPointer<AudioDeviceManager> deviceManager;
    AudioProcessorPlayer player;
    ScopedPointer<GPPluginAudioProcessor> filter;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // __MAINCOMPONENT_H_9C53AA1C__
