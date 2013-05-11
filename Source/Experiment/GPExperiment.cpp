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

GPExperiment::GPExperiment(GPParams* p, GPRandom* rng, unsigned s, String target, String path, float* constants, bool* rq) :
    params(p), seed(s), targetPath(target), savePath(path),
    constantValues(constants),
    requestedQuit(rq),
    dBRef(dbRef)
{
    // AUDIO SANITY TEST
    if (params->experimentNumber == 0) {
        synth = NULL;
        sanityTest(rng);
    }

    // EXPERIMENT
    else {
        // TARGET DATA CONTAINERS
        getWavFileInfo(target, &numTargetFrames, &targetSampleRate);
        targetNyquist = targetSampleRate / 2;
        targetFrames = (float*) malloc(sizeof(float) * numTargetFrames);
        targetLengthSeconds = numTargetFrames / targetSampleRate;
        loadWavFile(target, params->wavFileBufferSize, numTargetFrames, targetFrames);
        if (params->backupTarget)
            saveWavFile(savePath + String("targetcopy.wav"), String(""), String("target"), targetSampleRate, params->wavFileBufferSize, numTargetFrames, targetFrames);

        fillEvaluationBuffers(params->numConstantValues, constantValues, 0, NULL);

        // EXPERIMENT PARAMETERS THAT USE SAMPLE RATE
        params->delayNodeMaxBufferSize = params->delayNodeBufferMaxSeconds * targetSampleRate;

        // EXPERIMENT STATE
        numGenerations = p->numGenerations;
        fitnessThreshold = p->thresholdFitness;
        minFitnessAchieved = INFINITY;
        numEvaluatedGenerations = 0;

        // AVAILABLE SYNTH NODES
        std::vector<GPNode*>* nodes = new std::vector<GPNode*>();

        // MUTATABLE PARAMS
        GPMutatableParam* constantValue = new GPMutatableParam("constantvalue", true, 0.0, params->valueNodeMinimum, params->valueNodeMaximum);
        GPMutatableParam* constantTwo = new GPMutatableParam("two", false, 2.0f, 0.0f, 0.0f);
        GPMutatableParam* constantPi = new GPMutatableParam("pi", false, (float) M_PI, 0.0f, 0.0f);

        GPMutatableParam* oscilPartial = new GPMutatableParam("oscilpartial", true, 1, 1, params->oscilNodeMaxPartial);
        GPMutatableParam* oscilModIndex = new GPMutatableParam("oscilmodindex", true, 1.0, params->oscilNodeMinIndexOfModulation, params->oscilNodeMaxIndexOfModulation);

        GPMutatableParam* filterCenterFrequencyMultiplierMin = new GPMutatableParam("filtercenterfrequencymin", true, 1.0, params->filterNodeCenterFrequencyMinimum/constantValues[0], (targetNyquist * params->filterNodeCenterFrequencyMaximumProportionOfNyquist)/constantValues[0]);
        GPMutatableParam* filterCenterFrequencyMultiplierMax = new GPMutatableParam("filtercenterfrequencymax", true, 1.0, params->filterNodeCenterFrequencyMinimum/constantValues[0], (targetNyquist * params->filterNodeCenterFrequencyMaximumProportionOfNyquist)/constantValues[0]);
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

        GPMutatableParam* specialValues = new GPMutatableParam("special", false, 0, 0, params->numConstantValues);

        // CURRENT DEFAULT EXPERIMENT
        if (params->experimentNumber == 1) {
            // SUPPLY AVAILABLE NODES
            //nodes->push_back(new FunctionNode(add, NULL, NULL));
            //nodes->push_back(new FunctionNode(multiply, NULL, NULL));
            nodes->push_back(new ConstantNode(true, false, constantValue->getCopy(), NULL));
            nodes->push_back(new OscilNode(true, specialValues->getCopy(), oscilPartial->getCopy(), NULL, NULL));
            nodes->push_back(new OscilNode(false, specialValues->getCopy(), oscilPartial->getCopy(), oscilModIndex->getCopy(), NULL));
            nodes->push_back(new NoiseNode(rng));
            //nodes->push_back(new FilterNode(2, 3, params->renderBlockSize, targetSampleRate, 0, filterCenterFrequencyMultiplierMin->getCopy(), filterCenterFrequencyMultiplierMax->getCopy(), filterBandwidth->getCopy(), NULL, NULL, NULL));
            //nodes->push_back(new FilterNode(3, 3, params->renderBlockSize, targetSampleRate, 0, filterCenterFrequencyMultiplierMin->getCopy(), filterCenterFrequencyMultiplierMax->getCopy(), filterBandwidth->getCopy(), NULL, NULL, NULL));
            nodes->push_back(new ADSRNode(true, ADSRDelay->getCopy(), ADSRAttack->getCopy(), ADSRAttackHeight->getCopy(), ADSRDecay->getCopy(), ADSRSustain->getCopy(), ADSRSustainHeight->getCopy(), ADSRRelease->getCopy(), NULL));
            nodes->push_back(new ADSRNode(false, ADSRDelay->getCopy(), ADSRAttack->getCopy(), ADSRAttackHeight->getCopy(), ADSRDecay->getCopy(), ADSRSustain->getCopy(), ADSRSustainHeight->getCopy(), ADSRRelease->getCopy(), NULL));
        }
        // TESTING ENVELOPE FUNCTIONS ON INPUTS OF AN LENGTH
        if (params->experimentNumber == 10) {
            // SAVE WAVEFORM
            //saveTextFile(String("./waveform.txt"), floatBuffersToGraphText(String("x> y^ xi yf"), String("Sample #"), String("Magnitude (amp)"), true, numTargetFrames, NULL, targetFrames));

            // TEST ENVELOPE FOLLOWER
            float* envelope = (float*) malloc(sizeof(float) * numTargetFrames);
            float* smoothEnv = (float*) malloc(sizeof(float) * numTargetFrames);
            followEnvelope(numTargetFrames, targetFrames, envelope, 1, 300, targetSampleRate);
            findEnvelope(true, numTargetFrames, envelope, smoothEnv);
            saveTextFile(String("./smoothenva1r300.txt"), floatBuffersToGraphText(String("x> y^ xi yf"), String("Sample #"), String("Envelope (amp)"), true, numTargetFrames, NULL, smoothEnv, NULL));

            free(smoothEnv);
            free(envelope);
            exit(-1);
        }
        // TESTING VARIOUS WAVEFORM FUNCTIONS ON INPUTS WITH 1024 SAMPLES
        if (params->experimentNumber == 11) {
            // SAVE WAVEFORM
            saveTextFile(String("./waveform.txt"), floatBuffersToGraphText(String("x> y^ xi yf"), String("Sample #"), String("Magnitude (amp)"), true, numTargetFrames, nullptr, targetFrames, NULL));

            // TEST FFT
            unsigned fftOutSize = 513;
            unsigned fftSize = 1024;
            float* freqAxis = (float*) malloc(sizeof(float) * fftOutSize);
            fillFrequencyAxisBuffer(fftSize, targetSampleRate, freqAxis);

            float* magAxis = (float*) malloc(sizeof(float) * fftOutSize);
            for (unsigned i = 0; i < fftOutSize; i++) {
                magAxis[i] = (float) targetSpectrumMagnitudes[i];
            }
            
            saveTextFile(String("./spectrum.txt"), floatBuffersToGraphText(String("x> y^ xf yf"), String("Frequency (Hz)"), String("Magnitude (amp)"), false, fftOutSize, freqAxis, magAxis, NULL));

            // TEST WINDOWING
            float* hannWindow = (float*) malloc(sizeof(float) * numTargetFrames);
            float* windowed = (float*) malloc(sizeof(float) * numTargetFrames);
            window("hann", numTargetFrames, hannWindow);
            applyEnvelope(numTargetFrames, targetFrames, hannWindow, windowed);

            saveTextFile(String("./windowed.txt"), floatBuffersToGraphText(String("x> y^ xi yf"), String("Sample #"), String("Magnitude (amp)"), true, numTargetFrames, nullptr, windowed, NULL));

            // TEST MOVING AVERAGE
            float* mac = (float*) malloc(sizeof(float) * fftOutSize);
            //findMovingAverage(fftOutSize, magAxis, mac, 10);
            saveTextFile(String("./mac10.txt"), floatBuffersToGraphText(String("x> y^ xf yf"), String("Frequency (Hz)"), String("Power (dB)"), false, fftOutSize, freqAxis, mac, NULL));
            //findMovingAverage(fftOutSize, magAxis, mac, 20);
            saveTextFile(String("./mac20.txt"), floatBuffersToGraphText(String("x> y^ xf yf"), String("Frequency (Hz)"), String("Power (dB)"), false, fftOutSize, freqAxis, mac, NULL));
            //findMovingAverage(fftOutSize, magAxis, mac, 30);
            saveTextFile(String("./mac30.txt"), floatBuffersToGraphText(String("x> y^ xf yf"), String("Frequency (Hz)"), String("Power (dB)"), false, fftOutSize, freqAxis, mac, NULL));

            // FREE AND DONT EVOLVE
            free(mac);
            free(windowed);
            free(hannWindow);
            free(magAxis);
            free(freqAxis);
            exit(-1);
        }

        // set parameters that vary by fitness function
        if (params->fitnessFunctionType == 0) {
            p->bestPossibleFitness = 0;
            p->lowerFitnessIsBetter = true;
            lowerFitnessIsBetter = params->lowerFitnessIsBetter;
        }
        else {
            p->bestPossibleFitness = 0;
            p->lowerFitnessIsBetter = true;
            lowerFitnessIsBetter = params->lowerFitnessIsBetter;
        }
        
        // create population
        synth = new GPSynth(p, rng, nodes);

        // DELETE MUTATABLE PARAMS
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
}

GPExperiment::~GPExperiment() {
    if (params->experimentNumber != 0) {
        free(targetFrames);
        free(targetEnvelope);
        free(targetSampleTimes);
        if (params->fitnessFunctionType == 1) {
            free(analysisWindow);
            free(targetSpectrum);
            free(targetSpectrumMagnitudes);
            free(targetSpectrumPhases);
            free(binOvershootingPenalty);
            free(binUndershootingPenalty);
            free(fftFrameWeight);
        }
        delete synth;
    }
}

/*
    =================
    EVOLUTION CONTROL
    =================
*/

GPNetwork* GPExperiment::evolve() {
    // if we're sanity testing
    if (params->experimentNumber == 0) {
        return NULL;
    }

    // temp fields for evolution
    unsigned champGeneration = 0;
    int numEvaluatedThisGeneration = 0;
    int numUnevaluatedThisGeneration = 0;

    // temp buffers for rendering
    float* candidateData = (float*) malloc(sizeof(float) * numTargetFrames);
    float* champBuffer = (float*) malloc(sizeof(float) * numTargetFrames);

    while (minFitnessAchieved > fitnessThreshold && numEvaluatedGenerations < numGenerations && !(*requestedQuit)) {
        // render candidate
        GPNetwork* candidate = synth->getIndividual();
        candidate->prepareToRender(targetSampleRate, params->renderBlockSize, targetLengthSeconds);
        suboptimizeAndCompareToTarget(params->suboptimizeType, candidate, candidateData);
        candidate->doneRendering();
        
        // evaluate candidate
        double fitness = compareToTarget(params->fitnessFunctionType, candidateData);
        numUnevaluatedThisGeneration = synth->assignFitness(candidate, fitness);
        numEvaluatedThisGeneration++;

        // if we're done with this generation...
        if (numUnevaluatedThisGeneration == 0) {
            // grab and render the generation champ
            GPNetwork* generationChamp = synth->generationChamp;
            generationChamp->prepareToRender(targetSampleRate, params->renderBlockSize, targetLengthSeconds);
            renderIndividualByBlockPerformance(generationChamp, params->renderBlockSize, numConstantValues, constantValues, numTargetFrames, targetSampleTimes, champBuffer);
            generationChamp->doneRendering();

            // save generation champions
            if (params->saveGenerationChampions) {
              char buffer[100];
              snprintf(buffer, 100, "%d.gen.%d.best.wav", seed, numEvaluatedGenerations);
              saveWavFile(savePath + String(buffer), String(generationChamp->toString(params->savePrecision).c_str()), String(generationChamp->origin.c_str()), targetSampleRate, params->wavFileBufferSize, numTargetFrames, champBuffer);
            }

            // increment number of evaluted generations
            numEvaluatedGenerations++;
            numEvaluatedThisGeneration = 0;
        }
    }

    free(candidateData);

    // print evolution summary
    std::cerr << "-------------------------------- SUMMARY ---------------------------------" << std::endl;

    // print a message if we met the threshold
    if (minFitnessAchieved <= fitnessThreshold) {
        std::cerr << "Evolution found a synthesis algorithm at or below the specified fitness threshold" << std::endl;
    }

    // print the number of generations evolution ran for
    if (numUnevaluatedThisGeneration != 0)
        std::cerr << "Evolution ran for " << numEvaluatedGenerations + (params->populationSize - numUnevaluatedThisGeneration)/float(params->populationSize) << " generations" << std::endl;
    else
        std::cerr << "Evolution ran for " << numEvaluatedGenerations << " generations" << std::endl;

    // render the champion
    GPNetwork* champ = synth->champ;
    if (champ != NULL) {
        champ->prepareToRender(targetSampleRate, params->renderBlockSize, targetLengthSeconds);
        renderIndividualByBlockPerformance(champ, params->renderBlockSize, numConstantValues, constantValues, numTargetFrames, targetSampleTimes, champBuffer);
        champ->doneRendering();
        std::cerr << "The best synthesis algorithm found was number " << champ->ID << " from generation " << champ->ID/params->populationSize << " made by " << champ->origin << " with height " << champ->height << ", fitness " << champ->fitness << " and structure " << champ->toString(params->savePrecision) << " and had a fitness of " << minFitnessAchieved << std::endl;
        char buffer[100];
        snprintf(buffer, 100, "%d.champion.wav", seed);
        saveWavFile(savePath + String(buffer), String(champ->toString(params->savePrecision).c_str()), String(champ->origin.c_str()), targetSampleRate, params->wavFileBufferSize, numTargetFrames, champBuffer);
    }
    free(champBuffer);
    return champ;
}

/*
    =======================
    FILL EVALUATION BUFFERS
    =======================
*/

// PRECONDITIONS:
// NUMTARGETFRAMES, TARGETSAMPLERATE, TARGETFRAMES ALL FILLED IN
void GPExperiment::fillEvaluationBuffers(unsigned numconstantvalues, float* constantvalues, unsigned numvariablevalues, float* variablevalues) {
    // FILL BUFFERS WITH SPECIAL VALUES
    targetSampleTimes = (float*) malloc(sizeof(double) * numTargetFrames);
    fillTimeAxisBuffer(numTargetFrames, targetSampleRate, targetSampleTimes);
    /*
    numSpecialValues = numConstantSpecialValues + numVariableSpecialValues;
    specialValuesByFrame = (double*) malloc(sizeof(double) * numTargetFrames * numSpecialValues);
    for (int frame = 0; frame < numTargetFrames; frame++) {
        sampleTimes[frame] = frame/targetSampleRate;
        for (int val = 0; val < numConstantSpecialValues; val++) {
            *(specialValuesByFrame + (frame * numSpecialValues) + val) = constantSpecialValues[val];
        }
        for (int val = 0; val < numVariableSpecialValues; val++) {
            *(specialValuesByFrame + (frame * numSpecialValues) + numConstantSpecialValues + val) = variableSpecialValues[val]; // TODO: RHS of this assignment is placeholder
        }
    }
    */

    // FILL ENVELOPE OF TARGET BUFFER
    targetEnvelope = (float*) malloc(sizeof(float) * numTargetFrames);
    followEnvelope(numTargetFrames, targetFrames, targetEnvelope, params->envelopeFollowerAttack, params->envelopeFollowerDecay, targetSampleRate);
    if (params->saveTargetEnvelope) {
        char buffer[100];
        snprintf(buffer, 100, "targetInfo/targetenvelope.txt");
        saveTextFile(savePath + String(buffer), floatBuffersToGraphText(String("x> y^ xi yf"), String("Sample Number"), String("Amplitude"), true, numTargetFrames, NULL, targetEnvelope, NULL));
    }

    // FILL FREQUENCY SPECTRUM OF TARGET
    if (params->fitnessFunctionType > 0) {
        // calculate with fft window size/overlap
        unsigned n = params->fftSize;
        unsigned overlap = params->fftOverlap;
        unsigned fftOutputBufferSize = calculateFftBufferSize(numTargetFrames, n, overlap);
        unsigned numBins = (n/2) + 1;
        unsigned numFftFrames = fftOutputBufferSize / numBins;
        
        // allocate window
        analysisWindow = (float*) malloc(sizeof(float) * n);
        window(params->windowType, n, analysisWindow);

        // allocate output buffers
        targetSpectrum = (kiss_fft_cpx*) malloc(sizeof(kiss_fft_cpx) * fftOutputBufferSize);
        targetSpectrumMagnitudes = (double*) malloc(sizeof(double) * fftOutputBufferSize);
        targetSpectrumPhases = (double*) malloc(sizeof(double) * fftOutputBufferSize);
        binOvershootingPenalty = (double*) malloc(sizeof(double) * fftOutputBufferSize);
        binUndershootingPenalty = (double*) malloc(sizeof(double) * fftOutputBufferSize);
        fftFrameWeight = (double*) malloc(sizeof(double) * numFftFrames);

        // take fft of target data
        FftReal(numTargetFrames, targetFrames, n, overlap, analysisWindow, targetSpectrum, params->dBMagnitude, dBRef, targetSpectrumMagnitudes, targetSpectrumPhases);

        // allocate buffers
        double* timeAxis = (double*) malloc(sizeof(double) * (numBins - 1));
        double* mac = (double*) malloc(sizeof(double) * (numBins - 1));
        float* floatFreqAxis = (float*) malloc(sizeof(float) * numBins);
        fillFrequencyAxisBuffer(n, targetSampleRate, floatFreqAxis);
        double* freqAxis = (double*) malloc(sizeof(double) * (numBins));
        for (unsigned i = 0; i < numBins; i++) {
            freqAxis[i] = floatFreqAxis[i];
        }
        free(floatFreqAxis);

        // calculate penalties for each frame
        double base = params->baseComparisonFactor;
        double good = params->goodComparisonFactor;
        double bad = params->badComparisonFactor;
        double frameAverageSum = 0.0;
        for (unsigned i = 0; i < numFftFrames; i++) {
            // find moving average
            unsigned dataOffset = (i * numBins) + 1;
            double frameAverage, maxDeviationAboveMean, maxDeviationBelowMean, maxRatioAboveMean, maxRatioBelowMean;
            findMovingAverage(params->averageComparisonType, numBins - 1, targetSpectrumMagnitudes + dataOffset, mac, params->movingAveragePastRadius, params->movingAverageFutureRadius, params->exponentialMovingAverageAlpha, &frameAverage, &maxDeviationAboveMean, &maxDeviationBelowMean, &maxRatioAboveMean, &maxRatioBelowMean);
            fftFrameWeight[i] = frameAverage;
            frameAverageSum += frameAverage;

            // compare each bin EXCEPT DC OFFSET to the moving average magnitude
            for (unsigned j = 1; j < numBins; j++) {
                unsigned binIndex = (i * numBins) + j;
                double binMagnitude = targetSpectrumMagnitudes[binIndex];
                double binAverage = mac[j - 1];
                
                // calculate bin comparison metric data
                double numerator, denominator, proportion;
                if (params->compareToMaxDeviation) {
                    numerator = fabs(binMagnitude - binAverage);
                    if (binMagnitude > binAverage) {
                        denominator = maxDeviationAboveMean;
                    }
                    else {
                        denominator = maxDeviationBelowMean;
                    }
                }
                else {
                    if (binMagnitude > binAverage) {
                        numerator = fabs(binAverage / binMagnitude);
                        denominator = maxRatioAboveMean;
                    }
                    else {
                        double binAbsMagnitude = (binAverage - binMagnitude) + binAverage;
                        numerator = fabs(binAverage / binAbsMagnitude);
                        denominator = maxRatioBelowMean;
                    }
                }

                // calculate bin proportion for penalty assignments
                proportion = pow(numerator / denominator, params->penaltyComparisonExponent);

                // check to make sure proportions are correct
                assert(proportion <= 1);

                // if we are above the mean penalize undershooting more
                if (binMagnitude > binAverage) {
                    //double proportionOfMax = log((binMagnitude - binAverage)) / log(maxDeviationAboveMean);
                    binUndershootingPenalty[binIndex] = (proportion * bad) + base;
                    binOvershootingPenalty[binIndex] = (proportion * good) + base;
                }

                // if we are below the mean penalize overshooting more
                else {
                    //double proportionOfMin = log((binAverage - binMagnitude)) / log(maxDeviationBelowMean);
                    binUndershootingPenalty[binIndex] = (proportion * good) + base;
                    binOvershootingPenalty[binIndex] = (proportion * bad) + base;
                }
            }

            // save spectrum files if requested
            if (params->saveTargetSpectrum) {
                char buffer[200];
                snprintf(buffer, 200, "targetInfo/%d.", i);
                String tag(buffer);
                double time = i * (n - overlap);
                time = double(time)/targetSampleRate;
                for (unsigned j = 0; j < numBins - 1; j++) {
                    timeAxis[j] = time;
                }
                saveTextFile(savePath + tag + String("magnitude.txt"), doubleBuffersToGraphText(String(overlap), String(""), String(""), String(""), false, numBins - 1, timeAxis, freqAxis + 1, targetSpectrumMagnitudes + dataOffset));
                saveTextFile(savePath + tag + String("movingAverage.txt"), doubleBuffersToGraphText(String(""), String(""), String(""), String(""), false, numBins - 1, freqAxis + 1, mac, NULL));
                saveTextFile(savePath + tag + String("undershootPenalty.txt"), doubleBuffersToGraphText(String(""), String(""), String(""), String(""), false, numBins - 1, freqAxis + 1, targetSpectrumMagnitudes + dataOffset, binUndershootingPenalty + dataOffset));
                saveTextFile(savePath + tag + String("overshootPenalty.txt"), doubleBuffersToGraphText(String(""), String(""), String(""), String(""), false, numBins - 1, freqAxis + 1, targetSpectrumMagnitudes + dataOffset, binOvershootingPenalty + dataOffset));
            }
        }
        // free temp buffers
        free(freqAxis);
        free(mac);
        free(timeAxis);
        
        // calculate frame weights
        double frameAverage = frameAverageSum / double(numFftFrames);
        if (params->weightFftFrames) {
            //std::cerr << "average: " << frameAverage << std::endl;
            for (unsigned i = 0; i < numFftFrames; i++) {
                fftFrameWeight[i] = frameAverage / fftFrameWeight[i];
                //std::cerr << i << " weight: " << fftFrameWeight[i] << std::endl;
                fftFrameWeight[i] = pow(fftFrameWeight[i], params->frameWeightExponent);
                //std::cerr << i << " power weight: " << fftFrameWeight[i] << std::endl;
            }
        }
        else {
            for (unsigned i = 0; i < numFftFrames; i++) {
                fftFrameWeight[i] = 1.0;
            }
        }
    }
}

/*
    ================
    FITNESS FUNCTION
    ================
*/

double GPExperiment::suboptimizeAndCompareToTarget(unsigned suboptimizeType, GPNetwork* candidate, float* buffer) {
    // suboptimize according to suboptimize type
    if (suboptimizeType == 0) {
        renderIndividualByBlockPerformance(candidate, params->renderBlockSize, numConstantValues, constantValues, numTargetFrames, targetSampleTimes, buffer);
    }
    return -1;
}

// TODO: change numconstantvariables to numconstantvalues
void GPExperiment::renderIndividualByBlockPerformance(GPNetwork* candidate, unsigned renderblocksize, unsigned numconstantvariables, float* constantvariables, int64 numsamples, float* sampletimes, float* buffer) {
    int64 numRemaining = numsamples;
    int64 numCompleted = 0;
    int64 bufferIndex = 0;
    unsigned numToRender;
    while (numRemaining > 0) {
        numToRender = renderblocksize < numRemaining ? renderblocksize : numRemaining;
        candidate->evaluateBlockPerformance(numCompleted, numToRender, sampletimes + numCompleted, numconstantvariables, constantvariables, buffer + numCompleted);
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

        FftReal(numTargetFrames, candidateFrames, n, overlap, analysisWindow, output, params->dBMagnitude, dBRef, magnitude, phase);

        double magnitudeError = 0;
        double phaseError = 0;
        unsigned numBins = (n/2) + 1;
        unsigned numFftFrames = fftOutputBufferSize / numBins;
        for (unsigned i = 0; i < numFftFrames; i++) {
            double frameMagnitudeError = 0;
            double framePhaseError = 0;
            unsigned frameIndex = (i * numBins);
            for (unsigned j = 1; j < numBins; j++) {
                unsigned binIndex = frameIndex + j;
                if (magnitude[binIndex] < targetSpectrumMagnitudes[binIndex]) {
                    frameMagnitudeError += pow(targetSpectrumMagnitudes[binIndex] - magnitude[binIndex], binUndershootingPenalty[binIndex]);
                }
                else {
                    frameMagnitudeError += pow(magnitude[binIndex] - targetSpectrumMagnitudes[binIndex], binOvershootingPenalty[binIndex]);
                }
                framePhaseError += pow(fabs(phase[binIndex] - targetSpectrumPhases[binIndex]), params->penalizeBadPhase);
            }
            magnitudeError += fftFrameWeight[i] * frameMagnitudeError;
            //std::cerr << i << ": " << fftFrameWeight[i] << ", " << frameMagnitudeError << ", " << magnitudeError << std::endl;
            phaseError += framePhaseError;
        }
        ret = params->magnitudeWeight * magnitudeError + params->phaseWeight * phaseError;
        free(phase);
        free(magnitude);
        free(output);
    }
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

void GPExperiment::FftReal(unsigned numFrames, const float* input, unsigned n, unsigned overlap, const float* window, kiss_fft_cpx* out, bool dB, float dBref, double* magnitude, double* phase) {
    kiss_fftr_cfg cfg;
    // TODO: we shouldnt need to continuously realloc this
    cfg = kiss_fftr_alloc(n, 0/*is_inverse_fft*/, NULL, NULL);
    kiss_fft_scalar in[n];

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
        if (dB) {
            for (size_t bin = numFftOutputUsed; bin < numFftOutputUsed + fftOutputSize; bin++) {
                magnitude[bin] = 10 * log10(out[bin].r * out[bin].r + out[bin].i * out[bin].i) - dBref;
                if (out[bin].r == 0 && out[bin].i == 0) {
                    phase[bin] = 0;
                }
                else {
                    phase[bin] = atan(out[bin].i / out[bin].r);
                }
            }
        }
        else {
            for (size_t bin = numFftOutputUsed; bin < numFftOutputUsed + fftOutputSize; bin++) {
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
        }
        numFftOutputUsed += fftOutputSize;
    }
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

void GPExperiment::findMovingAverage(unsigned type, unsigned n, const double* buffer, double* movingaverage, unsigned pastRadius, unsigned futureRadius, double alpha, double* frameaverage, double* maxdeviationabove, double* maxdeviationbelow, double* maxratioabove, double* maxratiobelow) {
    // NON-MOVING AVERAGE
    if (type == 0) {
        double sum = 0;
        double max = std::numeric_limits<double>::min();
        double min = std::numeric_limits<double>::max();
        // EXCLUDE DC OFFSET
        for (unsigned i = 0; i < n; i++) {
            double magnitude = buffer[i];
            sum += magnitude;
            if (magnitude > max)
                max = magnitude;
            if (magnitude < min)
                min = magnitude;
        }
        double average = sum / ((double) n);
        for (unsigned i = 0; i < n; i++) {
            movingaverage[i] = average;
        }
        *maxdeviationabove = max - average;
        *maxdeviationbelow = average - min;
        *frameaverage = average;
        return;
    }
    
    // CREATE TEMPORARY BUFFER FOR WEIGHTS
    unsigned weightArraySize = (pastRadius + futureRadius) + 1;
    float* weights = (float*) malloc(sizeof(float) * weightArraySize);

    // ASSIGN WEIGHTS BY TYPE
    // CONSTANT WEIGHT
    if (type == 1) {
        for (unsigned i = 0; i < weightArraySize; i++) {
            weights[i] = 1.0;
        }
    }
    // EXPONENTIAL WEIGHT
    else if (type == 2) {
        for (unsigned i = 0; i < pastRadius; i++) {
            unsigned numAlpha = pastRadius - i + 1;
            weights[i] = pow(alpha, numAlpha);
        }
        weights[pastRadius] = alpha;
        for (unsigned i = pastRadius + 1; i < weightArraySize; i++) {
            unsigned numAlpha = i - pastRadius + 1;
            weights[i] = pow(alpha, numAlpha);
        }
    }
    
    // PRINT WEIGHT ARRAY
    /*
    for (unsigned i = 0; i < weightArraySize; i++) {
        std::cerr << weights[i] << std::endl;
    }
    */

    // CALCULATE MOVING AVERAGE BASED ON WEIGHTS
    int leftrad = pastRadius;
    int rightrad = futureRadius;
    for (int i = 0; i < (int) n; i++) {
        int lowerIndex = i - leftrad < 0 ? 0 : i - leftrad;
        int upperIndex = i + rightrad + 1 > n ? n : i + rightrad + 1;
        int weightIndex = i - leftrad < 0 ? leftrad - i: 0;
        double sum = 0.0;
        double weightsum = 0.0;
        //std::cerr << i << ": (" << lowerIndex << ", " << upperIndex << ", " << weightIndex << ")" << std::endl;
        for (int j = lowerIndex, k = weightIndex; j < upperIndex; j++, k++) {
            sum += buffer[j] * weights[k];
            weightsum += weights[k];
        }
        movingaverage[i] = (sum / weightsum);
    }
    free(weights);

    // CALCULATE MIN/MAX DEVIATION INEFFICIENTLY
    double maxdeva = std::numeric_limits<double>::min();
    double maxdevb = std::numeric_limits<double>::min();
    double maxrata = std::numeric_limits<double>::min();
    double maxratb = std::numeric_limits<double>::min();
    double sum = 0.0;
    for (unsigned i = 0; i < n; i++) {
        double pointValue = buffer[i];
        sum += pointValue;
        double pointAverage = movingaverage[i];
        // if value is below average
        if (pointValue < pointAverage) {
            if (pointAverage - pointValue > maxdevb)
                maxdevb = pointAverage - pointValue;

            // absolute value over moving average
            double absValue = (pointAverage - pointValue) + pointAverage;
            if (pointAverage/absValue > maxratb)
                maxratb = pointAverage/absValue;
        }
        else {
            if (pointValue - pointAverage > maxdeva)
                maxdeva = pointValue - pointAverage;
            if (pointAverage/pointValue > maxrata)
                maxrata = pointAverage/pointValue;
        }
    }
    *maxdeviationabove = maxdeva;
    *maxdeviationbelow = maxdevb;
    *maxratioabove = maxrata;
    *maxratiobelow = maxratb;
    *frameaverage = sum / double(n);
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

double GPExperiment::compareWaveforms(unsigned type, unsigned numSamples, float* samplesOne, float* samplesTwo) {
    double ret = -1;
    if (type == 0) {
        double sum = 0;
        for (unsigned frameNum = 0; frameNum < numSamples; frameNum++) {
            double error = fabs(samplesTwo[frameNum] - samplesOne[frameNum]);
            sum += error;
            /*
            if (frameNum % 128 == 0) {
                std::cout << "frame " << frameNum << "; one: " << samplesOne[frameNum] << ", two: " << samplesTwo[frameNum] << ", error: " << error << ", sum: " << sum << std::endl;
            }
            */
        }
        ret = sum;
    }
    return ret;
}

/*
    =============
    GRAPH HELPERS
    =============
*/

void GPExperiment::fillTimeAxisBuffer(unsigned numSamples, float sr, float* buffer) {
    for (unsigned frame = 0; frame < numSamples; frame++) {
        buffer[frame] = float(frame)/sr;
    }
}

void GPExperiment::fillFrequencyAxisBuffer(unsigned fftSize, double sr, float* buffer) {
    for (unsigned i = 0; i < (fftSize/2) + 1; i++) {
        buffer[i] = (sr / fftSize) * i;
    }
}

String GPExperiment::floatBuffersToGraphText(String options, String xlab, String ylab, bool indexAsX, unsigned n, const float* x, const float* y, const float* z) {
    String ret;
    ret += options;
    ret += "\n";
    ret += xlab;
    ret += "\t";
    ret += ylab;
    ret += "\n";
    /*
    ret += "\t";
    ret += zlab;
    ret += "\n";
    */
    for (unsigned i = 0; i < n; i++) {
        if (indexAsX)
            ret += String(i);
        else
            ret += String(x[i]);
        if (y != NULL) {
            ret += "\t";
            ret += String(y[i]);
        }
        if (z != NULL) {
            ret += "\t";
            ret += String(z[i]);
        }
        ret += "\n";
    }
    return ret;
}

String GPExperiment::doubleBuffersToGraphText(String options, String xlab, String ylab, String zlab, bool indexAsX, unsigned n, const double* x, const double* y, const double* z) {
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
        if (y != NULL) {
            ret += "\t";
            ret += String(y[i]);
        }
        if (z != NULL) {
            ret += "\t";
            ret += String(z[i]);
        }
        ret += "\n";
    }
    return ret;
}

/*
    ===========
    SANITY TEST
    ===========
*/

void GPExperiment::sanityTest(GPRandom* rng) {
    // buffers for tests
    unsigned wavchunk = 256;
    unsigned numframes = 88200;
    float maxSeconds = 2.0;
    float samplerate = 44100.0;
    unsigned numconstantvariables = 2;
    unsigned renderblocksize = 256;
    float* testBuffer = (float*) malloc(sizeof(float) * numframes);
    float* comparisonBuffer = (float*) malloc(sizeof(float) * numframes);
    float* silenceBuffer = (float*) malloc(sizeof(float) * numframes);
    float* variables = (float*) malloc(sizeof(float) * numconstantvariables);
    variables[0] = 440.0;
    variables[1] = 659.26;
    float* times = (float*) malloc(sizeof(float) * numframes);
    fillTimeAxisBuffer(numframes, samplerate, times);
    loadWavFile("./tests/silenceTestTarget.wav", wavchunk, numframes, silenceBuffer);

    // test network strings
    std::string silenceTest = "(silence)";
    std::string noiseTest = "(noise)";
    std::string variableNodeTest = "(sin (* (* (const {d 1 2 5}) (pi)) (* (time) (var {d 0 0 1} {c 0.0 0.0 22050.0})))))";

    // SIN TEST NETWORK
    std::cout << "----TESTING BASIC SINE WAVE----" << std::endl;
    std::string sinTest = "(sin (* (* (const {d 1 2 5}) (pi)) (* (time) (const {c 0.0 440.0 22050.0}))))";
    GPNetwork* sinTestNet = new GPNetwork(rng, sinTest);

    std::cout << "Network before trace:" << std::endl << sinTestNet->toString(10) << std::endl;
    std::cout << "Height: " << sinTestNet->height << std::endl;
    std::cout << "Min: " << sinTestNet->minimum << std::endl;
    std::cout << "Max: " << sinTestNet->maximum << std::endl;
    renderIndividualByBlockPerformance(sinTestNet, renderblocksize, numconstantvariables, variables, numframes, times, testBuffer);
    sinTestNet->doneRendering();
    assert(compareWaveforms(0, numframes, silenceBuffer, testBuffer) == 0);

    sinTestNet->traceNetwork();
    std::cout << "Network after trace:" << std::endl << sinTestNet->toString(10) << std::endl;
    std::cout << "Height: " << sinTestNet->height << std::endl;
    std::cout << "Min: " << sinTestNet->minimum << std::endl;
    std::cout << "Max: " << sinTestNet->maximum << std::endl;
    renderIndividualByBlockPerformance(sinTestNet, renderblocksize, numconstantvariables, variables, numframes, times, testBuffer);
    sinTestNet->doneRendering();
    assert(compareWaveforms(0, numframes, silenceBuffer, testBuffer) == 0);

    sinTestNet->prepareToRender(samplerate, renderblocksize, maxSeconds); 
    std::cout << "Network after prepare:" << std::endl << sinTestNet->toString(10) << std::endl;
    std::cout << "Height: " << sinTestNet->height << std::endl;
    std::cout << "Min: " << sinTestNet->minimum << std::endl;
    std::cout << "Max: " << sinTestNet->maximum << std::endl;
    renderIndividualByBlockPerformance(sinTestNet, renderblocksize, numconstantvariables, variables, numframes, times, testBuffer);
    sinTestNet->doneRendering();
    loadWavFile("./tests/sineWave440TestTarget.wav", wavchunk, numframes, comparisonBuffer);
    double error = compareWaveforms(0, numframes, comparisonBuffer, testBuffer);
    std::cout << "Comparison error to Audacity sine wave: " << error << std::endl;
    assert(error < 10);
    saveWavFile("./sineWaveTest.wav", String(sinTestNet->toString(10).c_str()), String("test"), samplerate, wavchunk, numframes, testBuffer);

    // test copy/mutatable params
    std::cout << "----TESTING MUTATABLE PARAMS----" << std::endl;
    GPNetwork* sinTestNetNewCenter = sinTestNet->getCopy("string");
    delete sinTestNet;
    std::cout << "Network before trace:" << std::endl << sinTestNetNewCenter->toString(10) << std::endl;
    sinTestNetNewCenter->traceNetwork();
    std::vector<GPMutatableParam*>* amp = sinTestNetNewCenter->getAllMutatableParams();
    GPMutatableParam* cf = amp->at(amp->size() - 1);
    cf->setCValue(30);
    cf->setCRange(15, 300);
    std::cout << "Network after mutate:" << std::endl << sinTestNetNewCenter->toString(10) << std::endl;
    renderIndividualByBlockPerformance(sinTestNetNewCenter, renderblocksize, numconstantvariables, variables, numframes, times, testBuffer);
    sinTestNetNewCenter->prepareToRender(samplerate, renderblocksize, maxSeconds); 
    renderIndividualByBlockPerformance(sinTestNetNewCenter, renderblocksize, numconstantvariables, variables, numframes, times, testBuffer);
    saveWavFile("./sineWaveTestNewCenter.wav", String(sinTestNetNewCenter->toString(10).c_str()), String("test"), samplerate, wavchunk, numframes, testBuffer);
    delete sinTestNetNewCenter;

    // adsr test network
    std::string ADSRTest = "(adsr* {c 0 0.2 2} {c 0 0.2 2} {c 0 1.0 1.0} {c 0 0.2 2} {c 0 0.2 2} {c 0 0.5 1.0} {c 0 0.2 2} " + sinTest + ")";
    GPNetwork* ADSRTestNet = new GPNetwork(rng, ADSRTest);
    ADSRTestNet->traceNetwork();
    ADSRTestNet->prepareToRender(samplerate, renderblocksize, maxSeconds);
    std::cout << "----TESTING ADSR----" << std::endl;
    std::cout << "Network: " << std::endl << ADSRTestNet->toString(10) << std::endl;
    std::cout << "Height: " << ADSRTestNet->height << std::endl;
    std::cout << "Min: " << ADSRTestNet->minimum << std::endl;
    std::cout << "Max: " << ADSRTestNet->maximum << std::endl;
    renderIndividualByBlockPerformance(ADSRTestNet, renderblocksize, numconstantvariables, variables, numframes, times, testBuffer);
    ADSRTestNet->doneRendering();
    saveWavFile("./ADSRsineWaveTest.wav", String(ADSRTestNet->toString(10).c_str()), String("test"), samplerate, wavchunk, numframes, testBuffer);
    delete ADSRTestNet;

    // constant node envelope test network
    std::string constantNodeEnvelopeTest = "(const* {c 0 0.5 1.0} " + sinTest + ")";
    GPNetwork* constantNodeEnvelopeTestNet = new GPNetwork(rng, constantNodeEnvelopeTest);
    constantNodeEnvelopeTestNet->traceNetwork();
    constantNodeEnvelopeTestNet->prepareToRender(samplerate, renderblocksize, maxSeconds);
    std::cout << "----TESTING CONSTANT NODE ENVELOPE----" << std::endl;
    std::cout << "Network: " << std::endl << constantNodeEnvelopeTestNet->toString(10) << std::endl;
    std::cout << "Height: " << constantNodeEnvelopeTestNet->height << std::endl;
    std::cout << "Min: " << constantNodeEnvelopeTestNet->minimum << std::endl;
    std::cout << "Max: " << constantNodeEnvelopeTestNet->maximum << std::endl;
    renderIndividualByBlockPerformance(constantNodeEnvelopeTestNet, renderblocksize, numconstantvariables, variables, numframes, times, testBuffer);
    constantNodeEnvelopeTestNet->doneRendering();
    saveWavFile("./constantNodeEnvelopesineWaveTest.wav", String(constantNodeEnvelopeTestNet->toString(10).c_str()), String("test"), samplerate, wavchunk, numframes, testBuffer);
    delete constantNodeEnvelopeTestNet;

    // additive synthesis test
    std::string additiveSynthesisTest = "(+ (const* {c 0 0.5 1.0} (osc {d 0 0 1} {d 0 1 10})) (const* {c 0 0.5 1.0} (osc {d 0 1 1} {d 0 1 10})))";
    GPNetwork* additiveSynthesisTestNet = new GPNetwork(rng, additiveSynthesisTest);
    additiveSynthesisTestNet->traceNetwork();
    additiveSynthesisTestNet->prepareToRender(samplerate, renderblocksize, maxSeconds);
    std::cout << "----TESTING ADDITIVE SYNTHESIS----" << std::endl;
    std::cout << "Network: " << std::endl << additiveSynthesisTestNet->toString(10) << std::endl;
    std::cout << "Height: " << additiveSynthesisTestNet->height << std::endl;
    std::cout << "Min: " << additiveSynthesisTestNet->minimum << std::endl;
    std::cout << "Max: " << additiveSynthesisTestNet->maximum << std::endl;
    renderIndividualByBlockPerformance(additiveSynthesisTestNet, renderblocksize, numconstantvariables, variables, numframes, times, testBuffer);
    additiveSynthesisTestNet->doneRendering();
    saveWavFile("./additiveSynthesisTest.wav", String(additiveSynthesisTestNet->toString(10).c_str()), String("test"), samplerate, wavchunk, numframes, testBuffer);
    delete additiveSynthesisTestNet;

    // FM synthesis test
    std::string FMSynthesisTest = "(fm {d 0 1 1} {d 0 1 10} {c 0 2.0 3.0} " + sinTest + ")";
    GPNetwork* FMSynthesisTestNet = new GPNetwork(rng, FMSynthesisTest);
    FMSynthesisTestNet->traceNetwork();
    FMSynthesisTestNet->prepareToRender(samplerate, renderblocksize, maxSeconds);
    std::cout << "----TESTING FM SYNTHESIS----" << std::endl;
    std::cout << "Network: " << std::endl << FMSynthesisTestNet->toString(10) << std::endl;
    std::cout << "Height: " << FMSynthesisTestNet->height << std::endl;
    std::cout << "Min: " << FMSynthesisTestNet->minimum << std::endl;
    std::cout << "Max: " << FMSynthesisTestNet->maximum << std::endl;
    renderIndividualByBlockPerformance(FMSynthesisTestNet, renderblocksize, numconstantvariables, variables, numframes, times, testBuffer);
    FMSynthesisTestNet->doneRendering();
    saveWavFile("./FMSynthesisTest.wav", String(FMSynthesisTestNet->toString(10).c_str()), String("test"), samplerate, wavchunk, numframes, testBuffer);
    delete FMSynthesisTestNet;

    // free test buffer
    free(variables);
    free(times);
    free(testBuffer);
    free(comparisonBuffer);
    free(silenceBuffer);
}
