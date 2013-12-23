#include "assert.h"
#include <iostream>

#include "TSystem.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TString.h"
#include "TChain.h"
#include "TFile.h"


void formChain800(){
  cout << "Begin loading..." << endl;
  const TString treedirectory = "demo/T_JetTag";
  TChain* InputChain = new TChain(treedirectory);

  InputChain->Add("rfio:/castor/cern.ch/user/k/kreis/CUSusy/QCD/T_TagJet_QCD_Pt800_*.root");
  
  InputChain->Merge("/cu1/kreis/QCD/T_TagJet_QCD_Pt800.root");

  cout << "Loaded " << InputChain->GetListOfFiles()->GetEntries() << " files" << endl;

 return;

}
