#!/bin/sh
cd /afs/cern.ch/user/k/kreis/scratch0/PATtest/CMSSW_3_3_6/src/qcdABCD/FillABCD/test
eval `scramv1 runtime -sh`
cd -
cmsRun /afs/cern.ch/user/k/kreis/scratch0/PATtest/CMSSW_3_3_6/src/qcdABCD/FillABCD/test/fillabcd_QCD_Pt300_2_cfg.py
rfcp T_ABCD_QCD_Pt300_2.root /castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/trees/T_ABCD_QCD_Pt300_2.root

