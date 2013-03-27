#!/bin/bash

set -e

# test
#./build/GeneticProgrammingSynthesizer --target ./samples/AMAnswer.wav --fft 1024 --exp 1 --threshold 0.01 --fitnesstype 1 --numgenerations 100 --values 440 1 --popsize 500 --mid 4 --md 5 --ctype 0 --rselect 0 --cselect 0 --crosspercent 0.9 --mutatechance 0.0 --verbose
# piano
#./build/GeneticProgrammingSynthesizer --target ./samples/PianoC5.wav --fft 256 --exp 3 --threshold 0.1 --fitnesstype 1 --numgenerations 2 --values 523.251 --popsize 50 --mid 4 --md 12 --ctype 0 --rselect 0 --cselect 0 --crosspercent 0.9 --mutatechance 0.0 --verbose
#trumpet
#./build/GeneticProgrammingSynthesizer2 --target ./samples/TrumpetEb5.wav --fft 1024 --exp 4 --threshold 0.1 --fitnesstype 1 --numgenerations 200 --values 622.25 --popsize 1000 --mid 4 --md 11 --ctype 0 --rselect 0 --cselect 0 --crosspercent 0.9 --mutatechance 0.0 --verbose
# valgrind
#./build/GeneticProgrammingSynthesizer --target ./samples/PianoC51024.wav --seed 0 --fft 1024 --exp 3 --threshold 0.1 --fitnesstype 1 --numgenerations 2 --values 523.251 --popsize 5 --mid 4 --md 12 --ctype 0 --rselect 0 --cselect 0 --crosspercent 0.9 --mutatechance 0.0 --verbose
# fourier transform analysis
#./build/GeneticProgrammingSynthesizer --target ./samples/PianoC5.wav --seed 0 --fft 256 --exp 9 --threshold 0.1 --fitnesstype 1 --numgenerations 10 --values 523.251 --popsize 500 --mid 4 --md 12 --ctype 0 --rselect 0 --cselect 0 --crosspercent 0.9 --mutatechance 0.0 --verbose

#./build/GeneticProgrammingSynthesizer --target ./samples/SinWaveC51024.wav --seed 0 --fft 1024 --exp 0 --threshold 0.1 --fitnesstype 1 --numgenerations 2 --values 523.251 --popsize 5 --mid 4 --md 10 --ctype 0 --rselect 0 --cselect 0 --crosspercent 0.9 --mutatechance 0.0 --verbose


#./build/GeneticProgrammingSynthesizer --target ./samples/SinWaveC51024.wav --seed 0 --fft 1024 --exp 4 --threshold 0.1 --fitnesstype 1 --numgenerations 2 --values 523.251 --popsize 5 --mid 4 --md 10 --ctype 0 --rselect 0 --cselect 0 --crosspercent 0.9 --mutatechance 0.0 --verbose

#valgrind -v --leak-check=full --show-reachable=yes --log-file="2GenValgrindAll.txt" ./build/GeneticProgrammingSynthesizer --target ./samples/SinWaveC51024.wav --seed 0 --fft 1024 --exp 4 --threshold 0.1 --fitnesstype 1 --numgenerations 2 --values 523.251 --popsize 5 --mid 4 --md 10 --ctype 0 --rselect 0 --cselect 0 --crosspercent 0.9 --mutatechance 0.0 --verbose > 2GenValgrindAll.txt

./build/GeneticProgrammingSynthesizer --target ./samples/TrumpetEb5.wav --verbose --values 622.25 --fft 1024 --fitnesstype 1 --experiment 4 --generations 4 --erc --popsize 20 --mid 1 -- md 3 --numericmutation 0.5 --nmselect 1 --nmselectparam 0.05 --nmtemperature 0.9 --crossover 0.9 --cselect 0 --ctype 0 --reproduction 0.05 --rselect 0

set +e
