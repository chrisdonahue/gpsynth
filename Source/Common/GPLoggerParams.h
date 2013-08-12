#ifndef GPPARAMS_H
#define GPPARAMS_H

#include "GPRandom.h"

struct GPLoggerParams {
	// synth and experiment shared params
    bool verbose;
    unsigned savePrecision;
    unsigned printPrecision;
    bool lowerFitnessIsBetter;
    double bestPossibleFitness;

    // auxiliary experiment parameters
    bool saveGenerationChampions;
    bool backupInputFiles;
    unsigned wavFileBufferSize;
    unsigned renderBlockSize;
};

#endif
