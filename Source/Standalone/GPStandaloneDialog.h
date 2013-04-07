#ifndef __JUCE_STANDALONEFILTERWINDOW_JUCEHEADER__
#define __JUCE_STANDALONEFILTERWINDOW_JUCEHEADER__

#include "../Common/GPParams.h"
#include "../Plugin/GPPluginProcessor.h"
#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
// This is the main host window. It gets instatiated in StandaloneFilterApp.cpp
//==============================================================================
class StandaloneFilterWindow    : public DocumentWindow,
    public Button::Listener,
    public ChangeListener,
    public Timer,
    public ActionListener
{
public:
    //==============================================================================
    StandaloneFilterWindow (const String& title,
                            const Colour& backgroundColour, GPParams* p);

    ~StandaloneFilterWindow();

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

private:
    void timerCallback();
    ScopedPointer<GeneticProgrammingSynthesizerAudioProcessor> filter;
    ScopedPointer<AudioDeviceManager> deviceManager;
    AudioProcessorPlayer player;
    TextButton optionsButton;
    void deleteFilter();
    bool isGUIOn;
    int currentLine;
    bool pipeOpenedOk;
    String consoleMessages;
    
    // GP stuff
    GPParams* p;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StandaloneFilterWindow);
};

#endif   // __JUCE_STANDALONEFILTERWINDOW_JUCEHEADER__
