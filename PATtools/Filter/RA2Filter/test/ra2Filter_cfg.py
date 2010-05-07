import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("Test")

process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_1.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_2.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_3.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_4.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_5.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_6.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_7.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_8.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_9.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_10.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_11.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_12.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_13.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_14.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_15.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_16.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_17.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_18.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_19.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_20.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_21.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_22.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_23.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_24.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_25.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_26.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_27.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_28.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_29.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_30.root',
    'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/TTbar-7TeV/TTbar-7TeV_31.root',
    )
)

#process.MessageLogger = cms.Service("MessageLogger")

process.myFilter = cms.EDFilter("RA2Filter",
  #photonSrc   = cms.untracked.InputTag("cleanLayer1Photons"),
  electronSrc = cms.untracked.InputTag("cleanLayer1Electrons"),
  #electronSrc = cms.untracked.InputTag("selectedLayer1Electrons"),
  #muonSrc     = cms.untracked.InputTag("selectedLayer1Muons"),                                             
  muonSrc     = cms.untracked.InputTag("cleanLayer1Muons"),                                             
  #tauSrc      = cms.untracked.InputTag("cleanLayer1Taus"),
  #jetSrc      = cms.untracked.InputTag("selectedLayer1Jets"),
  jetSrc      = cms.untracked.InputTag("cleanLayer1Jets"),
  metSrc      = cms.untracked.InputTag("layer1METs"),
  triggerResults = cms.untracked.InputTag("TriggerResults", "", "HLT"),
  pvSrc = cms.InputTag("offlinePrimaryVertices"),
  bsSrc= cms.InputTag("offlineBeamSpot")                                
)

process.p = cms.Path(process.myFilter)

#Output events that pass filters
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('TTbar_RA2selection.root'),
                               SelectEvents = cms.untracked.PSet (SelectEvents = cms.vstring('p'))
)

process.outpath = cms.EndPath(process.out)

#process.MessageLogger.cerr.FwkReport.reportEvery = 1000

