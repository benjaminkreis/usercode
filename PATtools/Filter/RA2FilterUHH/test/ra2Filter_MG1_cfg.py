import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("Test")

process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
  'rfio:/castor/cern.ch/user/w/wteo/susy/LM3_SPRING10_SUSYPAT_v000703_21_1.root',
    )
)

#process.MessageLogger = cms.Service("MessageLogger")

process.myFilter = cms.EDFilter("RA2FilterUHH",
                                #photonSrc   = cms.untracked.InputTag("cleanLayer1Photons"),
                                #electronSrc = cms.untracked.InputTag("cleanLayer1Electrons"),
                                electronSrc = cms.untracked.InputTag("cleanPatElectrons"),
                                #electronSrc = cms.untracked.InputTag("selectedLayer1Electrons"),
                                #muonSrc     = cms.untracked.InputTag("selectedLayer1Muons"),
                                #muonSrc     = cms.untracked.InputTag("cleanLayer1Muons"),
                                muonSrc     = cms.untracked.InputTag("cleanPatMuons"),
                                #tauSrc      = cms.untracked.InputTag("cleanLayer1Taus"),
                                #jetSrc      = cms.untracked.InputTag("selectedLayer1Jets"),
                                #jetSrc      = cms.untracked.InputTag("cleanLayer1Jets"),
                                jetSrc      = cms.untracked.InputTag("cleanPatJetsAK5Calo"),
                                #metSrc      = cms.untracked.InputTag("layer1METs"),
                                metSrc      = cms.untracked.InputTag("patMETsAK5Calo"),
                                triggerResults = cms.untracked.InputTag("TriggerResults", "", "HLT"),
                                pvSrc = cms.InputTag("offlinePrimaryVertices"),
                                bsSrc= cms.InputTag("offlineBeamSpot"),
                                doABCD=cms.untracked.double(1.0),
                                pfjetIdLoose = cms.PSet( #for pf jets
                                    version = cms.string('FIRSTDATA'), #these are the 384 defaults
                                    quality = cms.string('LOOSE')
                                )
                                
)

process.p = cms.Path(process.myFilter)

#Output events that pass filters
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('RA2UHHSelection_QCD_MG1.root'),
                               SelectEvents = cms.untracked.PSet (SelectEvents = cms.vstring('p'))
)

process.outpath = cms.EndPath(process.out)

#process.MessageLogger.cerr.FwkReport.reportEvery = 1000

