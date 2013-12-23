import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/QCD_Pt30_RA2selection.root',
    )
)

process.demo = cms.EDAnalyzer('TagFraction',
jetSrc      = cms.untracked.InputTag("cleanLayer1Jets")                              
)


process.TFileService = cms.Service("TFileService",fileName = cms.string('T_TagFraction_QCD_Pt30.root') )


process.p = cms.Path(process.demo)
