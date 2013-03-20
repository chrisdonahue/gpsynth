#!/bin/bash

set -e

# test
#./build/GeneticProgrammingSynthesizer --target ./samples/AMAnswer.wav --fft 1024 --exp 1 --threshold 0.01 --fitnesstype 1 --numgenerations 100 --values 440 1 --popsize 500 --mid 4 --md 5 --ctype 0 --rselect 0 --cselect 0 --crosspercent 0.9 --mutatechance 0.0 --verbose
# piano
#./build/GeneticProgrammingSynthesizer --target ./samples/PianoC5.wav --fft 256 --exp 3 --threshold 0.1 --fitnesstype 1 --numgenerations 2 --values 523.251 --popsize 50 --mid 4 --md 12 --ctype 0 --rselect 0 --cselect 0 --crosspercent 0.9 --mutatechance 0.0 --verbose
# trumpet
#./build/GeneticProgrammingSynthesizer --target ./samples/TrumpetEb5.wav seed=10 --fft 1024 --exp 4 --threshold 0.1 --fitnesstype 1 --numgenerations 100 --values 622.25 --popsize 500 --mid 4 --md 10 --ctype 0 --rselect 0 --cselect 0 --crosspercent 0.9 --mutatechance 0.0 --verbose
# valgrind
valgrind -v --leak-check=full --show-reachable=yes --log-file="2GenValgrind.txt" ./build/GeneticProgrammingSynthesizer --target ./samples/PianoC5.wav --seed 0 --fft 256 --exp 3 --threshold 0.1 --fitnesstype 1 --numgenerations 2 --values 523.251 --popsize 5 --mid 4 --md 12 --ctype 0 --rselect 0 --cselect 0 --crosspercent 0.9 --mutatechance 0.0 --verbose > 2GenValgrind.txt
# fourier transform analysis
#./build/GeneticProgrammingSynthesizer --target ./samples/PianoC5.wav --seed 0 --fft 256 --exp 9 --threshold 0.1 --fitnesstype 1 --numgenerations 10 --values 523.251 --popsize 500 --mid 4 --md 12 --ctype 0 --rselect 0 --cselect 0 --crosspercent 0.9 --mutatechance 0.0 --verbose

#./build/GeneticProgrammingSynthesizer --target ./samples/PianoC5.wav --seed 0 --fft 256 --exp 3 --threshold 0.1 --fitnesstype 1 --numgenerations 2 --values 523.251 --popsize 5 --mid 4 --md 12 --ctype 0 --rselect 0 --cselect 0 --crosspercent 0.9 --mutatechance 0.0 --verbose

set +e
