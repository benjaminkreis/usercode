import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("Test")

process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10000)
)


process.source = cms.Source("PoolSource",
 fileNames = cms.untracked.vstring(
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_1.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_10.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_100.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_101.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_103.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_104.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_105.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_106.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_107.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_108.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_109.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_11.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_110.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_111.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_112.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_113.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_114.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_115.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_116.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_117.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_118.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_119.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_12.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_121.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_122.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_123.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_125.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_126.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_127.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_128.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_129.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_13.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_130.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_131.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_132.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_133.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_134.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_135.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_136.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_137.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_138.root',
  'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_139.root',
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
  bsSrc= cms.InputTag("offlineBeamSpot"),                                
  useWeight=cms.untracked.double(1)
)

process.TFileService = cms.Service("TFileService",fileName = cms.string('T_DonRA2_QCD_Pt15_1.root') )

process.p = cms.Path(process.myFilter)

#Output events that pass filters
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('LM9_RA2selection.root'),
                               SelectEvents = cms.untracked.PSet (SelectEvents = cms.vstring('p'))
)

#process.outpath = cms.EndPath(process.out)

#process.MessageLogger.cerr.FwkReport.reportEvery = 1000

