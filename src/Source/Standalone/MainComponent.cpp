/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

// MUST DFEFINE SOMEWHERE
extern GPPluginAudioProcessor* JUCE_CALLTYPE createGPPluginFilter();

//==============================================================================
MainContentComponent::MainContentComponent()
{
    setSize (500, 400);
    JUCE_TRY
    {
        filter = createGPPluginFilter();
        filter->addChangeListener(this);
        filter->addActionListener(this);
        filter->sendChangeMessage();
        filter->createGUI("");
    }
    JUCE_CATCH_ALL
    if (filter == nullptr)
    {
        jassertfalse    // Your filter didn't create correctly! In a standalone app that's not too great.
        JUCEApplication::quit();
    }

    filter->setPlayConfigDetails (JucePlugin_MaxNumInputChannels,
                                  JucePlugin_MaxNumOutputChannels,
                                  44100, 512);

    PropertySet* const globalSettings = getGlobalSettings();

    deviceManager = new AudioDeviceManager();
    deviceManager->addAudioCallback (&player);
    deviceManager->addMidiInputCallback (String::empty, &player);

    player.setProcessor (filter);

    deviceManager->initialise (filter->getNumInputChannels(),
                               filter->getNumOutputChannels(),
                               savedState,
                               true);

    deviceManager->closeAudioDevice();
}

MainContentComponent::~MainContentComponent()
{
    deviceManager->removeMidiInputCallback (String::empty, &player);
    deviceManager->removeAudioCallback (&player);
    deviceManager = nullptr;

    player.setProcessor (nullptr);

    if (filter != nullptr && getContentComponent() != nullptr)
    {
        filter->editorBeingDeleted (dynamic_cast <AudioProcessorEditor*> (getContentComponent()));
        clearContentComponent();
    }
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xffeeddff));

    g.setFont (Font (16.0f));
    g.setColour (Colours::black);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
