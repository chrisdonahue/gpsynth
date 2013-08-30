#!/bin/bash

set -e

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../../../../Source/Dependencies/PACC/lib:../../../../Source/Dependencies/OpenBEAGLE/lib

../../build/GPAudioTargetMatchingExperimentCondor\
    --sys_info \
    --logger_cfg ./condor_cfg/logger.cfg \
    --synth_cfg ./condor_cfg/synth.cfg \
    --primitives ./condor_cfg/primitives.cfg \
    --me_cfg ./condor_cfg/me.cfg \
    --beagle_cfg ./condor_cfg/beagle.cfg \
    --target ../../samples/TrumpetEb5.wav \
    --output_dir ./output/ \
    --seed $1 \
    --constants 622.25

set +e
