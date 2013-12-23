#include "TFile.h"
#include "TChain.h"
#include "TRandom.h"
#include "TH1.h"
#include "TH2F.h"
#include "TF1.h"
#include "TFormula.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TAxis.h"
#include "TMatrixT.h"
#include "TVectorT.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TROOT.h"
#include "TFitter.h"
#include <iostream>
#include <vector>
#include <exception>
#include <cmath> 
#include <iomanip>
#include <assert.h>

using namespace std;

void analyzeTagRatio(){
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptStat("euo");
  
  float pi = 4*atan(1.0);

  TString treename = "T_tagRatio";
  TString filename = "plots_QCD_Pt170";
  TString xaxis = "MET";
  TString yaxisT = "ngoodbjets";

  TFile* file1 = TFile::Open("results/"+filename+".root","READ");
  TTree* tree1 = (TTree*) gDirectory->Get(treename);
  
  int numEntries = tree1->GetEntries();
  cout << "numEntries: " << numEntries << endl;

  //Histogram
  TH2F* hist1 = new TH2F(yaxisT+"_"+xaxis, filename, 50, 0, 500, 100, 0, 10); //met
  //TH2F* hist1 = new TH2F("hist1", filename, 50, 0, 1500, 100, 0, 1); //jetpt1
  //TH2F* hist1 = new TH2F("hist1", filename, 50, -pi, pi, 100, 0, 1); //jeteta1
  
  hist1->Sumw2();
    
  float x = -1;
  float njetsH = -1;
  float yT = -1;
  tree1->SetBranchAddress(xaxis,&x); 
  tree1->SetBranchAddress("njets",&njetsH);
  tree1->SetBranchAddress(yaxisT,&yT);
 
  /////LOOP OVER EVENTS/////
  for(int i = 0; i< numEntries; i++){ //loop over tree
    
    tree1->GetEvent(i+1);
    float y = yT/njetsH;
    hist1->Fill(x,y);
   
  }

  hist1->Draw("COLZ");
  
  //PROFILE HISTOGRAM
  TProfile *tpf = hist1->ProfileX();
  tpf->SetLineWidth(3);
  TAxis* xax = tpf->GetXaxis();
  TAxis* yax = tpf->GetYaxis();
  xax->SetTitle(xaxis);
  yax->SetTitle(yaxisT+"/nJets");
  yax->SetTitleOffset(1.15);

  TCanvas* C_profile = new TCanvas("C_profile", "Profile Canvas",800,600);

  tpf->Draw();

  C_profile->Print(filename+"_"+xaxis+".pdf");

  TFile f("results/"+filename+"_out.root","UPDATE");
  tpf->Write();
}
