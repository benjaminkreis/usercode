#! /usr/bin/env python

import os

name = "ZnnHiggs0PH_M-125p6_8TeV-JHUGenV4-private"


for i in range (0,10):
    inName = name + "_" + str(i) + ".lhe"
    outName = name + "_" + str(i) + ".root"
    
    lheFile = "/eos/uscms/store/user/kreis/20140110_production/" + name + "/" + inName
    rootFile = "/eos/uscms/store/user/kreis/20140110_production/" + name + "/" + outName
    #rootFile = "rootFiles/" + outName
    
    os.system(('./hbbTreeMaker %s %s')%(lheFile,rootFile))
    
