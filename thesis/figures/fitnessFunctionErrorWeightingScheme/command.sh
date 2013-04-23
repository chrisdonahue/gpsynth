#!/bin/bash

# WARNING: This is not a makefile! It is provided only to give a sense of the commands
# necessary to build this whole project. 

set -e

python graphSpectrumComparison.py "Error Comparison Exponent For Undershooting" 0.9 2.0 $1 $2 1
python graphSpectrumComparison.py "Error Comparison Exponent For Overshooting" 0.9 2.0 $1 $3 1

set +e
