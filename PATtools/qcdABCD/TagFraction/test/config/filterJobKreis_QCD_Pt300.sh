#!/bin/sh
cd /afs/cern.ch/user/k/kreis/scratch0/PATtest/CMSSW_3_3_6/src/qcdABCD/TagFraction/test
eval `scramv1 runtime -sh`
cd -
cmsRun /afs/cern.ch/user/k/kreis/scratch0/PATtest/CMSSW_3_3_6/src/qcdABCD/TagFraction/test/tagfraction_QCD_Pt300_cfg.py
rfcp T_TagFraction_QCD_Pt300.root /castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/trees/T_TagFraction_QCD_Pt300_double.root

