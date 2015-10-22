#!/bin/bash

for mass in 1 #10 100 1000
  do
    echo $mass 
    combine datacard/datacard_13TeV_monoHgg_EFTscalar_DMmass_${mass}.txt -M Asymptotic -m ${mass} --run=blind    
  done 


