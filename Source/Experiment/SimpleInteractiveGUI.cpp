#include "SimpleInteractiveGUI.h"

//==============================================================================
//  Somewhere in the codebase of your plugin, you need to implement this function
//  and make it create an instance of the filter subclass that you're building.
extern GPPluginAudioProcessor* JUCE_CALLTYPE createGPPluginFilter(GPNetwork* net, double sampleRate);


//==============================================================================
InteractiveFilterWindow::InteractiveFilterWindow (const String& title,
        const Colour& backgroundColour, GPNetwork* net, double sampleRate)
    : DocumentWindow (title, backgroundColour,
                      DocumentWindow::minimiseButton
                      | DocumentWindow::closeButton)
{
    gui = new SimpleInteractiveComponent(net, sampleRate);
    setContentOwned(gui, true);
    centreWithSize(getWidth(), getHeight());
    setVisible(true);

    // AUDIO STUFF
    JUCE_TRY
    {
        filter = createGPPluginFilter(net, sampleRate);
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
InteractiveFilterWindow::~InteractiveFilterWindow()
{
    deviceManager->removeMidiInputCallback (String::empty, &player);
    deviceManager->removeAudioCallback (&player);
    deviceManager = nullptr;

    deleteFilter();
}

void InteractiveFilterWindow::timerCallback()
{   
}

//==============================================================================
// action Callback - updates instrument according to changes in source code
//==============================================================================
void InteractiveFilterWindow::actionListenerCallback (const String& message) {
    /*
    	if(message == "GUI Update"){
    	}
    */
}

void InteractiveFilterWindow::changeListenerCallback(juce::ChangeBroadcaster* /*source*/)
{
}

//==============================================================================
// Delete filter
//==============================================================================
void InteractiveFilterWindow::deleteFilter()
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
void InteractiveFilterWindow::resetFilter()
{
    deleteFilter();
    deviceManager->closeAudioDevice();
    //filter = createGPPluginFilter();
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
void InteractiveFilterWindow::saveState()
{
}

void InteractiveFilterWindow::loadState()
{
}

//==============================================================================
PropertySet* InteractiveFilterWindow::getGlobalSettings()
{
}

void InteractiveFilterWindow::showAudioSettingsDialog()
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
void InteractiveFilterWindow::closeButtonPressed()
{
    JUCEApplication::quit();
}

void InteractiveFilterWindow::resized()
{
    DocumentWindow::resized();
    optionsButton.setBounds (8, 6, 60, getTitleBarHeight() - 8);
}

/*
//==============================================================================
// Export plugin method
//==============================================================================
int InteractiveFilterWindow::exportPlugin(String type, bool saveAs)
{
File dll;
File loc_csdFile;
File thisFile(File::getSpecialLocation(File::currentApplicationFile));

if(!csdFile.exists()){
					showMessage("You need to open a GP instrument before you can export one as a plugin!", lookAndFeel);
					return 0;
				}
#ifdef LINUX
	FileChooser saveFC(String("Save as..."), File::nonexistent, String(""));
	String VST;
	if (saveFC.browseForFileToSave(true)){
		if(type.contains("VSTi"))
			VST = thisFile.getParentDirectory().getFullPathName() + String("/GPPluginSynth.so");
		else if(type.contains(String("VSTfx")))
			VST = thisFile.getParentDirectory().getFullPathName() + String("GPPluginEffect.so");
		else if(type.contains(String("AU"))){
			showMessage("This feature only works on computers running OSX");
		}
		showMessage(VST);
		File VSTData(VST);
		if(VSTData.exists())showMessage("lib exists");
		else{
			File dll(saveFC.getResult().withFileExtension(".so").getFullPathName());
			showMessage(dll.getFullPathName());
			if(VSTData.copyFileTo(dll))	showMessage("moved");
			File loc_csdFile(saveFC.getResult().withFileExtension(".csd").getFullPathName());
			loc_csdFile.replaceWithText(csdFile.loadFileAsString());
		}
	}
#elif WIN32
	FileChooser saveFC(String("Save plugin as..."), File::nonexistent, String("*.dll"));
	String VST;
		if(type.contains("VSTi"))
			VST = thisFile.getParentDirectory().getFullPathName() + String("\\GPPluginSynth.dat");
		else if(type.contains(String("VST")))
			VST = thisFile.getParentDirectory().getFullPathName() + String("\\GPPluginEffect.dat");

		File VSTData(VST);

		if(!VSTData.exists()){
			showMessage("GP cannot find the plugin libraries. Make sure that GP is situated in the same directory as GPPluginSynth.dat and GPPluginEffect.dat", lookAndFeel);
			return 0;
		}
		else{
			if(saveAs){
			if (saveFC.browseForFileToSave(true)){
			dll = saveFC.getResult().withFileExtension(".dll").getFullPathName();
			loc_csdFile = saveFC.getResult().withFileExtension(".csd").getFullPathName();
			}
			else
				return 0;
			}
			else{
			dll = csdFile.withFileExtension(".dll").getFullPathName();
			loc_csdFile = csdFile.withFileExtension(".csd").getFullPathName();

			}
			//showMessage(dll.getFullPathName());
			if(!VSTData.copyFileTo(dll))
				showMessage("Problem moving plugin lib, make sure it's not currently open in your plugin host!", lookAndFeel);

			loc_csdFile.replaceWithText(csdFile.loadFileAsString());
			setUniquePluginID(dll, loc_csdFile);
			String info;
			info = String("Your plugin has been created. It's called:\n\n")+dll.getFullPathName()+String("\n\nIn order to modify this plugin you only have to edit the associated .csd file. You do not need to export every time you make changes.\n\nTo turn off this notice visit 'Preferences' in the main 'options' menu");

			int val = appProperties->getUserSettings()->getValue("DisablePluginInfo", var(0)).getFloatValue();
			if(!val)
			showMessage(info, lookAndFeel);

#ifdef GP_Named_Pipe
			sendMessageToWinXound("GP_PLUGIN_FILE_UPDATE", csdFile.getFullPathName()+String("|")+loc_csdFile.getFullPathName());
			csdFile = loc_csdFile;
			GPCsoundEditor->setName(csdFile.getFullPathName());
			sendMessageToWinXound("GP_SHOW_MESSAGE|Info", "WinXound has been updated\nyour .csd file");
#endif
		}

#endif

#if MACOSX

	FileChooser saveFC(String("Save as..."), File::nonexistent, String(".vst"));
	String VST;
	if (saveFC.browseForFileToSave(true)){
		//showMessage("name of file is:");
		//showMessage(saveFC.getResult().withFileExtension(".vst").getFullPathName());

		if(type.contains("VSTi"))
			//VST = thisFile.getParentDirectory().getFullPathName() + String("//GPPluginSynth.dat");
			VST = thisFile.getFullPathName()+"/Contents/GPPluginSynth.dat";
		else if(type.contains(String("VST")))
			//VST = thisFile.getParentDirectory().getFullPathName() + String("//GPPluginEffect.dat");
			VST = thisFile.getFullPathName()+"/Contents/GPPluginEffect.dat";
		else if(type.contains(String("AU"))){
			showMessage("this feature is coming soon");
			//VST = thisFile.getParentDirectory().getFullPathName() + String("\\GPVSTfx.component");
		}
		//showMessage(thisFile.getFullPathName()+"/Contents/GPPluginSynth.dat");

		String plist  = String("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
		plist.append(String("<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"), MAXBYTES);
		plist.append(String("<plist version=\"1.0\">\n"), MAXBYTES);
		plist.append(String("<dict>\n"), MAXBYTES);
		plist.append(String("<key>CFBundleExecutable</key>\n"), MAXBYTES);
		plist.append(String("<string>")+String(saveFC.getResult().getFileNameWithoutExtension())+String("</string>\n"), MAXBYTES);
		plist.append(String("<key>CFBundleIdentifier</key>\n"), MAXBYTES);
		plist.append(String("<string>com.GP.GPPlugin</string>\n"), MAXBYTES);
		plist.append(String("<key>CFBundleName</key>\n"), MAXBYTES);
		plist.append(String("<string>GPPlugin</string>\n"), MAXBYTES);
		plist.append(String("<key>CFBundlePackageType</key>\n"), MAXBYTES);
		plist.append(String("<string>BNDL</string>\n"), MAXBYTES);
		plist.append(String("<key>CFBundleShortVersionString</key>\n"), MAXBYTES);
		plist.append(String("<string>1.0.0</string>\n"), MAXBYTES);
		plist.append(String("<key>CFBundleSignature</key>\n"), MAXBYTES);
		plist.append(String("<string>PTul</string>\n"), MAXBYTES);
		plist.append(String("<key>CFBundleVersion</key>\n"), MAXBYTES);
		plist.append(String("<string>1.0.0</string>\n"), MAXBYTES);
		plist.append(String("</dict>\n"), MAXBYTES);
		plist.append(String("</plist>\n"), MAXBYTES);

		//create a copy of the data package and write it to the new location given by user
		File VSTData(VST);
		if(!VSTData.exists()){
			showMessage("GP cannot find the plugin libraries. Make sure that GP is situated in the same directory as GPPluginSynth.dat and GPPluginEffect.dat", lookAndFeel);
			return 0;
		}


		String plugType;
		if(type.contains(String("AU")))
			plugType = String(".component");
		else plugType = String(".vst");

		File dll(saveFC.getResult().withFileExtension(plugType).getFullPathName());

		VSTData.copyFileTo(dll);
		//showMessage("copied");



		File pl(dll.getFullPathName()+String("/Contents/Info.plist"));
		//showMessage(pl.getFullPathName());
		pl.replaceWithText(plist);



		File loc_csdFile(dll.getFullPathName()+String("/Contents/")+saveFC.getResult().getFileNameWithoutExtension()+String(".csd"));
		loc_csdFile.replaceWithText(csdFile.loadFileAsString());
		//showMessage(loc_csdFile.getFullPathName());
		//showMessage(csdFile.loadFileAsString());
		csdFile = loc_csdFile;

		//showMessage(VST+String("/Contents/MacOS/")+saveFC.getResult().getFileNameWithoutExtension());
		//if plugin already exists there is no point in rewriting the binaries
		if(!File(saveFC.getResult().withFileExtension(".vst").getFullPathName()+("/Contents/MacOS/")+saveFC.getResult().getFileNameWithoutExtension()).exists()){
		File bin(dll.getFullPathName()+String("/Contents/MacOS/GPPlugin"));
		bin.moveFileTo(dll.getFullPathName()+String("/Contents/MacOS/")+saveFC.getResult().getFileNameWithoutExtension());
		setUniquePluginID(bin, loc_csdFile);
		}

		String info;
		info = String("Your plugin has been created. It's called:\n\n")+dll.getFullPathName()+String("\n\nIn order to modify this plugin you only have to edit the current .csd file. You do not need to export every time you make changes. To open the current csd file with GP in another session, go to 'Open GP Instrument' and select the .vst file. GP will the load the associated .csd file. \n\nTo turn off this notice visit 'Preferences' in the main 'options' menu");

		int val = appProperties->getUserSettings()->getValue("DisablePluginInfo", var(0)).getFloatValue();
		if(!val)
			showMessage(info, lookAndFeel);


	}

#endif

	return 0;
}
*/
