#include <iostream>
#include <vector>
#include <exception>
#include <cmath>
#include <iomanip>
#include <assert.h>

#include "TH1D.h"
#include "TH2D.h"
#include "TChain.h"

//not necessary for anything except plot
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TROOT.h"

#include "analyzeFillABCDInput.h"

using namespace std;

bool bcontinue(int nbtags, TString bcut){
  bool returnTrue = false; 
  if(bcut=="eq1"){
    if(nbtags==1) returnTrue = true;
  }
  else if(bcut=="ge1"){
    if(nbtags>=1) returnTrue = true;
  }
  else if(bcut=="ge2"){
    if(nbtags>=2) returnTrue = true;
  }
  else{assert(0);}
  return returnTrue;
}

double abError(double a, double aE, double b, double bE){
  double Err = 0;
  Err = sqrt( a*a*bE*bE + b*b*aE*aE);
  return Err;
}


double aObError(double a, double aE, double b, double bE){
  double Err = 0;
  Err = a/b*sqrt( aE*aE/(a*a)+bE*bE/(b*b) );
  return Err;
}


TString doRatioConst(TString bcut, double singleLow, double singleHigh){
  gROOT->SetStyle("CMS");

  //double singleLow=85;
  //double singleHigh=135;
  bool verbose = true;
  double borderv2a=150.0;
  double borderv2b=1000000000.;
  double borderh1a=0.0;
  double borderh1b=0.3;
  double borderh2a=0.3;
  double borderh2b=3.142;


  TH2D* histA = new TH2D("histA", "histA", 1, singleLow, singleHigh, 1, borderh1a, borderh1b);
  TH2D* histB = new TH2D("histB", "histB", 1, singleLow, singleHigh, 1, borderh2a, borderh2b);
  TH2D* histC = new TH2D("histC", "histC", 1, borderv2a, borderv2b, 1, borderh2a, borderh2b);
  TH2D* histD = new TH2D("histD", "histD", 1, borderv2a, borderv2b, 1, borderh1a, borderh1b);

  TH2D* histAB = new TH2D("histAB", "histAB", 1, singleLow, singleHigh, 1, borderh1a, borderh2b);
  TH2D* histCD = new TH2D("histCD", "histCD", 1, borderv2a, borderv2b, 1, borderh1a, borderh2b);

  TH2D* histMU = new TH2D("histMU", "histMU", 1, 50., 100., 1, borderh2a, borderh2b);
  TH2D* histML = new TH2D("histML", "histML", 1, 50., 100., 1, borderh1a, borderh1b);
  TH2D* histMUL = new TH2D("histMUL", "histMUL", 1, 50., 100., 1, borderh1a, borderh2b);

  histA->Sumw2();
  histB->Sumw2();
  histC->Sumw2();
  histD->Sumw2();
  histAB->Sumw2();
  histCD->Sumw2();
  histMU->Sumw2();
  histML->Sumw2();
  histMUL->Sumw2();
  
  TChain* InputChain = FormChainJosh("pfpf");
  const int numEntries = InputChain->GetEntries();
  if(verbose)cout <<"numEntries: " << numEntries << endl;

  double x, y, weight;
  int nbtags;
  InputChain->SetBranchAddress("MET",&x);
  InputChain->SetBranchAddress("minDeltaPhiMET",&y);
  InputChain->SetBranchAddress("weight",&weight);
  InputChain->SetBranchAddress("nbSSVM",&nbtags);
  
  TH1D* histmL = new TH1D("mediumMET","l",50,0., 3.142);
  TH1D* histmU = new TH1D("highMET","u",50,0., 3.142);
  histmL->Sumw2();
  histmU->Sumw2();
  histmL->SetLineWidth(2);
  histmU->SetLineWidth(2);
  histmL->SetLineColor(kBlue);
  histmU->SetLineColor(kRed);
  histmL->SetMarkerSize(0);
  histmU->SetMarkerSize(0);

  double singleLowPlot=50;
  double singleHighPlot=150;
  TString sLP = "100";
  TString sHP = "150";
  
  for(int i = 0; i<numEntries; i++){
    InputChain->GetEvent(i);
    
    if(!(bcontinue(nbtags, bcut))) continue;
 
    histB->Fill(x,y, weight);
    histA->Fill(x,y, weight);
    histC->Fill(x,y, weight);
    histD->Fill(x,y, weight);
    histAB->Fill(x,y, weight);
    histCD->Fill(x,y, weight);
    histML->Fill(x,y, weight);
    histMU->Fill(x,y, weight);
    histMUL->Fill(x,y, weight);

    if(x>singleLowPlot && x<singleHighPlot) histmL->Fill(y,weight);
    if(x>singleHighPlot) histmU->Fill(y,weight);
  }
  double single = histB->GetBinContent(1,1)/histA->GetBinContent(1,1);
  double single_err = aObError( histB->GetBinContent(1,1), histB->GetBinError(1,1), histA->GetBinContent(1,1), histA->GetBinError(1,1) );
  double nA=histA->GetBinContent(1,1);
  double nA_err = histA->GetBinError(1,1);
  double nB=histB->GetBinContent(1,1);
  double nB_err = histB->GetBinError(1,1);
  double nC=histC->GetBinContent(1,1);
  double nC_err = histC->GetBinError(1,1);
  double nD = histD->GetBinContent(1,1);
  double nD_err = histD->GetBinError(1,1);
  double nAB = histAB->GetBinContent(1,1);
  double nAB_err = histAB->GetBinError(1,1);
  double nCD = histCD->GetBinContent(1,1);
  double nCD_err = histCD->GetBinError(1,1);
  
  double nMU = histMU->GetBinContent(1,1);
  double nMU_err = histMU->GetBinError(1,1);
  double nML = histML->GetBinContent(1,1);
  double nML_err = histML->GetBinError(1,1);
  double nMUL = histMUL->GetBinContent(1,1);
  double nMUL_err = histMUL->GetBinError(1,1);

  TCanvas * c1 = new TCanvas("c1", "c1", 640, 480);
  histmU->GetXaxis()->SetTitle("minDeltaPhiMET");
  histmU->GetYaxis()->SetTitle("Events (unit normalized)");
  histmU->GetXaxis()->SetRangeUser(0,0.5);
  histmU->DrawNormalized("HIST E");
  histmL->DrawNormalized("HIST SAMES E");
  TLegend *leg = new TLegend(0.35, 0.80, 0.7, 0.88);
  leg->AddEntry(histmU, "MET > "+sHP+" GeV", "l");
  leg->AddEntry(histmL, sLP+" GeV < MET < "+sHP+" GeV", "l");
  leg->SetFillColor(0);
  leg->SetLineColor(0);
  leg->SetTextSize(0.04);
  leg->SetBorderSize(0);
  leg->SetLineStyle(0);
  leg->Draw();
  c1->Print(bcut+"_mdp_zoom.pdf");

  TString out = "";
  out+= nD*single;
  out += " +- ";
  out += abError(nD, nD_err, single, single_err);

  cout << bcut << ", " << singleLow << ", " << singleHigh << endl;
  cout << "nA: " << nA << " +- " << nA_err << endl;
  cout << "nB: " << nB << " +- " << nB_err << endl;
  cout << "nC: " << nC << " +- " << nC_err << endl;
  cout << "nD: " << nD << " +- " << nD_err << endl;
  cout << "nAB: " << nAB << " +- " << nAB_err << endl;
  cout << "nCD: " << nCD << " +- " << nCD_err << endl;
  cout << "r: " << single << " +- " << single_err << endl;
  cout << "ABCD: " << out << endl;
  cout << endl;


  cout << "nA/nB: " << nA/nB << " +- " << aObError(nA, nA_err, nB, nB_err) << endl;
  cout << "nD/nC: " << nD/nC << " +- " << aObError(nD, nD_err, nC, nC_err) << endl;
  cout << "nD/nA: " << nD/nA << " +- " << aObError(nD, nD_err, nA, nA_err) << endl;
  cout << "nC/nB: " << nC/nB << " +- " << aObError(nC, nC_err, nB, nB_err) << endl;
  cout << endl;

  cout << "nMU/nMUL: " << nMU/nMUL << " +- " << aObError(nMU, nMU_err, nMUL, nMUL_err) << endl;
  cout << "nB/nAB: " << nB/nAB << " +- " << aObError(nB, nB_err, nAB, nAB_err) << endl;
  cout << "nC/nCD: " << nC/nCD << " +- " << aObError(nC, nC_err, nCD, nCD_err) << endl;
  

  histC->Clear();
  histD->Clear();
  histB->Clear();
  histA->Clear();
  histAB->Clear();
  histCD->Clear();
  histMU->Clear();
  histML->Clear();
  histMUL->Clear();
  histmL->Clear();
  histmU->Clear();
  
  return out;
}


void ratioConst(){
  
  doRatioConst("eq1", 100, 150);
  doRatioConst("ge1", 100, 150);
  doRatioConst("ge2", 100, 150);
  return;

  TString tag;
  const int highMax = 1;
  const int lowMax = 15;
  TString o1[lowMax], o2[lowMax], o3[lowMax];

  double high;
  double highA[highMax] = {150};
  double lowA[lowMax]={60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130};
  
  for(int j = 0; j<highMax; j++){
    high = highA[j];
      
    tag="eq1";
    cout << endl;
    cout << tag << endl;
    for(int i = 0; i<lowMax; i++){
      o1[i] = doRatioConst(tag, lowA[i], high);
    }
    
    tag="ge1";
    cout << endl;
    cout << tag << endl;
    for(int i = 0; i<lowMax; i++){
      o2[i] = doRatioConst(tag, lowA[i], high);
    }
    
    tag="ge2";
    cout << endl;
    cout << tag << endl;
    for(int i = 0; i<lowMax; i++){
      o3[i] = doRatioConst(tag, lowA[i], high);
    }
    
    
    
    // print results /////////////////////
    cout << "HIGH: " << high << endl;
    cout << "eq1 results: " << endl;
    for(int i = 0; i<lowMax; i++){
      cout << o1[i] << endl;
    }
    cout << endl;
    
    cout << "ge1 results: " << endl;
    for(int i = 0; i<lowMax; i++){
      cout << o2[i] << endl;
    }
    cout << endl;
    
    cout << "ge2 results: " << endl;
    for(int i = 0; i<lowMax; i++){
      cout << o3[i] << endl;
    }
    cout << endl;

  }// end j loop
}
