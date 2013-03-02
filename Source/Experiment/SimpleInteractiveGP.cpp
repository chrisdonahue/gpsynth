/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  2 Mar 2013 12:42:40am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "SimpleInteractiveGP.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
SimpleInteractiveGP::SimpleInteractiveGP ()
    : playsound (0),
      freqslider (0),
      freqlabel (0),
      upvote (0),
      downvote (0),
      fitnesslabel (0),
      fitnessamount (0),
      savebutton (0),
      nextbutton (0)
{
    addAndMakeVisible (playsound = new TextButton (L"play"));
    playsound->setButtonText (L"play sound");
    playsound->addListener (this);

    addAndMakeVisible (freqslider = new Slider (L"frequency"));
    freqslider->setRange (20, 10000, 0);
    freqslider->setSliderStyle (Slider::LinearVertical);
    freqslider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    freqslider->addListener (this);

    addAndMakeVisible (freqlabel = new Label (L"freq",
                                              L"frequency"));
    freqlabel->setFont (Font (15.0000f, Font::plain));
    freqlabel->setJustificationType (Justification::centredLeft);
    freqlabel->setEditable (false, false, false);
    freqlabel->setColour (TextEditor::textColourId, Colours::black);
    freqlabel->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (upvote = new TextButton (L"upfitness"));
    upvote->setButtonText (L"+ cool sound +");
    upvote->addListener (this);

    addAndMakeVisible (downvote = new TextButton (L"downfitness"));
    downvote->setButtonText (L"- no good -");
    downvote->addListener (this);

    addAndMakeVisible (fitnesslabel = new Label (L"fitnesslabel",
                                                 L"fitness: "));
    fitnesslabel->setFont (Font (15.0000f, Font::plain));
    fitnesslabel->setJustificationType (Justification::centredLeft);
    fitnesslabel->setEditable (false, false, false);
    fitnesslabel->setColour (TextEditor::textColourId, Colours::black);
    fitnesslabel->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (fitnessamount = new Label (L"fitnessamount",
                                                  L"0"));
    fitnessamount->setFont (Font (20.0000f, Font::plain));
    fitnessamount->setJustificationType (Justification::centredLeft);
    fitnessamount->setEditable (false, false, false);
    fitnessamount->setColour (TextEditor::textColourId, Colours::black);
    fitnessamount->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (savebutton = new TextButton (L"save"));
    savebutton->addListener (this);

    addAndMakeVisible (nextbutton = new TextButton (L"next"));
    nextbutton->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (300, 300);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

SimpleInteractiveGP::~SimpleInteractiveGP()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (playsound);
    deleteAndZero (freqslider);
    deleteAndZero (freqlabel);
    deleteAndZero (upvote);
    deleteAndZero (downvote);
    deleteAndZero (fitnesslabel);
    deleteAndZero (fitnessamount);
    deleteAndZero (savebutton);
    deleteAndZero (nextbutton);


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SimpleInteractiveGP::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff7ec883));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SimpleInteractiveGP::resized()
{
    playsound->setBounds (120, 24, 150, 24);
    freqslider->setBounds (8, 16, 100, 275);
    freqlabel->setBounds (24, 0, 80, 20);
    upvote->setBounds (120, 112, 150, 24);
    downvote->setBounds (120, 144, 150, 24);
    fitnesslabel->setBounds (160, 80, 60, 24);
    fitnessamount->setBounds (208, 78, 30, 30);
    savebutton->setBounds (120, 216, 150, 24);
    nextbutton->setBounds (120, 248, 150, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SimpleInteractiveGP::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == playsound)
    {
        //[UserButtonCode_playsound] -- add your button handler code here..
        //[/UserButtonCode_playsound]
    }
    else if (buttonThatWasClicked == upvote)
    {
        //[UserButtonCode_upvote] -- add your button handler code here..
        //[/UserButtonCode_upvote]
    }
    else if (buttonThatWasClicked == downvote)
    {
        //[UserButtonCode_downvote] -- add your button handler code here..
        //[/UserButtonCode_downvote]
    }
    else if (buttonThatWasClicked == savebutton)
    {
        //[UserButtonCode_savebutton] -- add your button handler code here..
        //[/UserButtonCode_savebutton]
    }
    else if (buttonThatWasClicked == nextbutton)
    {
        //[UserButtonCode_nextbutton] -- add your button handler code here..
        //[/UserButtonCode_nextbutton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void SimpleInteractiveGP::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == freqslider)
    {
        //[UserSliderCode_freqslider] -- add your slider handling code here..
        //[/UserSliderCode_freqslider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SimpleInteractiveGP" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="1" initialWidth="300" initialHeight="300">
  <BACKGROUND backgroundColour="ff7ec883"/>
  <TEXTBUTTON name="play" id="d1651f7331580171" memberName="playsound" virtualName=""
              explicitFocusOrder="0" pos="120 24 150 24" buttonText="play sound"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="frequency" id="b1fd5022e65c98eb" memberName="freqslider"
          virtualName="" explicitFocusOrder="0" pos="8 16 100 275" min="20"
          max="10000" int="0" style="LinearVertical" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="freq" id="cb797af8c648844" memberName="freqlabel" virtualName=""
         explicitFocusOrder="0" pos="24 0 80 20" edTextCol="ff000000"
         edBkgCol="0" labelText="frequency" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="upfitness" id="7e7e5d6f277f7317" memberName="upvote" virtualName=""
              explicitFocusOrder="0" pos="120 112 150 24" buttonText="+ cool sound +"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="downfitness" id="88c41903f4bb2a90" memberName="downvote"
              virtualName="" explicitFocusOrder="0" pos="120 144 150 24" buttonText="- no good -"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="fitnesslabel" id="58496c056cf2397e" memberName="fitnesslabel"
         virtualName="" explicitFocusOrder="0" pos="160 80 60 24" edTextCol="ff000000"
         edBkgCol="0" labelText="fitness: " editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="fitnessamount" id="7daa4e2075f79620" memberName="fitnessamount"
         virtualName="" explicitFocusOrder="0" pos="208 78 30 30" edTextCol="ff000000"
         edBkgCol="0" labelText="0" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="20"
         bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="save" id="2045631adaf97770" memberName="savebutton" virtualName=""
              explicitFocusOrder="0" pos="120 216 150 24" buttonText="save"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="next" id="c3ed1a1d9fc27749" memberName="nextbutton" virtualName=""
              explicitFocusOrder="0" pos="120 248 150 24" buttonText="next"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
