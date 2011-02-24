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

bool passmdp(double mdp){
  if(mdp>0.0){
    return true;
  }
  else{
    return false;
  }
}


void simpleABCD(){

  double pi=4*atan(1.0)+.0001;

  TString xName;
  TString yName;
  double borderv1a;
  double borderv1b;
  double borderv2a;
  double borderv2b;
  double borderh1a;
  double borderh1b;
  double borderh2a;
  double borderh2b;
  TH2D* histA;
  TH2D* histB;
  TH2D* histC;
  TH2D* histD;

  // MET - minDeltaPhiMET ////////////////////
  xName="MET";
  yName="minDeltaPhiMET";
  //x
  borderv1a = 0.0;
  borderv1b = 150.0;
  borderv2a=150.0;
  borderv2b=1e10;
  //y
  borderh1a=0.0;
  borderh1b=0.3;
  borderh2a=0.3;
  borderh2b=pi;
  histA = new TH2D("histA", "histA", 1, borderv1a, borderv1b, 1, borderh1a, borderh1b);
  histB = new TH2D("histB", "histB", 1, borderv1a, borderv1b, 1, borderh2a, borderh2b);
  histC = new TH2D("histC", "histC", 1, borderv2a, borderv2b, 1, borderh2a, borderh2b);
  histD = new TH2D("histD", "histD", 1, borderv2a, borderv2b, 1, borderh1a, borderh1b);
  

  // MET - jet1eta ///////////////////////
  xName="MET";
  yName="jet1eta";
  //x
  borderv1a = 0.0;
  borderv1b = 150.0;
  borderv2a=150.0;
  borderv2b=1e10;
  //y
  borderh1a=0.0;
  borderh1b=1.2;
  borderh2a=1.2;
  borderh2b=2.4001;
  histB = new TH2D("histB", "histB", 1, borderv1a, borderv1b, 1, borderh1a, borderh1b);
  histA = new TH2D("histA", "histA", 1, borderv1a, borderv1b, 1, borderh2a, borderh2b);
  histD = new TH2D("histD", "histD", 1, borderv2a, borderv2b, 1, borderh2a, borderh2b);
  histC = new TH2D("histC", "histC", 1, borderv2a, borderv2b, 1, borderh1a, borderh1b);
  
  
  
  histA->Sumw2();
  histB->Sumw2();
  histC->Sumw2();
  histD->Sumw2();

  TChain* InputChain = FormChainJosh("pfpf");
  int numEntries = InputChain->GetEntries();
  cout <<"numEntries: " << numEntries << endl;
  

  double x, y, weight, mdp;
  int nbtags;
  InputChain->SetBranchAddress(xName, &x);
  InputChain->SetBranchAddress(yName, &y);
  InputChain->SetBranchAddress("weight", &weight);
  InputChain->SetBranchAddress("nbSSVM", &nbtags);
  InputChain->SetBranchAddress("minDeltaPhiMET", &mdp);


  double nMET=0, nMETMdp=0, nMETTagMdp=0;
  for(int i = 0; i<numEntries; i++){
    InputChain->GetEvent(i);
    if(x>150) nMET+=weight;
    if(!passmdp(mdp)) continue;   
    if(x>150) nMETMdp+=weight;
    if(!passb(nbtags)) continue;   
    if(x>150) nMETTagMdp+=weight;

    y=fabs(y);

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

  
  cout << "nMET: " << nMET << endl;
  cout << "nMETMdp: " << nMETMdp << endl;
  cout << "nMETTagMdp: " << nMETTagMdp << endl;
  cout << endl;
  cout << "nA true: " << histA->GetBinContent(1,1) << " +- " << histA->GetBinError(1,1) << endl;
  cout << "nB true: " << histB->GetBinContent(1,1) << " +- " << histB->GetBinError(1,1) << endl;
  cout << "nC true: " << histC->GetBinContent(1,1) << " +- " << histC->GetBinError(1,1) << endl;
  cout << "nD true: " << histD->GetBinContent(1,1) << " +- " << histD->GetBinError(1,1) << endl;
  cout << "ABCD: " << nC << " +- " << nC_err << endl;
  
}
