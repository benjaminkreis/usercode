import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/QCD_Pt15_RA2selection_1.root',
        'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/QCD_Pt15_RA2selection_2.root',
        'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/QCD_Pt15_RA2selection_3.root',
        'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/QCD_Pt15_RA2selection_4.root',
        'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/QCD_Pt15_RA2selection_5.root',
        'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/QCD_Pt15_RA2selection_6.root',
        'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/QCD_Pt15_RA2selection_7.root',
        'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/QCD_Pt15_RA2selection_8.root',
        'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/QCD_Pt15_RA2selection_9.root',
        )
)

process.demo = cms.EDAnalyzer('FillABCD',
jetSrc      = cms.untracked.InputTag("cleanLayer1Jets")                              
)


process.TFileService = cms.Service("TFileService",fileName = cms.string('T_ABCD_QCD_Pt15.root') )


process.p = cms.Path(process.demo)
