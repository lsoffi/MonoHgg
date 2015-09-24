#! /bin/sh
# run this script to copy plots in diPhoPlots dir to ~/www/
# and add indexing file

cp -r diPhoPlots/ ~/www/Plots
echo ~/www/Plots/diPhoPlots/*/* | xargs -n 1 cp /afs/cern.ch/user/m/mzientek/public/index.php 



