#include <iostream>
#include <iomanip>
#include <assert.h>

#include "TROOT.h"
#include "TStyle.h"

#include "TMinuit.h"

#include "TCanvas.h"
#include "TFile.h"
#include "TLegend.h"
#include "TString.h"
#include "TChain.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TF1.h"
#include "TGraphErrors.h"

#include "analyzeFillABCDInput.h"


bool passb(int nbtags){
  if(nbtags>=1){
    return true;
  }
  else{
    return false;
  }
}

bool passmdp(double mdp){
  if(mdp>0.3){
    return true;
  }
  else{
    return false;
  }
}


void simpleABCD(){

  TFile fout("simple.root", "RECREATE");

  double pi=4*atan(1.0)+.0001;

  TString xName;
  TString yName;
  double borderv1a;
  double borderv1b;
  double borderv2a;
  double borderv2b;
  double borderh1a;
  double borderh1b;
  double borderh2a;
  double borderh2b;
  TH2D* histA;
  TH2D* histB;
  TH2D* histC;
  TH2D* histD;

  /*
  // MET - minDeltaPhiMET ////////////////////
  xName="MET";
  yName="minDeltaPhiMET";
  //x
  borderv1a = 0.0;
  borderv1b = 150.0;
  borderv2a=150.0;
  borderv2b=1e10;
  //y
  borderh1a=0.0;
  borderh1b=0.3;
  borderh2a=0.3;
  borderh2b=pi;
  histA = new TH2D("histA", "histA", 1, borderv1a, borderv1b, 1, borderh1a, borderh1b);
  histB = new TH2D("histB", "histB", 1, borderv1a, borderv1b, 1, borderh2a, borderh2b);
  histC = new TH2D("histC", "histC", 1, borderv2a, borderv2b, 1, borderh2a, borderh2b);
  histD = new TH2D("histD", "histD", 1, borderv2a, borderv2b, 1, borderh1a, borderh1b);
  */

  // MET - jet1eta ///////////////////////
  xName="MET";
  yName="jet1eta";
  //x
  borderv1a = 0.0;
  borderv1b = 150.0;
  borderv2a=150.0;
  borderv2b=1e10;
  //y
  borderh1a=0.0;
  borderh1b=1.5;
  borderh2a=1.5;
  borderh2b=2.4001;
  histB = new TH2D("histB", "histB", 1, borderv1a, borderv1b, 1, borderh1a, borderh1b);
  histA = new TH2D("histA", "histA", 1, borderv1a, borderv1b, 1, borderh2a, borderh2b);
  histD = new TH2D("histD", "histD", 1, borderv2a, borderv2b, 1, borderh2a, borderh2b);
  histC = new TH2D("histC", "histC", 1, borderv2a, borderv2b, 1, borderh1a, borderh1b);
  TH2D* histABCD = new TH2D("histABCD", "histABCD", 100,0,500,50,0,3);

  TH1D*  histMET1 = new TH1D("histMET1", "histMET1", 20, -3,3);
  TH1D*  histMET2 = new TH1D("histMET2", "histMET2", 20, -3,3);
  TH1D*  histMET3 = new TH1D("histMET3", "histMET3", 20, -3,3);
  histMET1->Sumw2();
  histMET2->Sumw2();
  histMET3->Sumw2();
  
  histA->Sumw2();
  histB->Sumw2();
  histC->Sumw2();
  histD->Sumw2();


  TChain* InputChain = FormChainJosh("pfpf");
  int numEntries = InputChain->GetEntries();
  cout <<"numEntries: " << numEntries << endl;
  

  double x, y, weight, mdp;
  int nbtags;
  InputChain->SetBranchAddress(xName, &x);
  InputChain->SetBranchAddress(yName, &y);
  InputChain->SetBranchAddress("weight", &weight);
  InputChain->SetBranchAddress("nbSSVM", &nbtags);
  InputChain->SetBranchAddress("minDeltaPhiMET", &mdp);


  double nMET=0, nMETMdp=0, nMETTagMdp=0;
  for(int i = 0; i<numEntries; i++){
    InputChain->GetEvent(i);
    if(x>150) nMET+=weight;
    if(!passmdp(mdp)) continue;   
    if(x>150) nMETMdp+=weight;
    if(!passb(nbtags)) continue;   
    if(x>150) nMETTagMdp+=weight;

    if(x>50 && x<100) histMET1->Fill(y,weight);
    if(100<x && x<150) histMET2->Fill(y,weight);
    if(x>150) histMET3->Fill(y,weight);

    y=fabs(y);
    histA->Fill(x,y,weight);
    histB->Fill(x,y,weight);
    histC->Fill(x,y,weight);
    histD->Fill(x,y,weight);
    histABCD->Fill(x,y,weight);
    
  }//end loop over InputChain                                      

  
  double nC = histD->GetBinContent(1,1)*histB->GetBinContent(1,1)/histA->GetBinContent(1,1);
  double nC_err = nC*sqrt( histD->GetBinError(1,1)*histD->GetBinError(1,1)/histD->GetBinContent(1,1)/histD->GetBinContent(1,1)
			   + histB->GetBinError(1,1)*histB->GetBinError(1,1)/histB->GetBinContent(1,1)/histB->GetBinContent(1,1)
			   + histA->GetBinError(1,1)*histA->GetBinError(1,1)/histA->GetBinContent(1,1)/histA->GetBinContent(1,1)
			   );


  TCanvas * C1 = new TCanvas("myC", "myC");
  C1->cd();
  histMET1->SetLineColor(kBlack);
  histMET2->SetLineColor(kBlue);
  histMET3->SetLineColor(kRed);
  histMET1->SetLineWidth(2);
  histMET2->SetLineWidth(2);
  histMET3->SetLineWidth(2);
  histMET3->GetXaxis()->SetTitle("lead jet eta");
  histMET3->DrawNormalized();
  histMET1->DrawNormalized("SAME");
  histMET2->DrawNormalized("SAME");
  TLegend *leg = new TLegend(.14,.75,.23,.89);
  leg->AddEntry(histMET1, "50 GeV < MET < 100 GeV", "l");
  leg->AddEntry(histMET2, "100 GeV < MET < 150 GeV", "l");
  leg->AddEntry(histMET3, "MET > 150 GeV ", "l");
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetLineStyle(0);
  leg->SetTextFont(42);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.04);
  leg->Draw();

  TCanvas *C2 = new TCanvas("myC2", "myC2");
  C2->cd();
  histABCD->GetXaxis()->SetTitle("MET [GeV]");
  histABCD->GetYaxis()->SetTitle("abs(lead jet eta)");
  histABCD->Draw("COLZ");
  gPad->SetRightMargin(.18);
  gPad->Modified();
  C2->Print("h1.pdf");


  cout << "n after MET cut: " << nMET << endl;
  cout << "n after MET and mdp: " << nMETMdp << endl;
  cout << "n after MET, mdp, and btag: " << nMETTagMdp << endl;
  cout << endl;
  cout << "nA true: " << histA->GetBinContent(1,1) << " +- " << histA->GetBinError(1,1) << endl;
  cout << "nB true: " << histB->GetBinContent(1,1) << " +- " << histB->GetBinError(1,1) << endl;
  cout << "nC true: " << histC->GetBinContent(1,1) << " +- " << histC->GetBinError(1,1) << endl;
  cout << "nD true: " << histD->GetBinContent(1,1) << " +- " << histD->GetBinError(1,1) << endl;
  cout << "ABCD: " << nC << " +- " << nC_err << endl;
  
  C1->Write();
  C2->Write();
  histMET1->Write();
  histMET2->Write();
  histMET3->Write();
  histA->Write();
  histB->Write();
  histC->Write();
  histD->Write();
  histABCD->Write();
  fout.Close();
  
}
