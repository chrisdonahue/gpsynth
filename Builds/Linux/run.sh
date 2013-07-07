#!/bin/bash

set -e

# LOCAL TRUMPET TEST
<<COMMENT
#gdb --args \
valgrind -v --leak-check=full --show-reachable=yes --log-file="2GenValgrind.txt" \
./build/GPAudioTargetMatchingExperiment\
  TEMP FIELDS\
  --target ./samples/TrumpetEb5.wav\
  --path ./\
  --values 622.25\
  --expinfo\
  SHARED PARAMS\
  --verbose\
  --saveprecision 5\
  --printprecision 3\
  EXPERIMENT PARAMS\
  --experiment 1\
  --suboptimizetype 0\
  --fitnesstype 1\
  --generations 2\
  --threshold 0.1\
  --erc\
  AUXILIARY EXPERIMENT PARAMS\
  --savegenchamps\
  --nosavetargetenv\
  --nosavetargetspectrum\
  --backuptarget\
  --loadwavblock 512\
  --render 1024\
  FITNESS FUNCTION WEIGHTS\
  --ffmagweight 1.0\
  --ffphaseweight 0.5\
  --ffenvweight 0.5\
  TIME DOMAIN PARAMS\
  --envatk 1\
  --envdcy 300\
  --envskip 9\
  FREQUENCY DOMAIN PARAMS\
  --window hann\
  --fft 1024\
  --fftoverlap 0\
  --ampmag\
  --averagetype 2\
  --averagepastradius 40\
  --averagefutureradius 40\
  --alpha 0.95\
  --comparedev\
  --penaltycompexp 1.0\
  --weightframes\
  --frameweightexp 0.33\
  --phasepenalty 2.0\
  --goodmagcomp 0.1\
  --badmagcomp 1.1\
  --basemagcomp 0.9\
  SYNTH EVO PARAMS\
  --popsize 10\
  --nobackup\
  --backupprecision 50\
  --maxinitheight 10\
  --maxheight 10\
  SYNTH GENETIC PARAMS\
  --greedy 0.0\
  --numericmutation 0.25 --nmselect 1 --nmselectparam 0.05 --nmtemperature 0.9\
  --mutation 0.25 --mselect 1 --mtype 1 --mselectparam 0.25\
  --crossover 0.25 --cselect 0 --ctype 0\
  --reproduction 0.25 --rselect 0\
  > 2GenValgrind.txt
COMMENT

# SANITY TEST
#<<COMMENT
#gdb --args \
#valgrind -v --leak-check=full --show-reachable=yes --log-file="testValgrind.txt" \
./build/GPAudioTargetMatchingExperiment\
  --path ./\
  --experiment 0 \
  --loadwavblock 256 \
#> testValgrind.txt
#COMMENT

# FITNESS FUNCTION SAVE
<<COMMENT
./build/GPAudioTargetMatchingExperiment\
  TEMP FIELDS\
  --target ./samples/TrumpetEb51024.wav\
  --path ./\
  --seed 1\
  --values 622.25\
  AUX PARAMS\
  --savetargetspectrum\
  EXPERIMENT PARAMS\
  --experiment 1\
  --fitnesstype 1\
  --generations 0\
  FITNESS FUNCTION WEIGHTS\
  --ffmagweight 1.0\
  --ffphaseweight 0.2\
  --ffenvweight 0.5\
  TIME DOMAIN PARAMS\
  --nosavetargetenv\
  --envatk 1\
  --envdcy 300\
  --envskip 9\
  FREQUENCY DOMAIN PARAMS\
  --window hann\
  --fft 1024\
  --fftoverlap 0\
  --ampmag\
  --averagetype 2\
  --averagepastradius 200\
  --averagefutureradius 200\
  --alpha 0.9\
  --comparedev\
  --penaltycompexp 0.4\
  --weightframes\
  --frameweightexp 0.33\
  --phasepenalty 2.0\
  --goodmagcomp 0.1\
  --badmagcomp 1.1\
  --basemagcomp 0.9
COMMENT

# DEBUG MEMORY
<<COMMENT
valgrind -v --leak-check=full --show-reachable=yes --log-file="2GenValgrind.txt"\
  > 2GenValgrind.txt
COMMENT

# DEBUG HEAP
<<COMMENT
valgrind -v --tool=massif --log-file="3GenValgrindMassif.txt"\
  > 3GenValgrindMassif.txt
COMMENT

set +e
