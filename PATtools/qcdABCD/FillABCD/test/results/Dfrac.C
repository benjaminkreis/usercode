#include "TFile.h"
#include "TChain.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLine.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TROOT.h"
#include <iostream>
#include <cmath> 
#include <iomanip>


//#include "analyzeFillABCDInput.h"

using namespace std;

double *Dfrac(TString joshType = "calo", int nbcut = 2){
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  //  gStyle->SetOptStat("");

   bool josh = true;

  //LOAD WEIGHTS
  TFile finweight("/afs/cern.ch/user/k/kreis/scratch0/CMSSW_3_6_3/src/Filter/RA2Filter/test/results/weight_MG.root","READ");

  TH1D* HweightP = 0;
  if(!finweight.IsZombie()){
    HweightP = (TH1D*)finweight.Get("Hweight");
  }
  else{
    cout << "ERROR loading weights" << endl;
  }
  TFile fout("Dfrac_plots.root", "RECREATE");
  TH1D Hweight = *HweightP;
  finweight.Close();
 

  TChain* InputChain = 0;// FormChain();
  if(josh){
    InputChain = FormChainJosh(joshType);
  }
  else{
    InputChain = FormChain();
  }
  int numEntries = InputChain->GetEntries();
  cout <<"numEntries: " << numEntries << endl;
  cout << endl;
  
  double pi=4*atan(1.0);
  //const int nbin_wide = 12;
  //  Double_t wideBinArray[nbin_wide+1]={0., 0.3, 0.6, 0.9, 1.2, 2., pi};
  // Double_t wideBinArray[nbin_wide+1]={0., 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.7, 0.9, 1.2, 2., pi};
  const int nbin_wide = 5;
  Double_t wideBinArray[nbin_wide+1]={0., 0.3, 0.6, 0.9, 1.5, pi};
  
  //TH2D* h2D = new TH2D("h2D_minDPhi", "h2D_minDPhi", nbin_wide, 0.0, pi, 2, -.5, 1.5);
  TH2D* h2D = new TH2D("h2D_minDPhi", "h2D_minDPhi", nbin_wide, wideBinArray, 2, -.5, 1.5);
  TH1D* hDpass = new TH1D("hDpass", "hDpass", 1, 0.0, .3);
  TH1D* hDall = new TH1D("hDall", "hDall", 1, 0.0, .3);
  TH1D* hDeff = new TH1D("hDeff", "hDeff", 1, 0.0, .3);
  TH1D* hCpass = new TH1D("hCpass", "hCpass", 1, 0.3, pi);
  TH1D* hCall = new TH1D("hCall", "hCall", 1, 0.3, pi);
  TH1D* hCeff = new TH1D("hCeff", "hCeff", 1, 0.3, pi);
  // TH1D* h1pass_wide = new TH1D("h1pass_wide", "h1pass_wide", nbin_wide, 0.0, pi);
  // TH1D* h1all_wide = new TH1D("h1all_wide", "h1all_wide", nbin_wide, 0.0, pi);
  // TH1D* h1eff_wide = new TH1D("h1eff_wide", "h1eff_wide", nbin_wide, 0.0, pi);
  TH1D* h1pass_wide = new TH1D("h1pass_wide", "h1pass_wide", nbin_wide, wideBinArray);
  TH1D* h1all_wide = new TH1D("h1all_wide", "h1all_wide", nbin_wide, wideBinArray);
  TH1D* h1eff_wide = new TH1D("h1eff_wide", "h1eff_wide", nbin_wide,wideBinArray);
  
  
  h2D->Sumw2();
  hCpass->Sumw2();
  hCall->Sumw2();
  hCeff->Sumw2();
  hDpass->Sumw2();
  hDall->Sumw2();
  hDeff->Sumw2();
  h1pass_wide->Sumw2();
  h1all_wide->Sumw2();
  h1eff_wide->Sumw2();
  
  double x, y, MG=0., weightJosh=0.;
  int nbtags, nbjets;
  TString xTitle = "MET";
  // InputChain->SetBranchAddress("minDPhi",&y);
  // InputChain->SetBranchAddress("MHT",&x);
  // InputChain->SetBranchAddress("MG",&MG);
  // InputChain->SetBranchAddress("btag", &nbtags);
  // InputChain->SetBranchAddress("bjet", &nbjets);
  if(josh){
    InputChain->SetBranchAddress("MET",&x);
    InputChain->SetBranchAddress("minDeltaPhiMET",&y);
    InputChain->SetBranchAddress("weight",&weightJosh);
    InputChain->SetBranchAddress("nbSSVM",&nbtags);
  }
  else{
    InputChain->SetBranchAddress("MHT",&x);
    InputChain->SetBranchAddress("minDPhi",&y);
    //InputChain->SetBranchAddress("PtHat", &pthat);
    InputChain->SetBranchAddress("MG",&MG);
    InputChain->SetBranchAddress("btag", &nbtags);
  }
  

  double nD=0, Dfrac_t=0;
  
  for(int i = 0; i<numEntries; i++){
    InputChain->GetEvent(i);
    
        
    //GET WEIGHT
    int bin = Hweight.FindBin(MG);
    double weight=Hweight.GetBinContent(bin);
    if(josh) weight=weightJosh;    

    int pass = 0;
    if(nbtags>=nbcut)pass =1;
    
    //if(true){
    if(x>=150.0){
      //if(weight>0.0001) cout << "pass: " << pass <<  ", weight: " << weight <<", minDPhi: " << y << endl;
      
      h2D->Fill(y,pass, weight);
      hCall->Fill(y, weight);
      hDall->Fill(y, weight);
      h1all_wide->Fill(y, weight);
      
      if(pass){
	hCpass->Fill(y, weight);
	hDpass->Fill(y, weight);
	h1pass_wide->Fill(y, weight);
      }
      
    }

    if(x>=150.0 && x < 1000.0 && y<.3){
      nD+=weight;
      Dfrac_t+=(double)pass*weight;
    }
  }
  //double Dfrac_man=Dfrac_t/nD;
  TProfile* p1_minDPhi = h2D->ProfileX(); 
  hCeff->Divide(hCpass, hCall, 1., 1., "B");
  hDeff->Divide(hDpass, hDall, 1., 1., "B");
  h1eff_wide->Divide(h1pass_wide, h1all_wide, 1., 1., "B");
  
  double Dfrac = hDeff->GetBinContent(1);
  double Dfrac_e = hDeff->GetBinError(1);
  double Cfrac = hCeff->GetBinContent(1);
  double Cfrac_e = hCeff->GetBinError(1);
  
  TCanvas* myC = new TCanvas("myC", "myC", 640, 480);
  myC->cd();
   
  h1eff_wide->GetXaxis()->SetTitle("MinDPhi("+xTitle+", jet123)");
  h1eff_wide->GetYaxis()->SetTitle("nPassBtagCut/nEvents");
  h1eff_wide->GetYaxis()->SetTitleOffset(1.2);
  h1eff_wide->SetTitle("b-tag cut rate after all cuts except angular cut");
  h1eff_wide->SetLineWidth(2);
  h1eff_wide->GetYaxis()->SetRangeUser(0,.5);
  h1eff_wide->Draw();
  TLine* line1 = new TLine(0, Dfrac, pi, Dfrac);
  line1->SetLineWidth(1);
  line1->SetLineColor(2);
  line1->Draw();

  cout << "nD: " << hDall->GetBinContent(1) << " +- " << hDall->GetBinError(1) << endl;
  cout << "Dfrac: " << Dfrac <<" +- " << Dfrac_e <<  endl;
  cout << "nC: " << hCall->GetBinContent(1) << " +- " << hCall->GetBinError(1) << endl;
  cout << "Cfrac: " << Cfrac <<" +- " << Cfrac_e <<  endl;

  double *result = new double[4];
  result[0] = Dfrac;
  result[1] = Dfrac_e;
  result[2] = Cfrac;
  result[3] = Cfrac_e;
  
  TString printName = "Dfrac_";
  printName+=nbcut;
  myC->Print(printName+".pdf");

  
  myC->Write();
  h2D->Write();
  hDpass->Write();
  hDall->Write();
  hDeff->Write();
  hCpass->Write();
  hCall->Write();
  hCeff->Write();
  h1pass_wide->Write();
  h1all_wide->Write();
  h1eff_wide->Write();
  p1_minDPhi->Write();
  fout.Close();
  
  return result;
}
