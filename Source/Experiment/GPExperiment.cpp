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
    // TARGET DATA CONTAINERS
    sampleRate = 44100.0;
    targetFrames = NULL;
    numTargetFrames = 0;
    wavFileBufferSize = p->wavFileBufferSize;
    loadTargetWavFile(target);
    fillEvaluationBuffers(numTargetFrames, specialValues, NULL, p->numVariables, 0);

    // EXPERIMENT PARAMETERS THAT USE SAMPLE RATE
    params->delayNodeMaxBufferSize = params->delayNodeBufferMaxSeconds * sampleRate;
    params->filterNodeMaxBufferSize = params->filterNodeBufferMaxSeconds * sampleRate;
    params->noiseNodeMaxBufferSize = params->noiseNodeBufferMaxSeconds * sampleRate;

    // EXPERIMENT STATE
    numGenerations = p->numGenerations;
    fitnessThreshold = p->thresholdFitness;
    minFitnessAchieved = INFINITY;
    numEvaluatedGenerations = 0;

    // SYNTH
    std::vector<GPNode*>* nodes = new std::vector<GPNode*>();

    if (params->experimentNumber == 0) {
        // EVALUATE TARGET STRING
        std::string AMstring("(* (sin (* (* (time) (v0)) (* (2) (pi)))) (sin (* (time) (* (2) (pi)))))");
        GPNetwork* answer = new GPNetwork(AMstring);
        answer->traceNetwork();
        std::cout << "Target network: " << answer->toString() << std::endl;
        sampleRate = 44100.0;
        numTargetFrames = 88200;
        targetFrames = (float*) malloc(sizeof(float) * numTargetFrames);
        renderIndividual(answer, numTargetFrames, targetFrames);
        saveWavFile("./Answer.wav", String(answer->toString().c_str()), numTargetFrames, targetFrames);
        loadTargetWavFile("./Answer.wav");
        delete answer;

        // ASSIGN SPECIAL FITNESS VALUES
        p->bestPossibleFitness = 0;
        p->penaltyFitness = std::numeric_limits<float>::max();
        p->lowerFitnessIsBetter = true;

        // SUPPLY AVAILABLE NODES
        nodes->push_back(new FunctionNode(multiply, NULL, NULL));
        nodes->push_back(new OscilNode(1, 0));
        nodes->push_back(new OscilNode(1, 1));
    }
    // AM 440 hz experiment easy
    if (params->experimentNumber == 1) {
        // ASSIGN SPECIAL FITNESS VALUES
        p->bestPossibleFitness = 0;
        p->penaltyFitness = std::numeric_limits<float>::max();
        p->lowerFitnessIsBetter = true;

        // SUPPLY AVAILABLE NODES
        nodes->push_back(new FunctionNode(multiply, NULL, NULL));
        nodes->push_back(new OscilNode(1, 0));
        nodes->push_back(new OscilNode(1, 1));
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
        nodes->push_back(new ConstantNode(2));
        nodes->push_back(new ConstantNode(M_PI));
        nodes->push_back(new TimeNode());
        nodes->push_back(new VariableNode(0));
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
        nodes->push_back(new VariableNode(0));
        nodes->push_back(new ConstantNode(0));
        nodes->push_back(new ModOscilNode(NULL, NULL));
        nodes->push_back(new NoiseNode(rng));
        //nodes->push_back(new FilterNode(0, 1, 1024, sampleRate, 1, 1, 1, NULL));
        //nodes->push_back(new FilterNode(1, 1, 1024, sampleRate, 1, 1, 1, NULL));
        //nodes->push_back(new FilterNode(2, 1, 1024, sampleRate, 1, 1, 1, NULL));
        //nodes->push_back(new FilterNode(3, 1, 1024, sampleRate, 1, 1, 1, NULL));
    }
    if (params->experimentNumber == 10) {
        /*
        // EVALUATE TARGET STRING
        std::string AMstring("(* (sin (* (* (time) (v0)) (* (2) (pi)))) (sin (* (time) (* (2) (pi)))))");
        GPNetwork* answer = new GPNetwork(AMstring);
        answer->traceNetwork();
        std::cout << "Target network: " << answer->toString() << std::endl;
        sampleRate = 44100.0;
        numTargetFrames = 88200;
        //targetFrames = renderIndividual(answer);
        saveWavFile("./AnswerString.wav", String(answer->toString().c_str()), numTargetFrames, targetFrames);
        loadTargetWavFile("./AnswerString.wav");

        GPNetwork* equivalent = new GPNetwork(new FunctionNode(multiply, new OscilNode(1, 0), new OscilNode(1, 1)));

        //float* equivalentFrames = renderIndividual(equivalent);
        saveWavFile("./EquivalentNetwork.wav", String(equivalent->toString().c_str()), numTargetFrames, equivalentFrames);

        //float* equivalentFrames = renderIndividual(answer);
        //saveWavFile("./SameNetwork.wav", String(answer->toString().c_str()), numTargetFrames, equivalentFrames);

        std::cout << "Amplitude fitness: " << compareToTarget(0, equivalentFrames) << std::endl;
        std::cout << "Frequency fitness: " << compareToTarget(1, equivalentFrames) << std::endl;

        delete answer;
        delete equivalent;

        exit(0);
        */
    }

    // EXPERIMENT PARAMS THAT VARY BY EXPERIMENT NUMBER
    bestPossibleFitness = params->bestPossibleFitness;
    penaltyFitness = params->penaltyFitness;
    lowerFitnessIsBetter = params->lowerFitnessIsBetter;

    synth = new GPSynth(rng, p, nodes);
}

GPExperiment::~GPExperiment() {
    free(targetFrames);
    if (params->fitnessFunctionType == 1) {
        free(targetSpectrum);
        free(targetSpectrumMagnitudes);
        free(targetSpectrumPhases);
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

            if (fitness == params->bestPossibleFitness) {
                saveWavFile("./perfect.wav", String(champ->toString().c_str()), numTargetFrames, candidateData);
            }
        }

        int numUnevaluatedThisGeneration = synth->assignFitness(candidate, fitness);
        if (numUnevaluatedThisGeneration == 0) {
            generationMinimumFitness = INFINITY;

            float* genchampbuffer = (float*) malloc(sizeof(float) * numTargetFrames);
            generationChamp->traceNetwork();
            renderIndividualByBlock(generationChamp, numTargetFrames, params->renderBlockSize, genchampbuffer);
            char buffer[100];
            snprintf(buffer, 100, "./gen.%d.best.wav", numEvaluatedGenerations);
            saveWavFile(String(buffer), String(generationChamp->toString().c_str()), numTargetFrames, genchampbuffer);
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
        renderIndividual(champ, numTargetFrames, champbuffer);
        std::cout << "The best synthesis algorithm found was number " << champ->ID << " with network " << champ->toString() << " and had a fitness of " << minFitnessAchieved << std::endl;
        saveWavFile("./champion.wav", String(champ->toString().c_str()), numTargetFrames, champbuffer);
        free(champbuffer);
    }
    return champ;
}

/*
    =======================
    FILL EVALUATION BUFFERS
    =======================
*/

void GPExperiment::fillEvaluationBuffers(int64 numFrames, double* constantSpecialValues, double* variableSpecialValues, unsigned numConstantSpecialValues, unsigned numVariableSpecialValues) {
    numSpecialValues = numConstantSpecialValues + numVariableSpecialValues;
    specialValuesByFrame = (double*) malloc(sizeof(double) * numFrames * numSpecialValues);
    sampleTimes = (double*) malloc(sizeof(double) * numFrames);
    for (int frame = 0; frame < numFrames; frame++) {
        sampleTimes[frame] = frame/sampleRate;
        for (int val = 0; val < numConstantSpecialValues; val++) {
            *(specialValuesByFrame + (frame * numSpecialValues) + val) = constantSpecialValues[val];
        }
        for (int val = 0; val < numVariableSpecialValues; val++) {
            *(specialValuesByFrame + (frame * numSpecialValues) + numConstantSpecialValues + val) = variableSpecialValues[val]; // TODO: RHS of this assignment is placeholder
        }
    }
}

/*
    =============
    WAV INTERFACE
    =============
*/

void GPExperiment::loadTargetWavFile(String path) {
    File input(path);
    assert(input.existsAsFile());
    FileInputStream* fis = input.createInputStream();
    //AudioSampleBuffer asb(1, wavFileBufferSize);
    ScopedPointer<AudioFormatReader> afr(wavFormat->createReaderFor(fis, true));

    // get info on target
    numTargetFrames = afr->lengthInSamples;
    sampleRate = afr->sampleRate;

    std::cout << "Target file has " << numTargetFrames << " frames at a sample rate of " <<  sampleRate << std::endl;

    // get waveform of target
    free(targetFrames);
    targetFrames = (float*) malloc(sizeof(float) * numTargetFrames);

    AudioSampleBuffer asb(1, numTargetFrames);
    afr->read(&asb, 0, numTargetFrames, 0, false, true);
    float* chanData = asb.getSampleData(0);
    memcpy(targetFrames, chanData, sizeof(float) * numTargetFrames);
    int64 numRemaining = numTargetFrames;
    int64 numCompleted = 0;
    /*
    while (numRemaining > 0) {
        int numToRead = numRemaining > wavFileBufferSize ? wavFileBufferSize : numRemaining;
        afr->read(&asb, 0, numToRead, numCompleted, true, false);
        memcpy(targetFrames + numCompleted, chanData, numToRead);
        numRemaining -= numToRead;
        numCompleted += numToRead;
    }
    assert (numCompleted == numTargetFrames && numRemaining == 0);
    */

    // get frequency spectrum of target
    if (params->fitnessFunctionType == 1) {
        // calculate data concerning fft window size
        unsigned n = params->fftSize;
        unsigned numFftCalls = numTargetFrames / n;
        if (numTargetFrames % n > 0)
            numFftCalls++;
        
        // allocate input buffer
        kiss_fft_scalar* in = (kiss_fft_scalar*) malloc(sizeof(kiss_fft_scalar) * n);

        // allocate output buffers
        unsigned fftOutputDataArraySize = numFftCalls * (n/2 + 1);
        targetSpectrum = (kiss_fft_cpx*) malloc(sizeof(kiss_fft_cpx) * fftOutputDataArraySize);
        targetSpectrumMagnitudes = (double*) malloc(sizeof(double) * fftOutputDataArraySize);
        targetSpectrumPhases = (double*) malloc(sizeof(double) * fftOutputDataArraySize);

        // take fft of target data
        numRemaining = numTargetFrames;
        numCompleted = 0;
        unsigned numFftCompleted = 0;
        unsigned numFftOutputUsed = 0;
        while (numRemaining > 0) {
            unsigned numToTransform = numRemaining > n ? n : numRemaining;
            for (size_t i = 0; i < numToTransform; i++) {
                in[i] = targetFrames[numCompleted];
                numCompleted++;
                numRemaining--;
            }
            unsigned targetSpectrumIndex = numFftCompleted * (n/2 + 1);
            FftReal(n, in, targetSpectrum + targetSpectrumIndex, targetSpectrumMagnitudes + targetSpectrumIndex, targetSpectrumPhases + targetSpectrumIndex);
           /* 
            if (numRemaining < 100) {
                for (int i = 0; i < (n/2 + 1); i++) {
                    std::cout << targetSpectrumMagnitudes[targetSpectrumIndex + i] << ", " << targetSpectrumPhases[targetSpectrumIndex + i] << std::endl;
                }
            }
            */
            numFftCompleted++;
        }
        free(in);
    }
}

void GPExperiment::saveWavFile(String path, String desc, unsigned numFrames, float* data) {
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
    if (params->experimentNumber == 1 || params->experimentNumber == 3) {
        renderIndividualByBlock(candidate, numSamples, params->renderBlockSize, buffer);
    }
    else {
        renderIndividual(candidate, numSamples, buffer);
    }
    return compareToTarget(params->fitnessFunctionType, buffer);
}

void GPExperiment::renderIndividual(GPNetwork* candidate, int64 numSamples, float* buffer) {
    double time;
    for (int frameNum = 0; frameNum < numSamples; frameNum++) {
        time = frameNum/sampleRate;
        buffer[frameNum] = candidate->evaluate(&time, specialValues);
    }
}

void GPExperiment::renderIndividualByBlock(GPNetwork* candidate, int64 numSamples, unsigned n, float* buffer) {
    int64 numRemaining = numSamples;
    int64 numCompleted = 0;
    int64 bufferIndex = 0;
    unsigned numToRender;
    while (numRemaining > 0) {
        numToRender = n > numRemaining ? n : numRemaining;
        candidate->evaluateBlock(sampleTimes + numCompleted, numSpecialValues, specialValuesByFrame + (numCompleted * numSpecialValues), numToRender, buffer + numCompleted);
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

        kiss_fft_scalar* in = (kiss_fft_scalar*) malloc(sizeof(kiss_fft_scalar) * n);
        kiss_fft_cpx* out = (kiss_fft_cpx*) malloc(sizeof(kiss_fft_cpx) * (n/2 + 1));
        double* magnitude = (double*) malloc(sizeof(double) * (n/2 + 1));
        double* phase = (double*) malloc(sizeof(double) * (n/2 + 1));

        double MSEmag = 0;
        double MSEph = 0;
        int64 numCompared = 0;
        int64 numCompleted = 0;
        int64 numRemaining = numTargetFrames;
        int64 numFftOutputUsed = 0;
        while (numRemaining > 0) {
            unsigned numToTransform = numRemaining > n ? n : numRemaining;
            for (size_t i = 0; i < numToTransform; i++) {
                in[i] = candidateFrames[numCompleted];
                numCompleted++;
                numRemaining--;
            }
            FftReal(n, in, out, magnitude, phase);
            for (size_t i = 0; i < (n/2 + 1); i++) {
                double MAGXIJ = magnitude[i];
                double MAGTIJ = targetSpectrumMagnitudes[numFftOutputUsed];
                double angXIJ = phase[i];
                double angTIJ = targetSpectrumPhases[numFftOutputUsed];
                numFftOutputUsed++;
                MSEmag += pow(MAGXIJ - MAGTIJ, 2);
                MSEph += pow(angXIJ - angTIJ, 2); 
                silenceTest += MAGXIJ + angXIJ;
            }
        }
        free(in);
        free(out);
        free(magnitude);
        free(phase);
        ret = MSEmag + MSEph;
    }
    if (silenceTest == 0)
        return penaltyFitness;
    else
        return ret;
}

void GPExperiment::FftReal(unsigned n, const kiss_fft_scalar* in, kiss_fft_cpx* out, double* magnitude, double* phase)
{
    kiss_fftr_cfg cfg;

    // take FFT of input data and put it in output
    cfg = kiss_fftr_alloc(n, 0/*is_inverse_fft*/, NULL, NULL);
    size_t i;
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
