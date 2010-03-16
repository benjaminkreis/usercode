#include "TFile.h"
#include "TChain.h"
#include "TObject.h"


void addBranch(){

  TFile* f1  = TFile::Open("plots_QCD_Pt170_nBJetsGT2.root", "update");
  float weight;

  TTree *tree1 = (TTree*)f1->Get("T_minDPhi_MET");
  TBranch *newBranch = tree1->Branch("weight",&weight, "weight/F");

  //weight = 28.8377; //QCD_Pt80
  weight = 0.813167135;
  //weight = 0.002265; //LM1
  
  int nentreis = tree1->GetEntries();

  for (int i = 0; i< nentreis; i++){
    newBranch->Fill();
  }

  tree1->Write("", TObject::kOverwrite);


}
