#include "TLatex.h"
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

void doExtrapPlot(TString name1 = "multi_calo_nob", TString name2 = "multi_calo_nob_contB", TString name3="plots"){
  // gROOT->SetStyle("Plain");
  // gStyle->SetPalette(1);
  // gStyle->SetOptStat("");
  gROOT->SetStyle("CMS");

  TFile *noCont=TFile::Open(name1+".root","READ");
  TH1D* gr1=(TH1D*)noCont->Get("histUL");
  gr1->SetName("noC");
  gr1->UseCurrentStyle();
  
  TFile *yesCont=TFile::Open(name2+".root","READ");
  TH1D* gr2=(TH1D*)yesCont->Get("histUL");
  gr2->SetName("yesC");
  gr2->UseCurrentStyle();

  TFile *Cont3=TFile::Open(name3+".root","READ");
  TH1D* gr3=(TH1D*)Cont3->Get("histUL");
  gr3->SetName("Cont3");
  gr3->UseCurrentStyle();

  gr1->SetMarkerColor(kRed);
  gr1->SetLineColor(kRed);
  gr1->SetMarkerStyle(20);
  gr1->SetLineWidth(2);

  gr2->SetMarkerColor(kBlack);
  gr2->SetLineColor(kBlack);
  gr2->SetMarkerStyle(21);
  gr2->SetLineWidth(2);
  
  gr3->SetMarkerColor(kBlue);
  gr3->SetLineColor(kBlue);
  gr3->SetMarkerStyle(22);
  gr3->SetLineWidth(2);


  //  TCanvas *myC = new TCanvas("myC", "myC", 640, 480);                                                                                                   
  TCanvas *myC = new TCanvas("myC", "myC");
  myC->cd();
  
  TLegend *leg = new TLegend(.19,.15,.3,.3);
  leg->AddEntry(gr1, "MadGraph QCD MC", "P");
  leg->AddEntry(gr2, "PYTHIA Pileup QCD MC", "P");
  // leg->AddEntry(gr3, "All SM + LM13 MC", "P");
  //leg->AddEntry(gr1, "minDeltaPhiMET30_eta5", "P");
  //leg->AddEntry(gr2, "minDeltaPhiMET30_eta5, idVeto", "P");
  //leg->AddEntry(gr3, "3", "P"); 
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetLineStyle(0);
  leg->SetTextFont(42);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.04);

  TLatex* text1 = new TLatex(3.570061,23.08044,"CMS Preliminary");
  text1->SetNDC();
  text1->SetTextAlign(13);
  text1->SetX(0.184);
  text1->SetY(0.928);
  //text1->SetLineWidth(2);                                                        
  text1->SetTextFont(42);
  text1->SetTextSizePixels(24);// dflt=28 
  
  TLatex* text2 = new TLatex(3.570061,23.08044,"36.1 pb^{-1} at #sqrt{s} = 7 TeV");
  text2->SetNDC();
  text2->SetTextAlign(13);
  text2->SetX(0.184);
  text2->SetY(0.88);
  //text2->SetLineWidth(2);                                                                                                    
  text2->SetTextFont(42);
  text2->SetTextSizePixels(24);// dflt=28 

  gr1->GetYaxis()->SetRangeUser(0.001, 40);
  gr1->GetYaxis()->SetTitle("r(E_{T}^{miss})");
  gr1->GetXaxis()->SetTitle("E_{T}^{miss} [GeV]");
  //gr1->GetXaxis()->SetRangeUser(0,450);
  gr1->Draw();
 

  gr2->Draw("SAME");
  //gr3->Draw("SAME");
  //text1->Draw();
  //text2->Draw();
  gPad->SetRightMargin(0.05);
  myC->SetLogy(1);
  myC->Modified();
  leg->Draw();
  
  myC->Print(name1+".pdf");

}

void plotExUL3(){
  
  // doExtrapPlot( "plots_pfpf_nC", "plots_pfpf_yC");
  // doExtrapPlot( "plots_pfpf_nC", "plots_pfpf_yCB"); 
  // doExtrapPlot( "plots_calo_nC", "plots_calo_yC");
  // doExtrapPlot( "plots_calo_nC", "plots_calo_yCB");
  
  //doExtrapPlot("plots_QCD", "plots_QCDBS","plots_QCDB");
  //doExtrapPlot("plots_QCD", "plots_QCDb");
  //doExtrapPlot("plots_data22", "plots_MC22");
  //doExtrapPlot("plots_UHH", "plots_J");

  //  doExtrapPlot("plots3", "plotsAll", "plots30All");
  // doExtrapPlot("plots0", "plots5", "plots11");
  //doExtrapPlot("plots_jan24_30_eta5All", "plots_jan24_30_eta5All_idVeto", "plots0");

  //note jan30 
  //doExtrapPlot("plotsQCDpre", "plotsQCDSMpre", "plotsQCDSMLM13pre");
  //  doExtrapPlot("plotsQCDe1", "plotsQCDSMe1", "plotsQCDSMLM13e1");
  //doExtrapPlot("plotsQCDge1", "plotsQCDSMge1", "plotsQCDSMLM13ge1");
  //doExtrapPlot("plotsQCDge2", "plotsQCDSMge2", "plotsQCDSMLM13ge2");

  doExtrapPlot("plotsMG", "plotsPUfull", "plots0");

}
