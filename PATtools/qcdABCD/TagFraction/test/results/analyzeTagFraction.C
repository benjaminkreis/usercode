#include "assert.h"
#include <iostream>

#include "TSystem.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TString.h"
#include "TChain.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"

#include "analyzeTagFractionInput.h"

using namespace std;

void analyzeTagFraction(){
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptStat("euo");
    
  //INPUT
  TFile finweight("/afs/cern.ch/user/k/kreis/scratch0/PATtest/CMSSW_3_3_6/src/qcdABCD/weight_QCD.root","READ");
  TH1D* Hweight = 0;
  if(!finweight.IsZombie()){
    Hweight = (TH1D*)finweight.Get("Hweight");
  }
  else{
    cout << "ERROR loading weights" << endl;
  }
    

  TFile fout("TagFraction_plots.root", "RECREATE");
  TCanvas *C_2D = new TCanvas("C_2D", "2D", 640, 480);
  C_2D->cd();
  TCanvas *C_P = new TCanvas("C_P", "P", 640, 480);
  C_P->cd();
  TH1F *hP0 = new TH1F("hP0", "hP0", 220, 0, 1);
  TH1F *hP1 = new TH1F("hP1", "hP1", 220, 0, 1);
  TH1F *hP2 = new TH1F("hP2", "hP2", 220, 0, 1);
  TH1F *hPge2 = new TH1F("hPge2", "hPge2", 220, 0, 1);
  TH1F *hPge3 = new TH1F("hPge3", "hPge3", 220, 0, .2);
  TH2F *hPvT = new TH2F("hPvT", "hPvT",  6, -.5, 5.5, 100, 0, 1);
  

  TChain* InputChain = FormChain(); 
  int fillNum = 0;
  int numEntries = InputChain->GetEntries();
  cout <<"numEntries: " << numEntries << endl;
  
  float P2Tot = 0, dP2Tott = 0, Pge2Tot = 0,dPge2Tott = 0, Pge3Tot=0, dPge3Tott = 0;

  double pthat, P0, P1, P2, dP2, Pge2, dPge2, Pge3, dPge3, btag;
  InputChain->SetBranchAddress("PtHat", &pthat);
  InputChain->SetBranchAddress("P0", &P0);
  InputChain->SetBranchAddress("P1", &P1);
  InputChain->SetBranchAddress("P2", &P2);
  InputChain->SetBranchAddress("dP2", &dP2);
  InputChain->SetBranchAddress("Pge2", &Pge2);
  InputChain->SetBranchAddress("dPge2", &dPge2);
  InputChain->SetBranchAddress("Pge3", &Pge3);
  InputChain->SetBranchAddress("dPge3", &dPge3);
  InputChain->SetBranchAddress("btag", &btag);
  // InputChain->SetBranchStatus("*",1);

  cout << "    P0       P1        P2        Pge2       Pge3" << endl;
  for(int i = 0; i<numEntries; i++){
    InputChain->GetEvent(i);
    
    if(P0<0 || P1<0 || P2<0 || Pge2<0 || Pge3<0 || P0>1 || P1>1 || P2>1 || Pge2>1 || Pge3>1){
      cout << P0 << " " << P1 << " " << P2  << " " << Pge2 << " " << Pge3 << endl;  
    }
      
      if(Pge3<10E-10) Pge3 = 0.0;
      

    //get weight
    int bin = Hweight->FindBin(pthat);
    double eventweight=Hweight->GetBinContent(bin);

    hP0->Fill(P0,eventweight);
    hP1->Fill(P1,eventweight);
    hP2->Fill(P2,eventweight);
    hPge2->Fill(Pge2,eventweight);
    hPge3->Fill(Pge3,eventweight);
    hPvT->Fill(btag,Pge2, eventweight);

    P2Tot += P2*eventweight;
    dP2Tott += dP2*dP2*eventweight*eventweight;
    Pge2Tot += Pge2*eventweight;
    dPge2Tott += dPge2*dPge2*eventweight*eventweight;
    Pge3Tot += Pge3*eventweight;
    dPge3Tott += dPge3*dPge3*eventweight*eventweight;

   }
  
  std::cout << "P2Tot: " << P2Tot << " +- " << sqrt(dP2Tott) << std::endl;
  std::cout << "Pge2Tot: " << Pge2Tot << " +- " << sqrt(dPge2Tott) << std::endl;
  std::cout << "Pge3Tot: " << Pge3Tot << " +- " << sqrt(dPge3Tott) << std::endl;  


  hP0->SetLineColor(2);
  hP1->SetLineColor(3);
  hP2->SetLineColor(4);
  hP0->SetLineWidth(2);
  hP1->SetLineWidth(2);
  hP2->SetLineWidth(2);
  TLegend* leg = new TLegend(0.82, 0.70, 0.89, 0.85);
  leg->AddEntry(hP0, "P0", "l");
  leg->AddEntry(hP1, "P1", "l");
  leg->AddEntry(hP2, "P2", "l");
  leg->SetFillColor(0);
  leg->SetLineColor(0);
  C_P->cd();
  hP2->Draw(); 
  leg->Draw();
  hP1->Draw("SAME");
  hP0->Draw("SAME"); 
  
  C_2D->cd();
  hPvT->GetYaxis()->SetTitle("Probability of 2 or more tags");
  hPvT->GetXaxis()->SetTitle("Number of tags");
  hPvT->GetYaxis()->SetTitleOffset(1.15);
  hPvT->GetYaxis()->SetRangeUser(0,.2);
  hPvT->Draw("COLZ");


  C_P->Write();
  C_2D->Write();
  fout.Write();
}
