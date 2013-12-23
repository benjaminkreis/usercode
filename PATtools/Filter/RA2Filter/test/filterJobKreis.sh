#!/bin/sh
cd /afs/cern.ch/user/k/kreis/scratch0/PATtest/CMSSW_3_3_6/src/Filter/RA2Filter/test
eval `scramv1 runtime -sh`
cd -
cmsRun /afs/cern.ch/user/k/kreis/scratch0/PATtest/CMSSW_3_3_6/src/Filter/RA2Filter/test/ra2Filter_cfg.py
rfcp TTbar_RA2selection.root /castor/cern.ch/user/k/kreis/TTbar_RA2selection.root

