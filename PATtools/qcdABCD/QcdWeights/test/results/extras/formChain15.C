#include "assert.h"
#include <iostream>

#include "TSystem.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TString.h"
#include "TChain.h"
#include "TFile.h"


void formChain15(){
  cout << "Begin loading..." << endl;
  const TString treedirectory = "demo/T_weights";
  TChain* InputChain = new TChain(treedirectory);

  InputChain->Add("rfio:/castor/cern.ch/user/k/kreis/CUSusy/QCD/weight2/T_weight_QCD_Pt15_*.root");
  
  cout <<"Begin merging..."<<endl;

  InputChain->Merge("/cu1/kreis/QCD/T_weights_QCD_Pt15.root");

  cout << "Loaded and merged " << InputChain->GetListOfFiles()->GetEntries() << " files" << endl;

 return;

}
