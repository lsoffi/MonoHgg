#!/bin/bash
model=$1
echo "python scripts/limitPlotter_livia.py -M Asymptotic  -v -p ../${model}"
python scripts/limitPlotter_livia.py -M Asymptotic -v -p ${model}  -e -r


