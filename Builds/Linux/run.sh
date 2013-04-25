#!/bin/bash

set -e

# DEBUG MOVING AVERAGE STUFF
#<<COMMENT
./build/GPAudioTargetMatchingExperiment\
  TEMP FIELDS\
  --target ./samples/TrumpetEb5.wav\
  --path ./\
  --seed 1\
  --values 622.25\
  AUX PARAMS\
  --savetargetspectrum\
  EXPERIMENT PARAMS\
  --experiment 4\
  --fitnesstype 1\
  --generations 0\
  FITNESS FUNCTION WEIGHTS\
  --ffmagweight 1.0\
  --ffphaseweight 0.2\
  --ffenvweight 0.5\
  TIME DOMAIN PARAMS\
  --savetargetenv\
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
  --alpha 0.8\
  --comparedev\
  --penaltycompexp 0.4\
  --weightframes\
  --frameweightexp 0.33\
  --phasepenalty 2.0\
  --goodmagcomp 0.1\
  --badmagcomp 1.1\
  --basemagcomp 0.9
#COMMENT

# DEBUG MEMORY
#valgrind -v --leak-check=full --show-reachable=yes --log-file="2GenValgrindAll.txt" ./build/GPAudioTargetMatchingExperiment --target ./samples/SinWaveC51024.wav --seed 0 --fft 1024 --exp 4 --threshold 0.1 --fitnesstype 1 --numgenerations 2 --values 523.251 --popsize 5 --mid 4 --md 10 --ctype 0 --rselect 0 --cselect 0 --crosspercent 0.9 --mutatechance 0.0 --verbose > 2GenValgrindAll.txt

# DEBUG MEMORY
<<COMMENT
valgrind -v --leak-check=full --show-reachable=yes --log-file="2GenValgrind.txt"\
  ./build/GPAudioTargetMatchingExperiment\
  --target ./samples/TrumpetEb5.wav\
  --path ./\
  --seed 245316\
  --values 622.25\
  --expinfo\
  --experiment 4\
  --fitnesstype 1\
  --generations 2\
  --threshold 0.1\
  --erc\
  --verbose\
  --nosavegenchamps\
  --printprecision 3\
  --saveprecision 5\
  --loadwavblock 512\
  --render 1024\
  --ffmagweight 1.0\
  --ffphaseweight 0.2\
  --ffenvweight 0.5\
  --nosavetargetenv\
  --envatk 1\
  --envdcy 300\
  --envskip 15\
  --window rect\
  --fft 1024\
  --fftoverlap 0\
  --phasepenalty 2.0\
  --goodmagcomp 0.1\
  --badmagcomp 1.1\
  --basemagcomp 0.9\
  --popsize 16\
  --nobackup\
  --backupprecision 50\
  --bestfitness 0.0\
  --mid 5\
  --md 11\
  --greedy 0.0\
  --numericmutation 0.25 --nmselect 1 --nmselectparam 0.05 --nmtemperature 0.9\
  --mutation 0.25 --mselect 1 --mtype 1 --mselectparam 0.25\
  --crossover 0.25 --cselect 0 --ctype 0\
  --reproduction 0.25 --rselect 0\
  > 2GenValgrind.txt
COMMENT

# DEBUG HEAP
<<COMMENT
valgrind -v --tool=massif --log-file="3GenValgrindMassif.txt"\
  ./build/GPAudioTargetMatchingExperiment\
  --target ./samples/TrumpetEb5.wav\
  --path ./\
  --seed 0\
  --verbose\
  --savegenchamps\
  --expinfo\
  --precision 3\
  --values 622.25\
  --fft 1024\
  --fitnesstype 1\
  --enviter 3\
  --experiment 4\
  --generations 3\
  --threshold 0.1\
  --erc\
  --popsize 10\
  --mid 5\
  --md 11\
  --numericmutation 0.25 --nmselect 1 --nmselectparam 0.05 --nmtemperature 0.9\
  --mutation 0.25 --mtype 1 --mselctparam 0.25\
  --crossover 0.25 --cselect 0 --ctype 0\
  --reproduction 0.25 --rselect 0\
  --render 1024\
  > 3GenValgrindMassif.txt
COMMENT

# LOCAL TRUMPET TEST
<<COMMENT
./build/GPAudioTargetMatchingExperiment\
  TEMP FIELDS\
  --target ./samples/TrumpetEb5.wav\
  --path ./\
  --seed 24531\
  --values 622.25\
  --expinfo\
  EXPERIMENT PARAMS\
  --experiment 4\
  --fitnesstype 1\
  --generations 10\
  --threshold 0.1\
  --erc\
  AUXILLARY PARAMS\
  --verbose\
  --savegenchamps\
  --nosavetargetenv\
  --nosavetargetspectrum\
  --backuptarget\
  --printprecision 3\
  --saveprecision 5\
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
  --popsize 200\
  --nobackup\
  --backupprecision 50\
  --bestfitness 0.0\
  --mid 5\
  --md 11\
  SYNTH GENETIC PARAMS\
  --greedy 0.0\
  --numericmutation 0.05 --nmselect 1 --nmselectparam 0.05 --nmtemperature 0.9\
  --mutation 0.05 --mselect 1 --mtype 1 --mselectparam 0.25\
  --crossover 0.9 --cselect 0 --ctype 0\
  --reproduction 0.05 --rselect 0
COMMENT

# LOCAL BASSOON TEST
<<COMMENT
./build/GPAudioTargetMatchingExperiment\
  --target ./samples/BassoonC2.wav\
  --path ./\
  --seed 245316\
  --verbose\
  --savegenchamps\
  --expinfo\
  --precision 3\
  --values 622.25\
  --fft 1024\
  --fitnesstype 1\
  --enviter 3\
  --experiment 4\
  --generations 100\
  --threshold 0.1\
  --erc\
  --popsize 2000\
  --mid 5\
  --md 11\
  --numericmutation 0.05 --nmselect 1 --nmselectparam 0.05 --nmtemperature 0.9\
  --mutation 0.05 --mtype 1 --mselctparam 0.25\
  --crossover 0.9 --cselect 0 --ctype 0\
  --reproduction 0.05 --rselect 0\
  --render 1024
COMMENT

# TEST NEW ADSR
#./build/GPAudioTargetMatchingExperiment --target ./samples/PianoC5.wav --verbose --precision 3 --values 622.25 --fft 1024 --fitnesstype 1 --enviter 3 --experiment 8 --generations 0 --erc --popsize 0 --mid 5 -- md 11 --numericmutation 0.05 --nmselect 1 --nmselectparam 0.05 --nmtemperature 0.9 --mutation 0.05 --mtype 1 --mselectparam 0.25 --crossover 0.9 --cselect 0 --ctype 0 --reproduction 0.05 --rselect 0 --render 1024 --seed 245316

# TEST ENVELOPE TRACKERS
#./build/GPAudioTargetMatchingExperiment --target ./samples/TrumpetEb5.wav --verbose --precision 3 --values 622.25 --fft 1024 --fitnesstype 1 --enviter 3 --experiment 9 --generations 0 --erc --popsize 0 --mid 5 -- md 11 --numericmutation 0.05 --nmselect 1 --nmselectparam 0.05 --nmtemperature 0.9 --mutation 0.05 --mtype 1 --mselectparam 0.25 --crossover 0.9 --cselect 0 --ctype 0 --reproduction 0.05 --rselect 0 --render 1024 --seed 245316

# TEST FFT
#./build/GPAudioTargetMatchingExperiment --target ./samples/SinWaveC51024.wav --verbose --precision 3 --values 622.25 --fft 1024 --fitnesstype 1 --enviter 3 --experiment 10 --generations 0 --erc --popsize 0 --mid 5 -- md 11 --numericmutation 0.05 --nmselect 1 --nmselectparam 0.05 --nmtemperature 0.9 --mutation 0.05 --mtype 1 --mselectparam 0.25 --crossover 0.9 --cselect 0 --ctype 0 --reproduction 0.05 --rselect 0 --render 1024 --seed 245316

#./build/GPAudioTargetMatchingExperiment --target ./samples/SinWaveC51024.wav --verbose --precision 3 --values 622.25 --fft 1024 --fitnesstype 1 --enviter 1 --experiment 10 --generations 0 --erc --popsize 0 --mid 5 -- md 11 --numericmutation 0.05 --nmselect 1 --nmselectparam 0.05 --nmtemperature 0.9 --mutation 0.05 --mtype 1 --mselectparam 0.25 --crossover 0.9 --cselect 0 --ctype 0 --reproduction 0.05 --rselect 0 --render 1024 --seed 245316

# DEBUG FILTER
#./build/GPAudioTargetMatchingExperiment --target ./samples/TrumpetEb5.wav --verbose --precision 3 --values 622.25 --fft 1024 --fitnesstype 1 --experiment 3 --generations 100 --erc --popsize 2000 --mid 5 -- md 11 --numericmutation 0.05 --nmselect 1 --nmselectparam 0.05 --nmtemperature 0.9 --mutation 0.05 --mtype 1 --mselectparam 0.5 --crossover 0.9 --cselect 0 --ctype 0 --reproduction 0.05 --rselect 0 --render 1024 --seed 0

#valgrind -v --leak-check=full --show-reachable=yes --log-file="3GenValgrind.txt" ./build/GPAudioTargetMatchingExperiment --target ./samples/TrumpetEb5.wav --verbose --precision 3 --values 622.25 --fft 1024 --fitnesstype 1 --experiment 4 --generations 3 --erc --popsize 5 --mid 2 -- md 6 --numericmutation 0.5 --nmselect 1 --nmselectparam 0.05 --nmtemperature 0.9 --crossover 0.9 --cselect 0 --ctype 0 --reproduction 0.05 --rselect 0 > 3GenValgrind.txt

set +e
