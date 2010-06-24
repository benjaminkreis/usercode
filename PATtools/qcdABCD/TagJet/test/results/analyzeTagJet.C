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

#include "analyzeTagJetInput.h"

using namespace std;

void analyzeTagJet(){
  cout << "Begin analyzeTagJet" << endl;
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptStat("e");

  TFile fout("tagjeteff_QCD_double.root", "RECREATE");

  TString xtitle = "jet p_T [GeV]";
  TString ytitle = "jet eta";

  TChain* InputChain = FormChain(); //function in analyzeTagJetInput.h

  int fillNum = 0;
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

  int nptBins = 21; 
  double ptBinArray[21] = {0.,30.,50.,100.,200.,300.,400.,500.,600.,700.,800.,900.,1000., 1200.,1400.,1600.,1800.,2000.,2300.,2600.,2900.};
  
  TH2D* Htag = new TH2D("Htag", "H tag", nptBins-1, ptBinArray, 10, -5.0, 5.0);
  TH2D* Hjet = new TH2D("Hjet", "H jet", nptBins-1, ptBinArray, 10, -5.0, 5.0);
  TH2D* Hall = new TH2D("Hall", "H all", nptBins-1, ptBinArray, 10, -5.0, 5.0);
  TH2D* Heff_t = new TH2D("Heff_t", "H eff_t", nptBins-1, ptBinArray, 10, -5.0, 5.0);
  TH2D* Heff_j = new TH2D("Heff_j", "H eff_j", nptBins-1, ptBinArray, 10, -5.0, 5.0);
  TH2D* Heff_te = new TH2D("Heff_te", "H eff_te", nptBins-1, ptBinArray, 10, -5.0, 5.0);  
  TH2D* Heff_je = new TH2D("Heff_je", "H eff_je", nptBins-1, ptBinArray, 10, -5.0, 5.0);  
  
  Heff_t->SetMinimum(0.0);
  Heff_t->SetMaximum(.4);
  Heff_j->SetMinimum(0.0);
  Heff_j->SetMaximum(.4); 
  Heff_te->SetMinimum(0.0);
  Heff_je->SetMinimum(0.0);
  Htag->GetXaxis()->SetTitle(xtitle);
  Hall->GetXaxis()->SetTitle(xtitle);
  Heff_t->GetXaxis()->SetTitle(xtitle);
  Heff_j->GetXaxis()->SetTitle(xtitle);
  Heff_te->GetXaxis()->SetTitle(xtitle);
  Heff_je->GetXaxis()->SetTitle(xtitle);
  Hjet->GetXaxis()->SetTitle(xtitle);
  Htag->GetYaxis()->SetTitle(ytitle);
  Hjet->GetYaxis()->SetTitle(ytitle);
  Hall->GetYaxis()->SetTitle(ytitle);
  Heff_t->GetYaxis()->SetTitle(ytitle);
  Heff_j->GetYaxis()->SetTitle(ytitle);
  Heff_te->GetYaxis()->SetTitle(ytitle);
  Heff_je->GetYaxis()->SetTitle(ytitle);

  TCanvas* C_all = new TCanvas("C_all", "Canvas all", 2560, 960);
  C_all->Divide(4,2);

  double ptmax = 0.0;
  for(int i = 0; i<numEntries; i++){
    InputChain->GetEvent(i);

    bool fillCheck = false;
    //if(nLooseJets>2) fillCheck = true;
    fillCheck = true;
  
    if(fillCheck){
      fillNum++;
      Htag->Fill(pt, eta, btag);
      Hjet->Fill(pt, eta, bjet);
      Hall->Fill(pt, eta, 1.0);
    }
   
    if(pt>ptmax){
      ptmax = pt;
    }
    
  }

  cout << "ptmax: " << ptmax << endl;;

  //CALCULATE EFFICIENCY
  int ptBins = Htag->GetNbinsX();
  int etaBins = Htag->GetNbinsY();
  
  for(int i = 1; i <= ptBins; i++){
    for(int j = 1; j <= etaBins; j++){
      
      int globalBin = Hall->GetBin(i,j);
      double x = Hall->GetXaxis()->GetBinCenter(i); 
      double y = Hall->GetYaxis()->GetBinCenter(j);
    
      //eff_t
      double eff_t = -.01;
      double eff_te = -.01;
      if(Hall->GetBinContent(globalBin) > 0){
	double tag = Htag->GetBinContent(globalBin); 
	double all = Hall->GetBinContent(globalBin);
	eff_t = tag/all;
	eff_te = sqrt( (sqrt(tag)/tag)*(sqrt(tag)/tag)+(sqrt(all)/all)*(sqrt(all)/all) )*eff_t;
      }
      Heff_t->Fill(x,y,eff_t);
      Heff_te->Fill(x,y,eff_te);

      //eff_j
      double eff_j = -.01;
      double eff_je = -.01;
      if(Hall->GetBinContent(globalBin) > 0){
	double jet =  Hjet->GetBinContent(globalBin);
	double all = Hall->GetBinContent(globalBin);
	eff_j = jet/all;
	eff_je =  sqrt( (sqrt(jet)/jet)*(sqrt(jet)/jet)+(sqrt(all)/all)*(sqrt(all)/all) )*eff_j;
      }
      Heff_j->Fill(x,y,eff_j);
      Heff_je->Fill(x,y,eff_je);


    }
  }
  
  C_all->cd(1);
  gPad->SetRightMargin(.18);
  Htag->Draw("COLZ");
  C_all->cd(2);
  gPad->SetRightMargin(.18); 
  Hall->Draw("COLZ");
  C_all->cd(3);
  gPad->SetRightMargin(.18);
  Heff_t->Draw("COLZ");
  C_all->cd(4);
  gPad->SetRightMargin(.18);
  Heff_te->Draw("COLZ");
 
  C_all->cd(5);
  gPad->SetRightMargin(.18);
  Hjet->Draw("COLZ");
  C_all->cd(6);
  gPad->SetRightMargin(.18); 
  Hall->Draw("COLZ");
  C_all->cd(7);
  gPad->SetRightMargin(.18);
  Heff_j->Draw("COLZ");
  C_all->cd(8);
  gPad->SetRightMargin(.18);
  Heff_je->Draw("COLZ");

  //  C_all->Print("plots.pdf");

  cout << "fillNum: " << fillNum << endl;

  C_all->Write();
  Hall->Write();
  Hjet->Write();
  Heff_t->Write();
  Heff_j->Write();
  Heff_te->Write();
  Heff_je->Write();
  fout.Close();

  cout << "End analyzeTagJet" << endl;

}

