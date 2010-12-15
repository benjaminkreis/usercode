#include "TCanvas.h"
#include "TAxis.h"
#include "TLine.h"
#include "TFile.h"
#include "TLegend.h"
#include "TBox.h"
#include "TF1.h"
#include "TH1D.h"
#include "TH1D.h"
#include "TMultiGraph.h"
#include "TROOT.h"
#include "TStyle.h"
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

void plotFits(){
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptStat("");
  
  TCanvas *myC = new TCanvas("myC", "myC", 640,480);

  TFile *fileQCD=TFile::Open("plotsQCD.root","READ");
  TH1D* hQCD=(TH1D*)fileQCD->Get("histUL");
  TF1* f1=(TF1*)fileQCD->Get("fexp2b"); 
  hQCD->SetName("hQCD");
  hQCD->SetTitle("");
  hQCD->SetLineColor(kRed);
  hQCD->SetLineWidth(2);
  f1->SetName("f1");
  myC->cd();
  hQCD->Draw();

  TFile *file1=TFile::Open("plots0.root","READ");
  TH1D* h1=(TH1D*)file1->Get("histUL");
  h1->SetLineWidth(2);
  h1->SetLineColor(kBlack);
  h1->Draw("SAME");

  

  const int totalN=16;
  TF1* fA[totalN];
  TFile* fF[totalN];

  for(int i=0; i<totalN; i++){
    TString n = "";
    n+=i;
    fF[i]=TFile::Open("plots"+n+".root","READ");
    fA[i]=(TF1*)fF[i]->Get("fexp2b");    
    fA[i]->SetName("f"+n);
    fA[i]->SetLineColor(kGray);
    //fA[i]->SetLineWidth(1);
    fA[i]->Draw("SAME");
  }

 
  //  f1->Draw("SAME");

  hQCD->GetYaxis()->SetRangeUser(1.e-3,30);
  hQCD->GetYaxis()->SetTitle("minDPhi ratio");
  hQCD->GetXaxis()->SetTitle("MET [GeV]");
  myC->SetLogy(1);
  myC->Modified();
  myC->Print("extrap16.pdf");
  

}
