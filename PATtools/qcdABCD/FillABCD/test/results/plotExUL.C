#include "TCanvas.h"
#include "TAxis.h"
#include "TLine.h"
#include "TFile.h"
#include "TLegend.h"
#include "TBox.h"
#include "TH1D.h"
#include "TH1D.h"
#include "TMultiGraph.h"
#include "TROOT.h"
#include "TStyle.h"
#include <vector>
#include <iostream>
#include <fstream>

void doExtrapPlot(TString noCName = "multi_calo_nob", TString yesCName = "multi_calo_nob_contB"){
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptStat("");
  
  TFile *noCont=TFile::Open(noCName+".root","READ");
  TH1D* grNC=(TH1D*)noCont->Get("histUL");
  grNC->SetName("noC");
  
  TFile *yesCont=TFile::Open(yesCName+".root","READ");
  TH1D* grC=(TH1D*)yesCont->Get("histUL");
  grC->SetName("yesC");

  grNC->SetLineColor(kRed);
  grNC->SetMarkerColor(kRed);
  
  TLegend *leg = new TLegend(.2,.75,.3,.85);
  leg->AddEntry(grNC, "B", "l");
  leg->AddEntry(grC, "BS", "l");
  leg->SetFillColor(0);
  leg->SetLineColor(0);
  leg->SetTextSize(0.04);

  TCanvas *myC = new TCanvas("myC", "myC", 640, 480);
  myC->cd();


  
  //grNC->GetYaxis()->SetRangeUser(0.005, 1.6);
  grNC->GetYaxis()->SetTitle("minDPhi ratio");
  grNC->GetXaxis()->SetTitle("MET [GeV]");
  // grNC->GetXaxis()->SetRangeUser(0,200);
  grNC->Draw();
  grC->Draw("SAME");
  myC->SetLogy(1);
  myC->Modified();

  //  grC->Draw("AP");
  // grNC->Draw("SAME P");
  leg->Draw();
  
  myC->Print(yesCName+".pdf");

}

void plotExUL(){
  
  // doExtrapPlot( "plots_pfpf_nC", "plots_pfpf_yC");
  // doExtrapPlot( "plots_pfpf_nC", "plots_pfpf_yCB"); 
  // doExtrapPlot( "plots_calo_nC", "plots_calo_yC");
  // doExtrapPlot( "plots_calo_nC", "plots_calo_yCB");
  
  //  doExtrapPlot("plots_QCD", "plots_QCDcontBS");
  //doExtrapPlot("plots_QCDcontB", "plots_QCDcontBS");
  doExtrapPlot("plots_B", "plots_BS");
  //doExtrapPlot("plots_data22", "plots_MC22");
  //doExtrapPlot("plots_UHH", "plots_J");
}
