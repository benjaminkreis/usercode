import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.source = cms.Source("PoolSource",
 fileNames = cms.untracked.vstring(
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_27.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_270.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_271.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_272.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_273.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_274.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_275.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_276.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_277.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_278.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_279.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_28.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_280.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_281.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_282.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_283.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_284.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_285.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_286.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_287.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_288.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_289.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_29.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_290.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_291.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_292.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_293.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_294.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_295.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_296.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_297.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_298.root',
'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt15/QCD_Pt15_299.root',

    )
)    

process.demo = cms.EDAnalyzer('QcdWeights'
)

process.TFileService = cms.Service("TFileService",fileName = cms.string('T_weight_QCD_Pt15_6.root') )


process.p = cms.Path(process.demo)
