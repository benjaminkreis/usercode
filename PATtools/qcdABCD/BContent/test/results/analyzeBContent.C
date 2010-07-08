#include "TFile.h"
#include "TChain.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TF1.h"
#include "TFormula.h"
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

#include "analyzeBContentInput.h"

using namespace std;

void analyzeBContent(){
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptStat("eou");
  
  //INPUT
  TFile finweight("/afs/cern.ch/user/k/kreis/scratch0/PATtest/CMSSW_3_3_6/src/qcdABCD/weight_QCD.root");
  TH1D* Hweight = 0;
  if(!finweight.IsZombie()){
    Hweight = (TH1D*)finweight.Get("Hweight");
  }
  else{
    cout << "ERROR loading weights" << endl;
  }


  TFile fout("BContent_plots.root", "RECREATE");
  TH1D* hbjetFrac = new TH1D("hbjetFrac", "hbjetFrac", 20, -.025, 1.025);
  TH1D* hlightmistagFrac = new TH1D("hlightmistagFrac", "hlightmistagFrac", 20, -.025, 1.025);
  TH1D* hcmistagFrac = new TH1D("hcmistagFrac", "hcmistagFrac", 20, -.025, 1.025);


  TChain* InputChain = FormChain();
  int numEntries = InputChain->GetEntries();
  cout <<"numEntries: " << numEntries << endl;
  cout << endl;

  double pthat; //MHT minDPhi and qScale also available
  int nbtag, nbjet, nlightmistag, ncmistag;
  InputChain->SetBranchAddress("PtHat", &pthat);
  InputChain->SetBranchAddress("nbtag", &nbtag);
  InputChain->SetBranchAddress("nbjet", &nbjet);
  InputChain->SetBranchAddress("nlightmistag", &nlightmistag);
  InputChain->SetBranchAddress("ncmistag", &ncmistag);

  for(int i = 0; i<numEntries; i++){
    InputChain->GetEvent(i);

    //get weight
    int bin = Hweight->FindBin(pthat);
    double eventweight=Hweight->GetBinContent(bin);

    hbjetFrac->Fill((double)nbjet/nbtag, eventweight);
    hlightmistagFrac->Fill((double)nlightmistag/nbtag, eventweight);
    hcmistagFrac->Fill((double)ncmistag/nbtag, eventweight);

  }
  hbjetFrac->SetTitle("QCD events in signal region, 100 pb^-1");
  hlightmistagFrac->SetTitle("QCD, fraction of tagged jets in signal region");
  hcmistagFrac->SetTitle("QCD, fraction of tagged jets in signal region");

  hbjetFrac->GetXaxis()->SetTitle("b-jet fraction");
  hlightmistagFrac->GetXaxis()->SetTitle("light-flavor jet fraction");
  hcmistagFrac->GetXaxis()->SetTitle("c-jet fraction");
  hbjetFrac->GetYaxis()->SetTitle("events");
  hlightmistagFrac->GetYaxis()->SetTitle("events");
  hcmistagFrac->GetYaxis()->SetTitle("events");
  
  hbjetFrac->SetLineColor(kViolet+1);
  hlightmistagFrac->SetLineColor(kViolet+1);
  hcmistagFrac->SetLineColor(kViolet+1);

  hbjetFrac->SetLineWidth(2);
  hlightmistagFrac->SetLineWidth(2);
  hcmistagFrac->SetLineWidth(2);

  TCanvas* c1 = new TCanvas("c1", "c1", 640, 480);
  hbjetFrac->Draw();
  c1->Print("hbjetFrac.pdf");
  hlightmistagFrac->Draw();
  c1->Print("hlightmistagFrac.pdf");
  hcmistagFrac->Draw();
  c1->Print("hcmistagFrac.pdf");

  hbjetFrac->Write();
  hlightmistagFrac->Write();
  hcmistagFrac->Write();

}
