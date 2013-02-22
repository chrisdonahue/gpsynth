#!/bin/bash

# WARNING: This is not a makefile! It is provided only to give a sense of the commands
# necessary to build this whole project. 

set -e

gdb --args ./build/GeneticProgrammingSynthesizer --target ./build/AMExample.wav --exp 0 --popsize 5 --seed 0 --addchance 0.3 --removechance 0 --mutatechance 0.2 --crosschance 0.2 --threshold 10 --numgenerations 2 --selection 9 --cross 0 --values 220.0 1.0 1.0

set +e
