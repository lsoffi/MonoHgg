import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("diPhoAna")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
#process.GlobalTag.globaltag = 'POSTLS170_V5::All'     # Phys14 samples
process.GlobalTag.globaltag = 'MCRUN2_74_V9A'         # 50ns

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( 10000 ) )

process.source = cms.Source("PoolSource",
                            fileNames=cms.untracked.vstring(

	# Spring15 DATA
#"/store/group/phys_higgs/cmshgg/mdonega/flashgg/RunIISpring15-50ns/Spring15BetaV2/DoubleEG/RunIISpring15-50ns-Spring15BetaV2-v0-Run2015B-PromptReco-v1/150716_154839/0000/myMicroAODOutputFile_1.root ",
#"/store/group/phys_higgs/cmshgg/mdonega/flashgg/RunIISpring15-50ns/Spring15BetaV2/DoubleEG/RunIISpring15-50ns-Spring15BetaV2-v0-Run2015B-PromptReco-v1/150716_154839/0000/myMicroAODOutputFile_10.root ",
#"/store/group/phys_higgs/cmshgg/mdonega/flashgg/RunIISpring15-50ns/Spring15BetaV2/DoubleEG/RunIISpring15-50ns-Spring15BetaV2-v0-Run2015B-PromptReco-v1/150716_154839/0000/myMicroAODOutputFile_100.root "


#Spring15 MC
#"/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISpring15-50ns/Spring15BetaV2/GluGluHToGG_M-125_13TeV_powheg_pythia8/RunIISpring15-50ns-Spring15BetaV2-v0-RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/150722_193245/0000/myMicroAODOutputFile_1.root", 
#"/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISpring15-50ns/Spring15BetaV2/GluGluHToGG_M-125_13TeV_powheg_pythia8/RunIISpring15-50ns-Spring15BetaV2-v0-RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/150722_193245/0000/myMicroAODOutputFile_10.root", 
#"/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISpring15-50ns/Spring15BetaV2/GluGluHToGG_M-125_13TeV_powheg_pythia8/RunIISpring15-50ns-Spring15BetaV2-v0-RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/150722_193245/0000/myMicroAODOutputFile_2.root"

	"/store/user/mzientek/RunIISpring15-50ns/Higgs_scalar/Higgs_scalar_nohdecay_gg_1000GeV_13TeV_RunIISpring15-50ns-Spring15BetaV1_MetaV3-v0-RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/150818_162723/0000/myMicroAODOutputFile_1.root"
	#"/store/group/phys_higgs/cmshgg/mdonega/flashgg/RunIISpring15-50ns/Spring15BetaV2/GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8/RunIISpring15-50ns-Spring15BetaV2-v0-RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/150716_155016/0000/myMicroAODOutputFile_1.root" 
	#"/store/group/phys_higgs/soffi/flashgg/testMonoHLivia2/Phys14MicroAODV3-55-gc1f8d91/Higgs_scalar/testMonoHLivia2-Phys14MicroAODV3-55-gc1f8d91-v0-soffi-Higgs_scalar_nohdecay_gg_1000GeV_13TeV_MINIAODSIM_v11-7d492cb64f2cdaff326f939f96e45c96/150724_112944/0000/myMicroAODOutputFile_1.root"
	#"/store/group/phys_higgs/cmshgg/musella/flashgg/ExoPhys14ANv1/diphotonsPhys14AnV1/GJets_HT-100to200_Tune4C_13TeV-madgraph-tauola/ExoPhys14ANv1-diphotonsPhys14AnV1-v0-Phys14DR-PU20bx25_PHYS14_25_V1-v1/150330_141300/0000/diphotonsMicroAOD_1.root",
        )
                            )

process.load("flashgg/MicroAOD/flashggPhotons_cfi")
process.load("flashgg/MicroAOD/flashggDiPhotons_cfi")

process.TFileService = cms.Service("TFileService",fileName = cms.string("diPhotons.root"))

process.diPhoAna = cms.EDAnalyzer('DiPhoAnalyzer',
                                  VertexTag = cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
				  METTag=cms.untracked.InputTag('slimmedMETs'),
                                  genPhotonExtraTag = cms.InputTag("flashggGenPhotonsExtra"),    
                                  DiPhotonTag = cms.untracked.InputTag('flashggDiPhotons'),
                                  #reducedBarrelRecHitCollection = cms.InputTag('reducedEgamma','reducedEBRecHits'),
                                  #reducedEndcapRecHitCollection = cms.InputTag('reducedEgamma','reducedEERecHits'),
                                  PileupTag = cms.untracked.InputTag('addPileupInfo'),
                                  generatorInfo = cms.InputTag("generator"),
                                  dopureweight = cms.untracked.int32(1),
                                  bits         = cms.InputTag('TriggerResults::HLT'),
                                  #sampleIndex  = cms.untracked.int32(101),   
				  sampleIndex  = cms.untracked.int32(1),
                                  puWFileName  = cms.string('purwt.root'),   # chiara  
                                  xsec         = cms.untracked.double(1), #pb
                                  kfac         = cms.untracked.double(1.),
                                  sumDataset   = cms.untracked.double(100.0)   # chiara
                                  )

process.p = cms.Path(process.diPhoAna)
