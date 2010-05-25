import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
            'rfio:/castor/cern.ch/user/c/chadwick/TopAna/CMSSW_3_3_1/QCD_Pt30/QCD_Pt30_29.root',
                )
    )                            

process.demo = cms.EDAnalyzer('QcdWeights'
)

process.TFileService = cms.Service("TFileService",fileName = cms.string('T_weight_QCD_Pt30.root') )


process.p = cms.Path(process.demo)
