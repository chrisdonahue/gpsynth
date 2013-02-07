#ifndef __JUCE_STANDALONEFILTERWINDOW_JUCEHEADER__
#define __JUCE_STANDALONEFILTERWINDOW_JUCEHEADER__

#include "../Plugin/GPPluginProcessor.h"

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
                            const Colour& backgroundColour);

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
    int exportPlugin(String type, bool saveAs);
    void openFile();
    void saveFile();
    void saveFileAs();



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
    ScopedPointer<socketServer> server;
    ScopedPointer<socketConnection> ipConnection;  //*** MOD:STEFANO - ADDED
    OwnedArray <socketConnection, CriticalSection> activeConnections;
    ScopedPointer<GPPluginAudioProcessor> filter;
    ScopedPointer<AudioDeviceManager> deviceManager;
    AudioProcessorPlayer player;
    TextButton optionsButton;
    void deleteFilter();
    bool isGUIOn;
    int currentLine;
    bool pipeOpenedOk;
    ScopedPointer<GPLookAndFeel> lookAndFeel;
    ScopedPointer<GPEditorWindow> GPCsoundEditor;
    String consoleMessages;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StandaloneFilterWindow);
};

#endif   // __JUCE_STANDALONEFILTERWINDOW_JUCEHEADER__
