#include <iostream>
#include <iomanip>
#include <assert.h>


#include "TROOT.h"
#include "TStyle.h"

#include "TMinuit.h"

#include "TString.h"
#include "TChain.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TF1.h"
#include "TGraphErrors.h"

#include "analyzeFillABCDInput.h"


bool passb(int nbtags){
  if(nbtags>=1){
    return true;
  }
  else{
    return false;
  }
}


void simpleABCD(){

  double pi=4*atan(1.0)+.0001;

  //x
  double borderv1a = 0.0;
  double borderv1b = 150.0;
  double borderv2a=150.0;
  double borderv2b=1e10;
  
  //y
  double borderh1a=0.0;
  double borderh1b=0.3;
  double borderh2a=0.3;
  double borderh2b=pi;
  
  TH2D* histA = new TH2D("histA", "histA", 1, borderv1a, borderv1b, 1, borderh1a, borderh1b);
  TH2D* histB = new TH2D("histB", "histB", 1, borderv1a, borderv1b, 1, borderh2a, borderh2b);
  TH2D* histC = new TH2D("histC", "histC", 1, borderv2a, borderv2b, 1, borderh2a, borderh2b);
  TH2D* histD = new TH2D("histD", "histD", 1, borderv2a, borderv2b, 1, borderh1a, borderh1b);
  histA->Sumw2();
  histB->Sumw2();
  histC->Sumw2();
  histD->Sumw2();

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
    if(!passb(nbtags)) continue;
    
    histA->Fill(x,y,weight);
    histB->Fill(x,y,weight);
    histC->Fill(x,y,weight);
    histD->Fill(x,y,weight);
    
  }//end loop over InputChain                                      

  
  double nC = histD->GetBinContent(1,1)*histB->GetBinContent(1,1)/histA->GetBinContent(1,1);
  double nC_err = nC*sqrt( histD->GetBinError(1,1)*histD->GetBinError(1,1)/histD->GetBinContent(1,1)/histD->GetBinContent(1,1)
			   + histB->GetBinError(1,1)*histB->GetBinError(1,1)/histB->GetBinContent(1,1)/histB->GetBinContent(1,1)
			   + histA->GetBinError(1,1)*histA->GetBinError(1,1)/histA->GetBinContent(1,1)/histA->GetBinContent(1,1)
			   );

  
  cout << "nC true: " << histC->GetBinContent(1,1) << " +- " << histC->GetBinError(1,1) << endl;
  cout << "ABCD: " << nC << " +- " << nC_err << endl;
  
}
