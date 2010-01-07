import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("Test")

process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/k/kreis/LM3PAT/lm3_7tev_1.root',
    'rfio:/castor/cern.ch/user/k/kreis/LM3PAT/lm3_7tev_2.root',
    'rfio:/castor/cern.ch/user/k/kreis/LM3PAT/lm3_7tev_3.root',
    'rfio:/castor/cern.ch/user/k/kreis/LM3PAT/lm3_7tev_4.root',
    'rfio:/castor/cern.ch/user/k/kreis/LM3PAT/lm3_7tev_5.root',
    'rfio:/castor/cern.ch/user/k/kreis/LM3PAT/lm3_7tev_6.root',
    'rfio:/castor/cern.ch/user/k/kreis/LM3PAT/lm3_7tev_7.root',
    'rfio:/castor/cern.ch/user/k/kreis/LM3PAT/lm3_7tev_8.root',
    'rfio:/castor/cern.ch/user/k/kreis/LM3PAT/lm3_7tev_9.root',
    'rfio:/castor/cern.ch/user/k/kreis/LM3PAT/lm3_7tev_10.root',
    'rfio:/castor/cern.ch/user/k/kreis/LM3PAT/lm3_7tev_11.root',
    'rfio:/castor/cern.ch/user/k/kreis/LM3PAT/lm3_7tev_12.root',
    'rfio:/castor/cern.ch/user/k/kreis/LM3PAT/lm3_7tev_13.root',
    'rfio:/castor/cern.ch/user/k/kreis/LM3PAT/lm3_7tev_14.root',
    'rfio:/castor/cern.ch/user/k/kreis/LM3PAT/lm3_7tev_15.root',
    'rfio:/castor/cern.ch/user/k/kreis/LM3PAT/lm3_7tev_16.root',
    'rfio:/castor/cern.ch/user/k/kreis/LM3PAT/lm3_7tev_17.root',
    )
)

#process.MessageLogger = cms.Service("MessageLogger")

process.myFilter = cms.EDFilter("PreliminaryFilter",
  #photonSrc   = cms.untracked.InputTag("cleanLayer1Photons"),
  #electronSrc = cms.untracked.InputTag("cleanLayer1Electrons"),
  electronSrc = cms.untracked.InputTag("selectedLayer1Electrons"),
  muonSrc     = cms.untracked.InputTag("selectedLayer1Muons"),                                             
  #muonSrc     = cms.untracked.InputTag("cleanLayer1Muons"),                                             
  #tauSrc      = cms.untracked.InputTag("cleanLayer1Taus"),
  jetSrc      = cms.untracked.InputTag("selectedLayer1Jets"),
  #jetSrc      = cms.untracked.InputTag("cleanLayer1Jets"),
  metSrc      = cms.untracked.InputTag("layer1METs")
)

process.p = cms.Path(process.myFilter)

#Output events that pass filters
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('lm3_filtered.root'),
                               SelectEvents = cms.untracked.PSet (SelectEvents = cms.vstring('p'))
)

process.outpath = cms.EndPath(process.out)

#process.MessageLogger.cerr.FwkReport.reportEvery = 1000

