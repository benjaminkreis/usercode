import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
fileNames = cms.untracked.vstring(

'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_6.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_60.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_61.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_62.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_63.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_64.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_65.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_66.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_67.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_68.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_69.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_7.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_70.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_71.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_72.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_73.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_74.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_75.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_76.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_77.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_78.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_79.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_8.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_80.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_81.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_82.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_83.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_84.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_85.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_86.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_87.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_88.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_89.root',

    )
 )    

process.demo = cms.EDAnalyzer('QcdWeights'
)

process.TFileService = cms.Service("TFileService",fileName = cms.string('T_weight_QCD_Pt15_8.root') )


process.p = cms.Path(process.demo)
