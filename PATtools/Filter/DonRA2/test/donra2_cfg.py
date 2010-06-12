import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("Test")

process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10000)
)


process.source = cms.Source("PoolSource",
 fileNames = cms.untracked.vstring(
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/LM9-7TeV/LM9-7TeV_1.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/LM9-7TeV/LM9-7TeV_2.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/LM9-7TeV/LM9-7TeV_3.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/LM9-7TeV/LM9-7TeV_4.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/LM9-7TeV/LM9-7TeV_5.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/LM9-7TeV/LM9-7TeV_6.root',
                    )
)                            

#process.MessageLogger = cms.Service("MessageLogger")

process.myFilter = cms.EDFilter("DonRA2",
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

#process.TFileService = cms.Service("TFileService",fileName = cms.string('T_ABCD_TTbar.root') )

process.p = cms.Path(process.myFilter)

#Output events that pass filters
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('LM9_RA2selection.root'),
                               SelectEvents = cms.untracked.PSet (SelectEvents = cms.vstring('p'))
)

process.outpath = cms.EndPath(process.out)

#process.MessageLogger.cerr.FwkReport.reportEvery = 1000

