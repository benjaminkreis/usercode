#!/bin/sh
cd /afs/cern.ch/user/k/kreis/scratch0/CMSSW_3_5_6/src/qcdABCD/BContent/test
eval `scramv1 runtime -sh`
cd -
cmsRun /afs/cern.ch/user/k/kreis/scratch0/CMSSW_3_5_6/src/qcdABCD/BContent/test/bcontent_QCD_Pt170_cfg.py
rfcp T_BContent_QCD_Pt170.root /castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/trees/T_BContent_QCD_Pt170.root

