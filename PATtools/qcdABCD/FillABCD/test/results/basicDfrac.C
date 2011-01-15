#include <iostream>

#include "TChain.h"
#include "TH1D.h"

#include "analyzeFillABCDInput.h"

using namespace std;

void doBasicDfrac(int nbcut = 0, TString cutType = ""){
  cout << "nbcut: " << cutType << nbcut << endl;
  double pi=4*atan(1.0);

  TH1D* hDpass = new TH1D("hDpass", "hDpass", 1, 0.0, .3);
  TH1D* hDall  = new TH1D("hDall",  "hDall",  1, 0.0, .3);
  TH1D* hDeff  = new TH1D("hDeff",  "hDeff",  1, 0.0, .3);
  TH1D* hCpass = new TH1D("hCpass", "hCpass", 1, 0.3, pi);
  TH1D* hCall  = new TH1D("hCall",  "hCall",  1, 0.3, pi);
  TH1D* hCeff  = new TH1D("hCeff",  "hCeff",  1, 0.3, pi);
  hDpass->Sumw2();
  hDall->Sumw2();
  hDeff->Sumw2();
  hCpass->Sumw2();
  hCall->Sumw2();
  hCeff->Sumw2();

  TChain* InputChain = FormChainJosh("pfpf");
  int numEntries = InputChain->GetEntries();
  cout <<"numEntries: " << numEntries << endl;
  
  double x, y, weight;
  int nbtags;
  InputChain->SetBranchAddress("MET",&x);
  InputChain->SetBranchAddress("minDeltaPhiMET",&y);
  InputChain->SetBranchAddress("weight",&weight);
  InputChain->SetBranchAddress("nbSSVM",&nbtags);

  for(int i = 0; i<numEntries; i++){
    InputChain->GetEvent(i);
    
    int pass = 0;
    if(cutType=="e"){
          if(nbtags==nbcut)pass =1;
    }
    else if(cutType=="ge"){
      if(nbtags>=nbcut)pass =1;
    }
    else{
      cout << "INVALID CUT TYPE!" << endl;
    }

    if(x>=150.0){
      hCall->Fill(y, weight);
      hDall->Fill(y, weight);
      if(pass) hCpass->Fill(y, weight);
      if(pass) hDpass->Fill(y, weight);
    }//end MET check
    
  }//end loop over InputChain
  
  hCeff->Divide(hCpass, hCall, 1., 1., "B");
  hDeff->Divide(hDpass, hDall, 1., 1., "B");

  double Dfrac = hDeff->GetBinContent(1);
  double Dfrac_e = hDeff->GetBinError(1);
  double Cfrac = hCeff->GetBinContent(1);
  double Cfrac_e = hCeff->GetBinError(1);

  cout << "nD: " << hDall->GetBinContent(1) << " +- " << hDall->GetBinError(1) << endl;
  cout << "Dfrac: " << Dfrac <<" +- " << Dfrac_e <<  endl;
  cout << "nC: " << hCall->GetBinContent(1) << " +- " << hCall->GetBinError(1) << endl;
  cout << "Cfrac: " << Cfrac <<" +- " << Cfrac_e <<  endl;
  cout << endl;
  cout << endl;

  hDpass->Clear();
  hDall->Clear();
  hDeff->Clear();
  hCpass->Clear();
  hCall->Clear();
  hCeff->Clear();

}//end doBasicDfrac

void *basicDfrac(){
  
  doBasicDfrac(1,"e");
  doBasicDfrac(1,"ge");
  doBasicDfrac(2,"ge");

}
