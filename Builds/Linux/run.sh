#!/bin/bash

# WARNING: This is not a makefile! It is provided only to give a sense of the commands
# necessary to build this whole project. 

set -e

#./build/GeneticProgrammingSynthesizer --target ./build/AMExample.wav --exp 0 --popsize 5 --seed 0 --addchance 0.0 --removechance 0.0 --mutatechance 1.0 --crosschance 0.0 --threshold 10 --numgenerations 2 --selection 9 --cross 0 --values 220.0 1.0 1.0
#./build/GeneticProgrammingSynthesizer --target ./build/AMExample.wav --exp 0 --popsize 5 --addchance 0.0 --removechance 0.0 --mutatechance 1.0 --crosschance 0.0 --threshold 10 --numgenerations 5 --selection 9 --cross 0 --values 220.0 1.0 1.0
./build/GeneticProgrammingSynthesizer --target ./build/AMExample.wav --exp 0 --threshold 0 --fitnesstype 0 --numgenerations 100 --values 220 1 --popsize 500 --mid 4 --md 6 --ctype 0 --rselect 0 --cselect 0 --crosspercent 0.9 --mutatechance 0.0

set +e
