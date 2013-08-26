#include "GPExperiment.h"

/*
    ============
    CONSTRUCTION
    ============
*/

GPExperiment::GPExperiment(GPLogger* logger, GPMatchingExperimentParams* params, std::string beagle_cfg_file_path, GPSynth* synth, std::string target_file_path, std::string output_dir_path, std::vector<float>& constants) :
    is_sanity_test(false),
    logger(logger),
    params(params), beagle_cfg_file_path(beagle_cfg_file_path),
    seed_string(logger->get_seed_string()),
    synth(synth),
    target_file_path(target_file_path), output_dir_path(output_dir_path),
    numConstantValues(constants.size()), constantValues(constants.data())
{
    // TARGET DATA CONTAINERS
    getWavFileInfo(target_file_path, &numTargetFrames, &targetSampleRate);
    targetNyquist = targetSampleRate / 2;
    targetFrames = (float*) malloc(sizeof(float) * numTargetFrames);
    loadWavFile(target_file_path, params->aux_wav_file_buffer_size, numTargetFrames, targetFrames);
    if (params->log_save_target_copy)
        saveWavFile(String(output_dir_path) + String("target_copy.wav"), String(""), String("target"), targetSampleRate, params->aux_wav_file_buffer_size, numTargetFrames, targetFrames);

    fillEvaluationBuffers(numConstantValues, constantValues, 0, NULL);

    // EXPERIMENT STATE
    numGenerations = params->exp_generations;
    fitnessThreshold = params->exp_threshold;
    minFitnessAchieved = INFINITY;
    numEvaluatedGenerations = 0;
}

GPExperiment::GPExperiment(GPLogger* logger) :
    is_sanity_test(true),
    logger(logger)
{}

GPExperiment::~GPExperiment() {
    if (!is_sanity_test) {
        free(targetFrames);
        //free(targetEnvelope);
        free(targetSampleTimes);
        if (params->ff_type == 1) {
            free(analysisWindow);
            free(targetSpectra);
            free(targetMagnitude);
            free(targetPhase);
            free(binOvershootingPenalty);
            free(binUndershootingPenalty);
            free(fftFrameWeight);
			free(fftConfig);
			free(candidateAmplitudeBuffer);
			free(candidateSpectraBuffer);
			free(candidateMagnitudeBuffer);
			free(candidatePhaseBuffer);
        }
    }
}

/*
    =================
    EVOLUTION CONTROL
    =================
*/

GPNetwork* GPExperiment::evolve() {
    if (is_sanity_test) {
        return NULL;
    }

    // temp fields for evolution
    unsigned champGeneration = 0;
    int numEvaluatedThisGeneration = 0;
    int numUnevaluatedThisGeneration = 0;

    // temp buffers for rendering
    float* candidateData = (float*) malloc(sizeof(float) * numTargetFrames);
    float* champBuffer = (float*) malloc(sizeof(float) * numTargetFrames);

    while (minFitnessAchieved > fitnessThreshold && numEvaluatedGenerations < numGenerations) {
        // get individual from EA and suboptimize/evaluate it
        GPNetwork* candidate = synth->getIndividual();
        candidate->traceNetwork();
        candidate->prepareToRender(targetSampleRate, params->aux_render_block_size, numTargetFrames, targetLengthSeconds);
        double fitness = suboptimizeAndCompareToTarget(params->exp_suboptimize_type, candidate, candidateData);
        
        // report fitness to EA
        numUnevaluatedThisGeneration = synth->assignFitness(candidate, fitness);
        if (fitness < minFitnessAchieved) {
            minFitnessAchieved = fitness;
        }
        numEvaluatedThisGeneration++;

        // if we're done with this generation...
        if (numUnevaluatedThisGeneration == 0) {
            if (params->log_save_gen_champ_audio) {
                // grab and render the generation champ
                GPNetwork* generationChamp = synth->generationChamp;
                generationChamp->prepareToRender(targetSampleRate, params->aux_render_block_size, numTargetFrames, targetLengthSeconds);
                renderIndividualByBlockPerformance(generationChamp, params->aux_render_block_size, numConstantValues, constantValues, numTargetFrames, targetSampleTimes, champBuffer);
                generationChamp->doneRendering();

                // save generation champions
                std::stringstream ss;
                ss << output_dir_path << seed_string << ".gen." << numEvaluatedGenerations << ".best.wav";
                saveWavFile(String(ss.str()), String(logger->net_to_string_save(generationChamp).c_str()), String(generationChamp->origin.c_str()), targetSampleRate, params->aux_wav_file_buffer_size, numTargetFrames, champBuffer);
            }

            if (params->log_save_gen_summary_file) {
                std::stringstream ss;
                ss << output_dir_path << seed_string << ".gen." << numEvaluatedGenerations << ".summary";
                std::vector<GPNetwork*> currentGeneration(0);
                synth->getCurrentGeneration(currentGeneration);
                std::stringstream ss2;
                for (std::vector<GPNetwork*>::iterator i = currentGeneration.begin(); i != currentGeneration.end(); i++) {
                    GPNetwork* net = (*i);
                    ss2 << "(" << net->ID << ", " << net->fitness << ", " << net->origin << "): " << logger->net_to_string_save(net) << std::endl << std::endl;
                }
                saveTextFile(String(ss.str()), String(ss2.str())); 
            }

            // increment number of evaluted generations
            numEvaluatedGenerations++;
            numEvaluatedThisGeneration = 0;
        }
    }

    // check if we hit the threshold and need to print the last generation summary
    if (numUnevaluatedThisGeneration != 0) {
        synth->endGeneration();
        synth->printGenerationSummary();
    }

    free(candidateData);

    synth->printEvolutionSummary();

    // render the champion
    GPNetwork* champ = synth == NULL ? NULL : synth->champ;
    if (champ != NULL) {
        champ->prepareToRender(targetSampleRate, params->aux_render_block_size, numTargetFrames, targetLengthSeconds);
        renderIndividualByBlockPerformance(champ, params->aux_render_block_size, numConstantValues, constantValues, numTargetFrames, targetSampleTimes, champBuffer);
        champ->doneRendering();
        assert(champ->fitness == minFitnessAchieved);
        std::stringstream ss;
        ss << output_dir_path << seed_string << ".champion.wav";
        if (params->log_save_overall_champ_audio)
            saveWavFile(String(ss.str()), String(logger->net_to_string_save(champ)), String(champ->origin.c_str()), targetSampleRate, params->aux_wav_file_buffer_size, numTargetFrames, champBuffer);
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
    targetLengthSeconds = targetSampleTimes[numTargetFrames - 1];
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
    /*
    targetEnvelope = (float*) malloc(sizeof(float) * numTargetFrames);
    followEnvelope(numTargetFrames, targetFrames, targetEnvelope, params->envelopeFollowerAttack, params->envelopeFollowerDecay, targetSampleRate);
    //findEnvelope();
    if (params->log_save_target_envelope) {
        char buffer[100];
        snprintf(buffer, 100, "targetInfo/targetenvelope.txt");
        saveTextFile(savePath + String(buffer), floatBuffersToGraphText(String("x> y^ xi yf"), String("Time"), String("Amplitude"), false, numTargetFrames, targetSampleTimes, targetEnvelope, NULL));
    }
    */

    // FILL FREQUENCY SPECTRUM OF TARGET
    if (params->ff_type > 0) {
        // calculate with fft window size/overlap
        unsigned n = params->ff_fft_size;
        unsigned overlap = params->ff_fft_overlap;
        fftOutputBufferSize = GPAudioUtil::calculateFftBufferSize(numTargetFrames, n, overlap);
        unsigned numBins = (n/2) + 1;
        unsigned numFftFrames = fftOutputBufferSize / numBins;
        
        // allocate window
        analysisWindow = (float*) malloc(sizeof(float) * n);
        GPAudioUtil::window(params->ff_fft_window, n, analysisWindow);

        // allocate target FFT buffers
	    kiss_fft_scalar* targetAmplitudeBuffer = (kiss_fft_scalar*) malloc(sizeof(kiss_fft_scalar) * n);
        targetSpectra = (kiss_fft_cpx*) malloc(sizeof(kiss_fft_cpx) * fftOutputBufferSize);
        targetMagnitude = (double*) malloc(sizeof(double) * fftOutputBufferSize);
        targetPhase = (double*) malloc(sizeof(double) * fftOutputBufferSize);
        binOvershootingPenalty = (double*) malloc(sizeof(double) * fftOutputBufferSize);
        binUndershootingPenalty = (double*) malloc(sizeof(double) * fftOutputBufferSize);
        fftFrameWeight = (double*) malloc(sizeof(double) * numFftFrames);
        
        // allocate candidate FFT buffers for speedy comparison
        fftConfig = kiss_fftr_alloc(n, 0/*is_inverse_fft*/, NULL, NULL);
		candidateAmplitudeBuffer = (kiss_fft_scalar*) malloc(sizeof(kiss_fft_scalar) * n);
    	candidateSpectraBuffer = (kiss_fft_cpx*) malloc(sizeof(kiss_fft_cpx) * fftOutputBufferSize);
        candidateMagnitudeBuffer = (double*) malloc(sizeof(double) * fftOutputBufferSize);
        candidatePhaseBuffer = (double*) malloc(sizeof(double) * fftOutputBufferSize);

        // take fft of target data
        GPAudioUtil::FftReal(fftConfig, numTargetFrames, targetFrames, n, overlap, analysisWindow, targetAmplitudeBuffer, targetSpectra, false, targetMagnitude, targetPhase);
        free(targetAmplitudeBuffer);

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
        double base = params->ff_mag_base_comparison;
        double good = params->ff_mag_good_comparison;
        double bad = params->ff_mag_bad_comparison;
        double frameAverageSum = 0.0;
        for (unsigned i = 0; i < numFftFrames; i++) {
            // find moving average
            unsigned dataOffset = (i * numBins) + 1;
            double frameAverage, maxDeviationAboveMean, maxDeviationBelowMean, maxRatioAboveMean, maxRatioBelowMean;
            findMovingAverage(params->ff_moving_average_type, numBins - 1, targetMagnitude + dataOffset, mac, params->ff_moving_average_past_radius, params->ff_moving_average_future_radius, params->ff_moving_average_exponential_alpha, &frameAverage, &maxDeviationAboveMean, &maxDeviationBelowMean, &maxRatioAboveMean, &maxRatioBelowMean);
            fftFrameWeight[i] = frameAverage;
            frameAverageSum += frameAverage;

            // compare each bin EXCEPT DC OFFSET to the moving average magnitude
            for (unsigned j = 1; j < numBins; j++) {
                unsigned binIndex = (i * numBins) + j;
                double binMagnitude = targetMagnitude[binIndex];
                double binAverage = mac[j - 1];
                
                // calculate bin comparison metric data
                double numerator, denominator, proportion;
                /*
                if (params->compareToMaxDeviation) {
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
                */
                numerator = fabs(binMagnitude - binAverage);
                if (binMagnitude > binAverage) {
                    denominator = maxDeviationAboveMean;
                }
                else {
                    denominator = maxDeviationBelowMean;
                }

                // calculate bin proportion for penalty assignments
                //proportion = pow(numerator / denominator, params->penaltyComparisonExponent);
                proportion = numerator / denominator;

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
            if (params->log_save_target_spectrum) {
                std::stringstream ss;
                ss << output_dir_path << "target_info/" << i << ".";
                double time = i * (n - overlap);
                time = double(time)/targetSampleRate;
                for (unsigned j = 0; j < numBins - 1; j++) {
                    timeAxis[j] = time;
                }
                saveTextFile(String(ss.str() + "magnitude.txt"), doubleBuffersToGraphText(String(overlap), String(""), String(""), String(""), false, numBins - 1, timeAxis, freqAxis + 1, targetMagnitude + dataOffset));
                saveTextFile(String(ss.str() + "mac.txt"), doubleBuffersToGraphText(String(""), String(""), String(""), String(""), false, numBins - 1, freqAxis + 1, mac, NULL));
                saveTextFile(String(ss.str() + "undershoot.txt"), doubleBuffersToGraphText(String(""), String(""), String(""), String(""), false, numBins - 1, freqAxis + 1, targetMagnitude + dataOffset, binUndershootingPenalty + dataOffset));
                saveTextFile(String(ss.str() + "overshoot.txt"), doubleBuffersToGraphText(String(""), String(""), String(""), String(""), false, numBins - 1, freqAxis + 1, targetMagnitude + dataOffset, binOvershootingPenalty + dataOffset));
            }
        }
        // free temp buffers
        free(freqAxis);
        free(mac);
        free(timeAxis);
        
        // calculate frame weights
        double frameAverage = frameAverageSum / double(numFftFrames);
        if (params->ff_weight_frames) {
            //std::cerr << "average: " << frameAverage << std::endl;
            for (unsigned i = 0; i < numFftFrames; i++) {
                fftFrameWeight[i] = frameAverage / fftFrameWeight[i];
                //std::cerr << i << " weight: " << fftFrameWeight[i] << std::endl;
                fftFrameWeight[i] = pow(fftFrameWeight[i], params->ff_weight_frames_exponent);
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
        renderIndividualByBlockPerformance(candidate, params->aux_render_block_size, numConstantValues, constantValues, numTargetFrames, targetSampleTimes, buffer);
        double fitness = compareToTarget(params->ff_type, buffer);
        candidate->doneRendering();
        return fitness;
    }
    // run Beagle CMAES
    else if (suboptimizeType == 1) {
        std::vector<GPMutatableParam*>* candidateParams = candidate->getAllMutatableParams();
        
        if (candidateParams->size() != 0) {
            try {
                using namespace Beagle;

                // Build system
                System::Handle lSystem = new System;

                // Install the GA float vector and CMA-ES packages
                const unsigned int lVectorSize = candidateParams->size();
                lSystem->addPackage(new GA::PackageFloatVector(lVectorSize));
                lSystem->addPackage(new GA::PackageCMAES(lVectorSize));

                // Initialize state space
                DoubleArray::Handle initMinValueArray = new DoubleArray(lVectorSize);
                DoubleArray::Handle initMaxValueArray = new DoubleArray(lVectorSize);
                DoubleArray::Handle minValueArray = new DoubleArray(lVectorSize);
                DoubleArray::Handle maxValueArray = new DoubleArray(lVectorSize);
                DoubleArray::Handle incValueArray = new DoubleArray(lVectorSize);

                // populate min/max/inc vectors
                for (unsigned i = 0; i < candidateParams->size(); i++) {
                    GPMutatableParam* param = candidateParams->at(i);
                    float paramMin = param->getMin();
                    float paramMax = param->getMax();
                    if (param->isDiscrete()) {
                        // values are to allow any number in discrete range including min/max
                        initMinValueArray->at(i) = paramMin + 0.5f;
                        initMaxValueArray->at(i) = paramMax + 0.5f;
                        minValueArray->at(i) = paramMin + 0.4f;
                        maxValueArray->at(i) = paramMax + 0.6f;
                        incValueArray->at(i) = 1.0f;
                    }
                    else {
                        initMinValueArray->at(i) = paramMin;
                        initMaxValueArray->at(i) = paramMax;
                        minValueArray->at(i) = paramMin;
                        maxValueArray->at(i) = paramMax;
                        incValueArray->at(i) = 0.01f;
                    }
                }
                      
                // register state space with system
                Register::Description lDescription("Parameter state space", "Vector", "", "");
                lSystem->getRegister().insertEntry("ga.init.minvalue", initMinValueArray, lDescription);
                lSystem->getRegister().insertEntry("ga.init.maxvalue", initMaxValueArray, lDescription);
                lSystem->getRegister().insertEntry("ga.float.minvalue", minValueArray, lDescription);
                lSystem->getRegister().insertEntry("ga.float.maxvalue", maxValueArray, lDescription);
                lSystem->getRegister().insertEntry("ga.float.inc", incValueArray, lDescription);

                // Add evaluation operator allocator
                Register::Description hackDescription("Hack", "", "", "");
                AudioComparisonParams::Handle evalParams = new AudioComparisonParams();
                evalParams->experiment = this;
                evalParams->type = suboptimizeType;
                evalParams->candidate = candidate;
                evalParams->candidateFramesBuffer = buffer;
                lSystem->getRegister().insertEntry("audio.params", evalParams, hackDescription);

                // Set evaluation op
                lSystem->setEvaluationOp("AudioComparisonEvalOp", new AudioComparisonEvalOp::Alloc);

                // Set fitness type
                lSystem->getFactory().setConcept("Fitness", "FitnessSimpleMin");

                // Initialize the evolver
                Evolver::Handle lEvolver = new Evolver;
                lEvolver->initialize(lSystem, beagle_cfg_file_path);
                
                // Create population
                Vivarium::Handle lVivarium = new Vivarium;

                // Launch evolution
                lEvolver->evolve(lVivarium, lSystem);
            }
            catch(Beagle::Exception& inException) {
                //logger->debug("Beagle exception caught:");
                //logger->debug(inException.what());
                //inException.terminate(std::cerr);
                return -1;
            }
            catch (std::exception& inException) {
                //logger->debug("Standard exception caught:");
                //logger->debug(inException.what());
                return -1;
            }
        }

        renderIndividualByBlockPerformance(candidate, params->aux_render_block_size, numConstantValues, constantValues, numTargetFrames, targetSampleTimes, buffer);
        double fitness = compareToTarget(params->ff_type, buffer);
        candidate->doneRendering();
        return fitness;
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

	// amplitude comparison
	if (type == 0) {
		ret = GPAudioUtil::compareAmplitudesWeighted(numTargetFrames, candidateFrames, targetFrames, 1.0f);
	}
	
	// spectral comparison
	else if (type == 1) {
		ret = GPAudioUtil::compareSpectraWeighted(false, params->ff_fft_size, params->ff_fft_overlap, numTargetFrames, fftOutputBufferSize, fftConfig, candidateFrames, candidateAmplitudeBuffer, candidateSpectraBuffer, candidateMagnitudeBuffer, candidatePhaseBuffer, analysisWindow, targetMagnitude, targetPhase, binUndershootingPenalty, binOvershootingPenalty, fftFrameWeight, params->ff_phase_comparison_exponent, params->ff_spectrum_mag_weight, params->ff_spectrum_phase_weight);
	}
	
	return ret;
}

double GPExperiment::beagleComparisonCallback(unsigned type, GPNetwork* candidate, float* candidateFramesBuffer) {
		//logger->debug(candidate->toString(5));
        renderIndividualByBlockPerformance(candidate, params->aux_render_block_size, numConstantValues, constantValues, numTargetFrames, targetSampleTimes, candidateFramesBuffer);
        double fitness = compareToTarget(params->ff_type, candidateFramesBuffer);
        //std::cerr << " fitness: " << fitness;
        return compareToTarget(params->ff_type, candidateFramesBuffer);
}

/*
    ===================
    WAVEFORM OPERATIONS
    ===================
*/

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

// TODO: these get super slow when n is large. might want to use std::string and bring it to juce string at the very end
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

int GPExperiment::sanityTest(GPRandom* rng) {
    // TODO: timers for all tests

    // test random
    std::cout << "----TESTING DISCRETE RNG----" << std::endl;
    unsigned m = 4;
    unsigned vals[m];
    for (unsigned i = 0; i < m; i++) {
        vals[i] = 0;
    }
    unsigned counter = 0;
    unsigned numIterations = 1000000;
    while (counter < numIterations) {
        int val = rng->random(m);
        vals[val] += 1;
        counter++;
    }
    for (unsigned i = 0; i < m; i++) {
        std::cout << "Proportion for " << i << ": " << vals[i]/float(numIterations) << std::endl;
    }
    std::cout << "----TESTING CONTINUOUS RNG----" << std::endl;
    double sum = 0;
    counter = 0;
    while (counter < numIterations) {
        sum += rng->random();
        counter++;
    }
    std::cout << "Average of uniform real over 0, 1: " << sum/numIterations << std::endl;

    // buffers for tests
    unsigned wavchunk = 256;
    unsigned numframes = 88200;
    //float maxSeconds = 2.0;
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
    float maxSeconds = times[numframes - 1];
    loadWavFile("./tests/silenceTestTarget.wav", wavchunk, numframes, silenceBuffer);

    // test network strings
    std::string silenceTest = "(silence)";
    std::string noiseTest = "(noise)";
    std::string variableNodeTest = "(sin (* (* (const {d 1 2 5}) (pi)) (* (time) (var {d 0 0 1} {c 0.0 0.0 22050.0})))))";

    // SIN TEST NETWORK
    std::cout << "----TESTING BASIC SINE WAVE----" << std::endl;
    std::string sinTest = "(sin (* (* (const {d 2 2 2}) (pi)) (* (time) (const {c 440.0 440.0 440.0}))))";
    GPNetwork* sinTestNet = new GPNetwork(rng, sinTest);

    std::cout << "Network before trace:" << std::endl << sinTestNet->toString(10) << std::endl;
    std::cout << "Height: " << sinTestNet->height << std::endl;
    std::cout << "Min: " << sinTestNet->minimum << std::endl;
    std::cout << "Max: " << sinTestNet->maximum << std::endl;
    renderIndividualByBlockPerformance(sinTestNet, renderblocksize, numconstantvariables, variables, numframes, times, testBuffer);
    sinTestNet->doneRendering();
    assert(GPAudioUtil::compareAmplitudes(numframes, silenceBuffer, testBuffer) == 0);

    sinTestNet->traceNetwork();
    std::cout << "Network after trace:" << std::endl << sinTestNet->toString(10) << std::endl;
    std::cout << "Height: " << sinTestNet->height << std::endl;
    std::cout << "Min: " << sinTestNet->minimum << std::endl;
    std::cout << "Max: " << sinTestNet->maximum << std::endl;
    renderIndividualByBlockPerformance(sinTestNet, renderblocksize, numconstantvariables, variables, numframes, times, testBuffer);
    sinTestNet->doneRendering();
    assert(GPAudioUtil::compareAmplitudes(numframes, silenceBuffer, testBuffer) == 0);

    sinTestNet->prepareToRender(samplerate, renderblocksize, numframes, maxSeconds); 
    sinTestNet->prepareToRender(samplerate, renderblocksize, numframes, maxSeconds); 
    std::cout << "Network after prepare:" << std::endl << sinTestNet->toString(10) << std::endl;
    std::cout << "Height: " << sinTestNet->height << std::endl;
    std::cout << "Min: " << sinTestNet->minimum << std::endl;
    std::cout << "Max: " << sinTestNet->maximum << std::endl;
    renderIndividualByBlockPerformance(sinTestNet, renderblocksize, numconstantvariables, variables, numframes, times, testBuffer);
    sinTestNet->doneRendering();
    loadWavFile("./tests/sineWave440TestTarget.wav", wavchunk, numframes, comparisonBuffer);
    double error = GPAudioUtil::compareAmplitudes(numframes, comparisonBuffer, testBuffer);
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
    cf->setCData(15, 30, 300);
    std::cout << "Network after mutate:" << std::endl << sinTestNetNewCenter->toString(10) << std::endl;
    renderIndividualByBlockPerformance(sinTestNetNewCenter, renderblocksize, numconstantvariables, variables, numframes, times, testBuffer);
    sinTestNetNewCenter->prepareToRender(samplerate, renderblocksize, numframes, maxSeconds); 
    renderIndividualByBlockPerformance(sinTestNetNewCenter, renderblocksize, numconstantvariables, variables, numframes, times, testBuffer);
    saveWavFile("./sineWaveTestNewCenter.wav", String(sinTestNetNewCenter->toString(10).c_str()), String("test"), samplerate, wavchunk, numframes, testBuffer);
    delete sinTestNetNewCenter;

    // adsr test network
    std::string ADSRTest = "(adsr* {c 0 0.2 2} {c 0 0.2 2} {c 0 1.0 1.0} {c 0 0.2 2} {c 0 0.2 2} {c 0 0.5 1.0} {c 0 0.2 2} " + sinTest + ")";
    GPNetwork* ADSRTestNet = new GPNetwork(rng, ADSRTest);
    ADSRTestNet->traceNetwork();
    ADSRTestNet->prepareToRender(samplerate, renderblocksize, numframes, maxSeconds);
    std::cout << "---- TESTING ADSR----" << std::endl;
    std::cout << "Network: " << std::endl << ADSRTestNet->toString(10) << std::endl;
    std::cout << "Height: " << ADSRTestNet->height << std::endl;
    std::cout << "Min: " << ADSRTestNet->minimum << std::endl;
    std::cout << "Max: " << ADSRTestNet->maximum << std::endl;
    renderIndividualByBlockPerformance(ADSRTestNet, renderblocksize, numconstantvariables, variables, numframes, times, testBuffer);
    ADSRTestNet->doneRendering();
    saveWavFile("./ADSRsineWaveTest.wav", String(ADSRTestNet->toString(10).c_str()), String("test"), samplerate, wavchunk, numframes, testBuffer);
    delete ADSRTestNet;

    // constant node envelope test network
    std::string constantNodeEnvelopeTest = "(gain {c 0 0.5 1.0} " + sinTest + ")";
    GPNetwork* constantNodeEnvelopeTestNet = new GPNetwork(rng, constantNodeEnvelopeTest);
    constantNodeEnvelopeTestNet->traceNetwork();
    constantNodeEnvelopeTestNet->prepareToRender(samplerate, renderblocksize, numframes, maxSeconds);
    std::cout << "----TESTING CONSTANT NODE ENVELOPE----" << std::endl;
    std::cout << "Network: " << std::endl << constantNodeEnvelopeTestNet->toString(10) << std::endl;
    std::cout << "Height: " << constantNodeEnvelopeTestNet->height << std::endl;
    std::cout << "Min: " << constantNodeEnvelopeTestNet->minimum << std::endl;
    std::cout << "Max: " << constantNodeEnvelopeTestNet->maximum << std::endl;
    renderIndividualByBlockPerformance(constantNodeEnvelopeTestNet, renderblocksize, numconstantvariables, variables, numframes, times, testBuffer);
    constantNodeEnvelopeTestNet->doneRendering();
    saveWavFile("./constantNodeEnvelopesineWaveTest.wav", String(constantNodeEnvelopeTestNet->toString(10).c_str()), String("test"), samplerate, wavchunk, numframes, testBuffer);
    delete constantNodeEnvelopeTestNet;

    // sin oscillator test
    std::string sinOscTest = "(sinosc {d 0 0 1} {c 0.0 1.0 10.0} {c 0.0 0.0 1.0})";
    GPNetwork* sinOscTestNetPreCopy = new GPNetwork(rng, sinOscTest);
    sinOscTestNetPreCopy->traceNetwork();
    GPNetwork* sinOscTestNet = sinOscTestNetPreCopy->getCopy("test");
    delete sinOscTestNetPreCopy;
    sinOscTestNet->traceNetwork();
    sinOscTestNet->prepareToRender(samplerate, renderblocksize, numframes, maxSeconds);
    std::cout << "----TESTING SINE WAVE OSCILLATOR----" << std::endl;
    std::cout << "Network: " << std::endl << sinOscTestNet->toString(10) << std::endl;
    std::cout << "Height: " << sinOscTestNet->height << std::endl;
    std::cout << "Min: " << sinOscTestNet->minimum << std::endl;
    std::cout << "Max: " << sinOscTestNet->maximum << std::endl;
    renderIndividualByBlockPerformance(sinOscTestNet, renderblocksize, numconstantvariables, variables, numframes, times, testBuffer);
    sinOscTestNet->doneRendering();
    saveWavFile("./sinOscTest.wav", String(sinOscTestNet->toString(10).c_str()), String("test"), samplerate, wavchunk, numframes, testBuffer);
    delete sinOscTestNet;

    // saw oscillator test
    std::string sawOscTest = "(sawosc {d 0 0 1} {c 0.0 1.0 10.0} {c 0.0 0.0 1.0})";
    GPNetwork* sawOscTestNetPreCopy = new GPNetwork(rng, sawOscTest);
    sawOscTestNetPreCopy->traceNetwork();
    GPNetwork* sawOscTestNet = sawOscTestNetPreCopy->getCopy("test");
    delete sawOscTestNetPreCopy;
    sawOscTestNet->traceNetwork();
    sawOscTestNet->prepareToRender(samplerate, renderblocksize, numframes, maxSeconds);
    std::cout << "----TESTING SAWTOOTH WAVE OSCILLATOR----" << std::endl;
    std::cout << "Network: " << std::endl << sawOscTestNet->toString(10) << std::endl;
    std::cout << "Height: " << sawOscTestNet->height << std::endl;
    std::cout << "Min: " << sawOscTestNet->minimum << std::endl;
    std::cout << "Max: " << sawOscTestNet->maximum << std::endl;
    renderIndividualByBlockPerformance(sawOscTestNet, renderblocksize, numconstantvariables, variables, numframes, times, testBuffer);
    sawOscTestNet->doneRendering();
    saveWavFile("./sawOscTest.wav", String(sawOscTestNet->toString(10).c_str()), String("test"), samplerate, wavchunk, numframes, testBuffer);
    delete sawOscTestNet;

    // square oscillator test
    std::string squareOscTest = "(squareosc {d 0 0 1} {c 0.0 1.0 10.0} {c 0.0 0.0 1.0})";
    GPNetwork* squareOscTestNetPreCopy = new GPNetwork(rng, squareOscTest);
    squareOscTestNetPreCopy->traceNetwork();
    GPNetwork* squareOscTestNet = squareOscTestNetPreCopy->getCopy("test");
    delete squareOscTestNetPreCopy;
    squareOscTestNet->traceNetwork();
    squareOscTestNet->prepareToRender(samplerate, renderblocksize, numframes, maxSeconds);
    std::cout << "----TESTING SQUARE WAVE OSCILLATOR----" << std::endl;
    std::cout << "Network: " << std::endl << squareOscTestNet->toString(10) << std::endl;
    std::cout << "Height: " << squareOscTestNet->height << std::endl;
    std::cout << "Min: " << squareOscTestNet->minimum << std::endl;
    std::cout << "Max: " << squareOscTestNet->maximum << std::endl;
    renderIndividualByBlockPerformance(squareOscTestNet, renderblocksize, numconstantvariables, variables, numframes, times, testBuffer);
    squareOscTestNet->doneRendering();
    saveWavFile("./squareOscTest.wav", String(squareOscTestNet->toString(10).c_str()), String("test"), samplerate, wavchunk, numframes, testBuffer);
    delete squareOscTestNet;

    // triangle oscillator test
    std::string triangleOscTest = "(triangleosc {d 0 0 1} {c 0.0 1.0 10.0} {c 0.0 0.0 1.0})";
    GPNetwork* triangleOscTestNetPreCopy = new GPNetwork(rng, triangleOscTest);
    triangleOscTestNetPreCopy->traceNetwork();
    GPNetwork* triangleOscTestNet = triangleOscTestNetPreCopy->getCopy("test");
    delete triangleOscTestNetPreCopy;
    triangleOscTestNet->traceNetwork();
    triangleOscTestNet->prepareToRender(samplerate, renderblocksize, numframes, maxSeconds);
    std::cout << "----TESTING TRIANGLE WAVE OSCILLATOR----" << std::endl;
    std::cout << "Network: " << std::endl << triangleOscTestNet->toString(10) << std::endl;
    std::cout << "Height: " << triangleOscTestNet->height << std::endl;
    std::cout << "Min: " << triangleOscTestNet->minimum << std::endl;
    std::cout << "Max: " << triangleOscTestNet->maximum << std::endl;
    renderIndividualByBlockPerformance(triangleOscTestNet, renderblocksize, numconstantvariables, variables, numframes, times, testBuffer);
    triangleOscTestNet->doneRendering();
    saveWavFile("./triangleOscTest.wav", String(triangleOscTestNet->toString(10).c_str()), String("test"), samplerate, wavchunk, numframes, testBuffer);
    delete triangleOscTestNet;

    // erc spline test network
    std::string splineTest = "(spline {d 0 0 0} {d 0 0 5} {c 0 0 1.0} {c 0 0 0.2})";
    GPNetwork* splineTestNet = new GPNetwork(rng, splineTest);
    splineTestNet->ephemeralRandom(rng);
    splineTestNet->traceNetwork();
    splineTestNet->prepareToRender(samplerate, renderblocksize, numframes, maxSeconds);
    std::cout << "---- TESTING ERC SPLINE ----" << std::endl;
    std::cout << "Network: " << std::endl << splineTestNet->toString(10) << std::endl;
    std::cout << "Height: " << splineTestNet->height << std::endl;
    std::cout << "Min: " << splineTestNet->minimum << std::endl;
    std::cout << "Max: " << splineTestNet->maximum << std::endl;
    renderIndividualByBlockPerformance(splineTestNet, renderblocksize, numconstantvariables, variables, numframes, times, testBuffer);
    splineTestNet->doneRendering();
    saveWavFile("./splineEnvelopeTest.wav", String(splineTestNet->toString(10).c_str()), String("test"), samplerate, wavchunk, numframes, testBuffer);
    delete splineTestNet;

    // instantiated spline test network
    std::string splineInstantiatedTest = "(spline {d 0 0 0} {d 0 3 5} {c 0 0.4 1} {c 0 0.1 0.2} {c 0 0.2 1} {c 0 0 0.3} {c 0 0.5 1} {c 0 1.5 2.0} {c -1.0 -0.5 1.0})";
    GPNetwork* splineInstantiatedTestNet = new GPNetwork(rng, splineInstantiatedTest);
    splineInstantiatedTestNet->traceNetwork();
    splineInstantiatedTestNet->prepareToRender(samplerate, renderblocksize, numframes, maxSeconds);
    std::cout << "---- TESTING SPLINE ----" << std::endl;
    std::cout << "Network: " << std::endl << splineInstantiatedTestNet->toString(10) << std::endl;
    std::cout << "Height: " << splineInstantiatedTestNet->height << std::endl;
    std::cout << "Min: " << splineInstantiatedTestNet->minimum << std::endl;
    std::cout << "Max: " << splineInstantiatedTestNet->maximum << std::endl;
    renderIndividualByBlockPerformance(splineInstantiatedTestNet, renderblocksize, numconstantvariables, variables, numframes, times, testBuffer);
    splineInstantiatedTestNet->doneRendering();
    saveWavFile("./splineInstantiatedEnvelopeTest.wav", String(splineInstantiatedTestNet->toString(10).c_str()), String("test"), samplerate, wavchunk, numframes, testBuffer);
    delete splineInstantiatedTestNet;

    // spline sine envelope
    std::string splineEnvelopeTest = "(spline* {d 0 0 0} {d 0 0 10} {c 0 0 1.0} {c 0 0 0.2} " + sinTest + ")";
    GPNetwork* splineSineEnvelopeTestNet = new GPNetwork(rng, splineEnvelopeTest);
    splineSineEnvelopeTestNet->ephemeralRandom(rng);
    splineSineEnvelopeTestNet->traceNetwork();
    splineSineEnvelopeTestNet->prepareToRender(samplerate, renderblocksize, numframes, maxSeconds);
    std::cout << "---- TESTING SPLINE SINE ENVELOPE ----" << std::endl;
    std::cout << "Network: " << std::endl << splineSineEnvelopeTestNet->toString(10) << std::endl;
    std::cout << "Height: " << splineSineEnvelopeTestNet->height << std::endl;
    std::cout << "Min: " << splineSineEnvelopeTestNet->minimum << std::endl;
    std::cout << "Max: " << splineSineEnvelopeTestNet->maximum << std::endl;
    renderIndividualByBlockPerformance(splineSineEnvelopeTestNet, renderblocksize, numconstantvariables, variables, numframes, times, testBuffer);
    splineSineEnvelopeTestNet->doneRendering();
    saveWavFile("./splineSineWaveEnvelopeTest.wav", String(splineSineEnvelopeTestNet->toString(10).c_str()), String("test"), samplerate, wavchunk, numframes, testBuffer);
    delete splineSineEnvelopeTestNet;

    // additive synthesis test
    /*
    std::string additiveSynthesisTest = "(+ (gain {c 0 0.5 1.0} (sinosc {d 0 0 1} {d 0 1 10})) (gain {c 0 0.5 1.0} (sinosc {d 0 1 1} {d 0 1 10})))";
    GPNetwork* additiveSynthesisTestNet = new GPNetwork(rng, additiveSynthesisTest);
    additiveSynthesisTestNet->traceNetwork();
    additiveSynthesisTestNet->prepareToRender(samplerate, renderblocksize, numframes, maxSeconds);
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
    FMSynthesisTestNet->prepareToRender(samplerate, renderblocksize, numframes, maxSeconds);
    std::cout << "----TESTING FM SYNTHESIS----" << std::endl;
    std::cout << "Network: " << std::endl << FMSynthesisTestNet->toString(10) << std::endl;
    std::cout << "Height: " << FMSynthesisTestNet->height << std::endl;
    std::cout << "Min: " << FMSynthesisTestNet->minimum << std::endl;
    std::cout << "Max: " << FMSynthesisTestNet->maximum << std::endl;
    renderIndividualByBlockPerformance(FMSynthesisTestNet, renderblocksize, numconstantvariables, variables, numframes, times, testBuffer);
    FMSynthesisTestNet->doneRendering();
    saveWavFile("./FMSynthesisTest.wav", String(FMSynthesisTestNet->toString(10).c_str()), String("test"), samplerate, wavchunk, numframes, testBuffer);
    delete FMSynthesisTestNet;

    */

    // free test buffer
    free(variables);
    free(times);
    free(testBuffer);
    free(comparisonBuffer);
    free(silenceBuffer);
    
    return 1;
}
