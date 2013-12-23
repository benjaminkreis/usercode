#!/bin/sh
cd /afs/cern.ch/user/k/kreis/scratch0/PATtest/CMSSW_3_3_6/src/qcdABCD/QcdWeights/test
eval `scramv1 runtime -sh`
cd -
cmsRun /afs/cern.ch/user/k/kreis/scratch0/PATtest/CMSSW_3_3_6/src/qcdABCD/QcdWeights/test/qcdweights_QCD_Pt15_6_cfg.py
rfcp T_weight_QCD_Pt15_6.root /castor/cern.ch/user/k/kreis/CUSusy/QCD/weight2/T_weight_QCD_Pt15_6.root

