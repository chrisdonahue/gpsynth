/*
  ==============================================================================

    GPExperiment.cpp
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#include "GPExperiment.h"

/*
    ============
    CONSTRUCTION
    ============
*/

GPExperiment::GPExperiment(GPRandom* rng, unsigned s, String target, String path, GPParams* p, double* constants, bool* rq) :
    seed(s),
    savePath(path),
    params(p),
    specialValues(constants),
    requestedQuit(rq),
    dBRef(54),
    wavFormat(new WavAudioFormat())
{
    // TARGET DATA CONTAINERS
    wavFileBufferSize = p->wavFileBufferSize;
    getWavFileInfo(target, &numTargetFrames, &targetSampleRate);
    targetNyquist = targetSampleRate / 2;
    targetFrames = (float*) malloc(sizeof(float) * numTargetFrames);
    loadWavFile(target, numTargetFrames, targetFrames);
    fillEvaluationBuffers(specialValues, NULL, p->numVariables, 0);

    // EXPERIMENT PARAMETERS THAT USE SAMPLE RATE
    params->delayNodeMaxBufferSize = params->delayNodeBufferMaxSeconds * targetSampleRate;

    // EXPERIMENT STATE
    numGenerations = p->numGenerations;
    fitnessThreshold = p->thresholdFitness;
    minFitnessAchieved = INFINITY;
    numEvaluatedGenerations = 0;

    // SYNTH
    std::vector<GPNode*>* nodes = new std::vector<GPNode*>();

    // MUTATABLE PARAMS
    GPMutatableParam* constantValue = new GPMutatableParam("constantValue", true, 0.0, params->valueNodeMinimum, params->valueNodeMaximum);
    GPMutatableParam* constantTwo = new GPMutatableParam("two", false, 2.0, 0.0, 0.0);
    GPMutatableParam* constantPi = new GPMutatableParam("pi", false, M_PI, 0.0, 0.0);

    GPMutatableParam* oscilPartial = new GPMutatableParam("oscilpartial", true, 1, 1, params->oscilNodeMaxPartial);
    GPMutatableParam* oscilModIndex = new GPMutatableParam("oscilmodindex", true, 1.0, params->oscilNodeMinIndexOfModulation, params->oscilNodeMaxIndexOfModulation);

    GPMutatableParam* filterCenterFrequencyMultiplierMin = new GPMutatableParam("filtercenterfrequencymin", true, 1.0, params->filterNodeCenterFrequencyMinimum/specialValues[0], (targetNyquist * params->filterNodeCenterFrequencyMaximumProportionOfNyquist)/specialValues[0]);
    GPMutatableParam* filterCenterFrequencyMultiplierMax = new GPMutatableParam("filtercenterfrequencymax", true, 1.0, params->filterNodeCenterFrequencyMinimum/specialValues[0], (targetNyquist * params->filterNodeCenterFrequencyMaximumProportionOfNyquist)/specialValues[0]);
    GPMutatableParam* filterQualityMin = new GPMutatableParam("filterqualitymin", true, 0.0, 0.0, params->filterNodeQualityMinimum);
    GPMutatableParam* filterQualityMax = new GPMutatableParam("filterqualitymax", true, 0.0, 0.0, params->filterNodeQualityMaximum);
    GPMutatableParam* filterBandwidth = new GPMutatableParam("filterbandwidthmin", true, 10.0, params->filterNodeBandwidthMinimum, params->filterNodeBandwidthMaximum);

    GPMutatableParam* ADSRDelay = new GPMutatableParam("adsrdelay", true, 0.0, 0.0, numTargetFrames / targetSampleRate);
    GPMutatableParam* ADSRAttack = new GPMutatableParam("adsrattack", true, 0.0, 0.0, numTargetFrames / targetSampleRate);
    GPMutatableParam* ADSRAttackHeight = new GPMutatableParam("adsrattackheight", true, 0.0, params->ADSRNodeEnvelopeMin, params->ADSRNodeEnvelopeMax);
    GPMutatableParam* ADSRDecay = new GPMutatableParam("adsrdecay", true, 0.0, 0.0, numTargetFrames / targetSampleRate);
    GPMutatableParam* ADSRSustain = new GPMutatableParam("adsrsustain", true, 0.0, 0.0, numTargetFrames / targetSampleRate);
    GPMutatableParam* ADSRSustainHeight = new GPMutatableParam("adsrsustainheight", true, 0.0, params->ADSRNodeEnvelopeMin, params->ADSRNodeEnvelopeMax);
    GPMutatableParam* ADSRRelease = new GPMutatableParam("adsrrelease", true, 0.0, 0.0, numTargetFrames / targetSampleRate);

    if (params->experimentNumber == 3) {
        // PARAMS
        GPMutatableParam* partialThree = new GPMutatableParam("", false, 3, 0, 5);
        GPMutatableParam* filterLow = new GPMutatableParam("", false, 11.9511, 10.0, 15.0);
        GPMutatableParam* filterHigh = new GPMutatableParam("", false, 31.8139, 30.0, 32.0);
        GPMutatableParam* bandwidth = new GPMutatableParam("", false, 1000.532, 150.0, 155.0);

        // NODES
        GPNode* noiseNode = new NoiseNode(rng);
        GPNode* oscilPartialThree = new OscilNode(true, partialThree, 0, NULL, NULL);
        //GPNode* filter = new FilterNode(3, 3, params->renderBlockSize, targetSampleRate, 0, filterLow, filterHigh, bandwidth, noiseNode, oscilPartialThree, NULL);

        // NETWORK
        //GPNetwork* fucked = new GPNetwork(filter);
        //fucked->traceNetwork();

        float* fuckedBuffer = (float*) malloc(sizeof(float) * numTargetFrames);
        //renderIndividualByBlock(fucked, numTargetFrames, params->renderBlockSize, fuckedBuffer);

        //saveWavFile("./fucked.wav", String(fucked->toString(false, 10).c_str()), numTargetFrames, 44100, fuckedBuffer);

        free(fuckedBuffer);

        exit(-1);
    }
    // Eb5 Trumpet Additive Experiment
    if (params->experimentNumber == 4) {
        // SUPPLY AVAILABLE NODES
        nodes->push_back(new FunctionNode(add, NULL, NULL));
        nodes->push_back(new FunctionNode(multiply, NULL, NULL));
        nodes->push_back(new ConstantNode(constantValue->getCopy()));
        nodes->push_back(new OscilNode(true, oscilPartial->getCopy(), 0, NULL, NULL));
        nodes->push_back(new OscilNode(false, oscilPartial->getCopy(), 0, oscilModIndex->getCopy(), NULL));
        nodes->push_back(new NoiseNode(rng));
        //nodes->push_back(new FilterNode(2, 3, params->renderBlockSize, targetSampleRate, 0, filterCenterFrequencyMultiplierMin->getCopy(), filterCenterFrequencyMultiplierMax->getCopy(), filterBandwidth->getCopy(), NULL, NULL, NULL));
        //nodes->push_back(new FilterNode(3, 3, params->renderBlockSize, targetSampleRate, 0, filterCenterFrequencyMultiplierMin->getCopy(), filterCenterFrequencyMultiplierMax->getCopy(), filterBandwidth->getCopy(), NULL, NULL, NULL));
        nodes->push_back(new ADSRNode(false, true, targetSampleRate, ADSRDelay->getCopy(), ADSRAttack->getCopy(), ADSRAttackHeight->getCopy(), ADSRDecay->getCopy(), ADSRSustain->getCopy(), ADSRSustainHeight->getCopy(), ADSRRelease->getCopy(), NULL));
        nodes->push_back(new ADSRNode(false, false, targetSampleRate, ADSRDelay->getCopy(), ADSRAttack->getCopy(), ADSRAttackHeight->getCopy(), ADSRDecay->getCopy(), ADSRSustain->getCopy(), ADSRSustainHeight->getCopy(), ADSRRelease->getCopy(), NULL));
    }
    // TESTING NEW ADSR
    if (params->experimentNumber == 8) {
        GPMutatableParam* Delay = new GPMutatableParam("adsrdelay", true, 0.2, 0.0, numTargetFrames / targetSampleRate);
        GPMutatableParam* Attack = new GPMutatableParam("adsrattack", true, 0.2, 0.0, numTargetFrames / targetSampleRate);
        GPMutatableParam* AttackHeight = new GPMutatableParam("adsrattackheight", true, 0.8, params->ADSRNodeEnvelopeMin, params->ADSRNodeEnvelopeMax);
        GPMutatableParam* Decay = new GPMutatableParam("adsrdecay", true, 0.2, 0.0, numTargetFrames / targetSampleRate);
        GPMutatableParam* Sustain = new GPMutatableParam("adsrsustain", true, 0.2, 0.0, numTargetFrames / targetSampleRate);
        GPMutatableParam* SustainHeight = new GPMutatableParam("adsrsustainheight", true, 0.5, params->ADSRNodeEnvelopeMin, params->ADSRNodeEnvelopeMax);
        GPMutatableParam* Release = new GPMutatableParam("adsrrelease", true, 0.2, 0.0, numTargetFrames / targetSampleRate);

        GPMutatableParam* partialOne = new GPMutatableParam("", false, 1, 0, 5);

        GPNode* oscilPartialOne = new OscilNode(true, partialOne, 0, NULL, NULL);
        GPNode* ADSRterm = new ADSRNode(false, true, targetSampleRate, Delay->getCopy(), Attack->getCopy(), AttackHeight->getCopy(), Decay->getCopy(), Sustain->getCopy(), SustainHeight->getCopy(), Release->getCopy(), NULL);
        GPNode* ADSRnonterm = new ADSRNode(false, false, targetSampleRate, Delay->getCopy(), Attack->getCopy(), AttackHeight->getCopy(), Decay->getCopy(), Sustain->getCopy(), SustainHeight->getCopy(), Release->getCopy(), oscilPartialOne);
        GPNode* mult = new FunctionNode(multiply, oscilPartialOne, ADSRterm);

        GPNetwork* OSCIL = new GPNetwork(oscilPartialOne);
        OSCIL->traceNetwork();

        GPNetwork* ADSRTERM = new GPNetwork(mult);
        ADSRTERM->traceNetwork();

        GPNetwork* ADSRNONTERM = new GPNetwork(ADSRnonterm);
        ADSRNONTERM->traceNetwork();

        float* oscil = (float*) malloc(sizeof(float) * numTargetFrames);
        float* term = (float*) malloc(sizeof(float) * numTargetFrames);
        float* nonterm = (float*) malloc(sizeof(float) * numTargetFrames);
        renderIndividualByBlock(OSCIL, numTargetFrames, params->renderBlockSize, oscil);
        renderIndividualByBlock(ADSRTERM, numTargetFrames, params->renderBlockSize, term);
        renderIndividualByBlock(ADSRNONTERM, numTargetFrames, params->renderBlockSize, nonterm);

        saveWavFile("./oscil.wav", String(OSCIL->toString(true, 10).c_str()), numTargetFrames, 44100, oscil);
        saveWavFile("./term.wav", String(ADSRTERM->toString(true, 10).c_str()), numTargetFrames, 44100, term);
        saveWavFile("./nonterm.wav", String(ADSRNONTERM->toString(true, 10).c_str()), numTargetFrames, 44100, nonterm);

        free(oscil);
        free(nonterm);
        free(term);
        exit(-1);
    }
    // TESTING ENVELOPE FUNCTIONS ON INPUTS OF AN LENGTH
    if (params->experimentNumber == 9) {
        // SAVE WAVEFORM
        //saveTextFile(String("./waveform.txt"), floatBuffersToGraphText(String("x> y^ xi yf"), String("Sample #"), String("Magnitude (amp)"), true, numTargetFrames, NULL, targetFrames));

        // TEST ENVELOPE FOLLOWER
        float* envelope = (float*) malloc(sizeof(float) * numTargetFrames);
        float* smoothEnv = (float*) malloc(sizeof(float) * numTargetFrames);
        followEnvelope(numTargetFrames, targetFrames, envelope, 1, 300, targetSampleRate);
        findEnvelope(true, numTargetFrames, envelope, smoothEnv);
        saveTextFile(String("./smoothenva1r300.txt"), floatBuffersToGraphText(String("x> y^ xi yf"), String("Sample #"), String("Envelope (amp)"), true, numTargetFrames, NULL, smoothEnv));

        free(smoothEnv);
        free(envelope);
        exit(-1);
    }
    // TESTING VARIOUS WAVEFORM FUNCTIONS ON INPUTS WITH 1024 SAMPLES
    if (params->experimentNumber == 10) {
        // SAVE WAVEFORM
        saveTextFile(String("./waveform.txt"), floatBuffersToGraphText(String("x> y^ xi yf"), String("Sample #"), String("Magnitude (amp)"), true, numTargetFrames, nullptr, targetFrames));

        // TEST FFT
        unsigned fftOutSize = 513;
        unsigned fftSize = 1024;
        float* freqAxis = (float*) malloc(sizeof(float) * fftOutSize);
        fillFrequencyAxisBuffer(fftSize, targetSampleRate, freqAxis);

        float* magAxis = (float*) malloc(sizeof(float) * fftOutSize);
        for (unsigned i = 0; i < fftOutSize; i++) {
            magAxis[i] = (float) targetSpectrumMagnitudes[i];
        }
        
        saveTextFile(String("./spectrum.txt"), floatBuffersToGraphText(String("x> y^ xf yf"), String("Frequency (Hz)"), String("Magnitude (amp)"), false, fftOutSize, freqAxis, magAxis));

        // TEST WINDOWING
        float* hannWindow = (float*) malloc(sizeof(float) * numTargetFrames);
        float* windowed = (float*) malloc(sizeof(float) * numTargetFrames);
        window("hann", numTargetFrames, hannWindow);
        applyEnvelope(numTargetFrames, targetFrames, hannWindow, windowed);

        saveTextFile(String("./windowed.txt"), floatBuffersToGraphText(String("x> y^ xi yf"), String("Sample #"), String("Magnitude (amp)"), true, numTargetFrames, nullptr, windowed));

        // TEST MOVING AVERAGE
        float* mac = (float*) malloc(sizeof(float) * fftOutSize);
        findMovingAverage(fftOutSize, magAxis, mac, 10);
        saveTextFile(String("./mac10.txt"), floatBuffersToGraphText(String("x> y^ xf yf"), String("Frequency (Hz)"), String("Power (dB)"), false, fftOutSize, freqAxis, mac));
        findMovingAverage(fftOutSize, magAxis, mac, 20);
        saveTextFile(String("./mac20.txt"), floatBuffersToGraphText(String("x> y^ xf yf"), String("Frequency (Hz)"), String("Power (dB)"), false, fftOutSize, freqAxis, mac));
        findMovingAverage(fftOutSize, magAxis, mac, 30);
        saveTextFile(String("./mac30.txt"), floatBuffersToGraphText(String("x> y^ xf yf"), String("Frequency (Hz)"), String("Power (dB)"), false, fftOutSize, freqAxis, mac));

        // FREE AND DONT EVOLVE
        free(mac);
        free(windowed);
        free(hannWindow);
        free(magAxis);
        free(freqAxis);
        exit(-1);
    }
    // filtered noise test
    if (params->experimentNumber == 5) {
        GPNode* noiseNode = new NoiseNode(rng);
        GPNode* constantNode = new ConstantNode(constantValue->getCopy());
        //GPNode* bandPass = new FilterNode(2, 3, 1, targetSampleRate, 0, filterCenterFrequencyMultiplierMin->getCopy(), filterCenterFrequencyMultiplierMax->getCopy(), filterBandwidth->getCopy(), noiseNode->getCopy(), constantNode->getCopy(), constantNode->getCopy());
        //GPNode* bandPass = new FilterNode(2, 3, 11000, targetSampleRate, 0, filterCenterFrequencyMultiplierMin->getCopy(), filterCenterFrequencyMultiplierMax->getCopy(), filterBandwidth->getCopy(), noiseNode->getCopy(), noiseNode->getCopy(), constantNode->getCopy());
        //GPNode* bandStop = new FilterNode(3, 3, 1024, targetSampleRate, 0, filterCenterFrequencyMultiplierMin->getCopy(), filterCenterFrequencyMultiplierMax->getCopy(), filterBandwidth->getCopy(), noiseNode->getCopy(), constantNode->getCopy(), constantNode->getCopy());

        GPNetwork* noiseNetwork = new GPNetwork(noiseNode);
        noiseNetwork->traceNetwork();
        //GPNetwork* bandStopNoiseNetwork = new GPNetwork(bandStop);
        //bandStopNoiseNetwork->ephemeralRandom(rng);
        //bandStopNoiseNetwork->traceNetwork();
        //GPNetwork* bandPassNoiseNetwork = new GPNetwork(bandPass);
        //bandPassNoiseNetwork->ephemeralRandom(rng);
        //bandPassNoiseNetwork->traceNetwork();

        float* noise = (float*) malloc(sizeof(float) * numTargetFrames);
        float* passNoise = (float*) malloc(sizeof(float) * numTargetFrames);
        float* stopNoise = (float*) malloc(sizeof(float) * numTargetFrames);
        renderIndividualByBlock(noiseNetwork, numTargetFrames, params->renderBlockSize, noise);
        //renderIndividualByBlock(bandPassNoiseNetwork, numTargetFrames, params->renderBlockSize, passNoise);
        //renderIndividualByBlock(bandStopNoiseNetwork, numTargetFrames, params->renderBlockSize, stopNoise);

        saveWavFile("./noise.wav", String(noiseNetwork->toString(false, 10).c_str()), numTargetFrames, 44100, noise);
        //saveWavFile("./stopNoise.wav", String(bandStopNoiseNetwork->toString(false, 10).c_str()), numTargetFrames, 44100, stopNoise);
        //saveWavFile("./passNoise.wav", String(bandPassNoiseNetwork->toString(false, 10).c_str()), numTargetFrames, 44100, passNoise);

        free(stopNoise);
        free(passNoise);
        free(noise);

        exit(-1);

        // RENDER AND SAVE ONE NETWORk
        GPMutatableParam* partialOne = new GPMutatableParam("", false, 1, 0, 5);
        GPNode* oscilPartialOne = new OscilNode(true, partialOne, 0, NULL, NULL);
        GPNetwork* oscil = new GPNetwork(oscilPartialOne);
        oscil->traceNetwork();
        float* oscilBuffer = (float*) malloc(sizeof(float) * numTargetFrames);
        //renderEnvelopeAndEvaluate(true, oscil, oscilBuffer);
        saveWavFile("./oscilEnv.wav", String(oscil->toString(false, 10).c_str()), numTargetFrames, targetSampleRate, oscilBuffer);
        renderIndividualByBlock(oscil, numTargetFrames, params->renderBlockSize, oscilBuffer);
        saveWavFile("./oscil.wav", String(oscil->toString(false, 10).c_str()), numTargetFrames, targetSampleRate, oscilBuffer);
        free(oscilBuffer);
        exit(-1);

        // SUPPLY AVAILABLE NODES
        nodes->push_back(new NoiseNode(rng));
        //nodes->push_back(new FilterNode(2, 3, 1, targetSampleRate, 0, filterCenterFrequencyMultiplierMin->getCopy(), filterCenterFrequencyMultiplierMax->getCopy(), filterBandwidth->getCopy(), NULL, NULL, NULL));
        //nodes->push_back(new FilterNode(3, 3, 1, targetSampleRate, 0, filterCenterFrequencyMultiplierMin->getCopy(), filterCenterFrequencyMultiplierMax->getCopy(), filterBandwidth->getCopy(), NULL, NULL, NULL));
        nodes->push_back(new ConstantNode(constantValue->getCopy()));
    }

    // SET FF-VARYING PARAMS FROM FITNESS FUNCTION
    if (params->fitnessFunctionType == 0) {
        p->bestPossibleFitness = 0;
        p->penaltyFitness = std::numeric_limits<float>::max();
        p->lowerFitnessIsBetter = true;
        lowerFitnessIsBetter = params->lowerFitnessIsBetter;
    }
    else {
        p->bestPossibleFitness = 0;
        p->penaltyFitness = std::numeric_limits<float>::max();
        p->lowerFitnessIsBetter = true;
        lowerFitnessIsBetter = params->lowerFitnessIsBetter;
    }

    // INITIALIZE SYNTH
    synth = new GPSynth(rng, p, nodes);

    // DELETE ORIGIN GPMUTATABLEPARAMS
    delete constantValue;
    delete constantTwo;
    delete constantPi;
    delete oscilPartial;
    delete oscilModIndex;
    delete filterCenterFrequencyMultiplierMin;
    delete filterCenterFrequencyMultiplierMax;
    delete filterQualityMin;
    delete filterQualityMax;
    delete filterBandwidth;
    delete ADSRDelay;
    delete ADSRAttack;
    delete ADSRAttackHeight;
    delete ADSRDecay;
    delete ADSRSustain;
    delete ADSRSustainHeight;
    delete ADSRRelease;
}

GPExperiment::~GPExperiment() {
    free(targetFrames);
    free(targetEnvelope);
    if (params->fitnessFunctionType == 1) {
        free(analysisWindow);
        free(targetSpectrum);
        free(targetSpectrumMagnitudes);
        free(targetSpectrumPhases);
        free(weightMatrix);
        free(binOvershootingPenalty);
        free(binUndershootingPenalty);
    }
    free(sampleTimes);
    free(specialValuesByFrame);
    delete synth;
}

/*
    =================
    EVOLUTION CONTROL
    =================
*/

GPNetwork* GPExperiment::evolve() {
    GPNetwork* champ = NULL;
    GPNetwork* generationChamp = NULL;
    int numEvaluated = 0;
    int numUnevaluatedThisGeneration = 0;
    double generationMinimumFitness = INFINITY;
    float* candidateData = (float*) malloc(sizeof(float) * numTargetFrames);

    while (minFitnessAchieved > fitnessThreshold && numEvaluatedGenerations < numGenerations && !(*requestedQuit)) {
        GPNetwork* candidate = synth->getIndividual();

        double fitness;
        renderIndividualByBlock(candidate, numTargetFrames, params->renderBlockSize, candidateData);
        fitness = compareToTarget(params->fitnessFunctionType, candidateData);
        numEvaluated++;

        //TODO: handle lowerFitnessIsBetter

        if (fitness < generationMinimumFitness) {
            generationMinimumFitness = fitness;
            delete generationChamp;

            int backupID = candidate->ID;
            generationChamp = candidate->getCopy();
            generationChamp->ID = backupID;
            generationChamp->fitness = fitness;
        }

        if (fitness < minFitnessAchieved) {
            minFitnessAchieved = fitness;
            delete champ;

            int backupID = candidate->ID;
            champ = candidate->getCopy();
            champ->ID = backupID;
            champ->fitness = minFitnessAchieved;
        }

        numUnevaluatedThisGeneration = synth->assignFitness(candidate, fitness);
        if (numUnevaluatedThisGeneration == 0) {
            generationMinimumFitness = INFINITY;

            float* genchampbuffer = (float*) malloc(sizeof(float) * numTargetFrames);
            generationChamp->traceNetwork();
            renderIndividualByBlock(generationChamp, numTargetFrames, params->renderBlockSize, genchampbuffer);
            if (params->saveGenerationChampions) {
              char buffer[100];
              snprintf(buffer, 100, "%d.gen.%d.best.wav", seed, numEvaluatedGenerations);
              saveWavFile(savePath + String(buffer), String(generationChamp->toString(false, params->savePrecision).c_str()), numTargetFrames, targetSampleRate, genchampbuffer);
            }
            free(genchampbuffer);
            delete generationChamp;
            generationChamp = NULL;

            numEvaluatedGenerations++;
        }
    }
    free(candidateData);

    std::cerr << "-------------------------------- SUMMARY ---------------------------------" << std::endl;

    if (minFitnessAchieved <= fitnessThreshold) {
        std::cerr << "Evolution found a synthesis algorithm at or below the specified fitness threshold" << std::endl;
    }

    if (numUnevaluatedThisGeneration != 0)
        std::cerr << "Evolution ran for " << numEvaluatedGenerations + (params->populationSize - numUnevaluatedThisGeneration)/float(params->populationSize) << " generations" << std::endl;
    else
        std::cerr << "Evolution ran for " << numEvaluatedGenerations << " generations" << std::endl;

    if (champ != NULL) {
        float* champbuffer = (float*) malloc(sizeof(float) * numTargetFrames);
        champ->traceNetwork();
        renderIndividualByBlock(champ, numTargetFrames, params->renderBlockSize, champbuffer);
        std::cerr << "The best synthesis algorithm found was number " << champ->ID << " with network " << champ->toString(true, params->savePrecision) << " and had a fitness of " << minFitnessAchieved << std::endl;
        char buffer[100];
        snprintf(buffer, 100, "%d.champion.wav", seed);
        saveWavFile(savePath + String(buffer), String(champ->toString(false, params->savePrecision).c_str()), numTargetFrames, targetSampleRate, champbuffer);
        free(champbuffer);
    }
    return champ;
}

/*
    =======================
    FILL EVALUATION BUFFERS
    =======================
*/

// PRECONDITIONS:
// NUMTARGETFRAMES, TARGETSAMPLERATE, TARGETFRAMES ALL FILLED IN
void GPExperiment::fillEvaluationBuffers(double* constantSpecialValues, double* variableSpecialValues, unsigned numConstantSpecialValues, unsigned numVariableSpecialValues) {
    // FILL BUFFERS WITH SPECIAL VALUES
    numSpecialValues = numConstantSpecialValues + numVariableSpecialValues;
    specialValuesByFrame = (double*) malloc(sizeof(double) * numTargetFrames * numSpecialValues);
    sampleTimes = (double*) malloc(sizeof(double) * numTargetFrames);
    for (int frame = 0; frame < numTargetFrames; frame++) {
        sampleTimes[frame] = frame/targetSampleRate;
        for (int val = 0; val < numConstantSpecialValues; val++) {
            *(specialValuesByFrame + (frame * numSpecialValues) + val) = constantSpecialValues[val];
        }
        for (int val = 0; val < numVariableSpecialValues; val++) {
            *(specialValuesByFrame + (frame * numSpecialValues) + numConstantSpecialValues + val) = variableSpecialValues[val]; // TODO: RHS of this assignment is placeholder
        }
    }

    // FILL ENVELOPE OF TARGET BUFFER
    targetEnvelope = (float*) malloc(sizeof(float) * numTargetFrames);
    followEnvelope(numTargetFrames, targetFrames, targetEnvelope, params->envelopeFollowerAttack, params->envelopeFollowerDecay, targetSampleRate);
    if (params->saveTargetEnvelope) {
        char buffer[100];
        snprintf(buffer, 100, "%d.targetenvelope.txt", seed);
        saveTextFile(savePath + String(buffer), floatBuffersToGraphText(String("x> y^ xi yf"), String("Sample"), String("Amplitude"), true, numTargetFrames, NULL, targetEnvelope));
    }

    // FILL FREQUENCY SPECTRUM OF TARGET
    if (params->fitnessFunctionType > 0) {
        // calculate with fft window size/overlap
        unsigned n = params->fftSize;
        unsigned overlap = params->fftOverlap;
        unsigned fftOutputBufferSize = calculateFftBufferSize(numTargetFrames, n, overlap);
        
        // allocate window
        analysisWindow = (float*) malloc(sizeof(float) * n);
        window(params->windowType, n, analysisWindow);

        // allocate output buffers
        targetSpectrum = (kiss_fft_cpx*) malloc(sizeof(kiss_fft_cpx) * fftOutputBufferSize);
        targetSpectrumMagnitudes = (double*) malloc(sizeof(double) * fftOutputBufferSize);
        targetSpectrumPhases = (double*) malloc(sizeof(double) * fftOutputBufferSize);
        weightMatrix = (double*) malloc(sizeof(double) * fftOutputBufferSize);
        binOvershootingPenalty = (double*) malloc(sizeof(double) * fftOutputBufferSize);
        binUndershootingPenalty = (double*) malloc(sizeof(double) * fftOutputBufferSize);

        // take fft of target data
        FftReal(numTargetFrames, targetFrames, n, overlap, analysisWindow, targetSpectrum, false, targetSpectrumMagnitudes, targetSpectrumPhases);

        // calculate stats on each frame
        double base = params->baseComparisonFactor;
        double good = params->goodComparisonFactor;
        double bad = params->badComparisonFactor;
        //std::cout << good << ", " << bad << std::endl;
        unsigned numBins = (n/2) + 1;
        unsigned numFftFrames = fftOutputBufferSize / numBins;
        float* mac = (float*) malloc(sizeof(float) * numBins);
        for (unsigned i = 0; i < numFftFrames; i++) {
            // calculate frame average magnitude
            double sum = 0;
            double maxBin = std::numeric_limits<double>::min();
            double minBin = std::numeric_limits<double>::max();
            // EXCLUDE DC OFFSET
            for (unsigned j = 1; j < numBins; j++) {
                double binMagnitude = targetSpectrumMagnitudes[(i * numBins) + j];
                sum += binMagnitude;
                if (binMagnitude > maxBin)
                    maxBin = binMagnitude;
                if (binMagnitude < minBin)
                    minBin = binMagnitude;
            }
            double frameAverageMagnitude = sum / ((double) numBins);
            //std::cout << i << ": [" << minBin << ", " << maxBin << "] " << frameAverageMagnitude << std::endl;

            // compare each bin EXCEPT DC OFFSET to the average magnitude
            for (unsigned j = 1; j < numBins; j++) {
                unsigned binIndex = (i * numBins) + j;
                double binMagnitude = targetSpectrumMagnitudes[binIndex];

                // if we are above the mean penalize undershooting more
                if (binMagnitude > frameAverageMagnitude) {
                    double proportionOfMax = (binMagnitude - frameAverageMagnitude) / (maxBin - frameAverageMagnitude);
                    //std::cout << "ABOVE AVERAGE: " << j << ", " << proportionOfMax << std::endl;
                    binUndershootingPenalty[binIndex] = (proportionOfMax * bad) + base;
                    binOvershootingPenalty[binIndex] = (proportionOfMax * good) + base;
                }

                // if we are below the mean penalize overshooting more
                else {
                    double proportionOfMin = (frameAverageMagnitude - binMagnitude) / (frameAverageMagnitude - minBin);
                    //std::cout << "BELOW AVERAGE: " << j << ", " << proportionOfMin << std::endl;
                    binUndershootingPenalty[binIndex] = (proportionOfMin * good) + base;
                    binOvershootingPenalty[binIndex] = (proportionOfMin * bad) + base;
                }
                /*
                if (i == 3) {
                    std::cout << "BIN " << j << " MAG: " << binMagnitude;
                    std::cout << ", OVERSHOOT: " << binOvershootingPenalty[binIndex] << ", UNDERSHOOT: " << binUndershootingPenalty[binIndex] << std::endl;
                }
                */
            }
        }
        free(mac);
    }
}

/*
    ==============
    FILE INTERFACE
    ==============
*/

void GPExperiment::getWavFileInfo(String path, unsigned* numFrames, double* sampleRate) {
    File input(path);
    if (!(input.existsAsFile())) {
        std::cerr << "Invalid input file: " << path << std::endl;
    }
    assert(input.existsAsFile());
    FileInputStream* fis = input.createInputStream();
    ScopedPointer<AudioFormatReader> afr(wavFormat->createReaderFor(fis, true));

    // get info on target
    *numFrames = afr->lengthInSamples;
    *sampleRate = afr->sampleRate;
}

void GPExperiment::loadWavFile(String path, unsigned n, float* buffer) {
    File input(path);
    if (!(input.existsAsFile())) {
        std::cerr << "Invalid input file: " << path << std::endl;
    }
    assert(input.existsAsFile());
    FileInputStream* fis = input.createInputStream();
    ScopedPointer<AudioFormatReader> afr(wavFormat->createReaderFor(fis, true));

    // get waveform of target
    AudioSampleBuffer asb(1, n);
    afr->read(&asb, 0, n, 0, false, true);
    float* chanData = asb.getSampleData(0);
    memcpy(buffer, chanData, sizeof(float) * n);
}

void GPExperiment::saveWavFile(String path, String desc, unsigned numFrames, double sampleRate, float* data) {
    File output(path);
    if (output.existsAsFile()) {
        output.deleteFile();
    }
    output.create();
    FileOutputStream* fos = output.createOutputStream();

    StringPairArray metaData(true);
    metaData = WavAudioFormat::createBWAVMetadata(desc, ProjectInfo::versionString, "", Time::getCurrentTime(), int64(sampleRate), "JUCE");

    ScopedPointer<AudioFormatWriter> afw(wavFormat->createWriterFor(fos, sampleRate, 1, 32, metaData, 0));

    int64 numRemaining = numFrames;
    int64 numCompleted = 0;
    AudioSampleBuffer asb(1, wavFileBufferSize);
    float* chanData = asb.getSampleData(0);
    while (numRemaining > 0) {
        int numToWrite = numRemaining > wavFileBufferSize ? wavFileBufferSize : numRemaining;
        for (int samp = 0; samp < numToWrite; samp++, numCompleted++) {
            chanData[samp] = data[numCompleted];
        }
        afw->writeFromAudioSampleBuffer(asb, 0, numToWrite);
        numRemaining -= numToWrite;
    }
    assert (numCompleted == numFrames && numRemaining == 0);
}

void GPExperiment::saveTextFile(String path, String text) {
    File output(path);
    if (output.existsAsFile()) {
        output.deleteFile();
    }
    output.create();
    output.replaceWithText(text);
}

/*
    ================
    FITNESS FUNCTION
    ================
*/

void GPExperiment::renderIndividualByBlock(GPNetwork* candidate, int64 numSamples, unsigned n, float* buffer) {
    int64 numRemaining = numSamples;
    int64 numCompleted = 0;
    int64 bufferIndex = 0;
    unsigned numToRender;
    while (numRemaining > 0) {
        numToRender = n < numRemaining ? n : numRemaining;
        candidate->evaluateBlock(numCompleted, sampleTimes + numCompleted, numSpecialValues, specialValuesByFrame + (numCompleted * numSpecialValues), numToRender, buffer + numCompleted);
        numRemaining -= numToRender;
        numCompleted += numToRender;
    }
}

double GPExperiment::compareToTarget(unsigned type, float* candidateFrames) {
    double ret = -1;
    double silenceTest = 0;
    if (type == 0) {
        double sum = 0;
        for (int frameNum = 0; frameNum < numTargetFrames; frameNum++) {
            sum += pow(targetFrames[frameNum] - candidateFrames[frameNum], 2);
            silenceTest += candidateFrames[frameNum];
            /*
            if (frameNum % 128 == 0) {
                std::cout << targetFrames[frameNum] << ", " << candidateFrames[frameNum];
                std::cout << " sum: " << sum << " frameNum: " << frameNum << std::endl;
            }
            */
        }
        ret = sqrt(sum);
    }
    else if (type == 1) {
        unsigned n = params->fftSize;
        unsigned overlap = params->fftOverlap;

        unsigned fftOutputBufferSize = calculateFftBufferSize(numTargetFrames, n, overlap);
        kiss_fft_cpx* output = (kiss_fft_cpx*) malloc(sizeof(kiss_fft_cpx) * fftOutputBufferSize);
        double* magnitude = (double*) malloc(sizeof(double) * fftOutputBufferSize);
        double* phase = (double*) malloc(sizeof(double) * fftOutputBufferSize);

        FftReal(numTargetFrames, candidateFrames, n, overlap, analysisWindow, output, false, magnitude, phase);

        double MSEmag = 0;
        double MSEph = 0;
        unsigned numBins = (n/2) + 1;
        unsigned numFftFrames = fftOutputBufferSize / numBins;
        for (unsigned i = 0; i < numFftFrames; i++) {
            for (unsigned j = 1; j < numBins; j++) {
                unsigned binIndex = (i * numBins) + j;
                if (magnitude[binIndex] < targetSpectrumMagnitudes[binIndex]) {
                    MSEmag += pow(targetSpectrumMagnitudes[binIndex] - magnitude[binIndex], binUndershootingPenalty[binIndex]);
                }
                else {
                    MSEmag += pow(magnitude[binIndex] - targetSpectrumMagnitudes[binIndex], binOvershootingPenalty[binIndex]);
                }
                MSEph += pow(abs(phase[binIndex] - targetSpectrumPhases[binIndex]), params->penalizeBadPhase);
            }
        }
        //MSEmag = MSEmag / n;
        //MSEph = MSEph / n;
        ret = params->magnitudeWeight * MSEmag + params->phaseWeight * MSEph;
        free(phase);
        free(magnitude);
        free(output);
    }
    /*
    //std::cout << "SILENCE TEST: " << silenceTest << std::endl;
    if (silenceTest == 0)
        return penaltyFitness;
    else
        */
    return ret;
}

/*
    ===========
    FFT METHODS
    ===========
*/

unsigned GPExperiment::calculateFftBufferSize(unsigned numFrames, unsigned n, unsigned o) {
    unsigned numFftCalls = 0;
    unsigned shift = n - o;
    for (unsigned i = 0; i < numFrames;) {
        numFftCalls++;
        i += shift;
    }
    return numFftCalls * ((n/2) + 1);
}

void GPExperiment::FftReal(unsigned numFrames, const float* input, unsigned n, unsigned overlap, const float* window, kiss_fft_cpx* out, bool dB, double* magnitude, double* phase) {
    kiss_fftr_cfg cfg;
    cfg = kiss_fftr_alloc(n, 0/*is_inverse_fft*/, NULL, NULL);
    kiss_fft_scalar* in = (kiss_fft_scalar*) malloc(sizeof(kiss_fft_scalar) * n);

    unsigned fftOutputSize = (n/2 + 1);
    unsigned shift = n - overlap;
    int64 numCompleted = 0;
    int64 numRemaining = numFrames;
    int64 numFftOutputUsed = 0;
    while (numRemaining > 0) {
        // fill the input buffer
        unsigned numToTransform = numRemaining > n ? n : numRemaining;
        for (size_t i = 0; i < numToTransform; i++) {
            in[i] = input[numCompleted + i];
        }
        numCompleted += shift;
        numRemaining -= shift;
        // 0 out rest of input buffer if we're out of frames
        for (size_t i = numToTransform; i < n; i++) {
            in[i] = 0;
        }

        // apply window
        applyWindow(n, in, window);

        // perform fft
        kiss_fftr(cfg, in, out + numFftOutputUsed);

        // analyze output
        //printf("FREQ\t\tREAL\tIMAG\tMAG\tPHASE\n");
        for (size_t bin = numFftOutputUsed; bin < numFftOutputUsed + fftOutputSize; bin++) {
            if (dB)
                magnitude[bin] = 10 * log10(out[bin].r * out[bin].r + out[bin].i * out[bin].i) - dBRef;
            else
                magnitude[bin] = sqrt(out[bin].r * out[bin].r + out[bin].i * out[bin].i);
            if (out[bin].r == 0 && out[bin].i == 0) {
                phase[bin] = 0;
            }
            else {
                phase[bin] = atan(out[bin].i / out[bin].r);
            }
        //    printf("%.1lf\t\t%.2lf\t%.2lf\t%.2lf\t%.2lf\n", (44100.0 / n) * bin, out[bin].r, out[bin].i, magnitude[bin], phase[bin]);
            //std::cout << "BIN: " << bin << ", REAL: " << out[bin].r << ", IMAGINARY:" << out[bin].i << ", MAG: " << magnitude[bin] << ", PHASE: " << phase[bin] << std::endl;
        }
        numFftOutputUsed += fftOutputSize;
    }
    free(in);
    free(cfg);
}

/*
    ===================
    WAVEFORM OPERATIONS
    ===================
*/

void GPExperiment::window(const char* type, unsigned n, float* windowBuffer) {
    if (strcmp(type, "hann") == 0) {
        // 0.5 * (1 - cos(2*pi*n)/(N-1))
        double insideCosineValue = 0.0;
        double increment = (2 * M_PI)/(n - 1);
        for (unsigned i = 0; i < n; i++) {
            windowBuffer[i] = 0.5 * (1 - cos(insideCosineValue));
            insideCosineValue += increment;
        }
    }
    else if (strcmp(type, "rect") == 0) {
        for (unsigned i = 0; i < n; i++) {
            windowBuffer[i] = 1.0;
        }
    }
}

void GPExperiment::findMovingAverage(unsigned n, float* buffer, float* movingaverage, unsigned R) {
    unsigned D = 2 * R;
    assert(n > D);
    // based on http://radonc.wustl.edu/research/physics/5d/Papers/Lu_Medphys_Oct2006.pdf
    // TOP PART OF FORMULA FROM WEBSITE
    double firstRSum = 0.0;
    for (unsigned i = 0; i < R; i++) {
        firstRSum += buffer[i];
    }
    double firstRAvg = firstRSum / R;
    for (unsigned i = 0; i < R; i++) {
        firstRSum += buffer[i + R];
        movingaverage[i] = firstRSum / (i + R);
    }

    // MIDDLE PART OF FORMULA FROM WEBSITE
    double bufferIndexMinusRValue = buffer[0];
    double sumOfDiameter = 0.0;
    for (unsigned i = 0; i <= D; i++) {
        sumOfDiameter += buffer[i];
    }
    movingaverage[R] = sumOfDiameter / (D + 1);
    for (unsigned i = R + 1; i < n - R; i++) {
        sumOfDiameter = sumOfDiameter - bufferIndexMinusRValue;
        sumOfDiameter = sumOfDiameter + buffer[i + R];
        movingaverage[i] = sumOfDiameter / (D + 1);
        bufferIndexMinusRValue = buffer[i - R];
    }

    // BOTTOM PART OF FORMULA FROM WEBSITE
    for (unsigned i = n - R; i < n; i++) {
        sumOfDiameter = sumOfDiameter - bufferIndexMinusRValue;
        movingaverage[i] = sumOfDiameter / (R + n - i);
        bufferIndexMinusRValue = buffer[i - R];
    }
}

void GPExperiment::applyWindow(unsigned n, kiss_fft_scalar* buffer, const float* window) {
    for (unsigned i = 0; i < n; i++) {
        buffer[i] *= window[i];
    }
}

void GPExperiment::applyEnvelope(unsigned n, float* buffer, const float* envelope) {
  for (unsigned i = 0; i < n; i++) {
    buffer[i] *= envelope[i];
  }
}

void GPExperiment::applyEnvelope(unsigned n, const float* buffer, const float* envelope, float* envelopedBuffer) {
  for (unsigned i = 0; i < n; i++) {
    envelopedBuffer[i] = buffer[i] * envelope[i];
  }
}

// FROM: http://musicdsp.org/showArchiveComment.php?ArchiveID=136 
void GPExperiment::followEnvelope(unsigned n, float* buffer, float* envelope, double attack_in_ms, double release_in_ms, double samplerate) {
    double attack_coef = exp(log(0.01)/( attack_in_ms * samplerate * 0.001));
    double release_coef = exp(log(0.01)/( release_in_ms * samplerate * 0.001));
    
    double currentValue;
    envelope[0] = buffer[0];
    double currentEnvelope = envelope[0];
    for (unsigned i = 1; i < n; i++) {
        currentValue = fabs(buffer[i]);
        if (currentValue > currentEnvelope) {
            currentEnvelope = attack_coef * (currentEnvelope - currentValue) + currentValue;
        }
        else {
            currentEnvelope = release_coef * (currentEnvelope - currentValue) + currentValue;
        }
        envelope[i] = currentEnvelope;
    }
}

void GPExperiment::findEnvelope(bool ignoreZeroes, unsigned n, float* wav, float* env) {
    // MAKE AMPLITUDE ENVELOPE OF TARGET
    // x/y pairs for absolute waveform bound
    std::vector<unsigned> x;
    x.resize(0, 0);
    std::vector<float> y;
    y.resize(0, 0);

    // set initial value
    x.push_back(0);
    y.push_back(fabs(wav[0]));

    // find waveform minima/maxima
    float prevSlope = (wav[1] - wav[0]);
    float currSlope = 0;
    float slopeProduct = 0;
    for (unsigned i = 1; i < n - 2; i++) {
        currSlope = (wav[i + 1] - wav[i]);

        // if one slope is 0 we're at one edge of a plateau or silence
        float slopeProduct = currSlope * prevSlope;

        if (!ignoreZeroes) {
            if (slopeProduct == 0) {
                x.push_back(i);
                y.push_back(fabs(wav[i]));
            }
            // else if slope has changed we found a minimum or maximum
            else if (slopeProduct < 0 && prevSlope > 0) {
                x.push_back(i);
                y.push_back(fabs(wav[i]));
            }
        }
        else {
            //std::cout << i << ", " << n << std::endl;
            //std::cout << slopeProduct << std::endl;
            //std::cout << prevSlope << std::endl;
            if (slopeProduct < 0 && prevSlope > 0) {
                x.push_back(i);
                y.push_back(fabs(wav[i]));
            }
        }

        prevSlope = currSlope;
    }

    // set final value
    x.push_back(n - 1);
    y.push_back(fabs(wav[n - 1]));

    // fill env buffer
    for (unsigned i = 0; i < x.size() - 1; i++) {
        // calculate slope between points
        unsigned currFrameNumber = x[i];
        float currEnvValue = y[i];
        unsigned nextFrameNumber = x[i+1];
        float nextEnvValue = y[i+1];
        float slope = (nextEnvValue - currEnvValue)/(nextFrameNumber - currFrameNumber);

        // fill buffer from slope
        unsigned assignEnvelopeSample = currFrameNumber;
        while (assignEnvelopeSample < nextFrameNumber) {
            env[assignEnvelopeSample] = ((assignEnvelopeSample - currFrameNumber) * slope) + currEnvValue;
            assignEnvelopeSample++;
        }
    }
    env[n - 1] = wav[n - 1];
}

/*
    =============
    GRAPH HELPERS
    =============
*/

void GPExperiment::fillFrequencyAxisBuffer(unsigned fftSize, double sr, float* buffer) {
    for (unsigned i = 0; i < (fftSize/2) + 1; i++) {
        buffer[i] = (sr / fftSize) * i;
    }
}

String GPExperiment::floatBuffersToGraphText(String options, String xlab, String ylab, bool indexAsX, unsigned n, float* x, float* y) {
    String ret;
    ret += options;
    ret += "\n";
    ret += xlab;
    ret += "\t";
    ret += ylab;
    ret += "\n";
    for (unsigned i = 0; i < n; i++) {
        if (indexAsX)
            ret += String(i);
        else
            ret += String(x[i]);
        ret += "\t";
        ret += String(y[i]);
        ret += "\n";
    }
    return ret;
}
