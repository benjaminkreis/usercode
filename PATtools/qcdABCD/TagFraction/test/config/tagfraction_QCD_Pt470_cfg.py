import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_100_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_101_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_102_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_103_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_104_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_105_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_106_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_107_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_108_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_109_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_10_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_110_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_111_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_112_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_113_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_114_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_115_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_116_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_117_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_118_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_119_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_11_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_120_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_121_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_122_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_123_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_124_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_125_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_126_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_127_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_128_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_129_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_12_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_130_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_131_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_132_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_133_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_134_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_135_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_136_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_137_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_138_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_139_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_13_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_140_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_141_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_142_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_143_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_144_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_145_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_146_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_147_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_148_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_149_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_14_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_150_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_151_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_152_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_153_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_154_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_155_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_156_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_157_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_158_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_159_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_15_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_160_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_161_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_162_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_163_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_164_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_165_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_166_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_167_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_168_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_169_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_16_1.root',

    )
)

process.demo = cms.EDAnalyzer('TagFraction',
jetSrc      = cms.untracked.InputTag("cleanLayer1Jets")                              
)


process.TFileService = cms.Service("TFileService",fileName = cms.string('T_TagFraction_QCD_Pt470.root') )


process.p = cms.Path(process.demo)
