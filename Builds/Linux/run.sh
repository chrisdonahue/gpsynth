#!/bin/bash

set -e

# DEBUG MEMORY
#valgrind -v --leak-check=full --show-reachable=yes --log-file="2GenValgrindAll.txt" ./build/GPAudioTargetMatchingExperiment --target ./samples/SinWaveC51024.wav --seed 0 --fft 1024 --exp 4 --threshold 0.1 --fitnesstype 1 --numgenerations 2 --values 523.251 --popsize 5 --mid 4 --md 10 --ctype 0 --rselect 0 --cselect 0 --crosspercent 0.9 --mutatechance 0.0 --verbose > 2GenValgrindAll.txt

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
#<<COMMENT
./build/GPAudioTargetMatchingExperiment\
  --target ./samples/TrumpetEb5.wav\
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
#COMMENT

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
