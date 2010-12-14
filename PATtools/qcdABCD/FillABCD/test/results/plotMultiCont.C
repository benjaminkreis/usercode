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

void doPlotMultiCont(TString noCName = "multi_calo_nob", TString yesCName = "multi_calo_nob_contB", TString type = "calo_nob"){
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptStat("");
  
  TFile *noCont=TFile::Open("multiResults/"+noCName+".root","READ");
  TH1D* hnC=(TH1D*)noCont->Get("hrE");
  hnC->SetName("noC");

  TFile *yesCont=TFile::Open("multiResults/"+yesCName+".root","READ");
  TH1D* hC=(TH1D*)yesCont->Get("hrE");
  hC->SetName("yesC");


  double trueN;
 double trueN_err;
  

  if(type == "pfpf"){
    trueN = 0.269125;
    trueN_err = 0.107335;
  }
  else if( type == "pf"){
    trueN = 0.284114;
    trueN_err = 0.107509;
  }
  else if( type == "tc"){
    trueN = 0.463308;
    trueN_err = 0.151177;
  }
  else if(type == "calo"){
    trueN = 0.604643;
    trueN_err = 0.0657204;
  }
  else if(type == "pfpf_nob"){
    trueN = 12.1611;
    trueN_err = 3.0787;
  }
  else if(type == "pf_nob"){
    trueN = 9.95917;
    trueN_err = 2.53115;
  }
  else if(type == "tc_nob"){
    trueN = 7.02955;
    trueN_err = 0.585675;
  }
  else if(type == "calo_nob"){
    trueN = 15.8387;
    trueN_err = .869234;
  }
  
  int size = hnC->GetNbinsX();
  cout << "size: " << size << endl;
  TLine *line0 = new TLine(-0.5, trueN, size-0.5, trueN);
  TLine *lineA = new TLine(-0.5, trueN-trueN_err, size-0.5, trueN-trueN_err);
  TLine *lineB = new TLine(-0.5, trueN+trueN_err, size-0.5, trueN+trueN_err);
  TBox *box = new TBox();
  line0->SetLineWidth(3);
  line0->SetLineColor(kGray);
  lineA->SetLineColor(kGray);
  lineB->SetLineColor(kGray);
  
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
  line0->Draw();
  lineA->Draw();
  lineB->Draw();

  myC->Print("multiResults/"+yesCName+".pdf");

}

void plotMultiCont(){
  
  doPlotMultiCont( "multi_calo_nob", "multi_calo_nob_contB", "calo_nob");
  doPlotMultiCont( "multi_calo_nob", "multi_calo_nob_contBS", "calo_nob");
  doPlotMultiCont( "multi_calo", "multi_calo_contB", "calo");
  doPlotMultiCont( "multi_calo", "multi_calo_contBS", "calo");
  doPlotMultiCont( "multi_pfpf_nob", "multi_pfpf_nob_contB", "pfpf_nob");
  doPlotMultiCont( "multi_pfpf_nob", "multi_pfpf_nob_contBS", "pfpf_nob");
  doPlotMultiCont( "multi_pfpf", "multi_pfpf_contB", "pfpf");
  doPlotMultiCont( "multi_pfpf", "multi_pfpf_contBS", "pfpf");
    
}
