/*
  ===================================

  Evolutionary Synthesis Audio Plugin

  ===================================
*/

#include "GPPluginProcessor.h"
#include "GPPluginEditor.h"

// DECLARE THAT THIS IS THE PLUGIN CLASS WE WANT TO CREATE
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GeneticProgrammingSynthesizerAudioProcessor();
}

/*
	GP Synthesiser Sound
*/
class GPSound : public SynthesiserSound
{
public:
    GPSound() {}

    bool appliesToNote (const int /*midiNoteNumber*/)           {
        return true;
    }
    bool appliesToChannel (const int /*midiChannel*/)           {
        return true;
    }
};


/*
	GP Synthesiser Voice
		Delete and make new one if render settings change
*/
class GPVoice  : public SynthesiserVoice
{
public:
    GPVoice(GPNetwork* net, unsigned bs, unsigned nv, unsigned maxframes, float* st)
        : network(net),
		  buffer(nullptr),
		  numVariables(nv), variables(nullptr),
		  maxNumFrames(maxframes), sampleTimes(st),
		  frameNumber(0), cps(0.0), playing(false), level(0.0), tailOff(0.0)
    {
		buffer = (float*) malloc(sizeof(float) * bs);
		variables = (float*) malloc(sizeof(float) * numVariables);
    }
	
	~GPVoice()
	{
		free(variables);
		free(buffer);
	}

    void startNote (const int midiNoteNumber, const float velocity,
                    SynthesiserSound* /*sound*/, const int /*currentPitchWheelPosition*/)
    {
		// fill time info
		frameNumber = 0;

		// fill note info
        cps = (float) MidiMessage::getMidiNoteInHertz (midiNoteNumber);
		variables[0] = cps;

		// fill synth note state
		playing = true;
		level = velocity * 0.15;
		tailOff = 0.0;
    }

    void stopNote (const bool allowTailOff)
    {
        if (allowTailOff)
        {
            // start a tail-off by setting this flag. The render callback will pick up on
            // this and do a fade out, calling clearCurrentNote() when it's finished.

            if (tailOff == 0.0) // we only need to begin a tail-off if it's not already doing so - the
                // stopNote method could be called more than once.
                tailOff = 1.0;
        }
        else
        {
            // we're being told to stop playing immediately, so reset everything..

            clearCurrentNote();
        }
    }

    void pitchWheelMoved (const int /*newValue*/)
    {
        // can't be bothered implementing this for the demo!
    }

    void controllerMoved (const int /*controllerNumber*/, const int /*newValue*/)
    {
        // not interested in controllers in this case.
    }

    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
    {
		if (playing) {
			// fill info buffers
			if (tailOff > 0) {
				playing = false;
				clearCurrentNote();
			}
			else if (frameNumber + numSamples > maxNumFrames) {
				network->evaluateBlockPerformance(frameNumber, maxNumFrames - frameNumber, sampleTimes + frameNumber, numVariables, variables, buffer);
				outputBuffer.clear(maxNumFrames - frameNumber, numSamples - (maxNumFrames - frameNumber));
				playing = false;
				clearCurrentNote();
			}
			else {			
				// fill audio buffers
				network->evaluateBlockPerformance(frameNumber, numSamples, sampleTimes + frameNumber, numVariables, variables, buffer);
				frameNumber += numSamples;
			}
			for (int i = 0; i < outputBuffer.getNumChannels(); i++) {
				float* channelBuffer = outputBuffer.getSampleData(i, startSample);
				//memcpy(outputBuffer.getSampleData(i, startSample), buffer, blockSizeInBytes);
				for (int j = 0; j < numSamples; j++) {
					channelBuffer[j] = buffer[j] * level;
				}
			}
		}
    }

    bool canPlaySound (SynthesiserSound* sound)
    {
        return dynamic_cast <GPSound*> (sound) != 0;
    }

private:
	// Algorithm
    GPNetwork* network;

	// Render Buffer
	float* buffer;
	
	// Control Variables
    unsigned numVariables;
    float* variables;
	
	// Render Variables
	unsigned maxNumFrames;
    float* sampleTimes;

	// Current note info
	unsigned frameNumber;
    float cps;
	bool playing;
    float level;
	double tailOff;
};


/*
   ============
   CONSTRUCTION
   ============
*/

GeneticProgrammingSynthesizerAudioProcessor::GeneticProgrammingSynthesizerAudioProcessor()
	:	lastUIWidth(400), lastUIHeight(478),
		algorithm(0), algorithmFitness(0), gain(1.0f), fitnesses((double*) malloc(sizeof(double) * POPULATIONSIZE)),
		samplerate(0), numsamplesperblock(0),
		numvariables(1),
		maxnoteleninseconds(MAXNOTELEN), maxnumframes(0), allvoicessampletimes(nullptr),
		seed(time(NULL)), rng(seed), params((GPParams*) malloc(sizeof(GPParams))), currentPrimitives(nullptr), gpsynth(nullptr),
		currentAlgorithm(nullptr), currentGeneration(POPULATIONSIZE, nullptr),
		generationActive(false), algorithmPrepared(false),
		numSynthVoicesPerAlgorithm(NUMVOICES), currentCopies(nullptr), currentGenerationCopies(POPULATIONSIZE, nullptr)
{
    // set up default fitnesses
	for (unsigned i = 0; i < POPULATIONSIZE; i++) {
		fitnesses[i] = 0.0f;
	}

	// set up genetic algorithm
	// params
	// synth and experiment
	params->verbose = false;
	params->savePrecision = 20;
	params->printPrecision = 1;
	params->saveGenerationChampions = false;
	params->lowerFitnessIsBetter = false;
	params->bestPossibleFitness = 1.0f;

    // synth evolution params
    params->populationSize = POPULATIONSIZE;
    params->backupAllNetworks = false;
    params->backupPrecision = 100;
    params->lowerFitnessIsBetter = false; //should be done in experiment
    params->bestPossibleFitness = 2.0; //should be done in experiment
    params->maxInitialHeight = 0;
    params->maxHeight = 4;

    // synth genetic params
    params->proportionOfPopulationForGreedySelection = 0.0;
    // numeric mutation
    params->numericMutationSelectionType = 1;
    params->proportionOfPopulationFromNumericMutation = 0.00;
    params->percentileOfPopulationToSelectFromForNumericMutation = 0.05;
    params->numericMutationTemperatureConstant = 0.9;
    // mutation
    params->mutationSelectionType = 1;
    params->mutationType = 0;
    params->proportionOfPopulationFromMutation = 0.00;
    params->percentileOfPopulationToSelectFromForMutation = 0.05;
    // crossover
    params->crossoverSelectionType = 0;
    params->crossoverType = 0;
    params->proportionOfPopulationFromCrossover = 1.0;
    // reproduction
    params->reproductionSelectionType = 0;
    params->proportionOfPopulationFromReproduction = 0.00;

    // value node params
    params->valueNodeMinimum = -1.0;
    params->valueNodeMaximum = 1.0;

    // oscil node params
    params->oscilNodeMaxPartial = 30;
    params->oscilNodeMinIndexOfModulation = 0.0;
    params->oscilNodeMaxIndexOfModulation = 2.0;

    // modulation node
    params->LFONodeFrequencyRange = 10;

    // delay node
    params->delayNodeBufferMaxSeconds = 1.0;

    // filter node
    params->filterNodeCenterFrequencyMinimum = 20;
    params->filterNodeCenterFrequencyMaximumProportionOfNyquist = 0.9999;
    params->filterNodeQualityMinimum = 0.1;
    params->filterNodeQualityMaximum = 10.0;
    params->filterNodeBandwidthMinimum = 10;
    params->filterNodeBandwidthMaximum = 2000;

    // adsr node
    params->ADSRNodeEnvelopeMin = 0.0;
    params->ADSRNodeEnvelopeMax = 1.0;

	// primitives
	currentPrimitives = new std::vector<GPNode*>(0);
	currentPrimitives->push_back(createNode("(osc {d 0 0 1} {d 1 1 1})", &rng));
	currentPrimitives->push_back(createNode("(* (null) (null))", &rng));
	// create
	gpsynth = new GPSynth(params, &rng, currentPrimitives);

	// networks and population
	fillFromGeneration();

	// init synth
	synth.addSound(new GPSound());

	// remaining from default plugin
	lastPosInfo.resetToDefault();

	// start algorithm change timer
	startTimer(300);
}

GeneticProgrammingSynthesizerAudioProcessor::~GeneticProgrammingSynthesizerAudioProcessor()
{
	debugPrint("1\n");
	deleteGenerationState();
	debugPrint("2\n");
	delete gpsynth;
	debugPrint("3\n");
	free(params);
	debugPrint("4\n");
	free(fitnesses);
	debugPrint("5\n");
	if (allvoicessampletimes != nullptr) {
		for (unsigned i = 0; i < numSynthVoicesPerAlgorithm; i++) {
			free(allvoicessampletimes[i]);
		}
		free(allvoicessampletimes);
	}
	debugPrint("6\n");
}

/*
   ======================
   AUDIO ENGINE OVERRIDES
   ======================
*/

void GeneticProgrammingSynthesizerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    synth.setCurrentPlaybackSampleRate (sampleRate);

	// set render info
	if (samplerate != sampleRate) {
		samplerate = (float) sampleRate;
		updateSampleTimes();
	}
	numsamplesperblock = (unsigned) samplesPerBlock;
	numvariables = 1;

	// prepare copies for rendering and add voices
	for (unsigned i = 0; i < numSynthVoicesPerAlgorithm; i++) {
		currentCopies[i]->prepareToRender(samplerate, numsamplesperblock, maxnumframes, maxnoteleninseconds);
		synth.addVoice(new GPVoice(currentCopies[i], (unsigned) samplesPerBlock, numvariables, maxnumframes, allvoicessampletimes[i]));
	}
	algorithmPrepared = true;

    keyboardState.reset();
    //delayBuffer.clear();
}

void GeneticProgrammingSynthesizerAudioProcessor::donePlaying()
{
	// remove old voices
	if (algorithmPrepared) {
		for (unsigned i = 0; i < numSynthVoicesPerAlgorithm; i++) {
			currentCopies[i]->doneRendering();
		}

		for (unsigned i = 0; i < numSynthVoicesPerAlgorithm; i++) {
			synth.removeVoice(i);
		}
	}
	algorithmPrepared = false;
}

void GeneticProgrammingSynthesizerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    keyboardState.reset();
}

void GeneticProgrammingSynthesizerAudioProcessor::reset()
{
    // Use this method as the place to clear any delay lines, buffers, etc, as it
    // means there's been a break in the audio's continuity.
    //delayBuffer.clear();
}

void GeneticProgrammingSynthesizerAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int numSamples = buffer.getNumSamples();
    int channel, dp = 0;

    // Now pass any incoming midi messages to our keyboard state object, and let it
    // add messages to the buffer if the user is clicking on the on-screen keys
    keyboardState.processNextMidiBuffer (midiMessages, 0, numSamples, true);

    // and now get the synth to process these midi events and generate its output.
	if (algorithmPrepared)
		synth.renderNextBlock (buffer, midiMessages, 0, numSamples);

	// apply the gain
	buffer.applyGain(gain);

	// Go through the incoming data, and apply our gain to it...
	/*
    for (channel = 0; channel < getNumInputChannels(); ++channel)
        buffer.applyGain (channel, 0, buffer.getNumSamples(), gain);
		*/

	/*
    // Apply our delay effect to the new output..
    for (channel = 0; channel < getNumInputChannels(); ++channel)
    {
        float* channelData = buffer.getSampleData (channel);
        float* delayData = delayBuffer.getSampleData (jmin (channel, delayBuffer.getNumChannels() - 1));
        dp = delayPosition;

        for (int i = 0; i < numSamples; ++i)
        {
            const float in = channelData[i];
            channelData[i] += delayData[dp];
            delayData[dp] = (delayData[dp] + in) * delay;
            if (++dp >= delayBuffer.getNumSamples())
                dp = 0;
        }
    }

    delayPosition = dp;
	*/

    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // ask the host for the current time so we can display it...
    AudioPlayHead::CurrentPositionInfo newTime;

    if (getPlayHead() != nullptr && getPlayHead()->getCurrentPosition (newTime))
    {
        // Successfully got the current time from the host..
        lastPosInfo = newTime;
    }
    else
    {
        // If the host fails to fill-in the current time, we'll just clear it to a default..
        lastPosInfo.resetToDefault();
    }
}

/*
   =====================
   CUSTOM PLUGIN METHODS
   =====================
*/

void GeneticProgrammingSynthesizerAudioProcessor::updateSampleTimes() {
	maxnumframes = (unsigned) maxnoteleninseconds * samplerate;
	if (allvoicessampletimes != nullptr) {
		for (unsigned i = 0; i < numSynthVoicesPerAlgorithm; i++) {
			free(allvoicessampletimes[i]);
		}
		free(allvoicessampletimes);
	}
	
	// make sample timings for each voice to avoid concurrency issues
	allvoicessampletimes = (float**) malloc(sizeof(float*) * numSynthVoicesPerAlgorithm);
	for (unsigned i = 0; i < numSynthVoicesPerAlgorithm; i++) {
		allvoicessampletimes[i] = (float*) malloc(sizeof(float) * maxnumframes);
		for (unsigned j = 0; j < maxnumframes; j++) {
			allvoicessampletimes[i][j] = (float) (j / (double) samplerate);
			//debugPrint(String(allvoicessampletimes[i][j]) + "\n");
		}
	}
	if (numSynthVoicesPerAlgorithm > 0 && maxnumframes > 0)
		maxnoteleninseconds = allvoicessampletimes[0][maxnumframes - 1];

	if (algorithmPrepared)
		setAlgorithm(algorithm);
}

void GeneticProgrammingSynthesizerAudioProcessor::changeNumVariables(unsigned newnumvar) {
	numvariables = newnumvar;
	if (algorithmPrepared)
		setAlgorithm(algorithm);
}

// custom methods
void GeneticProgrammingSynthesizerAudioProcessor::setAlgorithm(unsigned newalgo) {
	// clear out old voices
	donePlaying();

	// update current
	currentAlgorithm = currentGeneration[newalgo];
	currentCopies = currentGenerationCopies[newalgo];

	// update slider params
	algorithm = newalgo;
	algorithmFitness = fitnesses[newalgo];

	// add new voices
	// if an algorithm was previously prepared
	if (samplerate != 0 && maxnoteleninseconds != 0) {
		// prepare the new one because we know sample rate is nonzero
		prepareToPlay(samplerate, numsamplesperblock);
	}
	// otherwise prepareToPlay will be called automatically soon
}

void GeneticProgrammingSynthesizerAudioProcessor::fillFromGeneration() {
	deleteGenerationState();

	gpsynth->getIndividuals(currentGeneration);

	// fill synthesizer voices
	for (unsigned i = 0; i < POPULATIONSIZE; i++) {
		currentGenerationCopies[i] = (GPNetwork**) malloc(sizeof(GPNetwork*) * numSynthVoicesPerAlgorithm);
		GPNetwork* net = currentGeneration[i];
		//net->traceNetwork();
		for (unsigned j = 0; j < numSynthVoicesPerAlgorithm; j++) {
			GPNetwork* copy = net->getCopy("clone");
			copy->traceNetwork();
			currentGenerationCopies[i][j] = copy;
		}
	}
	generationActive = true;

	setAlgorithm(algorithm);
}

void GeneticProgrammingSynthesizerAudioProcessor::deleteGenerationState() {
	if (generationActive) {
		//debugPrint("got to here\n");
		for (unsigned i = 0; i < POPULATIONSIZE; i++) {
			for (unsigned j = 0; j < numSynthVoicesPerAlgorithm; j++) {
				delete currentGenerationCopies[i][j];
			}
			free(currentGenerationCopies[i]);
		}
	}
	generationActive = false;
}

void GeneticProgrammingSynthesizerAudioProcessor::nextGeneration() {
	for (unsigned i = 0; i < POPULATIONSIZE; i++) {
		gpsynth->assignFitness(currentGeneration[i], fitnesses[i]);
	}
	fillFromGeneration();
	return;
}

void GeneticProgrammingSynthesizerAudioProcessor::saveCurrentNetwork() {
    WildcardFileFilter wildcardFilter ("*.synth", String::empty, "Foo files");
    FileBrowserComponent browser (FileBrowserComponent::saveMode,
                                  File::nonexistent,
                                  &wildcardFilter,
                                  nullptr);
    FileChooserDialogBox dialogBox ("Choose a location to save the synthesizer patch",
                                    String::empty,
                                    browser,
                                    false,
                                    Colours::lightgrey);
    if (dialogBox.show())
    {
        File selectedFile = browser.getSelectedFile (0);
		saveTextFile(selectedFile.getFileName() + String(".synth"), String(currentAlgorithm->toString(SAVEPRECISION).c_str()));
    }
}

void GeneticProgrammingSynthesizerAudioProcessor::timerCallback() {
	// only set algorithm if algorithm has changed from the last set algorithm but has been constant for two callbacks
	debugPrint("GPPlugin Timer Callback\n");
	return;
}

void GeneticProgrammingSynthesizerAudioProcessor::debugPrint(String dbgmsg) {
	appendToTextFile("./debug.txt", dbgmsg);
}

/*
   ===============
   AUDIO INTERFACE
   ===============
*/

const String GeneticProgrammingSynthesizerAudioProcessor::getInputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

const String GeneticProgrammingSynthesizerAudioProcessor::getOutputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

bool GeneticProgrammingSynthesizerAudioProcessor::isInputChannelStereoPair (int /*index*/) const
{
    return true;
}

bool GeneticProgrammingSynthesizerAudioProcessor::isOutputChannelStereoPair (int /*index*/) const
{
    return true;
}

bool GeneticProgrammingSynthesizerAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double GeneticProgrammingSynthesizerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

/*
   ================
   EDITOR OVERRIDES
   ================
*/

AudioProcessorEditor* GeneticProgrammingSynthesizerAudioProcessor::createEditor()
{
    return new GeneticProgrammingSynthesizerAudioProcessorEditor (this);
}

/*
   ====================
   PARAMETERS INTERFACE
   ====================
*/

int GeneticProgrammingSynthesizerAudioProcessor::getNumParameters()
{
    return totalNumParams;
}

float GeneticProgrammingSynthesizerAudioProcessor::getParameter (int index)
{
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
	case algorithmParam:
		return (float) algorithm;
		break;
	case algorithmFitnessParam:
		return algorithmFitness;
		break;
    case gainParam:
        return gain;
		break;
    default:
		break;
    }
	return 0.0f;
}

void GeneticProgrammingSynthesizerAudioProcessor::setParameter (int index, float newValue)
{
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
	case algorithmParam:
		algorithm = (unsigned) newValue;
		//setAlgorithm(algorithm);
		algorithmFitness = fitnesses[algorithm];
		break;
	case algorithmFitnessParam:
		algorithmFitness = newValue;
		fitnesses[algorithm] = (double) newValue;
		break;
    case gainParam:
        gain = newValue;
		break;
    default:
        break;
    }
}

const String GeneticProgrammingSynthesizerAudioProcessor::getParameterName (int index)
{
    switch (index)
    {
	case algorithmParam:
		return "algorithm";
		break;
	case algorithmFitnessParam:
		return "algorithmFitness";
		break;
    case gainParam:
        return "gain";
		break;
    default:
        break;
    }

    return String::empty;
}

const String GeneticProgrammingSynthesizerAudioProcessor::getParameterText (int index)
{
    return String (getParameter (index), 2);
}

/*
   ========================
   PROGRAM CHANGE INTERFACE
   ========================
*/

/*
   ==============
   MIDI INTERFACE
   ==============
*/

bool GeneticProgrammingSynthesizerAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool GeneticProgrammingSynthesizerAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

/*
   =======================
   SAVE SETTINGS INTERFACE
   =======================
*/

void GeneticProgrammingSynthesizerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // Here's an example of how you can use XML to make it easy and more robust:

    // Create an outer XML element..
    XmlElement xml ("MYPLUGINSETTINGS");

    // add some attributes to it..
    xml.setAttribute ("uiWidth", lastUIWidth);
    xml.setAttribute ("uiHeight", lastUIHeight);
    xml.setAttribute ("gain", gain);

    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (xml, destData);
}

void GeneticProgrammingSynthesizerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    // This getXmlFromBinary() helper function retrieves our XML from the binary blob..
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState != nullptr)
    {
        // make sure that it's actually our type of XML object..
        if (xmlState->hasTagName ("MYPLUGINSETTINGS"))
        {
            // ok, now pull out our parameters..
            lastUIWidth  = xmlState->getIntAttribute ("uiWidth", lastUIWidth);
            lastUIHeight = xmlState->getIntAttribute ("uiHeight", lastUIHeight);


            gain  = (float) xmlState->getDoubleAttribute ("gain", gain);
        }
    }
}