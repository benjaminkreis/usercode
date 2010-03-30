#include "TFile.h"
#include "TChain.h"
#include "TRandom.h"
#include "TH1.h"
#include "TH2F.h"
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

using namespace std;

void analyzeTagRatio(){

  TString treename = "T_tagRatio";
  TString filename = "plots_QCD_Pt170";

  TFile* file1 = TFile::Open(filename+".root","READ");
  
  TTree* tree1 = (TTree*) gDirectory->Get(treename);
  
  int numEntries = tree1->GetEntries();
  cout << "numEntries: " << numEntries << endl;

  //Histogram
  TH2F* hist1 = new TH2F("hist1", "histogram 1", 100, 0, 1000, 100, 0, 1);
  hist1->Sumw2();
  // TProfile* hist1prof = new TProfile("hist1prof", "histogram 1 profile", 100, 0, 1000, 0, 1);
 
  //Arrays for TGraph
  float r1x[numEntries];
  float r1y[numEntries];
  float r1x_e[numEntries];
  float r1y_e[numEntries];
  
  float x = -1;
  float njets = -1;
  float ntags = -1;
  tree1->SetBranchAddress("MET",&x);
  tree1->SetBranchAddress("njets",&njets);
  tree1->SetBranchAddress("ntags",&ntags);
  
  /////LOOP OVER EVENTS/////
  for(int i = 0; i< numEntries; i++){ //loop over tree
    tree1->GetEvent(i+1);
   
    float y = ntags/njets;

    //Fill arrays for TGraph
    r1x[i] = x;
    r1y[i] = y;
    r1x_e[i] = 0;
    r1x_e[i] = 0;

    hist1->Fill(x,y);
    //hist1prof->Fill(x,y);

  }

  TGraphErrors * gr1 = new TGraphErrors(numEntries, r1x, r1y, r1x_e, r1y_e);
  gr1->SetMarkerStyle(4);
  gr1->Draw("AP");
  
  hist1->Draw("COLZ");

  //hist1prof->SetLineWidth(3);
  //hist1prof->Draw();

  TProfile *tpf = hist1->ProfileX();
  tpf->SetAxisRange(.2, .4, "Y");
  tpf->SetLineWidth(3);
  tpf->Draw();
  
}
