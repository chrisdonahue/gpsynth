#!/bin/bash

set -e

../../build/GPAudioTargetMatchingExperimentCondor\
    --sys_info \
    --logger_cfg ./cfg/logger.cfg \
    --synth_cfg ./cfg/synth.cfg \
    --primitives ./cfg/primitives.cfg \
    --me_cfg ./cfg/me.cfg \
    --beagle_cfg ./cfg/beagle.cfg \
    --target ../../samples/TrumpetEb5.wav \
    --output_dir ./output/ \
    --seed $1 \
    --constants 622.25

set +e
