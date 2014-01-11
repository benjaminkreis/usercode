#! /usr/bin/env python

import os

name = "ZHiggs0P_M-125p6_7TeV-JHUGenV4"

for i in range (0,100):
    inName = name + "_" + str(i) + ".lhe"
    outName = name + "_" + str(i) + ".root"
    
    lheFile = "/eos/uscms/store/user/kreis/20140110_production/" + inName
    rootFile = "rootFiles/" + outName
    
    os.system(('./hbbTreeMaker %s %s')%(lheFile,rootFile))
    
