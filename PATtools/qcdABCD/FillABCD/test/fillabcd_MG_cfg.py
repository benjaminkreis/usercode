import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD_Spring10/RA2Selection_QCD_MG4_90_1_k7T.root',
    )
)

process.demo = cms.EDAnalyzer('FillABCD',
jetSrc      = cms.untracked.InputTag("cleanPatJetsAK5Calo"),
metSrc      = cms.untracked.InputTag("patMETsAK5Calo"),
MG          = cms.untracked.double(0.0)

)


process.TFileService = cms.Service("TFileService",fileName = cms.string('T_ABCD.root') )

process.p = cms.Path(process.demo)
