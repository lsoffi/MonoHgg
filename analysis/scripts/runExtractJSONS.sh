#! /bin/sh
#run extractJSONS.py for all samples in file

# All FLASHgg version Spring15BetaV4:

python extractJSONS.py -i datasets_746_betaV4/datasets_746_betaV4_all.json	-o DiPhoton		-d lists_50ns_betaV4/MC
python extractJSONS.py -i datasets_746_betaV4/datasets_746_betaV4_all.json      -o QCD_Pt-30to40	-d lists_50ns_betaV4/MC
python extractJSONS.py -i datasets_746_betaV4/datasets_746_betaV4_all.json      -o QCD_Pt-40toInf	-d lists_50ns_betaV4/MC
python extractJSONS.py -i datasets_746_betaV4/datasets_746_betaV4_all.json      -o QCD_Pt-30toInf	-d lists_50ns_betaV4/MC
python extractJSONS.py -i datasets_746_betaV4/datasets_746_betaV4_all.json      -o GJet_Pt-20to40	-d lists_50ns_betaV4/MC
python extractJSONS.py -i datasets_746_betaV4/datasets_746_betaV4_all.json      -o GJet_Pt-40toInf	-d lists_50ns_betaV4/MC
python extractJSONS.py -i datasets_746_betaV4/datasets_746_betaV4_all.json      -o GluGluHToGG_M-125	-d lists_50ns_betaV4/MC
python extractJSONS.py -i datasets_746_betaV4/datasets_746_betaV4_all.json      -o VHToGG_M125		-d lists_50ns_betaV4/MC
python extractJSONS.py -i datasets_746_betaV4/datasets_746_betaV4_all.json	-o DoubleEG		-d lists_50ns_betaV4/Data




## with mix of Flashgg versions (spring15 betaV2 and betaV4):

#python extractJSONS.py -i datasets_746_dipho.json -o DiPhoton				-d lists_50ns/MC
#python extractJSONS.py -i datasets_746_WZH.json -o WplusH				-d lists_50ns/MC
#python extractJSONS.py -i datasets_746_WZH.json -o WminusH				-d lists_50ns/MC
#python extractJSONS.py -i datasets_746_WZH.json -o ZH 					-d lists_50ns/MC
#python extractJSONS.py -i datasets_746_sig.json -o Higgs_scalar_nohdecay_gg_1000GeV	-d lists_50ns/MC
#python extractJSONS.py -i datasets_746_sig.json -o Higgs_scalar_nohdecay_gg_100GeV	-d lists_50ns/MC
#python extractJSONS.py -i datasets_746_sig.json -o Higgs_scalar_nohdecay_gg_10GeV	-d lists_50ns/MC
#python extractJSONS.py -i datasets_746_sig.json -o Higgs_scalar_nohdecay_gg_1GeV	-d lists_50ns/MC
#python extractJSONS.py -i datasets.json 	-o GJet_Pt-20to40			-d lists_50ns/MC
#python extractJSONS.py -i datasets.json 	-o GJet_Pt-40toInf 			-d lists_50ns/MC
#python extractJSONS.py -i datasets.json 	-o QCD_Pt-30to40  			-d lists_50ns/MC
#python extractJSONS.py -i datasets.json 	-o QCD_Pt-30toInf 			-d lists_50ns/MC
#python extractJSONS.py -i datasets.json 	-o QCD_Pt-40toInf 			-d lists_50ns/MC
#python extractJSONS.py -i datasets_746.json	-o GluGluHToGG				-d lists_50ns/MC
#python extractJSONS.py -i datasets.json 	-o ttHJetToGG_M125 			-d lists_50ns/MC
##Data
#python extractJSONS.py -i datasets.json 	-o SinglePhoton 			-d lists_50ns/Data
#python extractJSONS.py -i datasets_746dEG.json	-o DoubleEG	 			-d lists_50ns/Data
#python extractJSONS.py -i datasets.json 	-o EGamma	 			-d lists_50ns/Data
