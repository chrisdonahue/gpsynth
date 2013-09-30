#!/bin/bash

set -e

pdflatex new_thesis.tex
bibtex thesis.aux
pdflatex new_thesis.tex

set +e
