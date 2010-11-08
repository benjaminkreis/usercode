#include "TGraphErrors.h"
#include "TVectorD.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TLine.h"
#include "TLegend.h"
#include "TBox.h"
#include "TH1D.h"
#include "TROOT.h"
#include "TStyle.h"
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

void doMulti(TString type){
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptStat("");
  
  //  TString type = "calo_nob";
  bool expc;

  double trueN;
  double trueN_err;
  
  double range;

  if(type == "pfpf"){
    trueN = 0.269125;
    trueN_err = 0.107335;
    range =0.5;
    expc = true;
  }
  else if( type == "pf"){
    trueN = 0.284114;
    trueN_err = 0.107509;
    range = .5;
    expc = true;
  }
  else if( type == "tc"){
    trueN = 0.463308;
    trueN_err = 0.151177;
    range = 1.;
    expc = true;
  }
  else if(type == "calo"){
    trueN = 0.604643;
    trueN_err = 0.0657204;
    range = 2.;
    expc = true;
  }
  else if(type == "pfpf_nob"){
    trueN = 12.1611;
    trueN_err = 3.0787;
    range = 20;
    expc = true;
  }
  else if(type == "pf_nob"){
    trueN = 9.95917;
    trueN_err = 2.53115;
    range = 20;
    expc = true;
  }
  else if(type == "tc_nob"){
    trueN = 7.02955;
    trueN_err = 0.585675;
    range = 13;
    expc = true;
  }
  else if(type == "calo_nob"){
    trueN = 15.8387;
    trueN_err = .869234;
    range = 45;
    expc = true;
  }

  ifstream inFile("multiResults/multi_"+type+".dat", std::ios::in);
  if(!inFile.good()){
    cout << "Could not open inFile!" << endl;
    return;
  }
  
  TCanvas *myC = new TCanvas("myC", "myC", 640, 480);
  myC->cd();

  int size = 8;
  TH1D *hrE = new TH1D("hrE", "hrE", size, -0.5, size-0.5);
  TH1D *hrEb = new TH1D("hrEb", "hrEb", size, -0.5, size-0.5);

  double numE, numE_err;
  double numEb, numEb_err;
  string fill1, fill2, fill3;

  int i =0;
  while(inFile>>numE>>fill1>>numE_err>>fill2>>numEb>>fill3>>numEb_err){
    //  cout << numE << " " <<numE_err<< " " <<numEb<< " " <<numEb_err<<endl;
    hrE->Fill(i,numE);
    hrE->SetBinError(i+1, numE_err);
    hrEb->Fill(i,numEb);
    hrEb->SetBinError(i+1, numEb_err);
    i++;
  }
  
  
  TLine *line0 = new TLine(-0.5, trueN, size-0.5, trueN);
  TLine *lineA = new TLine(-0.5, trueN-trueN_err, size-0.5, trueN-trueN_err);
  TLine *lineB = new TLine(-0.5, trueN+trueN_err, size-0.5, trueN+trueN_err);
  TBox *box = new TBox();
  line0->SetLineWidth(3);
  line0->SetLineColor(kGray);
  lineA->SetLineColor(kGray);
  lineB->SetLineColor(kGray);
  

  TAxis *yA = hrE->GetYaxis();
  TAxis *xA = hrE->GetXaxis();
  yA->SetRangeUser(0,range);
  xA->SetRangeUser(-2.0,size+1);
  hrE->SetLineWidth(2);
  hrE->SetLineColor(kBlue);
  hrEb->SetLineWidth(2);
  hrEb->SetLineColor(kRed);
  
  hrE->SetTitle(type);
  yA->SetTitle("Number of QCD Events per 50/pb");
  yA->SetTitleOffset(1.2);
  xA->SetBinLabel(1,"30-150,8");
  xA->SetBinLabel(2,"50-150,8");
  xA->SetBinLabel(3,"70-150,8");
  xA->SetBinLabel(4,"90-150,8");
  xA->SetBinLabel(5,"50-140,8");
  xA->SetBinLabel(6,"50-130,8");
  xA->SetBinLabel(7,"50-150,5");
  xA->SetBinLabel(8,"50-150,12");
  xA->SetTitle("Fit range min-max, Number of bins");

  TLegend *leg = new TLegend(.65,.75,.85,.85);
  leg->AddEntry(hrE, "a*exp(b*x)", "l");
  leg->AddEntry(hrEb, "a+exp(b*x)+c", "l");
  leg->SetFillColor(0);
  leg->SetLineColor(0);
  leg->SetTextSize(0.04);

  hrE->Draw();
  if(expc) hrEb->Draw("SAME");
  
  leg->Draw();
  line0->Draw();
  lineA->Draw();
  lineB->Draw();
  myC->Print("multiResults/multi"+type+".pdf");
  hrE->Clear();
  hrEb->Clear();

}

void multi(){

  doMulti("pfpf_nob");
  doMulti("pfpf");
  doMulti("pf_nob");
  doMulti("pf");
  doMulti("tc_nob");
  doMulti("tc");
  doMulti("calo_nob");
  doMulti("calo");

}
