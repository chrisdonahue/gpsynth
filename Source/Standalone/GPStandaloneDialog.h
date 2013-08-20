#ifndef __JUCE_STANDALONEFILTERWINDOW_JUCEHEADER__
#define __JUCE_STANDALONEFILTERWINDOW_JUCEHEADER__

#include "../Common/GPMatchingExperimentParams.h"
#include "../Plugin/GPPluginProcessor.h"
#include "../../JuceLibraryCode/JuceHeader.h"

class StandaloneFilterWindow    : public DocumentWindow
{
public:
    // CONSTRUCTION
    StandaloneFilterWindow (const String& title,
                            const Colour& backgroundColour, GPParams* p);
    // DESTRUCTION
    ~StandaloneFilterWindow();

    // AUDIO SETTINGS DIALOGUE
    virtual void showAudioSettingsDialog();

    //
    void closeButtonPressed();
    void resized();
	PropertySet* getGlobalSettings();

    bool isGuiEnabled() {
        return isGUIOn;
    }

    void setGuiEnabled(bool onoff) {
        isGUIOn = onoff;
    }

private:
    void deleteFilter();

    ScopedPointer<GeneticProgrammingSynthesizerAudioProcessor> filter;
    ScopedPointer<AudioDeviceManager> deviceManager;
    ScopedPointer<AudioProcessorEditor> editor;
    AudioProcessorPlayer player;

    bool isGUIOn;
    
    // GP stuff
    GPParams* p;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StandaloneFilterWindow);
};

#endif   // __JUCE_STANDALONEFILTERWINDOW_JUCEHEADER__
