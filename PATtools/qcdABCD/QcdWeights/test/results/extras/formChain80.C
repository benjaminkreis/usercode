#include "assert.h"
#include <iostream>

#include "TSystem.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TString.h"
#include "TChain.h"
#include "TFile.h"


void formChain80(){
  cout << "Begin loading..." << endl;
  const TString treedirectory = "demo/T_weights";
  TChain* InputChain = new TChain(treedirectory);

  InputChain->Add("rfio:/castor/cern.ch/user/k/kreis/CUSusy/QCD/T_weight_QCD_Pt80.root");
  
  InputChain->Merge("/cu1/kreis/QCD/T_weights_QCD_Pt80.root");

  cout << "Loaded " << InputChain->GetListOfFiles()->GetEntries() << " files" << endl;

 return;

}
