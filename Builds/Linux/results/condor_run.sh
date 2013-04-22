#!/bin/bash

set -e

# LOCAL TRUMPET TEST
../../build/GPAudioTargetMatchingExperiment\
  TEMP PARAMS \
  --target ../../samples/TrumpetEb5.wav\
  --path ./\
  --seed $1\
  --values 622.25\
  --expinfo\
  EXP PARAMS \
  --experiment 4\
  --fitnesstype 1\
  --generations 200\
  --threshold 0.1\
  --erc\
  AUX PARAMS \
  --verbose\
  --nosavegenchamps\
  --printprecision 3\
  --saveprecision 10\
  --loadwavblock 256\
  --render 1024\
  FF WEIGHTS \
  --ffmagweight 1.0\
  --ffphaseweight 0.2\
  --ffenvweight 0.5\
  TIME DOMAIN \
  --nosavetargetenv\
  --envatk 1\
  --envdcy 300\
  --envskip 9\
  FREQ DOMAIN \
  --window hann\
  --fft 1024\
  --fftoverlap 0\
  --ampmag\
  --averagetype 0\
  --averageradius 10\
  --phasepenalty 2.0\
  --goodmagcomp 0.1\
  --badmagcomp 1.1\
  --basemagcomp 0.9\
  EVO PARAMS \
  --popsize 4000\
  --nobackup\
  --backupprecision 50\
  --bestfitness 0.0\
  --mid 5\
  --md 11\
  GENETIC PARAMS \
  --greedy 0.0\
  --numericmutation 0.05 --nmselect 1 --nmselectparam 0.05 --nmtemperature 0.9\
  --mutation 0.05 --mselect 1 --mtype 1 --mselectparam 0.25\
  --crossover 0.9 --cselect 0 --ctype 0\
  --reproduction 0.05 --rselect 0

set +e

