#
# usage: %prog [opts] --cfg cmssw.py dataset doPUreweighting(0/1) sampleIndex PUweightsFile x-section(in pb) kFactor
#
# Backgrounds: sampleID>0 && sampleID<100
# Signals:     sampleID>100
# Data:        sampleID=0


# 50ns samples

./submitBatchDiPho.py --cfg diPhoAnaBATCH.py DYJetsToLL                         1 17  /afs/cern.ch/work/s/soffi/CMSSW_7_4_6_patch2/src/MonoHgg/analysis/python/purwt.root 6025.2 1 
./submitBatchDiPho.py --cfg diPhoAnaBATCH.py DiPhoton				1 15  /afs/cern.ch/work/s/soffi/CMSSW_7_4_6_patch2/src/MonoHgg/analysis/python/purwt.root 84.0 1

./submitBatchDiPho.py --cfg diPhoAnaBATCH.py Higgs_scalar_nohdecay_gg_1000GeV	1 100 /afs/cern.ch/work/s/soffi/CMSSW_7_4_6_patch2/src/MonoHgg/analysis/python/purwt.root 0.01 1 #10fb xsec
./submitBatchDiPho.py --cfg diPhoAnaBATCH.py Higgs_scalar_nohdecay_gg_100GeV	1 101 /afs/cern.ch/work/s/soffi/CMSSW_7_4_6_patch2/src/MonoHgg/analysis/python/purwt.root 0.01 1
./submitBatchDiPho.py --cfg diPhoAnaBATCH.py Higgs_scalar_nohdecay_gg_10GeV	1 102 /afs/cern.ch/work/s/soffi/CMSSW_7_4_6_patch2/src/MonoHgg/analysis/python/purwt.root 0.01 1
./submitBatchDiPho.py --cfg diPhoAnaBATCH.py Higgs_scalar_nohdecay_gg_1GeV	1 103 /afs/cern.ch/work/s/soffi/CMSSW_7_4_6_patch2/src/MonoHgg/analysis/python/purwt.root 0.01 1

./submitBatchDiPho.py --cfg diPhoAnaBATCH.py GJet_Pt-20to40     		1  1  /afs/cern.ch/work/s/soffi/CMSSW_7_4_6_patch2/src/MonoHgg/analysis/python/purwt.root  218.6108 1  
./submitBatchDiPho.py --cfg diPhoAnaBATCH.py GJet_Pt-40toInf    		1  2  /afs/cern.ch/work/s/soffi/CMSSW_7_4_6_patch2/src/MonoHgg/analysis/python/purwt.root  863.1088 1

./submitBatchDiPho.py --cfg diPhoAnaBATCH.py QCD_Pt-30to40      		1  3  /afs/cern.ch/work/s/soffi/CMSSW_7_4_6_patch2/src/MonoHgg/analysis/python/purwt.root  24300   1
./submitBatchDiPho.py --cfg diPhoAnaBATCH.py QCD_Pt-30toInf     		1  4  /afs/cern.ch/work/s/soffi/CMSSW_7_4_6_patch2/src/MonoHgg/analysis/python/purwt.root  259296  1
./submitBatchDiPho.py --cfg diPhoAnaBATCH.py QCD_Pt-40toInf     		1  5  /afs/cern.ch/work/s/soffi/CMSSW_7_4_6_patch2/src/MonoHgg/analysis/python/purwt.root  108240  1

./submitBatchDiPho.py --cfg diPhoAnaBATCH.py GluGluHToGG	     		1  10 /afs/cern.ch/work/s/soffi/CMSSW_7_4_6_patch2/src/MonoHgg/analysis/python/purwt.root  43.92   1

./submitBatchDiPho.py --cfg diPhoAnaBATCH.py VH					1  11 /afs/cern.ch/work/s/soffi/CMSSW_7_4_6_patch2/src/MonoHgg/analysis/python/purwt.root  2.2496 1 #xsec=ZH+WH 


# 50ns Data
./submitBatchDiPho.py --cfg diPhoAnaBATCH.py DoubleEG           		1 0 /afs/cern.ch/work/s/soffi/CMSSW_7_4_6_patch2/src/MonoHgg/analysis/python/purwt.root  1 1
