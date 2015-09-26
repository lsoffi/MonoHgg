import sys
import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("diPhoAna")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")

process.GlobalTag.globaltag = 'MCRUN2_74_V9A'
#process.GlobalTag.globaltag = 'POSTLS170_V5::All'

process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )#( input = cms.untracked.int32( -1 ) )

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring('/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISpring15-50ns/Spring15BetaV4/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15-50ns-Spring15BetaV4-v0-RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/150813_120650/0000/myMicroAODOutputFile_180.root')
#('/store/group/phys_higgs/cmshgg/musella/flashgg/ExoPhys14_v2/diphotonsPhys14V2/RSGravToGG_kMpl001_M_5000_Tune4C_13TeV_pythia8/ExoPhys14_v2-diphotonsPhys14V2-v0-Phys14DR-PU20bx25_PHYS14_25_V1-v1/150128_133931/0000/myOutputFile_1.root'
                                                              )

process.load("flashgg/MicroAOD/flashggPhotons_cfi")
process.load("flashgg/MicroAOD/flashggDiPhotons_cfi")

process.TFileService = cms.Service("TFileService",fileName = cms.string("DYJetsToLL_346.root"))

process.diPhoAna = cms.EDAnalyzer('DiPhoAnalyzer',
                                  VertexTag = cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
				  METTag=cms.untracked.InputTag('slimmedMETs'),
                                  genPhotonExtraTag = cms.InputTag("flashggGenPhotonsExtra"),
                                  reducedBarrelRecHitCollection = cms.InputTag('reducedEgamma','reducedEBRecHits'),
                                  reducedEndcapRecHitCollection = cms.InputTag('reducedEgamma','reducedEERecHits'),
                                  DiPhotonTag = cms.untracked.InputTag('flashggDiPhotons'),
                                  PileupTag = cms.untracked.InputTag('addPileupInfo'),
                                  generatorInfo = cms.InputTag("generator"),
				  bits	        = cms.InputTag('TriggerResults::HLT'),
                                  dopureweight = cms.untracked.int32(1),
                                  sampleIndex  = cms.untracked.int32(17),
                                  puWFileName  = cms.string("/afs/cern.ch/work/s/soffi/CMSSW_7_4_6_patch2/src/MonoHgg/analysis/python/purwt.root"),
                                  xsec = cms.untracked.double(6025.2),
                                  kfac = cms.untracked.double(1),
                                  sumDataset = cms.untracked.double(780970432.0)
                                  )

process.p = cms.Path(process.diPhoAna)

