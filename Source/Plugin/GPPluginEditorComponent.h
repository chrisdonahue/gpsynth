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

#ifndef __JUCER_HEADER_GENETICPROGRAMMINGSYNTHESIZERCOMPONENT_GPPLUGINEDITORCOMPONENT_96566F03__
#define __JUCER_HEADER_GENETICPROGRAMMINGSYNTHESIZERCOMPONENT_GPPLUGINEDITORCOMPONENT_96566F03__

//[Headers]     -- You can add your own extra header files here --
#include "GPPluginProcessor.h"
#include "../../JuceLibraryCode/JuceHeader.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class GeneticProgrammingSynthesizerComponent  : public Component,
                                                public SliderListener,
                                                public ButtonListener
{
public:
    //==============================================================================
    GeneticProgrammingSynthesizerComponent ();
    ~GeneticProgrammingSynthesizerComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void setSliderListener(SliderListener* sl);
	void setButtonListener(ButtonListener* bl);
	void getSliders(std::map<String, Slider*>& sliders);
	void getButtons(std::map<String, Button*>& buttons);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void buttonClicked (Button* buttonThatWasClicked);



    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	SliderListener* sliderListener;
	ButtonListener* buttonListener;
    //[/UserVariables]

    //==============================================================================
    Slider* fitness;
    Slider* algonum;
    Label* title;
    Label* algolabel;
    Label* fitlabel;
    Label* gasetlabel;
    Label* playsetlabel;
    Slider* gain;
    Label* amplabel;
    TextButton* save;
    TextButton* nextgen;


    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    GeneticProgrammingSynthesizerComponent (const GeneticProgrammingSynthesizerComponent&);
    const GeneticProgrammingSynthesizerComponent& operator= (const GeneticProgrammingSynthesizerComponent&);
};


#endif   // __JUCER_HEADER_GENETICPROGRAMMINGSYNTHESIZERCOMPONENT_GPPLUGINEDITORCOMPONENT_96566F03__
