import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(

'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_170_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_171_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_172_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_173_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_174_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_175_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_176_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_177_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_178_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_179_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_17_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_180_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_181_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_182_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_183_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_184_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_185_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_186_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_187_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_188_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_189_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_18_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_190_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_191_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_192_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_193_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_194_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_195_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_196_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_197_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_198_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_199_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_19_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_1_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_200_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_201_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_202_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_203_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_204_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_205_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_206_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_207_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_208_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_209_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_20_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_210_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_211_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_212_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_213_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_214_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_215_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_21_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_22_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_23_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_24_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_25_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_26_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_27_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_28_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_29_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_2_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_30_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_31_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_32_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_33_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_34_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_35_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_36_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_37_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_38_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_39_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_3_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_40_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_41_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_42_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_43_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_44_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_45_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_46_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_47_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_48_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_49_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_4_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_50_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_51_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_52_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_53_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_54_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_55_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_56_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_57_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_58_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_59_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_5_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_60_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_61_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_62_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_63_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_64_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_65_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_66_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_67_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_68_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_69_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_6_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_70_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_71_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_72_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_73_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_74_2.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_75_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_76_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_77_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_78_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_79_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_7_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_80_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_81_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_82_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_83_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_84_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_85_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_86_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_87_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_88_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_89_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_8_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_90_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_91_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_92_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_93_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_94_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_95_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_96_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_97_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_98_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_99_1.root',
'rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/t2/QCD_Pt470_RA2selection_9_2.root',

    )
)

process.demo = cms.EDAnalyzer('TagFraction',
jetSrc      = cms.untracked.InputTag("cleanLayer1Jets")                              
)


process.TFileService = cms.Service("TFileService",fileName = cms.string('T_TagFraction_QCD_Pt470_2.root') )


process.p = cms.Path(process.demo)
