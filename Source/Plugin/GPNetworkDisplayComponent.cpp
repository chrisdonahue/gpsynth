/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "GPNetworkDisplayComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
GPNetworkDisplayComponent::GPNetworkDisplayComponent ()
{
    addAndMakeVisible (textEditor = new TextEditor ("new text editor"));
    textEditor->setMultiLine (false);
    textEditor->setReturnKeyStartsNewLine (false);
    textEditor->setReadOnly (false);
    textEditor->setScrollbarsShown (true);
    textEditor->setCaretVisible (true);
    textEditor->setPopupMenuEnabled (true);
    textEditor->setColour (TextEditor::textColourId, Colours::white);
    textEditor->setColour (TextEditor::backgroundColourId, Colours::black);
    textEditor->setColour (TextEditor::highlightColourId, Colours::black);
    textEditor->setColour (TextEditor::outlineColourId, Colour (0xff7575e8));
    textEditor->setText (String::empty);

    addAndMakeVisible (component = new Component());

    //[UserPreSize]
    //[/UserPreSize]

    setSize (300, 300);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

GPNetworkDisplayComponent::~GPNetworkDisplayComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    textEditor = nullptr;
    component = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void GPNetworkDisplayComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::black);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void GPNetworkDisplayComponent::resized()
{
    textEditor->setBounds (10, 10, 280, 135);
    component->setBounds (10, 155, 280, 135);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="GPNetworkDisplayComponent"
                 componentName="" parentClasses="public Component" constructorParams=""
                 variableInitialisers="" snapPixels="10" snapActive="1" snapShown="1"
                 overlayOpacity="0.330000013" fixedSize="1" initialWidth="300"
                 initialHeight="300">
  <BACKGROUND backgroundColour="ff000000"/>
  <TEXTEDITOR name="new text editor" id="6b24e72601b5c479" memberName="textEditor"
              virtualName="" explicitFocusOrder="0" pos="10 10 280 135" textcol="ffffffff"
              bkgcol="ff000000" hilitecol="ff000000" outlinecol="ff7575e8"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <JUCERCOMP name="" id="6ace1dd111daa0d7" memberName="component" virtualName=""
             explicitFocusOrder="0" pos="10 155 280 135" sourceFile="" constructorParams=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
