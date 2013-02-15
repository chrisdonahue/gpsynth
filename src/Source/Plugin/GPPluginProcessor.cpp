/*
  ==============================================================================

    GPPluginProcessor.cpp
    Created: 6 Feb 2013 7:17:28pm
    Author:  cdonahue

  ==============================================================================
*/

#include "../Synth/GPFunctions.h"
#include "GPPluginProcessor.h"

#define MAX_BUFFER_SIZE 1024
//===========================================================
// STANDALONE - CONSTRUCTOR
//===========================================================
GPPluginAudioProcessor::GPPluginAudioProcessor()
{
    createGUI();
}

//===========================================================
// STANDALONE - DESTRUCTOR
//===========================================================
GPPluginAudioProcessor::~GPPluginAudioProcessor()
{
}

//===========================================================
// PARSE CSD FILE AND FILL GUI/GUI-LAYOUT VECTORs.
// NO JUCE WIDGETS GET CREATED IN THIS CLASS. ALL
// GUI OBJECTS ARE CREATED ON THE FLY IN THE GP PLUGIN
// EDITOR FROM INFORMATION HELD IN THE GUICONTROLS VECTOR
//===========================================================
void GPPluginAudioProcessor::createGUI()
{
}

//==============================================================================
//#ifdef GP_Build_Standalone
GPPluginAudioProcessor* JUCE_CALLTYPE createGPPluginFilter()
{
    return new GPPluginAudioProcessor();
}
/*
#else
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GPPluginAudioProcessor();
}
#endif
*/

//==============================================================================
const String GPPluginAudioProcessor::getName() const
{
    return pluginName;
}

bool GPPluginAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool GPPluginAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

int GPPluginAudioProcessor::getNumPrograms()
{
    return 0;
}

int GPPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GPPluginAudioProcessor::setCurrentProgram (int /*index*/)
{
}

const String GPPluginAudioProcessor::getProgramName (int /*index*/)
{
    return String::empty;
}

void GPPluginAudioProcessor::changeProgramName (int /*index*/, const String& /*newName*/)
{
}

//==============================================================================
void GPPluginAudioProcessor::prepareToPlay (double /*sampleRate*/, int /*samplesPerBlock*/)
{
     // Use this method as the place to do any pre-playback
    // initialisation that you need..
    printf("preparing to play!\n");
    
    //cps = (float*) malloc(sizeof(float));
    //*cps = 220.0;
    //double* numtwo = (double*) malloc(sizeof(double));
    //double* numpi = (double*) malloc(sizeof(double));
    //*numtwo = 2.0;
    //*numpi = M_PI;

    /*
    info = new GPInfo();
    
    GPNode* leafone = new ValueNode(numtwo);
    GPNode* leaftwo = new ValueNode(numpi);
    GPNode* leafthree = new ValueNode(NULL, true, false);
    GPNode* leaffour = new ValueNode(NULL, false, true);
    GPNode* connectone = new FunctionNode(GPFunction::multiply, std::string("*"), leafthree, leaffour);
    GPNode* connecttwo = new FunctionNode(GPFunction::multiply, std::string("*"), leafone, leaftwo);
    GPNode* connectthree = new FunctionNode(GPFunction::multiply, std::string("*"), connectone, connecttwo);
    GPNode* root = new FunctionNode(GPFunction::sine, std::string("sin"), connectthree, NULL);
    */

    cycle = 0;
    vars = (double*) malloc(sizeof(double) * 4);
    vars[0] = 220.0;
    vars[1] = 1.0;
    vars[2] = 1.0;
    vars[3] = 2.0*M_PI;
    GPNode* one = new OscilNode(1, 1, NULL, NULL);
    GPNode* two = new OscilNode(1, 2, NULL, NULL);
    GPNode* three = new ValueNode(NULL, 3);
    GPNode* four = new FunctionNode(GPFunction::multiply, "", two, three);
    GPNode* root = new FunctionNode(GPFunction::multiply, "", one, four);
    net = new GPNetwork(0, root);

}
//==============================================================================
void GPPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}
//==============================================================================
//this following callback only runs in plugin mode, and only when one of the
//host widgets is being used
void GPPluginAudioProcessor::timerCallback() {
}
//==============================================================================
void GPPluginAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    float* audioBufferl = buffer.getSampleData(0, 0);
    float* audioBufferr = buffer.getSampleData(1, 0);
    const int numSamples = buffer.getNumSamples();
    double sr = double(this->getSampleRate());
    double inc = 1/sr; 
    double* manualtime = (double*) malloc(sizeof(double));
    float res;
    for (int i = 0; i < numSamples; ++i, ++cycle) {
        *manualtime = cycle/sr;
        float res = net->evaluate(manualtime, vars);
        //if (cycle < 1000) {
        //    printf("%d: %lf\n", cycle, res);
        //}

        /*
        res = sin(2.0*M_PI*manualtime*40.0);
        */

        audioBufferl[i] = res;
        audioBufferr[i] = res;
    }
}


int GPPluginAudioProcessor::getNumParameters()
{
    return 0;
}

float GPPluginAudioProcessor::getParameter (int index)
{
	return 0.f;
}

void GPPluginAudioProcessor::setParameter (int index, float newValue)
{
}


const String GPPluginAudioProcessor::getParameterName (int index)
{
return String::empty;
}

const String GPPluginAudioProcessor::getParameterText (int index)
{
return String::empty;
}

const String GPPluginAudioProcessor::getInputChannelName (int channelIndex) const
{
		return String::empty;
}

const String GPPluginAudioProcessor::getOutputChannelName (int channelIndex) const
{
		return String::empty;
}

bool GPPluginAudioProcessor::isInputChannelStereoPair (int /*index*/) const
{
    return true;
}

bool GPPluginAudioProcessor::isOutputChannelStereoPair (int /*index*/) const
{
    return true;
}

bool GPPluginAudioProcessor::silenceInProducesSilenceOut() const {
    return true;
}

//==============================================================================
// MIDI functions
//==============================================================================

//==============================================================================
bool GPPluginAudioProcessor::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* GPPluginAudioProcessor::createEditor()
{
    return NULL;
}

//==============================================================================
void GPPluginAudioProcessor::getStateInformation (MemoryBlock& /*destData*/)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GPPluginAudioProcessor::setStateInformation (const void* /*data*/, int /*sizeInBytes*/)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================


