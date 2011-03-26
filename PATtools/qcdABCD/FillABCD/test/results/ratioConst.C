#include <iostream>
#include <vector>
#include <exception>
#include <cmath>
#include <iomanip>
#include <assert.h>

#include "TH1D.h"
#include "TH2D.h"
#include "TChain.h"

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

  //double singleLow=85;
  //double singleHigh=135;
  bool verbose = true;
  double borderv2a=150.0;
  double borderv2b=1000000000.;
  double borderh1a=0.0;
  double borderh1b=0.3;
  double borderh2a=0.3;
  double borderh2b=3.142;

  TH2D*  histSU = new TH2D("histSU", "histSU", 1, singleLow, singleHigh, 1, borderh2a, borderh2b);
  TH2D*  histSL = new TH2D("histSL", "histSL", 1, singleLow, singleHigh, 1, borderh1a, borderh1b);
  TH2D* histC = new TH2D("histC", "histC", 1, borderv2a, borderv2b, 1, borderh2a, borderh2b);
  TH2D* histD = new TH2D("histD", "histD", 1, borderv2a, borderv2b, 1, borderh1a, borderh1b);
  histSL->Sumw2();
  histSU->Sumw2();
  histC->Sumw2();
  histD->Sumw2();

  TChain* InputChain = FormChainJosh("pfpf");
  const int numEntries = InputChain->GetEntries();
  if(verbose)cout <<"numEntries: " << numEntries << endl;

  double x, y, weight;
  int nbtags;
  InputChain->SetBranchAddress("MET",&x);
  InputChain->SetBranchAddress("minDeltaPhiMET",&y);
  InputChain->SetBranchAddress("weight",&weight);
  InputChain->SetBranchAddress("nbSSVM",&nbtags);
  
  TH1D* histmL = new TH1D("l","l",50,0., 3.142);
  TH1D* histmU = new TH1D("u","u",50,0., 3.142);
  histmL->Sumw2();
  histmU->Sumw2();
  histmL->SetLineWidth(2);
  histmU->SetLineWidth(2);
  histmL->SetLineColor(kBlue);
  histmU->SetLineColor(kRed);

  for(int i = 0; i<numEntries; i++){
    InputChain->GetEvent(i);
    
    if(!(bcontinue(nbtags, bcut))) continue;
 
    histSU->Fill(x,y, weight);
    histSL->Fill(x,y, weight);
    histC->Fill(x,y,weight);
    histD->Fill(x,y,weight);

    if(x>100. && x<150) histmL->Fill(y,weight);
    if(x>150) histmU->Fill(y,weight);
  }
  double single = histSU->GetBinContent(1,1)/histSL->GetBinContent(1,1);
  double single_err = aObError( histSU->GetBinContent(1,1), histSU->GetBinError(1,1), histSL->GetBinContent(1,1), histSL->GetBinError(1,1) );
  double nD = histD->GetBinContent(1,1);
  double nD_err = histD->GetBinError(1,1);
 
  histmL->DrawNormalized("E");
  histmU->DrawNormalized("SAMES E");

  TString out = "";
  out+= nD*single;
  out += " +- ";
  out += abError(nD, nD_err, single, single_err);

  cout << bcut << ", " << singleLow << ", " << singleHigh << endl;
  cout << "nA: " << histSL->GetBinContent(1,1) << " +- " << histSU->GetBinError(1,1) << endl;
  cout << "nB: " << histSU->GetBinContent(1,1) << " +- " << histSU->GetBinError(1,1) << endl;
  cout << "nC: " << histC->GetBinContent(1,1) << " +- " << histC->GetBinError(1,1) << endl; 
  cout << "nD: " << histD->GetBinContent(1,1) << " +- " << histD->GetBinError(1,1) << endl; 
  cout << "r: " << single << " +- " << single_err << endl;
  cout << "ABCD: " << out << endl;
    
  histC->Clear();
  histD->Clear();
  histSU->Clear();
  histSL->Clear();
  
  return out;
}


void ratioConst(){
  
  doRatioConst("eq1", 100, 150);
  //  doRatioConst("ge1", 100, 150);
  doRatioConst("ge2", 100, 150);
  return;

  TString tag;
  const int highMax = 1;
  const int lowMax = 15;
  TString o1[lowMax], o2[lowMax], o3[lowMax];

  double high;
  double highA[highMax] = {140};
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
