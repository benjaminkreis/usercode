#include "TCanvas.h"
#include "TAxis.h"
#include "TLine.h"
#include "TFile.h"
#include "TLegend.h"
#include "TBox.h"
#include "TH1D.h"
#include "TROOT.h"
#include "TStyle.h"
#include <vector>
#include <iostream>
#include <fstream>

void doPlotMultiCont(TString noCName = "multi_calo_nob", TString yesCName = "multi_calo_nob_contB"){
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptStat("");
  
  TFile *noCont=TFile::Open("multiResults/"+noCName+".root","READ");
  TH1D* hnC=(TH1D*)noCont->Get("hrE");
  hnC->SetName("noC");

  TFile *yesCont=TFile::Open("multiResults/"+yesCName+".root","READ");
  TH1D* hC=(TH1D*)yesCont->Get("hrE");
  hC->SetName("yesC");


  hnC->SetLineColor(kRed);
  
  TLegend *leg = new TLegend(.65,.75,.85,.85);
  leg->AddEntry(hnC, "QCD", "l");
  leg->AddEntry(hC, "QCD+cont.", "l");
  leg->SetFillColor(0);
  leg->SetLineColor(0);
  leg->SetTextSize(0.04);

  TCanvas *myC = new TCanvas("myC", "myC", 640, 480);
  myC->cd();

  hC->Draw();
  hnC->Draw("SAME");
  leg->Draw();
  myC->Print("multiResults/"+yesCName+".pdf");

}

void plotMultiCont(){
  
  doPlotMultiCont( "multi_calo_nob", "multi_calo_nob_contB");
  doPlotMultiCont( "multi_calo_nob", "multi_calo_nob_contBS");
  doPlotMultiCont( "multi_calo", "multi_calo_contB");
  doPlotMultiCont( "multi_calo", "multi_calo_contBS");
  doPlotMultiCont( "multi_pfpf_nob", "multi_pfpf_nob_contB");
  doPlotMultiCont( "multi_pfpf_nob", "multi_pfpf_nob_contBS");
  doPlotMultiCont( "multi_pfpf", "multi_pfpf_contB");
  doPlotMultiCont( "multi_pfpf", "multi_pfpf_contBS");
    
}
