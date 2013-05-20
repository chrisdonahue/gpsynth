/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  20 May 2013 3:14:44pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_GENETICPROGRAMMINGSYNTHESIZERCOMPONENT_GPPLUGINEDITORJUCER_D8BB6126__
#define __JUCER_HEADER_GENETICPROGRAMMINGSYNTHESIZERCOMPONENT_GPPLUGINEDITORJUCER_D8BB6126__

//[Headers]     -- You can add your own extra header files here --
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
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void buttonClicked (Button* buttonThatWasClicked);



    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    Slider* fitness;
    Slider* synthnum;
    Label* title;
    Label* algolabel;
    Label* fitlabel;
    Label* gasetlabel;
    Label* playsetlabel;
    Slider* amplitude;
    Label* amplabel;
    Label* label6;
    TextButton* save;
    TextButton* nextgen;


    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    GeneticProgrammingSynthesizerComponent (const GeneticProgrammingSynthesizerComponent&);
    const GeneticProgrammingSynthesizerComponent& operator= (const GeneticProgrammingSynthesizerComponent&);
};


#endif   // __JUCER_HEADER_GENETICPROGRAMMINGSYNTHESIZERCOMPONENT_GPPLUGINEDITORJUCER_D8BB6126__
