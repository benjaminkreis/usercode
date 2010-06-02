#include "assert.h"
#include <iostream>

#include "TSystem.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TString.h"
#include "TChain.h"
#include "TFile.h"


void formChain1400(){
  cout << "Begin loading..." << endl;
  const TString treedirectory = "demo/T_weights";
  TChain* InputChain = new TChain(treedirectory);

  InputChain->Add("rfio:/castor/cern.ch/user/k/kreis/CUSusy/QCD/T_weight_QCD_Pt1400_*.root");
  
  InputChain->Merge("/cu1/kreis/QCD/T_weight_QCD_Pt1400.root");

  cout << "Loaded " << InputChain->GetListOfFiles()->GetEntries() << " files" << endl;

 return;

}
