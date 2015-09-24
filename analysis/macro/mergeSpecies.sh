#! /bin/sh 
# this scripts creates a merged root file in the self-created mergedFinal

mkdir -p data/mergedFinal

hadd data/50ns_betaV4/GJets.root data/50ns_betaV4/GJet_Pt-20to40.root data/50ns_betaV4/GJet_Pt-40toInf.root 
hadd data/50ns_betaV4/QCD.root   data/50ns_betaV4/QCD_Pt-30to40.root  data/50ns_betaV4/QCD_Pt-30toInf.root   data/50ns_betaV4/QCD_Pt-40toInf.root 


