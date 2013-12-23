#include <iostream>
#include <vector>
#include <exception>
#include <cmath>
#include <iomanip>
#include <assert.h>

#include "TH1D.h"
#include "TH2D.h"
#include "TChain.h"



using namespace std;

bool bcontinue(int nbtags, TString bcut){
  bool returnTrue = false; 
  if(bcut=="eq1"){
    if(nbtags==1) returnTrue = true;
  }
  else if(bcut=="ge1"){
    if(nbtags>=1) returnTrue = true;
  }
  else if(bcut=="ge2"){
    if(nbtags>=2) returnTrue = true;
  }
  else{assert(0);}
  return returnTrue;
}

void dorcont(TString region="A", TString sample = "QCD", TString bcut="ge1"){
  double singleLow=100;
  double singleHigh=150;
  double borderv2a=150.0;
  double borderv2b=1000000000.;
  double borderh1a=0.0;
  double borderh1b=0.3;
  double borderh2a=0.3;
  double borderh2b=3.142;


  TString fileName = "/cu1/kreis/ABCDtrees/36_Mar26/ABCDtree.Baseline0_PF_JERbias_pfMEThigh_PFLepRA20e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.";
  fileName+=sample;
  fileName+=".root";

  TChain* InputChain = new TChain("ABCDtree");
  InputChain->Add(fileName);

  double x, y, weight;
  int nbtags;
  InputChain->SetBranchAddress("MET",&x);
  InputChain->SetBranchAddress("minDeltaPhiMET",&y);
  InputChain->SetBranchAddress("weight",&weight);
  InputChain->SetBranchAddress("nbSSVM",&nbtags);
  const int numEntries = InputChain->GetEntries();

  TH2D* histA = new TH2D("histA", "histA", 1, singleLow, singleHigh, 1, borderh1a, borderh1b);  
  TH2D* histB = new TH2D("histB", "histB", 1, singleLow, singleHigh, 1, borderh2a, borderh2b);
  TH2D* histC = new TH2D("histC", "histC", 1, borderv2a, borderv2b, 1, borderh2a, borderh2b);
  TH2D* histD = new TH2D("histD", "histD", 1, borderv2a, borderv2b, 1, borderh1a, borderh1b);
  histA->Sumw2();
  histB->Sumw2();
  histC->Sumw2();
  histD->Sumw2();
  
  for(int i = 0; i<numEntries; i++){
    InputChain->GetEvent(i);
    
    if(!(bcontinue(nbtags, bcut))) continue;
    
    histB->Fill(x,y, weight);
    histA->Fill(x,y, weight);
    histC->Fill(x,y,weight);
    histD->Fill(x,y,weight);
  }
  
  double nA=histA->GetBinContent(1,1);
  double nA_err = histA->GetBinError(1,1);
  double nB=histB->GetBinContent(1,1);
  double nB_err = histB->GetBinError(1,1);
  double nC=histC->GetBinContent(1,1);
  double nC_err = histC->GetBinError(1,1);
  double nD = histD->GetBinContent(1,1);
  double nD_err = histD->GetBinError(1,1);
 
  cout << sample << ": " ;
  if(region=="A") cout << nA << " +- " << nA_err << endl;
  else if(region=="B") cout << nB << " +- " << nB_err << endl;
  else if(region=="C") cout << nC << " +- " << nC_err << endl;
  else if(region=="D") cout << nD << " +- " << nD_err << endl;
  else {assert(0);}
  
  histA->Clear();
  histB->Clear();
  histC->Clear();
  histD->Clear();

}

void rcont(){

  TString QCD = "PythiaPUQCD" ;
  //"QCD" for MG, "PythiaPUQCDFlat 
  TString regionA[]={"A","B","C","D"};
  TString bcutA[]={"eq1","ge1","ge2"};
  for(int i=0; i<3; i++){
    TString bcut = bcutA[i];
    cout << endl;
    cout << "*****" << bcut << "*****" << endl;
    
    for(int j=0; j<4; j++){
      TString region = regionA[j];
      cout << endl;
      cout << "Region " << region << ":" << endl;
     
      dorcont(region,QCD,bcut);
      dorcont(region,"TTbarJets",bcut);
      dorcont(region,"SingleTop-sChannel",bcut);
      dorcont(region,"SingleTop-tChannel",bcut);
      dorcont(region,"SingleTop-tWChannel",bcut);
      dorcont(region,"WJets",bcut);
      dorcont(region,"ZJets",bcut);
      dorcont(region,"Zinvisible",bcut);
      dorcont(region,"LM9",bcut);
      dorcont(region,"LM13",bcut);

    }

  }


  
}


