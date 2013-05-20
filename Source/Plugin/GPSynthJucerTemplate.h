/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  19 May 2013 10:54:39pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_GPSYNTH_GPSYNTHJUCERTEMPLATE_AF1606C7__
#define __JUCER_HEADER_GPSYNTH_GPSYNTHJUCERTEMPLATE_AF1606C7__

//[Headers]     -- You can add your own extra header files here --
#include "juce.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class GPSynth  : public Component,
                 public SliderListener,
                 public ButtonListener
{
public:
    //==============================================================================
    GPSynth ();
    ~GPSynth();

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
    GPSynth (const GPSynth&);
    const GPSynth& operator= (const GPSynth&);
};


#endif   // __JUCER_HEADER_GPSYNTH_GPSYNTHJUCERTEMPLATE_AF1606C7__
