#!/bin/bash

set -e

python graph2Axis.py "Moving Average of FFT on 1024 samples of Eb5 Trumpet Note" "Trumpet Spectrum.txt" Moving*

set +e
