#!/bin/bash

set -e

#python graphAllPenalties.py "Penalties for Undershooting Target Bins" "under" 0.9 2.0 10
python graphAllPenalties.py "Penalties for Overshooting Target Bins" "over" 0 70 0.9 2.0 10

set +e
