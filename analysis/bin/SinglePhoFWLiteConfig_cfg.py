import FWCore.ParameterSet.Config as cms

process = cms.PSet()

process.fwliteInput = cms.PSet(
    fileNames   = cms.vstring(

        ## 5TeV
        'root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/flashgg/ExoPhys14/alphaV1-96-g5e4dc54/RSGravToGG_kMpl-02_M-5000_Tune4C_13TeV-pythia8/ExoPhys14-alphaV1-96-g5e4dc54-v1-Phys14DR-PU20bx25_PHYS14_25_V1-v1/141112_020912/0000/myOutputFile_1.root',
        'root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/flashgg/ExoPhys14/alphaV1-96-g5e4dc54/RSGravToGG_kMpl-02_M-5000_Tune4C_13TeV-pythia8/ExoPhys14-alphaV1-96-g5e4dc54-v1-Phys14DR-PU20bx25_PHYS14_25_V1-v1/141112_020912/0000/myOutputFile_2.root',
        'root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/flashgg/ExoPhys14/alphaV1-96-g5e4dc54/RSGravToGG_kMpl-02_M-5000_Tune4C_13TeV-pythia8/ExoPhys14-alphaV1-96-g5e4dc54-v1-Phys14DR-PU20bx25_PHYS14_25_V1-v1/141112_020912/0000/myOutputFile_3.root'
        
        # 1.5 TeV
        #'root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/flashgg/ExoPhys14/alphaV1-96-g5e4dc54/RSGravToGG_kMpl-001_M-1500_Tune4C_13TeV-pythia8/ExoPhys14-alphaV1-96-g5e4dc54-v1-Phys14DR-PU20bx25_PHYS14_25_V1-v1/141112_020908/0000/myOutputFile_2.root',
        #'root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/flashgg/ExoPhys14/alphaV1-96-g5e4dc54/RSGravToGG_kMpl-001_M-1500_Tune4C_13TeV-pythia8/ExoPhys14-alphaV1-96-g5e4dc54-v1-Phys14DR-PU20bx25_PHYS14_25_V1-v1/141112_020908/0000/myOutputFile_3.root'
        ),
    
    maxEvents   = cms.int32(100),                
    )

process.fwliteOutput = cms.PSet(
    fileName  = cms.string('analyzeFWLsinglePho.root'),  
    )

process.SinglePhoFWLite = cms.PSet(
    ## input specific for this analyzer
)
