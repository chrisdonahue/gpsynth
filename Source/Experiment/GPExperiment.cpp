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

GPExperiment::GPExperiment(GPRandom* rng, String target, GPParams* p, double* constants) :
    params(p),
    specialValues(constants),
    wavFormat(new WavAudioFormat())
{
    if (params->experimentNumber == 9) {
        // ASSIGN SPECIAL FITNESS VALUES
        p->bestPossibleFitness = 0;
        p->penaltyFitness = std::numeric_limits<float>::max();
        p->lowerFitnessIsBetter = true;

        unsigned fftSize = 1024;

        unsigned numSinSamples;
        unsigned numPianoSamples;
        double sinsr;
        double pnosr;
        String sinwave("./samples/SinWaveC51024.wav");
        String piano("./samples/PianoC51024.wav");
        getWavFileInfo(sinwave, &numSinSamples, &sinsr);
        getWavFileInfo(piano, &numPianoSamples, &pnosr);
        std::cout << "Sin sound has " << numSinSamples << " samples at a rate of " << sinsr << std::endl;
        std::cout << "Piano sound has " << numPianoSamples << " samples at a rate of " << pnosr << std::endl;

        float* sinFrames = (float*) malloc(sizeof(float) * numSinSamples);
        float* pnoFrames = (float*) malloc(sizeof(float) * numPianoSamples);
        loadWavFile(sinwave, numSinSamples, sinFrames);
        loadWavFile(piano, numPianoSamples, pnoFrames);

        unsigned fftOutputBufferSize = calculateFftBufferSize(numSinSamples, 1024);
        kiss_fft_cpx* output = (kiss_fft_cpx*) malloc(sizeof(kiss_fft_cpx) * fftOutputBufferSize);
        double* magnitude = (double*) malloc(sizeof(double) * fftOutputBufferSize);
        double* phase = (double*) malloc(sizeof(double) * fftOutputBufferSize);

        std::cout << "SIN FFT:" << std::endl;
        FftReal(numSinSamples, sinFrames, 1024, output, magnitude, phase);
        std::cout << "PNO FFT:" << std::endl;
        FftReal(numPianoSamples, pnoFrames, 1024, output, magnitude, phase);

        free(phase);
        free(magnitude);
        free(output);
        free(pnoFrames);
        free(sinFrames);
        exit(-1);
    }

    // TARGET DATA CONTAINERS
    wavFileBufferSize = p->wavFileBufferSize;
    getWavFileInfo(target, &numTargetFrames, &targetSampleRate);
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
    GPMutatableParam* filterCenterFrequencyMultiplierMin = new GPMutatableParam("filtercenterfrequencymin", true, 10.0, 1.0/specialValues[0], (targetSampleRate / 2)/specialValues[0]);
    GPMutatableParam* filterCenterFrequencyMultiplierMax = new GPMutatableParam("filtercenterfrequencymax", true, 10.0, 1.0/specialValues[0], (targetSampleRate / 2)/specialValues[0]);
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


    if (params->experimentNumber == 0) {
        // EVALUATE TARGET STRING
        std::string AMstring("(* (sin (* (* (time) (v0)) (* (2) (pi)))) (sin (* (time) (* (2) (pi)))))");
        GPNetwork* answer = new GPNetwork(params, AMstring);
        answer->traceNetwork();
        //std::cout << "Target network: " << answer->toString() << std::endl;
        double sampleRate = 44100.0;
        numTargetFrames = 88200;
        targetFrames = (float*) malloc(sizeof(float) * numTargetFrames);
        //renderIndividual(answer, numTargetFrames, targetFrames);
        //saveWavFile("./Answer.wav", String(answer->toString().c_str()), numTargetFrames, sampleRate, targetFrames);
        //loadTargetWavFile("./Answer.wav");
        delete answer;

        // ASSIGN SPECIAL FITNESS VALUES
        p->bestPossibleFitness = 0;
        p->penaltyFitness = std::numeric_limits<float>::max();
        p->lowerFitnessIsBetter = true;

        // SUPPLY AVAILABLE NODES
        nodes->push_back(new FunctionNode(multiply, NULL, NULL));
        //nodes->push_back(new OscilNode(oscilPartial->getCopy(), 0));
        //nodes->push_back(new OscilNode(oscilPartial->getCopy(), 1));
    }
    // AM 440 hz experiment easy
    if (params->experimentNumber == 1) {
        // ASSIGN SPECIAL FITNESS VALUES
        p->bestPossibleFitness = 0;
        p->penaltyFitness = std::numeric_limits<float>::max();
        p->lowerFitnessIsBetter = true;

        // SUPPLY AVAILABLE NODES
        nodes->push_back(new FunctionNode(multiply, NULL, NULL));
        //nodes->push_back(new OscilNode(oscilPartial->getCopy(), 0));
        //nodes->push_back(new OscilNode(oscilPartial->getCopy(), 1));
    }
    // AM 440 hz experiment hard
    if (params->experimentNumber == 2) {
        // ASSIGN SPECIAL FITNESS VALUES
        p->bestPossibleFitness = 0;
        p->penaltyFitness = std::numeric_limits<float>::max();
        p->lowerFitnessIsBetter = true;

        // SUPPLY AVAILABLE NODES
        nodes->push_back(new FunctionNode(multiply, NULL, NULL));
        nodes->push_back(new FunctionNode(sine, NULL, NULL));
        nodes->push_back(new ConstantNode(constantTwo->getCopy()));
        nodes->push_back(new ConstantNode(constantPi->getCopy()));
        nodes->push_back(new TimeNode());
        nodes->push_back(new VariableNode(0, 0, targetSampleRate/2));
    }
    // C4 Piano experiment hard
    if (params->experimentNumber == 3) {
        // ASSIGN SPECIAL FITNESS VALUES
        p->bestPossibleFitness = 0;
        p->penaltyFitness = std::numeric_limits<float>::max();
        p->lowerFitnessIsBetter = true;

        // SUPPLY AVAILABLE NODES
        nodes->push_back(new FunctionNode(add, NULL, NULL));
        nodes->push_back(new FunctionNode(multiply, NULL, NULL));
        nodes->push_back(new TimeNode());
        nodes->push_back(new VariableNode(0, 0, targetSampleRate/2));
        nodes->push_back(new ConstantNode(constantValue->getCopy()));
        nodes->push_back(new ModOscilNode(NULL, NULL));
        nodes->push_back(new NoiseNode(rng));
        //nodes->push_back(new FilterNode(0, 1, targetSampleRate, filterCenterFrequencyMin->getCopy(), filterCenterFrequencyMax->getCopy(), filterQualityMin->getCopy(), filterQualityMax->getCopy(), NULL, NULL, NULL));
        //nodes->push_back(new FilterNode(1, 1, targetSampleRate, filterCenterFrequencyMin->getCopy(), filterCenterFrequencyMax->getCopy(), filterQualityMin->getCopy(), filterQualityMax->getCopy(), NULL, NULL, NULL));
        //nodes->push_back(new FilterNode(2, 1, targetSampleRate, filterCenterFrequencyMin->getCopy(), filterCenterFrequencyMax->getCopy(), filterBandwidthMin->getCopy(), filterBandwidthMax->getCopy(), NULL, NULL, NULL));
        //nodes->push_back(new FilterNode(3, 1, targetSampleRate, filterCenterFrequencyMin->getCopy(), filterCenterFrequencyMax->getCopy(), filterBandwidthMin->getCopy(), filterBandwidthMax->getCopy(), NULL, NULL, NULL));
    }
    // Eb5 Trumpet Additive Experiment
    if (params->experimentNumber == 4) {
        // ASSIGN SPECIAL FITNESS VALUES
        p->bestPossibleFitness = 0;
        p->penaltyFitness = std::numeric_limits<float>::max();
        p->lowerFitnessIsBetter = true;

        // SUPPLY AVAILABLE NODES
        nodes->push_back(new FunctionNode(add, NULL, NULL));
        nodes->push_back(new FunctionNode(multiply, NULL, NULL));
        nodes->push_back(new ConstantNode(constantValue->getCopy(), params->valueNodeMinimum, params->valueNodeMaximum));
        nodes->push_back(new OscilNode(true, oscilPartial->getCopy(), 0, NULL, NULL));
        nodes->push_back(new OscilNode(false, oscilPartial->getCopy(), 0, oscilModIndex->getCopy(), NULL));
        nodes->push_back(new NoiseNode(rng));
        nodes->push_back(new FilterNode(2, 3, 1, targetSampleRate, 0, filterCenterFrequencyMultiplierMin->getCopy(), filterCenterFrequencyMultiplierMax->getCopy(), filterBandwidth->getCopy(), NULL, NULL, NULL));
        //nodes->push_back(new FilterNode(3, 3, 1, targetSampleRate, 0, filterCenterFrequencyMultiplierMin->getCopy(), filterCenterFrequencyMultiplierMax->getCopy(), filterBandwidth->getCopy(), NULL, NULL, NULL));
        nodes->push_back(new ADSRNode(true, targetSampleRate, ADSRDelay->getCopy(), ADSRAttack->getCopy(), ADSRAttackHeight->getCopy(), ADSRDecay->getCopy(), ADSRSustain->getCopy(), ADSRSustainHeight->getCopy(), ADSRRelease->getCopy(), NULL));
        nodes->push_back(new ADSRNode(false, targetSampleRate, ADSRDelay->getCopy(), ADSRAttack->getCopy(), ADSRAttackHeight->getCopy(), ADSRDecay->getCopy(), ADSRSustain->getCopy(), ADSRSustainHeight->getCopy(), ADSRRelease->getCopy(), NULL));
    }
    // filtered noise test
    if (params->experimentNumber == 5) {
        // ASSIGN SPECIAL FITNESS VALUES
        p->bestPossibleFitness = 0;
        p->penaltyFitness = std::numeric_limits<float>::max();
        p->lowerFitnessIsBetter = true;

        GPNode* noiseNode = new NoiseNode(rng);
        GPNode* constantNode = new ConstantNode(constantValue->getCopy(), params->valueNodeMinimum, params->valueNodeMaximum);
        GPNode* bandPass = new FilterNode(2, 3, 1, targetSampleRate, 0, filterCenterFrequencyMultiplierMin->getCopy(), filterCenterFrequencyMultiplierMax->getCopy(), filterBandwidth->getCopy(), noiseNode->getCopy(), constantNode->getCopy(), constantNode->getCopy());
        GPNode* bandStop = new FilterNode(3, 3, 1, targetSampleRate, 0, filterCenterFrequencyMultiplierMin->getCopy(), filterCenterFrequencyMultiplierMax->getCopy(), filterBandwidth->getCopy(), noiseNode->getCopy(), constantNode->getCopy(), constantNode->getCopy());

        GPNetwork* noiseNetwork = new GPNetwork(noiseNode);
        noiseNetwork->traceNetwork();
        GPNetwork* bandStopNoiseNetwork = new GPNetwork(bandStop);
        bandStopNoiseNetwork->ephemeralRandom(rng);
        bandStopNoiseNetwork->traceNetwork();
        GPNetwork* bandPassNoiseNetwork = new GPNetwork(bandPass);
        bandPassNoiseNetwork->ephemeralRandom(rng);
        bandPassNoiseNetwork->traceNetwork();

        float* noise = (float*) malloc(sizeof(float) * numTargetFrames);
        float* passNoise = (float*) malloc(sizeof(float) * numTargetFrames);
        float* stopNoise = (float*) malloc(sizeof(float) * numTargetFrames);
        renderIndividualByBlock(noiseNetwork, numTargetFrames, params->renderBlockSize, noise);
        renderIndividualByBlock(bandPassNoiseNetwork, numTargetFrames, params->renderBlockSize, passNoise);
        renderIndividualByBlock(bandStopNoiseNetwork, numTargetFrames, params->renderBlockSize, stopNoise);

        //saveWavFile("./noise.wav", String(noiseNetwork->toString().c_str()), numTargetFrames, 44100, noise);
        //saveWavFile("./stopNoise.wav", String(bandStopNoiseNetwork->toString().c_str()), numTargetFrames, 44100, stopNoise);
        //saveWavFile("./passNoise.wav", String(bandPassNoiseNetwork->toString().c_str()), numTargetFrames, 44100, passNoise);

        free(stopNoise);
        free(passNoise);
        free(noise);

        // SUPPLY AVAILABLE NODES
        nodes->push_back(new NoiseNode(rng));
        nodes->push_back(new FilterNode(2, 3, 1, targetSampleRate, 0, filterCenterFrequencyMultiplierMin->getCopy(), filterCenterFrequencyMultiplierMax->getCopy(), filterBandwidth->getCopy(), NULL, NULL, NULL));
        nodes->push_back(new FilterNode(3, 3, 1, targetSampleRate, 0, filterCenterFrequencyMultiplierMin->getCopy(), filterCenterFrequencyMultiplierMax->getCopy(), filterBandwidth->getCopy(), NULL, NULL, NULL));
        nodes->push_back(new ConstantNode(constantValue->getCopy(), params->valueNodeMinimum, params->valueNodeMaximum));
    }
    // filtered noise test
    if (params->experimentNumber == 5) {
    }
    if (params->experimentNumber == 10) {
        /*
        // ASSIGN SPECIAL FITNESS VALUES
        p->bestPossibleFitness = 0;
        p->penaltyFitness = std::numeric_limits<float>::max();
        p->lowerFitnessIsBetter = true;

        GPNode* noiseNode = new NoiseNode(rng);
        GPNode* constantNode = new ConstantNode(constantValue->getCopy());
        GPNode* filterNoiseNode = new FilterNode(0, 1, 1024, targetSampleRate, filterCenterFrequency->getCopy(), filterQuality->getCopy(), noiseNode);
        GPNode* filterSilenceNode = new FilterNode(0, 1, 1024, targetSampleRate, filterCenterFrequency->getCopy(), filterQuality->getCopy(), constantNode);

        GPNetwork* noiseNetwork = new GPNetwork(noiseNode);
        noiseNetwork->traceNetwork();
        GPNetwork* filteredNoiseNetwork = new GPNetwork(filterNoiseNode);
        filteredNoiseNetwork->traceNetwork();
        GPNetwork* filteredSilenceNetwork = new GPNetwork(filterSilenceNode);
        filteredSilenceNetwork->traceNetwork();

        float* noise = (float*) malloc(sizeof(float) * 88200);
        float* filtNoise = (float*) malloc(sizeof(float) * 88200);
        float* filtSilence = (float*) malloc(sizeof(float) * 88200);
        renderIndividualByBlock(noiseNetwork, 88200, params->renderBlockSize, noise);
        renderIndividualByBlock(filteredNoiseNetwork, 88200, params->renderBlockSize, filtNoise);
        renderIndividualByBlock(filteredSilenceNetwork, 88200, params->renderBlockSize, filtSilence);

        saveWavFile("./noise.wav", String(noiseNetwork->toString().c_str()), 88200, 44100, noise);
        saveWavFile("./lowpassnoise.wav", String(filteredNoiseNetwork->toString().c_str()), 88200, 44100, filtNoise);
        saveWavFile("./lowpasssilence.wav", String(filteredSilenceNetwork->toString().c_str()), 88200, 44100, filtSilence);

        free(filtSilence);
        free(filtNoise);
        free(noise);

        exit(0);
        */
    }

    // EXPERIMENT PARAMS THAT VARY BY EXPERIMENT NUMBER
    bestPossibleFitness = params->bestPossibleFitness;
    penaltyFitness = params->penaltyFitness;
    lowerFitnessIsBetter = params->lowerFitnessIsBetter;

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
    if (params->fitnessFunctionType == 1) {
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
    double generationMinimumFitness = INFINITY;

    while (minFitnessAchieved > fitnessThreshold && numEvaluatedGenerations < numGenerations) {
        GPNetwork* candidate = synth->getIndividual();

        float* candidateData = (float*) malloc(sizeof(float) * numTargetFrames);
        double fitness = suboptimize(candidate, numTargetFrames, candidateData);
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

        if (fitness > penaltyFitness) {
            penaltyFitness = fitness;
            params->penaltyFitness = penaltyFitness;
        }

        if (fitness < minFitnessAchieved) {
            minFitnessAchieved = fitness;
            delete champ;

            int backupID = candidate->ID;
            champ = candidate->getCopy();
            champ->ID = backupID;
            champ->fitness = minFitnessAchieved;
        }

        int numUnevaluatedThisGeneration = synth->assignFitness(candidate, fitness);
        if (numUnevaluatedThisGeneration == 0) {
            generationMinimumFitness = INFINITY;

            float* genchampbuffer = (float*) malloc(sizeof(float) * numTargetFrames);
            generationChamp->traceNetwork();
            renderIndividualByBlock(generationChamp, numTargetFrames, params->renderBlockSize, genchampbuffer);
            char buffer[100];
            snprintf(buffer, 100, "./gen.%d.best.wav", numEvaluatedGenerations);
            saveWavFile(String(buffer), String(generationChamp->toString(params->savePrecision).c_str()), numTargetFrames, targetSampleRate, genchampbuffer);
            free(genchampbuffer);
            delete generationChamp;
            generationChamp = NULL;

            numEvaluatedGenerations++;
        }

        free(candidateData);
    }

    std::cout << "-------------------------------- SUMMARY ---------------------------------" << std::endl;

    if (minFitnessAchieved <= fitnessThreshold) {
        std::cout << "Evolution found a synthesis algorithm at or below the specified fitness threshold" << std::endl;
    }
    // TODO: add decimal precision to numEvaluatedGeneratinos
    std::cout << "Evolution ran for " << numEvaluatedGenerations << " generations" << std::endl;
    if (champ != NULL) {
        float* champbuffer = (float*) malloc(sizeof(float) * numTargetFrames);
        champ->traceNetwork();
        renderIndividualByBlock(champ, numTargetFrames, params->renderBlockSize, champbuffer);
        std::cout << "The best synthesis algorithm found was number " << champ->ID << " with network " << champ->toString(params->printPrecision) << " and had a fitness of " << minFitnessAchieved << std::endl;
        saveWavFile("./champion.wav", String(champ->toString(params->savePrecision).c_str()), numTargetFrames, targetSampleRate, champbuffer);
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

    // FILL FREQUENCY SPECTRUM OF TARGET
    if (params->fitnessFunctionType > 0) {
        // calculate with fft window size
        unsigned n = params->fftSize;
        unsigned fftOutputBufferSize = calculateFftBufferSize(numTargetFrames, n);

        // allocate output buffers
        targetSpectrum = (kiss_fft_cpx*) malloc(sizeof(kiss_fft_cpx) * fftOutputBufferSize);
        targetSpectrumMagnitudes = (double*) malloc(sizeof(double) * fftOutputBufferSize);
        targetSpectrumPhases = (double*) malloc(sizeof(double) * fftOutputBufferSize);
        weightMatrix = (double*) malloc(sizeof(double) * fftOutputBufferSize);
        binOvershootingPenalty = (double*) malloc(sizeof(double) * fftOutputBufferSize);
        binUndershootingPenalty = (double*) malloc(sizeof(double) * fftOutputBufferSize);

        // take fft of target data
        FftReal(numTargetFrames, targetFrames, n, targetSpectrum, targetSpectrumMagnitudes, targetSpectrumPhases);

        // calculate stats on each frame
        double base = params->baseComparisonFactor;
        double good = params->goodComparisonFactor;
        double bad = params->badComparisonFactor;
        //std::cout << good << ", " << bad << std::endl;
        unsigned numBins = (n/2) + 1;
        unsigned numFftFrames = fftOutputBufferSize / numBins;
        for (unsigned i = 0; i < numFftFrames; i++) {
            // calculate frame average magnitude
            double sum = 0;
            double maxBin = std::numeric_limits<double>::min();
            double minBin = std::numeric_limits<double>::max();
            for (unsigned j = 0; j < numBins; j++) {
                double binMagnitude = targetSpectrumMagnitudes[(i * numBins) + j]; 
                sum += binMagnitude;
                if (binMagnitude > maxBin)
                    maxBin = binMagnitude;
                if (binMagnitude < minBin)
                    minBin = binMagnitude;
            }
            double frameAverageMagnitude = sum / ((double) numBins);
            //std::cout << i << ": [" << minBin << ", " << maxBin << "] " << frameAverageMagnitude << std::endl;

            // compare each bin to the average magnitude
            for (unsigned j = 0; j < numBins; j++) {
                unsigned binIndex = (i * numBins) + j;
                double binMagnitude = targetSpectrumMagnitudes[binIndex];

                // if we are above the mean penalize undershooting more
                if (binMagnitude > frameAverageMagnitude) {
                    double proportionOfMax = (binMagnitude - frameAverageMagnitude) / (maxBin - frameAverageMagnitude);
                    //std::cout << "ABOVE AVERAGE: " << j << ", " << proportionOfMax;
                    binUndershootingPenalty[binIndex] = (proportionOfMax * bad) + base;
                    binOvershootingPenalty[binIndex] = (proportionOfMax * good) + base;
                }

                // if we are below the mean penalize overshooting more
                else {
                    double proportionOfMin = (frameAverageMagnitude - binMagnitude) / (frameAverageMagnitude - minBin);
                    //std::cout << "BELOW AVERAGE: " << j << ", " << proportionOfMin;
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
    }
}

/*
    =============
    WAV INTERFACE
    =============
*/

void GPExperiment::getWavFileInfo(String path, unsigned* numFrames, double* sampleRate) {
    File input(path);
    assert(input.existsAsFile());
    FileInputStream* fis = input.createInputStream();
    ScopedPointer<AudioFormatReader> afr(wavFormat->createReaderFor(fis, true));

    // get info on target
    *numFrames = afr->lengthInSamples;
    *sampleRate = afr->sampleRate;
}

void GPExperiment::loadWavFile(String path, unsigned n, float* buffer) {
    File input(path);
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
    // TODO: gpsynth 0.1 replace with some controlled version string
    metaData = WavAudioFormat::createBWAVMetadata(desc, "gpsynth 0.1", "", Time::getCurrentTime(), int64(sampleRate), "JUCE");

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

/*
    ================
    FITNESS FUNCTION
    ================
*/

double GPExperiment::suboptimize(GPNetwork* candidate, int64 numSamples, float* buffer) {
    if (params->suboptimize) {
        // TODO: this
        std::vector<GPMutatableParam*>* candidateParams = candidate->getAllMutatableParams();
        return -1;
    }
    else {
        renderIndividualByBlock(candidate, numSamples, params->renderBlockSize, buffer);
        return compareToTarget(params->fitnessFunctionType, buffer);
    }
}

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

        unsigned fftOutputBufferSize = calculateFftBufferSize(numTargetFrames, n);
        kiss_fft_cpx* output = (kiss_fft_cpx*) malloc(sizeof(kiss_fft_cpx) * fftOutputBufferSize);
        double* magnitude = (double*) malloc(sizeof(double) * fftOutputBufferSize);
        double* phase = (double*) malloc(sizeof(double) * fftOutputBufferSize);

        FftReal(numTargetFrames, candidateFrames, n, output, magnitude, phase);

        double MSEmag = 0;
        double MSEph = 0;
        for (int i = 0; i < fftOutputBufferSize; i++) {
            if (magnitude[i] < targetSpectrumMagnitudes[i]) {
                MSEmag += pow(targetSpectrumMagnitudes[i] - magnitude[i], binUndershootingPenalty[i]);
            }
            else {
                MSEmag += pow(magnitude[i] - targetSpectrumMagnitudes[i], binOvershootingPenalty[i]);
            }
            //MSEmag += pow(abs(magnitude[i] - targetSpectrumMagnitudes[i]), params->penalizeBadMagnitude);
            MSEph += pow(abs(phase[i] - targetSpectrumPhases[i]), params->penalizeBadPhase);
        }
        MSEmag = MSEmag / n;
        MSEph = MSEph / n;
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

unsigned GPExperiment::calculateFftBufferSize(unsigned numFrames, unsigned n) {
    unsigned numFftCalls = numFrames / n;
    if (numFrames % n > 0)
        numFftCalls++;
    return numFftCalls * ((n/2) + 1);
}

void GPExperiment::FftReal(unsigned numFrames, const float* input, unsigned n, kiss_fft_cpx* out, double* magnitude, double* phase) {
    kiss_fftr_cfg cfg;
    cfg = kiss_fftr_alloc(n, 0/*is_inverse_fft*/, NULL, NULL);
    kiss_fft_scalar* in = (kiss_fft_scalar*) malloc(sizeof(kiss_fft_scalar) * n);

    unsigned fftOutputSize = (n/2 + 1);
    int64 numCompleted = 0;
    int64 numRemaining = numFrames;
    int64 numFftOutputUsed = 0;
    while (numRemaining > 0) {
        // fill the input buffer
        unsigned numToTransform = numRemaining > n ? n : numRemaining;
        for (size_t i = 0; i < numToTransform; i++) {
            in[i] = input[numCompleted];
            numCompleted++;
            numRemaining--;
        }
        // 0 out rest of input buffer if we're out of frames
        for (size_t i = numToTransform; i < n; i++) {
            in[i] = 0;
        }

        // perform fft
        kiss_fftr(cfg, in, out + numFftOutputUsed);

        // analyze output
        //printf("FREQ\t\tREAL\tIMAG\tMAG\tPHASE\n");
        for (size_t bin = numFftOutputUsed; bin < numFftOutputUsed + fftOutputSize; bin++) {
            magnitude[bin] = sqrt(out[bin].r * out[bin].r + out[bin].i * out[bin].i);
            if (out[bin].r == 0 && out[bin].i == 0) {
                phase[bin] = 0;
            }
            else {
                phase[bin] = atan(out[bin].i / out[bin].r);
            }
            //printf("%.1lf\t\t%.2lf\t%.2lf\t%.2lf\t%.2lf\n", (44100.0 / n) * bin, out[bin].r, out[bin].i, magnitude[bin], phase[bin]);
            //std::cout << "BIN: " << bin << ", REAL: " << out[bin].r << ", IMAGINARY:" << out[bin].i << ", MAG: " << magnitude[bin] << ", PHASE: " << phase[bin] << std::endl;
        }
        numFftOutputUsed += fftOutputSize;
    }
    free(in);
    free(cfg);
}

void GPExperiment::FftReal(unsigned n, const kiss_fft_scalar* in, kiss_fft_cpx* out, double* magnitude, double* phase)
{
    kiss_fftr_cfg cfg;

    // take FFT of input data and put it in output
    cfg = kiss_fftr_alloc(n, 0/*is_inverse_fft*/, NULL, NULL);
    kiss_fftr(cfg, in, out);
    free(cfg);

    // analyze output and fill magnitude and phase info
    for (size_t bin = 0; bin < (n/2 + 1); bin++) {
        double binmag = sqrt(pow(out[bin].r, 2) + pow(out[bin].i, 2));
        double binphase;
        if (out[bin].i == 0 && out[bin].r == 0) {
            binphase = 0;
        }
        else {
            binphase = atan(out[bin].i / out[bin].r);
        }
        magnitude[bin] = binmag;
        phase[bin] = binphase;
    }
}
