#!/bin/bash

set -e

pdflatex thesis.tex
bibtex thesis.aux
pdflatex thesis.tex

set +e
