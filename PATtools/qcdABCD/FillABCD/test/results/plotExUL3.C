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

void doExtrapPlot(TString noCName = "multi_calo_nob", TString yesCName = "multi_calo_nob_contB", TString Cont3Name="plots"){
  // gROOT->SetStyle("Plain");
  // gStyle->SetPalette(1);
  // gStyle->SetOptStat("");
  gROOT->SetStyle("CMS");

  TFile *noCont=TFile::Open(noCName+".root","READ");
  TH1D* grNC=(TH1D*)noCont->Get("histUL");
  grNC->SetName("noC");
  grNC->UseCurrentStyle();
  
  TFile *yesCont=TFile::Open(yesCName+".root","READ");
  TH1D* grC=(TH1D*)yesCont->Get("histUL");
  grC->SetName("yesC");
  grC->UseCurrentStyle();

  TFile *Cont3=TFile::Open(Cont3Name+".root","READ");
  TH1D* grC3=(TH1D*)Cont3->Get("histUL");
  grC3->SetName("Cont3");
  grC3->UseCurrentStyle();

  grNC->SetMarkerColor(kRed);
  grNC->SetLineColor(kRed);
  grNC->SetMarkerStyle(20);
  grNC->SetLineWidth(2);

  grC->SetMarkerColor(kBlack);
  grC->SetLineColor(kBlack);
  grC->SetMarkerStyle(21);
  grC->SetLineWidth(2);
  
  grC3->SetMarkerColor(kBlue);
  grC3->SetLineColor(kBlue);
  grC3->SetMarkerStyle(22);
  grC3->SetLineWidth(2);


  //  TCanvas *myC = new TCanvas("myC", "myC", 640, 480);                                                                                                   
  TCanvas *myC = new TCanvas("myC", "myC");
  myC->cd();
  
  TLegend *leg = new TLegend(.19,.15,.3,.3);
  leg->AddEntry(grNC, "QCD MC", "P");
  leg->AddEntry(grC3, "QCD+SM MC", "P");
  leg->AddEntry(grC, "QCD+SM+LM13 MC", "P");
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

  //grNC->GetYaxis()->SetRangeUser(0.005, 1.6);
  grNC->GetYaxis()->SetTitle("f(E_{T}^{miss})");
  grNC->GetXaxis()->SetTitle("E_{T}^{miss} [GeV]");
  // grNC->GetXaxis()->SetRangeUser(0,200);
  grNC->Draw();
 
  grC3->Draw("SAME");
   grC->Draw("SAME");
 //text1->Draw();
  text2->Draw();
  gPad->SetRightMargin(0.05);
  myC->SetLogy(1);
  myC->Modified();

  //  grC->Draw("AP");
  // grNC->Draw("SAME P");
  leg->Draw();
  
  myC->Print(yesCName+".pdf");

}

void plotExUL3(){
  
  // doExtrapPlot( "plots_pfpf_nC", "plots_pfpf_yC");
  // doExtrapPlot( "plots_pfpf_nC", "plots_pfpf_yCB"); 
  // doExtrapPlot( "plots_calo_nC", "plots_calo_yC");
  // doExtrapPlot( "plots_calo_nC", "plots_calo_yCB");
  
  doExtrapPlot("plots_QCD", "plots_QCDBS","plots_QCDB");
  //doExtrapPlot("plots_QCD", "plots_QCDb");
  //doExtrapPlot("plots_data22", "plots_MC22");
  //doExtrapPlot("plots_UHH", "plots_J");
}
