import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

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

process.demo = cms.EDAnalyzer('TagJet',
  jetSrc      = cms.untracked.InputTag("cleanLayer1Jets")
                              )

process.TFileService = cms.Service("TFileService",fileName = cms.string('T_TagJet_QCD_Pt15_1.root') )


process.p = cms.Path(process.demo)
