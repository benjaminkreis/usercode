
LHEAnalyzers
============

To compile:
c++ -o hbbTreeMaker `root-config --glibs --cflags` hbbTreeMaker.cpp

if you need to add something from RooFit, do:
c++ -o hbbTreeMaker `root-config --glibs --cflags` -I $ROOFITSYS/include hbbTreeMaker.cpp

To run:
./hbbTreeMaker <input lhe filename> <output root filename>

