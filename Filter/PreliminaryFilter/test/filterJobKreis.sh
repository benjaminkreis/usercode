#!/bin/sh
cd /afs/cern.ch/user/k/kreis/scratch0/CMSSW_3_3_2/src/Filter/PreliminaryFilter/test
eval `scramv1 runtime -sh`
cd -
cmsRun /afs/cern.ch/user/k/kreis/scratch0/CMSSW_3_3_2/src/Filter/PreliminaryFilter/test/filterPatEvents_LM3_cfg.py
#rfcp lm3_filtered.root /castor/cern.ch/user/k/kreis/LM3Filtered/lm3_filtered.root

