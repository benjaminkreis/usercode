
#include "TH2D.h"
#include "TChain.h"


#include <iostream>
using namespace std;

void studyHT(){

  double borderv1a = 0;
  double borderv2b_plot = 500;
  

  //histos
  TH2D * h2d = new TH2D("h2d", "h2d", 100, 0, 500, 100, 0,1000);
  TH1D * ht_met1 = new TH1D("ht_met1", "ht_met1", 50, 300, 3500);
  TH1D * ht_met2 = new TH1D("ht_met2", "ht_met2", 50, 300, 3500);
  TH1D * ht_met3 = new TH1D("ht_met3", "ht_met3", 50, 300, 3500);
  TH1D* histU1 = new TH1D("histU1", "histU1", 20, borderv1a, borderv2b_plot);
  TH1D* histL1 = new TH1D("histL1", "histL1", 20, borderv1a, borderv2b_plot);
  TH1D* histUL1 = new TH1D("histU1L", "histUL1", 20, borderv1a, borderv2b_plot);
  TH1D* histU2 = new TH1D("histU2", "histU2", 20, borderv1a, borderv2b_plot);
  TH1D* histL2 = new TH1D("histL2", "histL2", 20, borderv1a, borderv2b_plot);
  TH1D* histUL2 = new TH1D("histUL2", "histUL2", 20, borderv1a, borderv2b_plot);

  h2d->Sumw2();
  ht_met1->Sumw2();
  ht_met2->Sumw2();
  ht_met3->Sumw2();
  histU1->Sumw2();
  histL1->Sumw2();
  histUL1->Sumw2();
  histU2->Sumw2();
  histL2->Sumw2();
  histUL2->Sumw2();
  
  ht_met1->SetLineWidth(2);
  ht_met1->SetLineWidth(2);
  ht_met1->SetLineWidth(2);
  ht_met1->SetLineColor(kOrange-3);
  ht_met2->SetLineColor(kRed);
  ht_met3->SetLineColor(kViolet);

  histUL1->SetLineWidth(2);
  histUL2->SetLineWidth(2);
  histUL1->SetLineColor(kBlack);
  histUL2->SetLineColor(kRed);

  TChain* InputChain = new TChain("ABCDtree");
  InputChain->Add("/cu1/kreis/ABCDtrees/36_Jan22/ABCDtree.Baseline0_PF_pfMEThigh_PFLep0e0mu_minDP_NoMET_NoHT_NoDeltaPhi.ge0b.QCD.root");

  double myHT, x, y, weight;
  int nbtags;
  InputChain->SetBranchAddress("HT",&myHT);                                                                                                                                                                                    
  InputChain->SetBranchAddress("MET",&x);
  InputChain->SetBranchAddress("minDeltaPhiMET",&y);
  InputChain->SetBranchAddress("weight",&weight);
  InputChain->SetBranchAddress("nbSSVM",&nbtags);


  int numEntries = InputChain->GetEntries();
  for(int i = 0; i<numEntries; i++){
    InputChain->GetEvent(i);

    h2d->Fill(x,myHT,weight);

    if(x<120.) ht_met1->Fill(myHT,weight);
    if(x>150. && x<170.) ht_met2->Fill(myHT,weight);
    if(x>200.) ht_met3->Fill(myHT,weight);

    if(myHT>300){
      if(y<0.3) histL1->Fill(x,weight);
      if(y>=0.3) histU1->Fill(x,weight);
    }
    if(myHT>600){
      if(y<0.3) histL2->Fill(x,weight);
      if(y>=0.3) histU2->Fill(x,weight);
    }

  }
  histUL1->Divide(histU1, histL1, 1., 1.,"");
  histUL2->Divide(histU2, histL2, 1., 1.,"");

  h2d->Draw();
  
  ht_met1->DrawNormalized();
  ht_met2->DrawNormalized("SAME");
  ht_met3->DrawNormalized("SAME");

  histUL1->Draw();
  histUL2->Draw("SAME");
}
