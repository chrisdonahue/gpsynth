#!/bin/bash

# WARNING: This is not a makefile! It is provided only to give a sense of the commands
# necessary to build this whole project. 

set -e

export PATH=/lusr/opt/condor/bin:$PATH
condor_submit results/4.12.13.1.trumpet.lowmem/condor.desc

set +e
