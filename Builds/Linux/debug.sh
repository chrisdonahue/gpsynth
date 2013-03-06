#!/bin/bash

# WARNING: This is not a makefile! It is provided only to give a sense of the commands
# necessary to build this whole project. 

set -e

#gdb --args ./build/GeneticProgrammingSynthesizer --target ./build/AMExample.wav --exp 0 --popsize 5 --seed 0 --addchance 0.3 --removechance 0 --mutatechance 0.2 --crosschance 0.2 --threshold 10 --numgenerations 2 --selection 9 --cross 0 --values 220.0 1.0 1.0
#gdb --args ./build/GeneticProgrammingSynthesizer --target ./build/AMExample.wav --exp 0 --popsize 5 --addchance 0.0 --removechance 0.0 --mutatechance 1.0 --crosschance 0.0 --threshold 10 --numgenerations 5 --selection 9 --cross 0 --values 220.0 1.0 1.0
#gdb --args ./build/GeneticProgrammingSynthesizer --target ./build/AMExample.wav --exp 0 --thershold 0 --numgenerations 500 --values 220 1 1 --popsize 500 --mid 6 --md 17 --ctype 0 --rselect 0 --cselect 0 --crosspercent 0.9 --addchance 0.0 --removecance 0.0 --mutatechance 0.05
#gdb --args ./build/GeneticProgrammingSynthesizer --target ./build/AMExample.wav --exp 0 --threshold 0 --numgenerations 2 --values 220 1 1 --popsize 50 --mid 6 --md 8 --ctype 0 --rselect 0 --cselect 0 --crosspercent 0.9 --addchance 0.0 --removechance 0.0 --mutatechance 0.05
#gdb --args ./build/GeneticProgrammingSynthesizer --target ./build/AMExample.wav --exp 0 --threshold 0 --numgenerations 2 --values 220 1 1 --popsize 50 --mid 4 --md 8 --ctype 0 --rselect 0 --cselect 0 --crosspercent 0.9 --mutatechance 0.0
#gdb --args ./build/GeneticProgrammingSynthesizer --target ./build/AMExample.wav --exp 0 --threshold 0 --numgenerations 1 --values 220 1 --popsize 5 --mid 4 --md 6 --ctype 0 --rselect 0 --cselect 0 --crosspercent 0.9 --mutatechance 0.0
#gdb --args ./build/GeneticProgrammingSynthesizer --target ./build/AMExample.wav --exp 0 --threshold 0 --fitnesstype 0 --numgenerations 2 --values 220 1 --popsize 50 --mid 4 --md 6 --ctype 0 --rselect 0 --cselect 0 --crosspercent 0.9 --mutatechance 0.0
#gdb --args ./build/GeneticProgrammingSynthesizer --target ./build/AMExample.wav --exp 0 --threshold 0 --fitnesstype 0 --numgenerations 3 --values 220 1 --popsize 50 --mid 4 --md 6 --ctype 0 --rselect 0 --cselect 0 --crosspercent 0.9 --mutatechance 0.0
gdb --args ./build/GeneticProgrammingSynthesizer --target ./build/AMExample.wav --fft 16 --exp 0 --threshold 0 --fitnesstype 1 --numgenerations 10 --values 220 1 --popsize 500 --mid 4 --md 6 --ctype 0 --rselect 0 --cselect 0 --crosspercent 0.9 --mutatechance 0.0 --seed 3

set +e
