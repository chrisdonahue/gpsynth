#!/bin/bash

set -e

~/mount.sh
rm ~/windows/*.wav
cp *.wav ~/windows

set +e
