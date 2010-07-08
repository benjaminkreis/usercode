import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(

            )
)

process.demo = cms.EDAnalyzer('BContent',
                              jetSrc      = cms.untracked.InputTag("cleanLayer1Jets") 
)

process.TFileService = cms.Service("TFileService",fileName = cms.string('T_BContent_QCD_Pt15.root') )


process.p = cms.Path(process.demo)
