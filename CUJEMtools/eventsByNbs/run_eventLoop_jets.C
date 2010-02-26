{
 TString sample(gSystem->Getenv("SAMPLE"));
 TString index(gSystem->Getenv("INDEX"));
 TString nBJetsStatus(gSystem->Getenv("NBJETSSTATUS"));
    
  if (sample!="") {
    
    gSystem->Load("eventLoop_jets_C.so");
    selectData(sample,index);
    eventLoop_jets(nBJetsStatus);    
    
  }

}
