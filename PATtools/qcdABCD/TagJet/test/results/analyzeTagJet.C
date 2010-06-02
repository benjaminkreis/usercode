#include "assert.h"
#include <iostream>

#include "TSystem.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TString.h"
#include "TChain.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"

#include "analyzeTagJetInput.h"

using namespace std;

void analyzeTagJet(){
  cout << "Begin analyzeTagJet" << endl;
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptStat("e");


  TChain* InputChain = FormChain(); //function in analyzeTagJetInput.h

  int numEntries = InputChain->GetEntries();
  cout <<"numEntries: " << numEntries << endl;

  //load bjet btag eta nAllJets nLooseJets nTightJets pt
  float bjet = -1, btag = -1, eta = -1, nAllJets = -1, nLooseJets = -1, nTightJets = -1, pt = -1;
  InputChain->SetBranchAddress("bjet", &bjet);
  InputChain->SetBranchAddress("btag", &btag);
  InputChain->SetBranchAddress("eta", &eta);
  InputChain->SetBranchAddress("nAllJets", &nAllJets);
  InputChain->SetBranchAddress("nLooseJets", &nLooseJets);
  InputChain->SetBranchAddress("nTightJets", &nTightJets);
  InputChain->SetBranchAddress("pt", &pt);

  TH2F* Htag = new TH2F("Htag", "H tag", 10, 0.0, 1000.0, 10, -5.0, 5.0);
  TH2F* Hall = new TH2F("Hall", "H all", 10, 0.0, 1000.0, 10, -5.0, 5.0);
  TH2F* Heff = new TH2F("Heff", "H eff", 10, 0.0, 1000.0, 10, -5.0, 5.0);

  TCanvas* C_all = new TCanvas("C_all", "Canvas all", 1920, 480);
  C_all->Divide(3,1);

  for(int i = 1; i<=numEntries; i++){
    InputChain->GetEvent(i);

    bool fillCheck = true;
    //  if(nLooseJets<3) fillCheck = false;


    if(fillCheck){
      Htag->Fill(pt, eta, btag);
      Hall->Fill(pt, eta, 1.0);
    }
  }
  
  //CALCULATE EFFICIENCY
  int ptBins = Htag->GetNbinsX();
  int etaBins = Htag->GetNbinsY();
  
  for(int i = 1; i <= ptBins; i++){
    for(int j = 1; j <= etaBins; j++){
      
      int globalBin = Hall->GetBin(i,j);
      float x = Hall->GetXaxis()->GetBinCenter(i); 
      float y = Hall->GetYaxis()->GetBinCenter(j);
      float eff = -.01;
      if(Hall->GetBinContent(globalBin) > 0){
	eff = Htag->GetBinContent(globalBin) / Hall->GetBinContent(globalBin);
      }
      //      cout << "globalBin " << globalBin << ", eff " << eff << endl;
      Heff->Fill(x,y,eff);

    }
  }
  
  C_all->cd(1);
  Htag->Draw("COLZ");
  C_all->cd(2);
  Hall->Draw("COLZ");
  C_all->cd(3);
  Heff->Draw("COLZ");


  cout << "End analyzeTagJet" << endl;

}

