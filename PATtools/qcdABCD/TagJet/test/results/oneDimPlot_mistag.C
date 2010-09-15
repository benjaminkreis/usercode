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

void oneDimPlot_mistag(){
  cout << "Begin analyzeTagJet" << endl;
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptStat("e");

  TFile fout("tagjet_mistag.root", "RECREATE");

  TChain* InputChain = FormChain(); //function in analyzeTagJetInput.h

  int numEntries = InputChain->GetEntries();
  cout <<"numEntries: " << numEntries << endl;

  double MET, METproj,eta,pt,MHT,MHTproj;
  int btag, bjet, nLooseJets,nTracks;
  InputChain->SetBranchAddress("btag", &btag);
  InputChain->SetBranchAddress("bjet", &bjet);
  InputChain->SetBranchAddress("MET", &MET);
  InputChain->SetBranchAddress("METproj", &METproj);
  InputChain->SetBranchAddress("MHT", &MHT);
  InputChain->SetBranchAddress("MHTproj", &MHTproj);
  InputChain->SetBranchAddress("pt",&pt);
  InputChain->SetBranchAddress("eta",&eta);
  InputChain->SetBranchAddress("nTracks",&nTracks);
  InputChain->SetBranchAddress("nLooseJets",&nLooseJets);

  TH2D* h2D_MET[4];
  TH2D* h2D_METproj[4];
  TH2D* h2D_MHT[4];
  TH2D* h2D_MHTproj[4];
  TH2D* h2D_eta[4];
  TH2D* h2D_pt[4];
  TH2D* h2D_nTracks[4];
  TH2D* h2D_nLooseJets[4];
  
  for(int i=0; i<=3; i++){
    TString is="";
    is+=i;
    h2D_MET[i] = new TH2D("h2D_MET"+is,"h2D_MET"+is,100,0.0,1000.0,120,0.0,1.1);
    h2D_METproj[i] =  new TH2D("h2D_METproj"+is,"h2D_METproj"+is,100,-1000.0,1000.0,120,-0.1,1.1);
    h2D_MHT[i] = new TH2D("h2D_MHT"+is,"h2D_MHT"+is,100,0.0,1000.0,120,-0.1,1.1);
    h2D_MHTproj[i] =  new TH2D("h2D_MHTproj"+is,"h2D_MHTproj"+is,100,-1000.0,1000.0,120,-0.1,1.1);
    h2D_eta[i] = new TH2D("h2D_eta"+is, "h2D_eta"+is, 32, -4.0, 4.0, 120, -0.1, 1.1);
    h2D_pt[i] = new TH2D("h2D_pt"+is, "h2D_pt"+is, 100, 0.0, 2000.0, 120, -0.1, 1.1);
    h2D_nTracks[i] = new TH2D("h2D_nTracks"+is, "h2D_nTracks"+is,100,0.0,500.0,120,-0.1,1.1);
    h2D_nLooseJets[i] = new TH2D("h2D_nLooseJets"+is, "h2D_nLooseJets"+is, 20,0.0,20.0,120,-0.1,1.1);
  }

  for(int i = 0; i<numEntries; i++){
    InputChain->GetEvent(i);

    if(pt>30.0 && fabs(eta)<5.0){

      int j[4] = {0,0,0,0};
      if(bjet==1 && btag==1) j[0]=1; //truebtagged
      if(bjet==0 && btag==1) j[1]=1; //mistag
      if(bjet==1 && btag==0) j[2]=1; //truebnottagged
      if(bjet==0 && btag==0) j[3]=1; //notbnottagged
      
      for(int k=0; k<3; k++){
	h2D_MET[k]->Fill(MET,j[k]);
	h2D_METproj[k]->Fill(METproj,j[k]);
	h2D_MHT[k]->Fill(MHT,j[k]);
	h2D_MHTproj[k]->Fill(MHTproj,j[k]);
	h2D_eta[k]->Fill(eta,j[k]);
	h2D_pt[k]->Fill(pt,j[k]);
	h2D_nTracks[k]->Fill(nTracks,j[k]);
	h2D_nLooseJets[k]->Fill(nLooseJets,j[k]);
      }

    }

  }//end loop over tree entries

  TProfile* p1_MET[4];
  TProfile* p1_METproj[4];
  TProfile* p1_MHT[4];
  TProfile* p1_MHTproj[4];
  TProfile* p1_pt[4];
  TProfile* p1_eta[4]; 
  TProfile* p1_nTracks[4];
  TProfile* p1_nLooseJets[4];

  for(int i=0; i<=3; i++){
    p1_MET[i] = h2D_MET[i]->ProfileX(); 
    p1_METproj[i] = h2D_METproj[i]->ProfileX(); 
    p1_MHT[i] = h2D_MHT[i]->ProfileX();
    p1_MHTproj[i] = h2D_MHTproj[i]->ProfileX();
    p1_pt[i] = h2D_pt[i]->ProfileX();
    p1_eta[i] = h2D_eta[i]->ProfileX();
    p1_nTracks[i] = h2D_nTracks[i]->ProfileX();
    p1_nLooseJets[i] = h2D_nLooseJets[i]->ProfileX();
 
    p1_MET[i]->Write();
    p1_MHT[i]->Write();
    p1_METproj[i]->Write();
    p1_MHTproj[i]->Write();
    p1_pt[i]->Write();
    p1_eta[i]->Write();
    p1_nTracks[i]->Write();
    p1_nLooseJets[i]->Write();
    
    h2D_MET[i]->Write();
    h2D_MHT[i]->Write();
    h2D_METproj[i]->Write();
    h2D_MHTproj[i]->Write();
    h2D_pt[i]->Write();
    h2D_eta[i]->Write();
    h2D_nTracks[i]->Write();
    h2D_nLooseJets[i]->Write();
  }

  fout.Close();

}
