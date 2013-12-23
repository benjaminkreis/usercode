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

void doExtrapPlot(TString name1 = "multi_calo_nob"){
  gROOT->SetStyle("CMS");
  

  TFile *file1=TFile::Open(name1+".root","READ");
  TH1D* h1=(TH1D*)file1->Get("histUL");
  TH1D* h2=(TH1D*)file1->Get("histULSM");
  h1->SetName("h1");
  h1->UseCurrentStyle();
  h2->SetName("h2");
  h2->UseCurrentStyle();


  h1->SetMarkerColor(kBlack);
  h1->SetLineColor(kBlack);
  h1->SetMarkerStyle(20);
  h1->SetLineWidth(2);
  h2->SetMarkerColor(kBlue);
  h2->SetLineColor(kBlue);
  h2->SetMarkerStyle(22);
  h2->SetLineWidth(2);


  //  TCanvas *myC = new TCanvas("myC", "myC", 640, 480);                                                                                                   
  TCanvas *myC = new TCanvas("myC", "myC");
  myC->cd();
  
  TLegend *leg = new TLegend(.19,.15,.3,.3);
  leg->AddEntry(h1, "Data", "P");
  leg->AddEntry(h2, "Data - non-QCD SM MC", "P");
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

  h1->GetYaxis()->SetRangeUser(0.01, 40);
  h1->GetYaxis()->SetTitle("r(E_{T}^{miss})");
  h1->GetXaxis()->SetTitle("E_{T}^{miss} [GeV]");
  h1->GetXaxis()->SetRangeUser(0,85);
  h1->Draw();
  h2->Draw("SAME");

  //text1->Draw();
  text2->Draw();
  gPad->SetRightMargin(0.05);
  myC->SetLogy(1);
  myC->Modified();

  //  grC->Draw("AP");
  // h1->Draw("SAME P");
  leg->Draw();
  
  myC->Print(name1+"_SM.pdf");

}

void plotExUL1SM(){
  
  
 doExtrapPlot("plotsDATAe1");
 doExtrapPlot("plotsDATAge1");
 doExtrapPlot("plotsDATAge2");

}
