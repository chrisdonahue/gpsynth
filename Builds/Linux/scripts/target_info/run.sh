#!/bin/bash

set -e

python make_spectrogram.py "Spectrogram" 20
python graph_all_penalties.py "Penalties for Undershooting Target Bins" "under" 0 70 0.9 2.0 5
python graph_all_penalties.py "Penalties for Overshooting Target Bins" "over" 0 70 0.9 2.0 5

set +e
