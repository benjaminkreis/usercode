
#include "TH2D.h"
#include "TChain.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLegend.h"

#include <iostream>
using namespace std;

void studyHT(){
  gROOT->SetStyle("CMS");


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
  TH1D* histU3 = new TH1D("histU3", "histU3", 20, borderv1a, borderv2b_plot);
  TH1D* histL3 = new TH1D("histL3", "histL3", 20, borderv1a, borderv2b_plot);
  TH1D* histUL3 = new TH1D("histUL3", "histUL3", 20, borderv1a, borderv2b_plot);

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
  histU3->Sumw2();
  histL3->Sumw2();
  histUL3->Sumw2();

  ht_met1->SetLineWidth(2);
  ht_met1->SetLineWidth(2);
  ht_met1->SetLineWidth(2);
  ht_met1->SetLineColor(kBlack);
  ht_met2->SetLineColor(kBlue+1);
  ht_met3->SetLineColor(kBlue-9);
  ht_met1->SetMarkerColor(kBlack);
  ht_met2->SetMarkerColor(kBlue+1);
  ht_met3->SetMarkerColor(kBlue-9);
  ht_met1->GetXaxis()->SetTitle("HT");
  ht_met1->GetYaxis()->SetTitle("Events");
  TLegend *legOne = new TLegend(.65,.75,.89,.89);
  legOne->SetFillStyle(0);
  legOne->SetFillColor(0);
  legOne->SetBorderSize(0);
  legOne->SetLineStyle(0);
  legOne->AddEntry(ht_met1,"MET<120 GeV","P");
  legOne->AddEntry(ht_met2,"150<MET<170 GeV","P");
  legOne->AddEntry(ht_met3,"MET>200 GeV","P");

  histUL1->SetLineWidth(2);
  histUL2->SetLineWidth(2);
  histUL3->SetLineWidth(2);
  histUL1->SetLineColor(kBlack);
  histUL2->SetLineColor(kBlue+1);
  histUL3->SetLineColor(kBlue-9);
  histUL1->SetMarkerColor(kBlack);
  histUL2->SetMarkerColor(kBlue+1);
  histUL3->SetMarkerColor(kBlue-9);
  histUL1->GetXaxis()->SetTitle("MET");
  histUL1->GetYaxis()->SetTitle("r(MET)");
  TLegend *legR = new TLegend(.65,.75,.89,.89);
  legR->SetFillStyle(0);
  legR->SetFillColor(0);
  legR->SetBorderSize(0);
  legR->SetLineStyle(0);
  legR->AddEntry(histUL1, "HT>300 GeV", "P");
  legR->AddEntry(histUL2, "HT>600 GeV", "P");
  legR->AddEntry(histUL3, "HT>900 GeV", "P");

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
    if(myHT>900){
      if(y<0.3) histL3->Fill(x,weight);
      if(y>=0.3) histU3->Fill(x,weight);
    }

  }
  histUL1->Divide(histU1, histL1, 1., 1.,"");
  histUL2->Divide(histU2, histL2, 1., 1.,"");
  histUL3->Divide(histU3, histL3, 1., 1.,"");

  //h2d->Draw();
  
  TCanvas * canvasOne = new TCanvas("canvasOne", "canvasOne", 640, 480);
  canvasOne->cd();
  /* ht_met1->DrawNormalized();
  ht_met2->DrawNormalized("SAME");
  ht_met3->DrawNormalized("SAME");*/
  ht_met1->Draw();
  ht_met2->Draw("SAME");
  ht_met3->Draw("SAME");
  legOne->Draw();
  gPad->SetRightMargin(0.05);
  gPad->SetLogy(1);
  gPad->Modified();
  canvasOne->Print("HTdist_unN.pdf");

  TCanvas * rCanvas = new TCanvas("rCanvas", "rCanvas", 640, 480);
  rCanvas->cd();
  histUL1->Draw();
  histUL2->Draw("SAME");
  histUL3->Draw("SAME");
  gPad->SetRightMargin(0.05);
  gPad->SetLogy(1);
  gPad->Modified();
  legR->Draw();
  rCanvas->Print("rMET_HT.pdf");
}
