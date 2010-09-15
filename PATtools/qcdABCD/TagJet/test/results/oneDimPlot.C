#include "assert.h"
#include <iostream>

#include "TSystem.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TString.h"
#include "TChain.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TProfile.h"
#include "TCanvas.h"

#include "analyzeTagJetInput.h"

using namespace std;

void oneDimPlot(){
  cout << "Begin analyzeTagJet" << endl;
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptStat("e");

  TFile fout("tagjet.root", "RECREATE");

  TChain* InputChain = FormChain(); //function in analyzeTagJetInput.h

  int numEntries = InputChain->GetEntries();
  cout <<"numEntries: " << numEntries << endl;

  double MET, METproj,eta,pt,MHT,MHTproj;
  int btag, nLooseJets,nTracks;
  InputChain->SetBranchAddress("btag", &btag);
  InputChain->SetBranchAddress("MET", &MET);
  InputChain->SetBranchAddress("METproj", &METproj);
  InputChain->SetBranchAddress("MHT", &MHT);
  InputChain->SetBranchAddress("MHTproj", &MHTproj);
  InputChain->SetBranchAddress("pt",&pt);
  InputChain->SetBranchAddress("eta",&eta);
  InputChain->SetBranchAddress("nTracks",&nTracks);
  InputChain->SetBranchAddress("nLooseJets",&nLooseJets);

  TH2D* h2D_MET = new TH2D("h2D_MET","h2D_MET",100,0.0,1000.0,120,0.0,1.1);
  TH2D* h2D_METproj =  new TH2D("h2D_METproj","h2D_METproj",100,-1000.0,1000.0,120,-0.1,1.1);
  TH2D* h2D_MHT = new TH2D("h2D_MHT","h2D_MHT",100,0.0,1000.0,120,-0.1,1.1);
  TH2D* h2D_MHTproj =  new TH2D("h2D_MHTproj","h2D_MHTproj",100,-1000.0,1000.0,120,-0.1,1.1);
  TH2D* h2D_eta = new TH2D("h2D_eta", "h2D_eta", 32, -4.0, 4.0, 120, -0.1, 1.1);
  TH2D* h2D_pt = new TH2D("h2D_pt", "h2D_pt", 100, 0.0, 2000.0, 120, -0.1, 1.1);
  TH2D* h2D_nTracks = new TH2D("h2D_nTracks", "h2D_nTracks",100,0.0,500.0,120,-0.1,1.1);
  TH2D* h2D_nLooseJets = new TH2D("h2D_nLooseJets", "h2D_nLooseJets", 20,0.0,20.0,120,-0.1,1.1);
  

  for(int i = 0; i<numEntries; i++){
    InputChain->GetEvent(i);

    if(pt>30.0 && fabs(eta)<5.0){

    h2D_MET->Fill(MET,btag);
    h2D_METproj->Fill(METproj,btag);
    h2D_MHT->Fill(MHT,btag);
    h2D_MHTproj->Fill(MHTproj,btag);
    h2D_eta->Fill(eta,btag);
    h2D_pt->Fill(pt,btag);
    h2D_nTracks->Fill(nTracks,btag);
    h2D_nLooseJets->Fill(nLooseJets,btag);
   
    }

  }//end loop over tree entries

  TProfile* p1_MET = h2D_MET->ProfileX(); 
  TProfile* p1_METproj = h2D_METproj->ProfileX(); 
  TProfile* p1_MHT = h2D_MHT->ProfileX();
  TProfile* p1_MHTproj = h2D_MHTproj->ProfileX();
  TProfile* p1_pt = h2D_pt->ProfileX();
  TProfile* p1_eta = h2D_eta->ProfileX();
  TProfile* p1_nTracks = h2D_nTracks->ProfileX();
  TProfile* p1_nLooseJets = h2D_nLooseJets->ProfileX();

  p1_MET->Write();
  p1_MHT->Write();
  p1_METproj->Write();
  p1_MHTproj->Write();
  p1_pt->Write();
  p1_eta->Write();
  p1_nTracks->Write();
  p1_nLooseJets->Write();

  h2D_MET->Write();
  h2D_MHT->Write();
  h2D_METproj->Write();
  h2D_MHTproj->Write();
  h2D_pt->Write();
  h2D_eta->Write();
  h2D_nTracks->Write();
  h2D_nLooseJets->Write();

  fout.Close();


}
