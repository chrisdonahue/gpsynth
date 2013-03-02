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

#ifndef __JUCER_HEADER_SIMPLEINTERACTIVEGP_SIMPLEINTERACTIVEGP_401095A8__
#define __JUCER_HEADER_SIMPLEINTERACTIVEGP_SIMPLEINTERACTIVEGP_401095A8__

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Synth/GPNetwork.h"

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class SimpleInteractiveComponent  : public Component,
                             public ButtonListener,
                             public SliderListener
{
public:
    //==============================================================================
    SimpleInteractiveComponent (GPNetwork* net, double sr);
    ~SimpleInteractiveComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void sliderValueChanged (Slider* sliderThatWasMoved);

    bool userStillEvaluating;

    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //==============================================================================
    TextButton* playsound;
    Slider* freqslider;
    Label* freqlabel;
    TextButton* upvote;
    TextButton* downvote;
    Label* fitnesslabel;
    Label* fitnessamount;
    TextButton* savebutton;
    TextButton* nextbutton;
    GPNetwork* network;
    double* frequency;
    double* time;
    double sampleRate;
    int cycle;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    SimpleInteractiveComponent (const SimpleInteractiveComponent&);
    const SimpleInteractiveComponent& operator= (const SimpleInteractiveComponent&);
};


#endif   // __JUCER_HEADER_SIMPLEINTERACTIVEGP_SIMPLEINTERACTIVEGP_401095A8__
