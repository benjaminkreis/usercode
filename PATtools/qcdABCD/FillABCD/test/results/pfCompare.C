#include "TH1F.h"
#include "TFile.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TPaveStats.h"
#include <iostream>


using namespace std;

void pfCompare(){
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat("nemruo");
  
  int logTrue = 0;

  TFile* fCalo=TFile::Open("plots_calo.root","READ");// in-time
  TH1D* hx_caloP = 0;
  TH1D* hdp_caloP = 0;
  if(!fCalo->IsZombie()){
    hx_caloP=(TH1D*)fCalo->Get("H_x");
    hx_caloP->SetName("x calo");
    hdp_caloP=(TH1D*)fCalo->Get("H_dp");
    hdp_caloP->SetName("dp calo");
  }
  else{
    cout << "zombie!" << endl;
  }
  
  
  TFile* fpf=TFile::Open("plots_pf.root","READ");// in-time
  TH1D* hx_pfP = 0;
  TH1D* hdp_pfP = 0;
  if(!fpf->IsZombie()){
    hx_pfP=(TH1D*)fpf->Get("H_x");
    hx_pfP->SetName("x pf");
    hdp_pfP=(TH1D*)fpf->Get("H_dp");
    hdp_pfP->SetName("dp pf");
  }
  else{
    cout << "zombie!" << endl;
  }
  
  TFile fout("pfCompare.root", "RECREATE");
  TH1D hx_pf = *hx_pfP;
  TH1D hx_calo = *hx_caloP;
  TH1D hdp_pf = *hdp_pfP;
  TH1D hdp_calo = *hdp_caloP;
  TCanvas * C_x = new TCanvas("C_x", "C_x", 640, 480);
  C_x->cd();
  TCanvas * C_dp = new TCanvas("C_dp", "C_dp", 640, 480);
  C_dp->cd();  
 
  hx_pf.SetLineColor(kBlue);
  hdp_pf.SetLineColor(kBlue);
  hx_pf.SetLineWidth(2);
  hdp_pf.SetLineWidth(2);

  hx_calo.SetLineColor(kBlack);
  hdp_calo.SetLineColor(kBlack);
  hx_calo.SetLineWidth(2);
  hdp_calo.SetLineWidth(2);

 
  TLegend *leg_x = new TLegend(0.65, 0.75, 0.75, 0.85);
  leg_x->SetFillColor(0);
  leg_x->SetLineColor(0);
  leg_x->SetTextSize(0.04);
  leg_x->AddEntry(&hx_calo, "Calo", "l");
  leg_x->AddEntry(&hx_pf, "PFlow", "l");
 
  TLegend *leg_dp = new TLegend(0.65, 0.75, 0.75, 0.85);
  leg_dp->SetFillColor(0);
  leg_dp->SetLineColor(0);
  leg_dp->SetTextSize(0.04);
  leg_dp->AddEntry(&hdp_calo, "Calo", "l");
  leg_dp->AddEntry(&hdp_pf, "PFlow", "l");

  hdp_calo.Draw();
  hdp_calo.GetXaxis()->SetTitle("minDPhi(jet123,MET)");
  hdp_calo.GetYaxis()->SetTitle("Events");
  hdp_calo.GetYaxis()->SetRangeUser(0.000001,1e5);
  hdp_pf.Draw("SAMES");
  leg_dp->Draw();
  C_dp->SetLogy(logTrue);
  gPad->Update();
  TPaveStats *st = (TPaveStats*)hdp_pf.GetListOfFunctions()->FindObject("stats");
  st->SetY1NDC(.45);
  st->SetY2NDC(.7);
  TPaveStats *st2 = (TPaveStats*)hdp_calo.GetListOfFunctions()->FindObject("stats"); 
  st2->SetY1NDC(.72);
  st2->SetY2NDC(.97);
  C_dp->Modified();



  C_x->cd();
  hx_calo.Draw();
  hx_calo.GetXaxis()->SetTitle("MET");
  hx_calo.GetYaxis()->SetTitle("Events");
  hx_calo.GetXaxis()->SetRangeUser(0,500);
  hx_calo.GetYaxis()->SetRangeUser(0.000001,2e5);
  hx_pf.Draw("SAMES");
  leg_x->Draw();
  C_x->SetLogy(logTrue);
  gPad->Update();
  TPaveStats *st3 = (TPaveStats*)hx_pf.GetListOfFunctions()->FindObject("stats");
  st3->SetY1NDC(.45);
  st3->SetY2NDC(.7);
  TPaveStats *st4 = (TPaveStats*)hx_calo.GetListOfFunctions()->FindObject("stats"); 
  st4->SetY1NDC(.72);
  st4->SetY2NDC(.97);
  C_x->Modified();
  

  hx_pf.Write();
  hx_calo.Write();
  hdp_pf.Write();
  hdp_calo.Write();
  C_x->Write();
  C_dp->Write(); 
  fout.Close();

}
