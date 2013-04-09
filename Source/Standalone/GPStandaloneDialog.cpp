#include "GPStandaloneDialog.h"

extern GeneticProgrammingSynthesizerAudioProcessor* JUCE_CALLTYPE createPluginFilter();

StandaloneFilterWindow::StandaloneFilterWindow (const String& title,
        const Colour& backgroundColour, GPParams* p)
    : DocumentWindow (title, backgroundColour,
                      DocumentWindow::minimiseButton
                      | DocumentWindow::closeButton)
{
    //setResizable(true, false);
    setTitleBarHeight(20);

    JUCE_TRY
    {
        filter = createPluginFilter();
        editor = filter->createEditor();
    }
    JUCE_CATCH_ALL

    if (filter == nullptr)
    {
        JUCEApplication::quit();
    }

    filter->setPlayConfigDetails (//JucePlugin_MaxNumInputChannels,
                                  //JucePlugin_MaxNumOutputChannels,
                                  2,2,44100, 512);

    deviceManager = new AudioDeviceManager();
    deviceManager->addAudioCallback (&player);
    deviceManager->addMidiInputCallback (String::empty, &player);

    player.setProcessor (filter);

    showAudioSettingsDialog();
    std::cout << deviceManager->getCurrentAudioDeviceType() << std::endl;

    deviceManager->initialise (filter->getNumInputChannels(),
                               filter->getNumOutputChannels(),
                               0,
                               true);
    deviceManager->closeAudioDevice();

    setContentOwned(editor, true);
}

StandaloneFilterWindow::~StandaloneFilterWindow()
{
    deviceManager->removeMidiInputCallback (String::empty, &player);
    deviceManager->removeAudioCallback (&player);
    deviceManager = nullptr;

    deleteFilter();
}

void StandaloneFilterWindow::deleteFilter()
{
    player.setProcessor (nullptr);

    if (filter != nullptr && getContentComponent() != nullptr)
    {
        filter->editorBeingDeleted (dynamic_cast <AudioProcessorEditor*> (getContentComponent()));
        clearContentComponent();
    }
}

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

void StandaloneFilterWindow::closeButtonPressed()
{
    JUCEApplication::quit();
}

void StandaloneFilterWindow::resized()
{
    DocumentWindow::resized();
}
