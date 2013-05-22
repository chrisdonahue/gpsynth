/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  21 May 2013 8:26:52pm

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

#include "GPPluginEditorComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
GeneticProgrammingSynthesizerComponent::GeneticProgrammingSynthesizerComponent ()
    : fitness (0),
      algonum (0),
      title (0),
      algolabel (0),
      fitlabel (0),
      gasetlabel (0),
      playsetlabel (0),
      gain (0),
      amplabel (0),
      save (0),
      nextgen (0)
{
    addAndMakeVisible (fitness = new Slider (L"fitness"));
    fitness->setRange (0, 1, 0);
    fitness->setSliderStyle (Slider::LinearHorizontal);
    fitness->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    fitness->addListener (this);

    addAndMakeVisible (algonum = new Slider (L"algonum"));
    algonum->setRange (0, 9, 1);
    algonum->setSliderStyle (Slider::LinearHorizontal);
    algonum->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    algonum->addListener (this);

    addAndMakeVisible (title = new Label (L"title",
                                          L"gp synthesis"));
    title->setFont (Font (45.0000f, Font::plain));
    title->setJustificationType (Justification::centred);
    title->setEditable (false, false, false);
    title->setColour (TextEditor::textColourId, Colours::black);
    title->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (algolabel = new Label (L"algolabel",
                                              L"algorithm"));
    algolabel->setFont (Font (12.0000f, Font::plain));
    algolabel->setJustificationType (Justification::centredLeft);
    algolabel->setEditable (false, false, false);
    algolabel->setColour (TextEditor::textColourId, Colours::black);
    algolabel->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (fitlabel = new Label (L"fitlabel",
                                             L"fitness"));
    fitlabel->setFont (Font (12.0000f, Font::plain));
    fitlabel->setJustificationType (Justification::centred);
    fitlabel->setEditable (false, false, false);
    fitlabel->setColour (TextEditor::textColourId, Colours::black);
    fitlabel->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (gasetlabel = new Label (L"gasetlabel",
                                               L"genetic algorithm settings"));
    gasetlabel->setFont (Font (15.0000f, Font::plain));
    gasetlabel->setJustificationType (Justification::centred);
    gasetlabel->setEditable (false, false, false);
    gasetlabel->setColour (TextEditor::textColourId, Colours::black);
    gasetlabel->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (playsetlabel = new Label (L"playsetlabel",
                                                 L"playback settings"));
    playsetlabel->setFont (Font (15.0000f, Font::plain));
    playsetlabel->setJustificationType (Justification::centred);
    playsetlabel->setEditable (false, false, false);
    playsetlabel->setColour (TextEditor::textColourId, Colours::black);
    playsetlabel->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (gain = new Slider (L"gain"));
    gain->setRange (0, 1, 0);
    gain->setSliderStyle (Slider::LinearHorizontal);
    gain->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    gain->addListener (this);

    addAndMakeVisible (amplabel = new Label (L"amplabel",
                                             L"amplitude"));
    amplabel->setFont (Font (12.0000f, Font::plain));
    amplabel->setJustificationType (Justification::centredLeft);
    amplabel->setEditable (false, false, false);
    amplabel->setColour (TextEditor::textColourId, Colours::black);
    amplabel->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (save = new TextButton (L"save"));
    save->addListener (this);

    addAndMakeVisible (nextgen = new TextButton (L"nextgen"));
    nextgen->addListener (this);


    //[UserPreSize]
	algonum->setRange (0, POPULATIONSIZE - 1, 1);
    //[/UserPreSize]

    setSize (400, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

GeneticProgrammingSynthesizerComponent::~GeneticProgrammingSynthesizerComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (fitness);
    deleteAndZero (algonum);
    deleteAndZero (title);
    deleteAndZero (algolabel);
    deleteAndZero (fitlabel);
    deleteAndZero (gasetlabel);
    deleteAndZero (playsetlabel);
    deleteAndZero (gain);
    deleteAndZero (amplabel);
    deleteAndZero (save);
    deleteAndZero (nextgen);


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void GeneticProgrammingSynthesizerComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void GeneticProgrammingSynthesizerComponent::resized()
{
    fitness->setBounds (72, 168, 300, 24);
    algonum->setBounds (72, 128, 300, 24);
    title->setBounds (56, 8, 264, 56);
    algolabel->setBounds (0, 120, 64, 40);
    fitlabel->setBounds (-8, 160, 64, 40);
    gasetlabel->setBounds (48, 88, 280, 24);
    playsetlabel->setBounds (48, 232, 280, 24);
    gain->setBounds (73, 268, 300, 24);
    amplabel->setBounds (0, 264, 64, 40);
    save->setBounds (32, 336, 150, 24);
    nextgen->setBounds (200, 336, 150, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void GeneticProgrammingSynthesizerComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
	sliderListener->sliderValueChanged(sliderThatWasMoved);
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == fitness)
    {
        //[UserSliderCode_fitness] -- add your slider handling code here..
        //[/UserSliderCode_fitness]
    }
    else if (sliderThatWasMoved == algonum)
    {
        //[UserSliderCode_algonum] -- add your slider handling code here..
        //[/UserSliderCode_algonum]
    }
    else if (sliderThatWasMoved == gain)
    {
        //[UserSliderCode_gain] -- add your slider handling code here..
        //[/UserSliderCode_gain]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void GeneticProgrammingSynthesizerComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
	buttonListener->buttonClicked(buttonThatWasClicked);
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == save)
    {
        //[UserButtonCode_save] -- add your button handler code here..
        //[/UserButtonCode_save]
    }
    else if (buttonThatWasClicked == nextgen)
    {
        //[UserButtonCode_nextgen] -- add your button handler code here..
        //[/UserButtonCode_nextgen]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void GeneticProgrammingSynthesizerComponent::setSliderListener(SliderListener* sl) {
	sliderListener = sl;
}

void GeneticProgrammingSynthesizerComponent::setButtonListener(ButtonListener* bl) {
	buttonListener = bl;
}

void GeneticProgrammingSynthesizerComponent::getSliders(std::map<String, Slider*>& sliders) {
	sliders["algorithm"] = algonum;
	sliders["fitness"] = fitness;
	sliders["gain"] = gain;
}

void GeneticProgrammingSynthesizerComponent::getButtons(std::map<String, Button*>& buttons) {
	buttons["save"] = save;
	buttons["nextgen"] = nextgen;
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="GeneticProgrammingSynthesizerComponent"
                 componentName="" parentClasses="public Component" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330000013" fixedSize="0" initialWidth="400"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ffffffff"/>
  <SLIDER name="fitness" id="16a1b0db54c88216" memberName="fitness" virtualName=""
          explicitFocusOrder="0" pos="72 168 300 24" min="0" max="1" int="0"
          style="LinearHorizontal" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="algonum" id="58d24870e11eeb51" memberName="algonum" virtualName=""
          explicitFocusOrder="0" pos="72 128 300 24" min="0" max="9" int="1"
          style="LinearHorizontal" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="title" id="64fb69a603b235c5" memberName="title" virtualName=""
         explicitFocusOrder="0" pos="56 8 264 56" edTextCol="ff000000"
         edBkgCol="0" labelText="gp synthesis" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="45" bold="0" italic="0" justification="36"/>
  <LABEL name="algolabel" id="26fd3e265f6c4c7d" memberName="algolabel"
         virtualName="" explicitFocusOrder="0" pos="0 120 64 40" edTextCol="ff000000"
         edBkgCol="0" labelText="algorithm" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12"
         bold="0" italic="0" justification="33"/>
  <LABEL name="fitlabel" id="4461d1426f065552" memberName="fitlabel" virtualName=""
         explicitFocusOrder="0" pos="-8 160 64 40" edTextCol="ff000000"
         edBkgCol="0" labelText="fitness" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12"
         bold="0" italic="0" justification="36"/>
  <LABEL name="gasetlabel" id="180d29bebe10a4cf" memberName="gasetlabel"
         virtualName="" explicitFocusOrder="0" pos="48 88 280 24" edTextCol="ff000000"
         edBkgCol="0" labelText="genetic algorithm settings" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="playsetlabel" id="4b4f0b44a043419a" memberName="playsetlabel"
         virtualName="" explicitFocusOrder="0" pos="48 232 280 24" edTextCol="ff000000"
         edBkgCol="0" labelText="playback settings" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="gain" id="95c3e13e05b5350b" memberName="gain" virtualName=""
          explicitFocusOrder="0" pos="73 268 300 24" min="0" max="1" int="0"
          style="LinearHorizontal" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="amplabel" id="8fa2033431b21d08" memberName="amplabel" virtualName=""
         explicitFocusOrder="0" pos="0 264 64 40" edTextCol="ff000000"
         edBkgCol="0" labelText="amplitude" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12"
         bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="save" id="55746809785deaaa" memberName="save" virtualName=""
              explicitFocusOrder="0" pos="32 336 150 24" buttonText="save"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="nextgen" id="8f0862ebfb8bbfd0" memberName="nextgen" virtualName=""
              explicitFocusOrder="0" pos="200 336 150 24" buttonText="nextgen"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
