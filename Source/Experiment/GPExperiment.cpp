#include "GPExperiment.h"

/*
    ============
    CONSTRUCTION
    ============
*/

GPExperiment::GPExperiment(GPLogger* logger, GPMatchingExperimentParams* params, unsigned seed, std::string beagle_cfg_file_path, GPSynth* synth, GPAudioComparator* comparator, std::string output_dir_path, std::vector<float>& constants) :
    is_sanity_test(false),
    logger(logger),
    params(params), beagle_cfg_file_path(beagle_cfg_file_path),
    seed(seed), seed_string(logger->get_seed_string()),
    synth(synth), comparator(comparator),
    output_dir_path(output_dir_path),
    numConstantValues(constants.size()), constantValues(constants.data()),
    suboptimize_network(NULL), suboptimize_best_params(0), suboptimize_min_fitness(0.0f)
{
    // backup target if requested
    if (params->log_save_target_copy)
        comparator->save_target_backup(output_dir_path + "target_copy.wav");

    // save spectrum if requested
    if (params->log_save_target_spectrum)
        comparator->save_target_spectrum(output_dir_path);

    // retrieve target metadata
    target_sampling_frequency = (float) comparator->get_target_sampling_frequency();
    target_nyquist_frequency = (float) comparator->get_target_nyquist_frequency();
    target_num_frames = (unsigned) comparator->get_target_num_frames();
    target_last_sample_start_time = comparator->get_target_last_sample_start_time();
    target_sample_times = (float*) malloc(sizeof(float) * target_num_frames);
    GPAudioUtil::fill_time_domain_buffer(target_num_frames, comparator->get_target_sampling_frequency(), target_sample_times);

    // experiment state
    minFitnessAchieved = INFINITY;
    numEvaluatedGenerations = 0;
}

GPExperiment::GPExperiment(GPLogger* logger) :
    is_sanity_test(true),
    logger(logger)
{}

GPExperiment::~GPExperiment() {
    if (!is_sanity_test) {
        free(target_sample_times);
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
    float* candidateData = (float*) malloc(sizeof(float) * target_num_frames);
    float* champBuffer = (float*) malloc(sizeof(float) * target_num_frames);

    while (minFitnessAchieved > params->exp_threshold && numEvaluatedGenerations < params->exp_generations) {
        // get individual from EA and suboptimize/evaluate it
        GPNetwork* candidate = synth->getIndividual();
        candidate->traceNetwork();
        candidate->prepareToRender(target_sampling_frequency, params->aux_render_block_size, target_num_frames, target_last_sample_start_time);
        double fitness = suboptimizeAndCompareToTarget(params->exp_suboptimize_type, candidate, candidateData);
        
        // report fitness to EA
        numUnevaluatedThisGeneration = synth->assignFitness(candidate, fitness);
        if (fitness < minFitnessAchieved) {
            minFitnessAchieved = fitness;
            logger->debug << "NEW EXPERIMENT MIN FITNESS ACHIEVED: " << fitness << ", CANDIDATE FITNESS: " << candidate->fitness << std::flush;
        }
        numEvaluatedThisGeneration++;

        // if we're done with this generation...
        if (numUnevaluatedThisGeneration == 0) {
            if (params->log_save_gen_champ_audio) {
                // grab and render the generation champ
                GPNetwork* generationChamp = synth->generationChamp;
                generationChamp->prepareToRender(target_sampling_frequency, params->aux_render_block_size, target_num_frames, target_last_sample_start_time);
                renderIndividualByBlockPerformance(generationChamp, params->aux_render_block_size, numConstantValues, constantValues, target_num_frames, target_sample_times, champBuffer);
                generationChamp->doneRendering();

                // save generation champions
                std::stringstream ss;
                ss << output_dir_path << seed_string << ".gen." << numEvaluatedGenerations << ".best.wav";
                JUCEFileIO::save_wav_file(ss.str(), logger->net_to_string_save(generationChamp), generationChamp->origin, target_sampling_frequency, params->aux_wav_file_buffer_size, target_num_frames, champBuffer);
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
                JUCEFileIO::save_text_file(ss.str(), ss2.str()); 
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
        champ->prepareToRender(target_sampling_frequency, params->aux_render_block_size, target_num_frames, target_last_sample_start_time);
        renderIndividualByBlockPerformance(champ, params->aux_render_block_size, numConstantValues, constantValues, target_num_frames, target_sample_times, champBuffer);
        champ->doneRendering();
        if (champ->fitness != minFitnessAchieved) {
            logger->error << "Champ fitness: " << champ->fitness << " is not equal to experiment min fitness achieved: " << minFitnessAchieved << std::flush;
            assert(champ->fitness == minFitnessAchieved);
        }
        std::stringstream ss;
        ss << output_dir_path << seed_string << ".champion.wav";
        if (params->log_save_overall_champ_audio)
            JUCEFileIO::save_wav_file(ss.str(), logger->net_to_string_save(champ), champ->origin, target_sampling_frequency, params->aux_wav_file_buffer_size, target_num_frames, champBuffer);
    }
    free(champBuffer);
    return champ;
}

/*
    ================
    FITNESS FUNCTION
    ================
*/

double GPExperiment::suboptimizeAndCompareToTarget(unsigned suboptimizeType, GPNetwork* candidate, float* buffer) {
    // suboptimize according to suboptimize type
    if (suboptimizeType == 0) {
        renderIndividualByBlockPerformance(candidate, params->aux_render_block_size, numConstantValues, constantValues, target_num_frames, target_sample_times, buffer);
        double fitness = compareToTarget(params->exp_suboptimize_ff_type, buffer);
        candidate->doneRendering();
        return fitness;
    }
    // run Beagle CMAES
    else if (suboptimizeType == 1) {
        std::vector<GPMutatableParam*>* candidateParams = candidate->getAllMutatableParams();
        
        if (candidateParams->size() != 0) {
            try {
                // test initial conditions
                suboptimize_network = candidate;
                renderIndividualByBlockPerformance(suboptimize_network, params->aux_render_block_size, numConstantValues, constantValues, target_num_frames, target_sample_times, buffer);
                suboptimize_min_fitness = compareToTarget(params->exp_suboptimize_ff_type, buffer);

                // backup initial params
                logger->debug << "BEFORE(" << suboptimize_min_fitness << "): " << logger->net_to_string_print(suboptimize_network) << std::flush;
                std::vector<GPMutatableParam*>* candidate_params = suboptimize_network->getAllMutatableParams();
                unsigned num_params = candidate_params->size();
                suboptimize_best_params.resize(num_params);
                for (unsigned i = 0; i < num_params; i++) {
                    GPMutatableParam* param = candidate_params->at(i);
                    suboptimize_best_params[i] = param->getCopy();
                }

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
                unsigned seed_beagle = seed + 1;
                lSystem->getRegister().insertEntry("ec.rand.seed", new ULong(seed_beagle), lDescription);
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

                // Lamarckian!
                for (unsigned i = 0; i < num_params; i++) {
                    GPMutatableParam* param = suboptimize_best_params[i];
                    GPMutatableParam* stored_param = candidate_params->at(i);
                    if (param->isDiscrete()) {
                        stored_param->setDValue(param->getDValue());
                    }
                    else {
                        stored_param->setCValue(param->getCValue());
                    }
                    delete param;
                }
                logger->debug << "AFTER(" << suboptimize_min_fitness << "): " << logger->net_to_string_print(suboptimize_network) << std::flush;
            }
            catch(Beagle::Exception& inException) {
                logger->error << "Beagle exception caught:" << std::flush;
                logger->error << inException.what() << std::flush;
                inException.terminate(logger->error);
                return -1;
            }
            catch (std::exception& inException) {
                logger->error << "Standard exception caught:" << std::flush;
                logger->error << inException.what() << std::flush;
                return -1;
            }
        }
        candidate->doneRendering();
        return suboptimize_min_fitness;
    }
    return -1;
}

double GPExperiment::beagleComparisonCallback(unsigned type, float* candidateFramesBuffer) {
        renderIndividualByBlockPerformance(suboptimize_network, params->aux_render_block_size, numConstantValues, constantValues, target_num_frames, target_sample_times, candidateFramesBuffer);
        double fitness = compareToTarget(params->exp_suboptimize_ff_type, candidateFramesBuffer);
        if (fitness < suboptimize_min_fitness) {
            suboptimize_min_fitness = fitness;
            std::vector<GPMutatableParam*>* network_params = suboptimize_network->getAllMutatableParams();
            for (unsigned i = 0; i < suboptimize_best_params.size(); i++) {
                GPMutatableParam* param = network_params->at(i);
                GPMutatableParam* stored_param = suboptimize_best_params[i];
                if (param->isDiscrete()) {
                    stored_param->setDValue(param->getDValue());
                }
                else {
                    stored_param->setCValue(param->getCValue());
                }
            }
            logger->debug << "NEW(" << suboptimize_min_fitness << "): " << logger->net_to_string_print(suboptimize_network) << std::flush;
        }
        return compareToTarget(params->exp_suboptimize_ff_type, candidateFramesBuffer);
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
		ret = comparator->compare_amplitude(candidateFrames);
	}
	
	// weighted amplitude comparison
	else if (type == 1) {
		ret = comparator->compare_amplitude_weighted(candidateFrames);
	}

	// spectral comparison
	else if (type == 2) {
		ret = comparator->compare_spectra(candidateFrames);
	}

	// weighted spectral comparison
	else if (type == 3) {
		ret = comparator->compare_spectra_weighted(candidateFrames);
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
    GPAudioUtil::fill_time_domain_buffer(numframes, samplerate, times);
    float maxSeconds = times[numframes - 1];
    JUCEFileIO::load_wav_file("./tests/silenceTestTarget.wav", wavchunk, numframes, silenceBuffer);

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
    JUCEFileIO::load_wav_file("./tests/sineWave440TestTarget.wav", wavchunk, numframes, comparisonBuffer);
    double error = GPAudioUtil::compareAmplitudes(numframes, comparisonBuffer, testBuffer);
    std::cout << "Comparison error to Audacity sine wave: " << error << std::endl;
    assert(error < 10);
    JUCEFileIO::save_wav_file("./sineWaveTest.wav", sinTestNet->toString(10), "test", samplerate, wavchunk, numframes, testBuffer);

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
    JUCEFileIO::save_wav_file("./sineWaveTestNewCenter.wav", sinTestNetNewCenter->toString(10), "test", samplerate, wavchunk, numframes, testBuffer);
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
    JUCEFileIO::save_wav_file("./ADSRsineWaveTest.wav", ADSRTestNet->toString(10), "test", samplerate, wavchunk, numframes, testBuffer);
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
    JUCEFileIO::save_wav_file("./constantNodeEnvelopesineWaveTest.wav", constantNodeEnvelopeTestNet->toString(10), "test", samplerate, wavchunk, numframes, testBuffer);
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
    JUCEFileIO::save_wav_file("./sinOscTest.wav", sinOscTestNet->toString(10), "test", samplerate, wavchunk, numframes, testBuffer);
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
    JUCEFileIO::save_wav_file("./sawOscTest.wav", sawOscTestNet->toString(10), "test", samplerate, wavchunk, numframes, testBuffer);
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
    JUCEFileIO::save_wav_file("./squareOscTest.wav", squareOscTestNet->toString(10), "test", samplerate, wavchunk, numframes, testBuffer);
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
    JUCEFileIO::save_wav_file("./triangleOscTest.wav", triangleOscTestNet->toString(10), "test", samplerate, wavchunk, numframes, testBuffer);
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
    JUCEFileIO::save_wav_file("./splineEnvelopeTest.wav", splineTestNet->toString(10), "test", samplerate, wavchunk, numframes, testBuffer);
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
    JUCEFileIO::save_wav_file("./splineInstantiatedEnvelopeTest.wav", splineInstantiatedTestNet->toString(10), "test", samplerate, wavchunk, numframes, testBuffer);
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
    JUCEFileIO::save_wav_file("./splineSineWaveEnvelopeTest.wav", splineSineEnvelopeTestNet->toString(10), "test", samplerate, wavchunk, numframes, testBuffer);
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
    JUCEFileIO::save_wav_file("./additiveSynthesisTest.wav", String(additiveSynthesisTestNet->toString(10).c_str()), String("test"), samplerate, wavchunk, numframes, testBuffer);
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
    JUCEFileIO::save_wav_file("./FMSynthesisTest.wav", String(FMSynthesisTestNet->toString(10).c_str()), String("test"), samplerate, wavchunk, numframes, testBuffer);
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
