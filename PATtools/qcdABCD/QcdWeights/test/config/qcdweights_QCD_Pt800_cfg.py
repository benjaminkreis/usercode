import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
 fileNames = cms.untracked.vstring(
                'rfio:/castor/cern.ch/user/k/kreis/LM3PAT/lm3_7tev_1.root',
                        )
                     )    

process.demo = cms.EDAnalyzer('QcdWeights'
)

process.TFileService = cms.Service("TFileService",fileName = cms.string('T_weight_QCD_Pt800.root') )


process.p = cms.Path(process.demo)
