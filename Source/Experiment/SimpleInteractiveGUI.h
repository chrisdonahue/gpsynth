#ifndef __JUCE_STANDALONEFILTERWINDOW_JUCEHEADER__
#define __JUCE_STANDALONEFILTERWINDOW_JUCEHEADER__

#include "../Plugin/GPPluginProcessor.h"
#include "../Plugin/SimpleInteractiveComponent.h"
#include "../Synth/GPNetwork.h"
#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
// This is the main host window. It gets instatiated in StandaloneFilterApp.cpp
//==============================================================================
class InteractiveFilterWindow    : public DocumentWindow,
    public ChangeListener,
    public Timer,
    public ActionListener
{
public:
    //==============================================================================
    InteractiveFilterWindow (const String& title, const Colour& backgroundColour, GPNetwork* net, double sampleRate);

    ~InteractiveFilterWindow();

    //=================================================================
    //=================================================================
    void resetFilter();
    void saveState();
    void loadState();
    virtual void showAudioSettingsDialog();
    //==============================================================================
    void closeButtonPressed();
    void buttonClicked (Button*);
    void showInfoWindow(String text);
    void resized();
    void actionListenerCallback (const String& message);
    void changeListenerCallback(ChangeBroadcaster *source);
    int exportPlugin(String type, bool saveAs);
    void openFile();
    void saveFile();
    void saveFileAs();
	PropertySet* getGlobalSettings();

    bool isGuiEnabled() {
        return isGUIOn;
    }

    void setGuiEnabled(bool onoff) {
        isGUIOn = onoff;
    }

    void setCurrentLine(int line) {
        currentLine=line;
    }

    int getCurrentLine() {
        return currentLine;
    }

    bool userStillEvaluating() {
        return gui->userStillEvaluating;
    }

private:
    void timerCallback();
    ScopedPointer<GPPluginAudioProcessor> filter;
    ScopedPointer<SimpleInteractiveComponent> gui;
    ScopedPointer<AudioDeviceManager> deviceManager;
    AudioProcessorPlayer player;
    TextButton optionsButton;
    void deleteFilter();
    bool isGUIOn;
    int currentLine;
    bool pipeOpenedOk;
    String consoleMessages;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InteractiveFilterWindow);
};

#endif   // __JUCE_STANDALONEFILTERWINDOW_JUCEHEADER__
