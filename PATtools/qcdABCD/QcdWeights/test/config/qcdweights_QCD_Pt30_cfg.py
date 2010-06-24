import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.source = cms.Source("PoolSource",
fileNames = cms.untracked.vstring(
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_1.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_10.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_11.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_12.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_13.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_14.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_15.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_16.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_17.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_18.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_19.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_2.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_20.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_21.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_22.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_23.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_24.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_25.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_26.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_27.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_28.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_29.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_3.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_30.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_31.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_32.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_33.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_34.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_35.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_36.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_37.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_38.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_39.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_4.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_40.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_41.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_42.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_43.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_44.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_45.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_46.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_47.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_48.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_49.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_50.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_51.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_52.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_53.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_54.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_55.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_56.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_6.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_7.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_8.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_9.root',
    )
)    

process.demo = cms.EDAnalyzer('QcdWeights'
)

process.TFileService = cms.Service("TFileService",fileName = cms.string('T_weight_QCD_Pt30.root') )


process.p = cms.Path(process.demo)
