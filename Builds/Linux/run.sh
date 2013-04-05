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

#./build/GeneticProgrammingSynthesizer --target ./samples/TrumpetEb5.wav --verbose --precision 3 --values 622.25 --fft 1024 --fitnesstype 1 --experiment 4 --generations 5 --erc --popsize 1000 --mid 1 -- md 3 --numericmutation 0.5 --nmselect 1 --nmselectparam 0.05 --nmtemperature 0.9 --crossover 0.9 --cselect 0 --ctype 0 --reproduction 0.05 --rselect 0

#./build/GeneticProgrammingSynthesizer --target ./samples/TrumpetEb5.wav --verbose --precision 3 --values 622.25 --fft 1024 --fitnesstype 1 --experiment 4 --generations 200 --erc --popsize 20 --mid 5 -- md 11 --numericmutation 0.5 --nmselect 1 --nmselectparam 0.05 --nmtemperature 0.9 --mutation 0.0 --mtype 1 --mselectparam 0.05 --crossover 0.9 --cselect 0 --ctype 0 --reproduction 0.05 --rselect 0 --render 1024

#./build/GeneticProgrammingSynthesizer --target ./samples/TrumpetEb5.wav --verbose --precision 3 --values 622.25 --fft 1024 --fitnesstype 1 --experiment 4 --generations 200 --erc --popsize 100 --mid 5 -- md 11 --numericmutation 0.5 --nmselect 1 --nmselectparam 0.05 --nmtemperature 0.9 --mutation 0.0 --mtype 1 --mselectparam 0.05 --crossover 0.9 --cselect 0 --ctype 0 --reproduction 0.05 --rselect 0 --render 1024 --seed 0

# REAL ONE
./build/GeneticProgrammingSynthesizer --target ./samples/TrumpetEb5.wav --verbose --precision 3 --values 622.25 --fft 1024 --fitnesstype 1 --enviter 3 --experiment 4 --generations 100 --erc --popsize 2000 --mid 5 -- md 11 --numericmutation 0.05 --nmselect 1 --nmselectparam 0.05 --nmtemperature 0.9 --mutation 0.05 --mtype 1 --mselectparam 0.25 --crossover 0.9 --cselect 0 --ctype 0 --reproduction 0.05 --rselect 0 --render 1024 --seed 0



# DEBUG FILTER
#./build/GeneticProgrammingSynthesizer --target ./samples/TrumpetEb5.wav --verbose --precision 3 --values 622.25 --fft 1024 --fitnesstype 1 --experiment 3 --generations 100 --erc --popsize 2000 --mid 5 -- md 11 --numericmutation 0.05 --nmselect 1 --nmselectparam 0.05 --nmtemperature 0.9 --mutation 0.05 --mtype 1 --mselectparam 0.5 --crossover 0.9 --cselect 0 --ctype 0 --reproduction 0.05 --rselect 0 --render 1024 --seed 0



#valgrind -v --leak-check=full --show-reachable=yes --log-file="3GenValgrind.txt" ./build/GeneticProgrammingSynthesizer --target ./samples/TrumpetEb5.wav --verbose --precision 3 --values 622.25 --fft 1024 --fitnesstype 1 --experiment 4 --generations 3 --erc --popsize 5 --mid 2 -- md 6 --numericmutation 0.5 --nmselect 1 --nmselectparam 0.05 --nmtemperature 0.9 --crossover 0.9 --cselect 0 --ctype 0 --reproduction 0.05 --rselect 0 > 3GenValgrind.txt

set +e
