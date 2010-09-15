#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TString.h"
#include "TLegend.h"

#include <iostream>

using namespace std;

void plotOne(TString inName){
 
  TFile fin("tagjet.root","READ");
  TCanvas* myC = new TCanvas("myC", "myC", 640, 480); 
  myC->cd();
  
  TH1D* hIn = 0;
  
  if(!fin.IsZombie()){
    hIn = (TH1D*)fin.Get(inName);
  } 
  else{
    cout << "ERROR loading file" << endl;
  }
  
  hIn->GetYaxis()->SetTitle("nTags/nJets");
  hIn->Draw();
  myC->Print(inName+".pdf");
}

void plotMistag(TString inName){

  TFile fin("tagjet_mistag.root","READ");
  TCanvas* myC = new TCanvas("myC", "myC", 640, 480); 
  myC->cd();

  TH1D* hIn0 = 0;
  TH1D* hIn1 = 0;
  TH1D* hIn2 = 0;
  TH1D* hIn3 = 0;
  
  if(!fin.IsZombie()){
    hIn0 = (TH1D*)fin.Get(inName+"0_pfx");
    hIn1 = (TH1D*)fin.Get(inName+"1_pfx");
    hIn2 = (TH1D*)fin.Get(inName+"2_pfx");
    hIn3 = (TH1D*)fin.Get(inName+"3_pfx");
  } 
  else{
    cout << "ERROR loading file" << endl;
  }
 
  hIn0->SetLineColor(kGreen+1);
  hIn1->SetLineColor(kCyan+1);
  hIn2->SetLineColor(kBlue+1);
  hIn3->SetLineColor(kMagenta+3);
  hIn0->SetLineWidth(2);
  hIn1->SetLineWidth(2);
  hIn2->SetLineWidth(2);
  hIn3->SetLineWidth(2);
 
  TLegend *leg = new TLegend(0.55,0.7, 0.75, 0.8);
  leg->AddEntry(hIn0, "tagged b-jets", "l");
  leg->AddEntry(hIn1, "mistagged non b-jets", "l");
  leg->AddEntry(hIn2, "untagged b-jets", "l");
  // leg->AddEntry(&hIn3, "untagged non b-jets", "l");
  leg->SetLineColor(0);
  leg->SetFillColor(0);
  leg->SetTextSize(0.03);

  hIn1->GetYaxis()->SetTitle("n/nJets");
  // hIn1->GetXaxis()->SetRangeUser(0, 300);
  hIn1->Draw();
  hIn0->Draw("SAME");
  hIn2->Draw("SAME");
  //  hIn3->Draw("SAME");
  leg->Draw();

  myC->Print(inName+"_mistag.pdf");

}


void plot(){
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptStat("");
  /*
    plotOne("h2D_MET_pfx");
    plotOne("h2D_MHT_pfx");
    plotOne("h2D_METproj_pfx");
    plotOne("h2D_MHTproj_pfx");
    plotOne("h2D_eta_pfx");
    plotOne("h2D_pt_pfx");
    plotOne("h2D_nTracks_pfx");
    plotOne("h2D_nLooseJets_pfx");
  */

  plotMistag("h2D_MET");
  plotMistag("h2D_MHT");
  plotMistag("h2D_METproj");
  plotMistag("h2D_MHTproj");
  plotMistag("h2D_eta");
  plotMistag("h2D_pt");
  plotMistag("h2D_nTracks");
  plotMistag("h2D_nLooseJets");
 
}
