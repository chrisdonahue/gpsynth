#include "GPStandaloneDialog.h"

//==============================================================================
//  Somewhere in the codebase of your plugin, you need to implement this function
//  and make it create an instance of the filter subclass that you're building.
extern GPPluginAudioProcessor* JUCE_CALLTYPE createGPPluginFilter();


//==============================================================================
StandaloneFilterWindow::StandaloneFilterWindow (const String& title,
        const Colour& backgroundColour)
    : DocumentWindow (title, backgroundColour,
                      DocumentWindow::minimiseButton
                      | DocumentWindow::closeButton)
{
    JUCE_TRY
    {
        filter = createGPPluginFilter();
        filter->addChangeListener(this);
        filter->addActionListener(this);
        filter->sendChangeMessage();
        filter->createGUI();
    }
    JUCE_CATCH_ALL

    if (filter == nullptr)
    {
        jassertfalse    // Your filter didn't create correctly!
        JUCEApplication::quit();
    }

    filter->setPlayConfigDetails (//JucePlugin_MaxNumInputChannels,
                                  //JucePlugin_MaxNumOutputChannels,
                                  2,2,44100, 512);

    deviceManager = new AudioDeviceManager();
    deviceManager->addAudioCallback (&player);
    deviceManager->addMidiInputCallback (String::empty, &player);

    player.setProcessor (filter);

    ScopedPointer<XmlElement> savedState;
    showAudioSettingsDialog();
    std::cout << deviceManager->getCurrentAudioDeviceType() << std::endl;

    deviceManager->initialise (filter->getNumInputChannels(),
                               filter->getNumOutputChannels(),
                               savedState,
                               true);

    //deviceManager->closeAudioDevice();
}
//==============================================================================
// Destructor
//==============================================================================
StandaloneFilterWindow::~StandaloneFilterWindow()
{
    deviceManager->removeMidiInputCallback (String::empty, &player);
    deviceManager->removeAudioCallback (&player);
    deviceManager = nullptr;

    deleteFilter();
}

void StandaloneFilterWindow::timerCallback()
{   
}

//==============================================================================
// action Callback - updates instrument according to changes in source code
//==============================================================================
void StandaloneFilterWindow::actionListenerCallback (const String& message) {
    /*
    	if(message == "GUI Update"){
    	}
    */
}

void StandaloneFilterWindow::changeListenerCallback(juce::ChangeBroadcaster* /*source*/)
{
}

//==============================================================================
// Delete filter
//==============================================================================
void StandaloneFilterWindow::deleteFilter()
{
    player.setProcessor (nullptr);

    if (filter != nullptr && getContentComponent() != nullptr)
    {
        filter->editorBeingDeleted (dynamic_cast <AudioProcessorEditor*> (getContentComponent()));
        clearContentComponent();
    }
}

//==============================================================================
// Reset filter
//==============================================================================
void StandaloneFilterWindow::resetFilter()
{
    deleteFilter();
    deviceManager->closeAudioDevice();
    filter = createGPPluginFilter();
    filter->addChangeListener(this);
    filter->addActionListener(this);
    filter->sendChangeMessage();
    filter->createGUI();

    if (filter != nullptr)
    {
        if (deviceManager != nullptr) {
            player.setProcessor (filter);
            deviceManager->restartLastAudioDevice();
        }
        setContentOwned (filter->createEditorIfNeeded(), true);
    }
}

//==============================================================================
void StandaloneFilterWindow::saveState()
{
}

void StandaloneFilterWindow::loadState()
{
}

//==============================================================================
PropertySet* StandaloneFilterWindow::getGlobalSettings()
{
}

void StandaloneFilterWindow::showAudioSettingsDialog()
{
    const int numIns =  filter->getNumInputChannels();
    const int numOuts = filter->getNumOutputChannels();

    AudioDeviceSelectorComponent selectorComp (*deviceManager,
            numIns, numIns, numOuts, numOuts,
            true, false, true, false);

    selectorComp.setSize (400, 250);
    setAlwaysOnTop(false);
    //selectorComp.setLookAndFeel(lookAndFeel);
    Colour col(44, 44, 44);
    DialogWindow::showModalDialog(TRANS("Audio Settings"), &selectorComp, this, col, true, false, false);
    setAlwaysOnTop(true);
}
//==============================================================================
void StandaloneFilterWindow::closeButtonPressed()
{
    JUCEApplication::quit();
}

void StandaloneFilterWindow::resized()
{
    DocumentWindow::resized();
    optionsButton.setBounds (8, 6, 60, getTitleBarHeight() - 8);
}


//==============================================================================
// Button clicked method
//==============================================================================
void StandaloneFilterWindow::buttonClicked (Button*)
{
    if (filter == nullptr)
        return;
}

