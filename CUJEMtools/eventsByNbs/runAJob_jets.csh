#! /bin/csh -f

#make sure SAMPLE is set
#if there is more than one file, make sure INDEX is set to the max index value

#bsub -q 8nh -J $SAMPLE -o makeplots.${SAMPLE}.log runAJob.csh

cd /afs/cern.ch/user/k/kreis/scratch0/CMSSW_3_3_6/src/

eval `scramv1 runtime -csh`

cd UserCode/Kreis/CUJEMtools/eventsByNbs

root -b -l -q run_eventLoop_jets.C

