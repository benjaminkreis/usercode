#include "TFile.h"
#include "TChain.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TF1.h"
#include "TFormula.h"
#include "TLine.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TAxis.h"
#include "TMatrixT.h"
#include "TVectorT.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TROOT.h"
#include "TFitter.h"
#include <iostream>
#include <vector>
#include <exception>
#include <cmath> 
#include <iomanip>
#include <assert.h>

#include "analyzeFillABCDInput.h"

using namespace std;

void Dfrac(){
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  //  gStyle->SetOptStat("");

  //LOAD WEIGHTS
  TFile finweight("/afs/cern.ch/user/k/kreis/scratch0/CMSSW_3_6_3/src/Filter/RA2Filter/test/results/weight_MG.root","READ");

  TH1D* HweightP = 0;
  if(!finweight.IsZombie()){
    HweightP = (TH1D*)finweight.Get("Hweight");
  }
  else{
    cout << "ERROR loading weights" << endl;
  }
  TFile fout("Dfrac_plots.root", "RECREATE");
  TH1D Hweight = *HweightP;
  finweight.Close();

  TChain* InputChain = FormChain();
  int numEntries = InputChain->GetEntries();
  cout <<"numEntries: " << numEntries << endl;
  cout << endl;

  double pi=4*atan(1.0);
  TH2D* h2D = new TH2D("h2D_minDPhi", "h2D_minDPhi", 20, 0.0, pi, 100000, -.1, 1.1);
  h2D->Sumw2();

  double x, y, MG;
  int nbtags;
  InputChain->SetBranchAddress("minDPhi",&y);
  InputChain->SetBranchAddress("MHT",&x);
  InputChain->SetBranchAddress("MG",&MG);
  InputChain->SetBranchAddress("btag", &nbtags);
  InputChain->SetBranchAddress("btag", &nbtags);


  double nD=0, Dfrac=0;

  for(int i = 0; i<numEntries; i++){
    InputChain->GetEvent(i);
   

    //GET WEIGHT
    int bin = Hweight.FindBin(MG);
    double weight=Hweight.GetBinContent(bin);

    int pass = 0;
    if(nbtags>=2)pass =1;

    if(x>=150.0){
      //if(weight>1) cout << "pass: " << pass <<  ", weight: " << weight <<", minDPhi: " << y << endl;
      h2D->Fill(y,pass, weight);
    }

    if(x>=150.0 && x < 1000.0 && y<.3) {
      nD+=weight;
      Dfrac+=(double)pass*weight;
    }
  }
  Dfrac=Dfrac/nD;

  TCanvas* myC = new TCanvas("myC", "myC", 640, 480);
  myC->cd();
 
  TProfile* p1_minDPhi = h2D->ProfileX();
  p1_minDPhi->GetXaxis()->SetTitle("MinDPhi(MHT, jet123)");
  p1_minDPhi->GetYaxis()->SetTitle("nPassBtagCut/nEvents");
  p1_minDPhi->SetTitle("b-tag cut efficiency after 2010 RA2 minus angular cuts");
  p1_minDPhi->SetLineWidth(2);
  p1_minDPhi->Draw();

  TLine* line1 = new TLine(0, Dfrac, pi, Dfrac);
  line1->SetLineWidth(2);
  line1->SetLineColor(2);
  line1->Draw();

  
  myC->Write();
  h2D->Write();
  p1_minDPhi->Write();
  fout.Close();
  cout << "Dfrac: " << Dfrac << endl;


}
