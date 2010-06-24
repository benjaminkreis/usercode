#!/bin/sh
cd /afs/cern.ch/user/k/kreis/scratch0/PATtest/CMSSW_3_3_6/src/qcdABCD/TagJet/test
eval `scramv1 runtime -sh`
cd -
cmsRun /afs/cern.ch/user/k/kreis/scratch0/PATtest/CMSSW_3_3_6/src/qcdABCD/TagJet/test/tagjet_QCD_Pt15_3_cfg.py
rfcp T_TagJet_QCD_Pt15_3.root /castor/cern.ch/user/k/kreis/CUSusy/QCD/tagjet2/T_TagJet_QCD_Pt15_3.root

