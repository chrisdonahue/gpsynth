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

GPExperiment::GPExperiment(String target, GPParams* p) :
    wavFormat(new WavAudioFormat())
{
    // EXPERIMENT PARAMETERS
    params = p;
    specialValues = p->variableValues->data();
    numGenerations = p->numGenerations;
    fitnessThreshold = p->thresholdFitness;

    // TARGET DATA CONTAINERS
    sampleRate = 44100.0;
    targetFrames = NULL;
    numTargetFrames = 0;
    wavFileBufferSize = p->wavFileBufferSize;
    loadTargetWavFile(target);
    // TODO: fill in filterNodeMaxBufferSize/delayNodeMaxBufferSize from sample rate

    // EXPERIMENT STATE
    minFitnessAchieved = INFINITY;
    numEvaluatedGenerations = 0;

    // SYNTH
    std::vector<GPNode*>* nodes = new std::vector<GPNode*>();
    std::vector<GPFunction>* unaryFunctions = new std::vector<GPFunction>();
    std::vector<GPFunction>* binaryFunctions = new std::vector<GPFunction>();

    if (params->experimentNumber == 0) {
        // TODO remove once done with AM testing or make command line param
        // EVALUATE TARGET STRING
        std::string AMstring("(* (sin (* (* (time) (v0)) (* (2) (pi)))) (sin (* (time) (* (2) (pi)))))");
        GPNetwork* answer = new GPNetwork(AMstring);
        answer->traceNetwork();
        std::cout << "Target network: " << answer->toString() << std::endl;
        sampleRate = 44100.0;
        numTargetFrames = 88200;
        targetFrames = renderIndividual(answer);
        saveWavFile("./Answer.wav", String(answer->toString().c_str()), numTargetFrames, targetFrames);
        delete answer;

        // ASSIGN SPECIAL FITNESS VALUES
        p->lowerFitnessIsBetter = true;
        p->bestPossibleFitness = 0;
        worstFitness = std::numeric_limits<float>::max();

        std::string silentstring("(0)");
        GPNetwork* silent = new GPNetwork(silentstring);
        silent->traceNetwork();
        float* silence = renderIndividual(silent);
        silenceFitness = -1;
        silenceFitness = compareToTarget(silence);
        std::cout << silenceFitness << std::endl;
        delete silent;

        // SUPPLY AVAILABLE NODES
        //nodes->push_back(new FunctionNode(add, "+", NULL, NULL));
        nodes->push_back(new FunctionNode(multiply, NULL, NULL));
        //nodes->push_back(new FunctionNode(sine, NULL, NULL));
        //nodes->push_back(new ConstantNode(2));
        //nodes->push_back(new ConstantNode(M_PI));
        //nodes->push_back(new TimeNode());
        //nodes->push_back(new VariableNode(0));
        nodes->push_back(new OscilNode(1, 0, NULL, NULL));
        nodes->push_back(new OscilNode(1, 1, NULL, NULL));
        //nodes->push_back(new OscilNode(1, 1, NULL, NULL));
        //nodes->push_back(new OscilNode(1, 2, NULL, NULL));
        //nodes->push_back(new OscilNode(1, 3, NULL, NULL));

        binaryFunctions->push_back(add);
        binaryFunctions->push_back(multiply);
    }
    if (params->experimentNumber == 10) {
        // EVALUATE TARGET STRING
        std::string AMstring("(* (sin (* (* (time) (v0)) (* (2) (pi)))) (sin (* (time) (* (2) (pi)))))");
        GPNetwork* answer = new GPNetwork(AMstring);
        answer->traceNetwork();
        std::cout << "Target network: " << answer->toString() << std::endl;
        sampleRate = 44100.0;
        numTargetFrames = 88200;
        targetFrames = renderIndividual(answer);
        saveWavFile("./AnswerString.wav", String(answer->toString().c_str()), numTargetFrames, targetFrames);
        loadTargetWavFile("./AnswerString.wav");

        GPNetwork* equivalent = new GPNetwork(new FunctionNode(multiply, new OscilNode(1, 0, NULL, NULL), new OscilNode(1, 1, NULL, NULL)));

        //float* equivalentFrames = renderIndividual(equivalent);
        //saveWavFile("./EquivalentNetwork.wav", String(equivalent->toString().c_str()), numTargetFrames, equivalentFrames);

        float* equivalentFrames = renderIndividual(answer);
        saveWavFile("./SameNetwork.wav", String(answer->toString().c_str()), numTargetFrames, equivalentFrames);

        p->fitnessFunctionType = 0;
        std::cout << "Amplitude fitness: " << compareToTarget(equivalentFrames) << std::endl;
        p->fitnessFunctionType = 1;
        std::cout << "Frequency fitness: " << compareToTarget(equivalentFrames) << std::endl;

        delete answer;
        delete equivalent;

        exit(0);
    }

    p->availableUnaryFunctions = unaryFunctions;
    p->availableBinaryFunctions = binaryFunctions;

    synth = new GPSynth(p, nodes);
}

GPExperiment::~GPExperiment() {
    free(targetFrames);
    free(targetSpectrum);
    free(targetSpectrumMagnitudes);
    free(targetSpectrumPhases);
    delete params->availableUnaryFunctions;
    delete params->availableBinaryFunctions;
    delete synth;
}

/*
    =================
    EVOLUTION CONTROL
    =================
*/

GPNetwork* GPExperiment::evolve() {
    GPNetwork* champ;
    int numMinimum = 0;
    int numEvaluated = 0;
    double generationCumulativeFitness = 0;
    double generationAverageFitness = 0;
    double generationMinimumFitness = INFINITY;

    while (minFitnessAchieved > fitnessThreshold && numEvaluatedGenerations < numGenerations) {
        GPNetwork* candidate = synth->getIndividual();

        float* candidateData = renderIndividual(candidate);
        double fitness = compareToTarget(candidateData);
        generationCumulativeFitness += fitness;
        numEvaluated++;

        //TODO: handle lowerFitnessIsBetter

        if (fitness < generationMinimumFitness) {
            generationMinimumFitness = fitness;
        }
        
        if (fitness > worstFitness) {
            worstFitness = fitness;
        }

        if (fitness < minFitnessAchieved) {
            minFitnessAchieved = fitness;

            int backupID = candidate->ID;
            champ = candidate->getCopy();
            champ->ID = backupID;
            champ->fitness = minFitnessAchieved;

            char buffer[100];
            snprintf(buffer, 100, "New Minimum (%d).wav", ++numMinimum);
            //saveWavFile(String(buffer), String(candidate->toString().c_str()), numTargetFrames, candidateData);
            if (fitness == bestPossibleFitness) {
                saveWavFile("./Perfect.wav", String(champ->toString().c_str()), numTargetFrames, candidateData);
            }
        }

        int numUnevaluatedThisGeneration = synth->assignFitness(candidate, fitness);
        if (numUnevaluatedThisGeneration == 0) {
            generationMinimumFitness = INFINITY;
            numEvaluatedGenerations++;
        }

        free(candidateData);
    }
    std::cout << "-------------------------------- SUMMARY ---------------------------------" << std::endl;

    if (minFitnessAchieved <= fitnessThreshold) {
        std::cout << "Evolution found a synthesis algorithm at or below the specified fitness threshold" << std::endl;
    }
    std::cout << "Evolution ran for " << numEvaluatedGenerations << " generations" << std::endl;
    if (champ != NULL) {
        champ->traceNetwork();
        std::cout << "The best synthesis algorithm found was number " << champ->ID << " with network " << champ->toString() << " and had a fitness of " << minFitnessAchieved << std::endl;
        saveWavFile("./DiscoveredChampion.wav", String(champ->toString().c_str()), numTargetFrames, renderIndividual(champ));
    }
    return champ;
}

/*
    =============
    WAV INTERFACE
    =============
*/

void GPExperiment::loadTargetWavFile(String path) {
    // TODO: check if path exists
    File input(path);
    FileInputStream* fis = input.createInputStream();
    AudioSampleBuffer asb(1, wavFileBufferSize);
    ScopedPointer<AudioFormatReader> afr(wavFormat->createReaderFor(fis, true));

    // get info on target
    numTargetFrames = afr->lengthInSamples;
    sampleRate = afr->sampleRate;

    std::cout << "Target file has " << numTargetFrames << " frames at a sample rate of " <<  sampleRate << std::endl;

    // get waveform of target
    free(targetFrames);
    targetFrames = (float*) malloc(sizeof(float) * numTargetFrames);

    int64 numRemaining = numTargetFrames;
    int64 numCompleted = 0;
    float* chanData = asb.getSampleData(0);
    while (numRemaining > 0) {
        int numToRead = numRemaining > wavFileBufferSize ? wavFileBufferSize : numRemaining;
        afr->read(&asb, 0, numToRead, numCompleted, true, false);
        memcpy(targetFrames + numCompleted, chanData, numToRead);
        numRemaining -= numToRead;
        numCompleted += numToRead;
    }
    assert (numCompleted == numTargetFrames && numRemaining == 0);

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
            numFftCompleted++;
        }
        free(in);
    }
}

void GPExperiment::saveWavFile(String path, String metadata, unsigned numFrames, float* data) {
    File output(path);
    if (output.existsAsFile()) {
        output.deleteFile();
        output.create();
    }
    FileOutputStream* fos = output.createOutputStream();
    //StringPairArray metaData = WavAudioFormat::createBWAVMetadata(metadata, "", "", Time::getCurrentTime(), 0, "");
    AudioSampleBuffer asb(1, wavFileBufferSize);
    //ScopedPointer<AudioFormatWriter> afw(wavFormat->createWriterFor(fos, sampleRate, 1, 32, metaData, 0));
    ScopedPointer<AudioFormatWriter> afw(wavFormat->createWriterFor(fos, sampleRate, 1, 32, StringPairArray(), 0));

    int64 numRemaining = numFrames;
    int64 numCompleted = 0;
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

float* GPExperiment::renderIndividual(GPNetwork* candidate) {
    float* ret = (float*) malloc(sizeof(float) * numTargetFrames);
    double time;
    for (int frameNum = 0; frameNum < numTargetFrames; frameNum++) {
        time = frameNum/sampleRate;
        ret[frameNum] = candidate->evaluate(&time, specialValues);
    }
    return ret;
}

double GPExperiment::compareToTarget(float* candidateFrames) {
    double ret = -1;
    if (params->fitnessFunctionType == 0) {
        double sum = 0;
        for (int frameNum = 0; frameNum < numTargetFrames; frameNum++) {
            sum += pow(targetFrames[frameNum] - candidateFrames[frameNum], 2);
            std::cout << "sum: " << sum << " frameNum: " << frameNum << std::endl;
        }
        ret = sqrt(sum);
    }
    else if (params->fitnessFunctionType == 1) {
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
            }
        }
        free(in);
        free(out);
        free(magnitude);
        free(phase);
        ret = MSEmag + MSEph;
    }
    if (ret == silenceFitness)
        return worstFitness;
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
