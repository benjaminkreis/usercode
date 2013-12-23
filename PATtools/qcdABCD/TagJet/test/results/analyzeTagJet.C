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

  int nptBins1d = 25;
  //  double ptBinArray1d[25] = {30, 33, 36, 39, 42, 45, 48, 51, 54, 57, 60, 63, 66, 69, 72, 75, 78, 81, 84, 87, 90, 93, 96, 99, 102};
  double ptBinArray1d[25] = {30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150}; //max .1
  // double ptBinArray1d[25] = {30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 250, 260, 270};
  // double ptBinArray1d[25] = {30, 50, 70, 90, 110, 130, 150, 170, 190, 210, 230, 250, 270, 290, 310, 330, 350, 370, 390, 410, 430, 450, 470, 490, 510}; //max .13

  TH2D* Htag = new TH2D("Htag", "H tag", nptBins-1, ptBinArray, 10, -5.0, 5.0);
  TH2D* Hjet = new TH2D("Hjet", "H jet", nptBins-1, ptBinArray, 10, -5.0, 5.0);
  TH2D* Hall = new TH2D("Hall", "H all", nptBins-1, ptBinArray, 10, -5.0, 5.0);
  TH2D* Heff_t = new TH2D("Heff_t", "H eff_t", nptBins-1, ptBinArray, 10, -5.0, 5.0);
  TH2D* Heff_j = new TH2D("Heff_j", "H eff_j", nptBins-1, ptBinArray, 10, -5.0, 5.0);
  TH2D* Heff_te = new TH2D("Heff_te", "H eff_te", nptBins-1, ptBinArray, 10, -5.0, 5.0);  
  TH2D* Heff_je = new TH2D("Heff_je", "H eff_je", nptBins-1, ptBinArray, 10, -5.0, 5.0);  
  
  TH1D* HtagLow = new TH1D("HtagLow", "Htag Low", nptBins1d-1, ptBinArray1d);
  TH1D* HtagMid = new TH1D("HtagMid", "Htag Mid", nptBins1d-1, ptBinArray1d);
  TH1D* HtagHigh = new TH1D("HtagHigh", "Htag High", nptBins1d-1, ptBinArray1d);
  TH1D* HallLow = new TH1D("HallLow", "Hall Low", nptBins1d-1, ptBinArray1d);
  TH1D* HallMid = new TH1D("HallMid", "Hall Mid", nptBins1d-1, ptBinArray1d);
  TH1D* HallHigh = new TH1D("HallHigh", "Hall High", nptBins1d-1, ptBinArray1d);
  TH1D* HeffLow = new TH1D("HeffLow", "Heff Low", nptBins1d-1, ptBinArray1d);
  TH1D* HeffMid = new TH1D("HeffMid", "Heff Mid", nptBins1d-1, ptBinArray1d);
  TH1D* HeffHigh = new TH1D("HeffHigh", "Heff High", nptBins1d-1, ptBinArray1d);
  TH1D* Heff_eLow = new TH1D("Heff_eLow", "Heff_e Low", nptBins1d-1, ptBinArray1d);
  TH1D* Heff_eMid = new TH1D("Heff_eMid", "Heff_e Mid", nptBins1d-1, ptBinArray1d);
  TH1D* Heff_eHigh = new TH1D("Heff_eHigh", "Heff_e High", nptBins1d-1, ptBinArray1d);
  
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

  TCanvas* C_byEta = new TCanvas("C_byEta", "Canvas byEta", 640, 480);
  TCanvas* C_all = new TCanvas("C_all", "Canvas all", 2560, 960);
  C_all->Divide(4,2);

  //////////////////
  // LOOP TO FILL //
  //////////////////
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


      //
      if(fabs(eta)>=0. && fabs(eta)<0.7){ //LOW
	HtagLow->Fill(pt,btag);
	HallLow->Fill(pt,1.0);
      }
      else if(fabs(eta)>=0.7 && fabs(eta)<1.4){ //MID
	HtagMid->Fill(pt,btag);
	HallMid->Fill(pt,1.0);
      }
      else if(fabs(eta)>=1.4 && fabs(eta)<2.4){ // HIGH
	HtagHigh->Fill(pt,btag);
	HallHigh->Fill(pt,1.0);
      }
    }

    if(pt>ptmax){
      ptmax = pt;
    }
    
  }
  cout << "ptmax: " << ptmax << endl;;

  double tagLastBinLow = HtagLow->GetBinContent(nptBins1d-1);
  HtagLow->SetBinContent(nptBins1d-1, tagLastBinLow+HtagLow->GetBinContent(nptBins1d));
  double tagLastBinMid = HtagMid->GetBinContent(nptBins1d-1);
  HtagMid->SetBinContent(nptBins1d-1, tagLastBinMid+HtagMid->GetBinContent(nptBins1d));
  double tagLastBinHigh = HtagHigh->GetBinContent(nptBins1d-1);
  HtagHigh->SetBinContent(nptBins1d-1, tagLastBinHigh+HtagHigh->GetBinContent(nptBins1d));
  double allLastBinLow = HallLow->GetBinContent(nptBins1d-1);
  HallLow->SetBinContent(nptBins1d-1, allLastBinLow+HallLow->GetBinContent(nptBins1d));
  double allLastBinMid = HallMid->GetBinContent(nptBins1d-1);
  HallMid->SetBinContent(nptBins1d-1, allLastBinMid+HallMid->GetBinContent(nptBins1d));
  double allLastBinHigh = HallHigh->GetBinContent(nptBins1d-1);
  HallHigh->SetBinContent(nptBins1d-1, allLastBinHigh+HallHigh->GetBinContent(nptBins1d));

  ////////////////////////
  //CALCULATE EFFICIENCY//
  ////////////////////////
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
  
  int ptBins1d = HtagLow->GetNbinsX();
  for(int i = 1; i <= ptBins1d; i++){
    
    //LOW
    double effLow = -0.1;
    double eff_eLow = -0.1;
    if(HallLow->GetBinContent(i)>0){
      double tag = HtagLow->GetBinContent(i);
      double all = HallLow->GetBinContent(i);
      effLow = tag/all;
      eff_eLow =  sqrt( (sqrt(tag)/tag)*(sqrt(tag)/tag)+(sqrt(all)/all)*(sqrt(all)/all) )*effLow;
    }
    HeffLow->SetBinContent(i, effLow);
    HeffLow->SetBinError(i,eff_eLow);
    Heff_eLow->SetBinContent(i, eff_eLow);
 
    //MID
    double effMid = -0.1;
    double eff_eMid = -0.1;
    if(HallMid->GetBinContent(i)>0){
      double tag = HtagMid->GetBinContent(i);
      double all = HallMid->GetBinContent(i);
      effMid = tag/all;
      eff_eMid =  sqrt( (sqrt(tag)/tag)*(sqrt(tag)/tag)+(sqrt(all)/all)*(sqrt(all)/all) )*effMid;
    }
    HeffMid->SetBinContent(i, effMid);
    HeffMid->SetBinError(i,eff_eMid);
    Heff_eMid->SetBinContent(i, eff_eMid);

    //HIGH
    double effHigh = -0.1;
    double eff_eHigh = -0.1;
    if(HallHigh->GetBinContent(i)>0){
      double tag = HtagHigh->GetBinContent(i);
      double all = HallHigh->GetBinContent(i);
      effHigh = tag/all;
      eff_eHigh =  sqrt( (sqrt(tag)/tag)*(sqrt(tag)/tag)+(sqrt(all)/all)*(sqrt(all)/all) )*effHigh;
    }
    HeffHigh->SetBinContent(i, effHigh);
    HeffHigh->SetBinError(i,eff_eHigh);
    Heff_eHigh->SetBinContent(i, eff_eHigh);
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


  TLegend* leg = new TLegend(0.63, 0.15, 0.88, 0.3);
  leg->AddEntry(HeffLow, "0.0 <= |eta| < 0.7", "l"); 
  leg->AddEntry(HeffMid, "0.7 <= |eta| < 1.4", "l"); 
  leg->AddEntry(HeffHigh, "1.4 <= |eta| < 2.4", "l"); 
  leg->SetFillColor(0);
  leg->SetLineColor(0);
  
  HeffLow->SetMaximum(0.1);
  HeffLow->SetTitle("");
  HeffLow->GetXaxis()->SetTitle("pT");
  HeffLow->GetYaxis()->SetTitle("SSVM Tagging Rate");
  HeffLow->GetYaxis()->SetTitleOffset(1.5);
  HeffLow->SetLineColor(kRed);
  HeffMid->SetLineColor(kGreen);
  HeffHigh->SetLineColor(kBlue);
  C_byEta->cd();
  gPad->SetLeftMargin(0.12);
  HeffLow->Draw();
  HeffMid->Draw("SAME");
  HeffHigh->Draw("SAME");
  leg->Draw();
  

  cout << "fillNum: " << fillNum << endl;
  
  C_byEta->Write();
  C_all->Write();
  Hall->Write();
  Hjet->Write();
  Heff_t->Write();
  Heff_j->Write();
  Heff_te->Write();
  Heff_je->Write();

  HeffLow->Write();
  Heff_eLow->Write();
  HeffMid->Write();
  Heff_eMid->Write();
  HeffHigh->Write();
  Heff_eHigh->Write();

  fout.Close();

  cout << "End analyzeTagJet" << endl;

}

