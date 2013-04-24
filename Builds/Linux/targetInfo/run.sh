#!/bin/bash

set -e

python makeSpectrogram.py "Spectrogram" 20
python graphAllPenalties.py "Penalties for Undershooting Target Bins" "under" 0 70 0.9 2.0 5
python graphAllPenalties.py "Penalties for Overshooting Target Bins" "over" 0 70 0.9 2.0 5

set +e
