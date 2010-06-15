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

#include "analyzeTagFractionInput.h"

using namespace std;

void analyzeTagFraction(){
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptStat("euo");
    
  //INPUT
  TFile finweight("weight_QCD.root");
  TH1D* Hweight = 0;
  if(!finweight.IsZombie()){
    Hweight = (TH1D*)finweight.Get("Hweight");
  }
  else{
    cout << "ERROR loading weights" << endl;
  }
    

  TFile fout("TagFraction_plots.root", "RECREATE");
  TCanvas *C_P0 = new TCanvas("C_P0", "P0", 640, 480);
  TCanvas *C_P1 = new TCanvas("C_P1", "P1", 640, 480);
  TCanvas *C_P2 = new TCanvas("C_P2", "P2", 640, 480);
  TCanvas *C_Pge2 = new TCanvas("C_Pge2", "Pge2", 640, 480);
  TCanvas *C_Pge3 = new TCanvas("C_Pge3", "Pge3", 640, 480);
  TCanvas *C_dP0 = new TCanvas("C_dP0", "dP0", 640, 480);
  TCanvas *C_dP1 = new TCanvas("C_dP1", "dP1", 640, 480);
  TCanvas *C_dP2 = new TCanvas("C_dP2", "dP2", 640, 480);
  TCanvas *C_dPge2 = new TCanvas("C_dPge2", "dPge2", 640, 480);
  TCanvas *C_dPge3 = new TCanvas("C_dPge3", "dPge3", 640, 480);

  TChain* InputChain = FormChain(); 
  int fillNum = 0;
  int numEntries = InputChain->GetEntries();
  cout <<"numEntries: " << numEntries << endl;
  
  float P2Tot = 0, dP2Tott = 0, Pge2Tot = 0,dPge2Tott = 0, Pge3Tot=0, dPge3Tott = 0;

  float pthat, P0, P1, P2, dP2, Pge2, dPge2, Pge3, dPge3;
  InputChain->SetBranchAddress("PtHat", &pthat);
  InputChain->SetBranchAddress("P0", &P0);
  InputChain->SetBranchAddress("P1", &P1);
  InputChain->SetBranchAddress("P2", &P2);
  InputChain->SetBranchAddress("dP2", &dP2);
  InputChain->SetBranchAddress("Pge2", &Pge2);
  InputChain->SetBranchAddress("dPge2", &dPge2);
  InputChain->SetBranchAddress("Pge3", &Pge3);
  InputChain->SetBranchAddress("dPge3", &dPge3);
  
  C_P0->cd();
  InputChain->Draw("P0");
  C_P1->cd();
  InputChain->Draw("P1");
  C_P2->cd();
  InputChain->Draw("P2");
  C_Pge2->cd();
  InputChain->Draw("Pge2");
  C_Pge3->cd();
  InputChain->Draw("Pge3");
  C_dP0->cd();
  InputChain->Draw("dP0");
  C_dP1->cd();
  InputChain->Draw("dP1");
  C_dP2->cd();
  InputChain->Draw("dP2");
  C_dPge2->cd();
  InputChain->Draw("dPge2");
  C_dPge3->cd();
  InputChain->Draw("dPge3");

  for(int i = 1; i<=numEntries; i++){
    InputChain->GetEvent(i);
    
    if(P0<0 || P1<0 || P2<0 || Pge2<0 || Pge3<0){
      cout << P0 << " " << P1 << " " << P2  << " " << Pge2 << " " << Pge3 << endl;
    }

    //get weight
    int bin = Hweight->FindBin(pthat);
    double eventweight=Hweight->GetBinContent(bin);

    P2Tot += P2*eventweight;
    dP2Tott += dP2*dP2*eventweight*eventweight;
    Pge2Tot += Pge2*eventweight;
    dPge2Tott += dPge2*dPge2*eventweight*eventweight;
    Pge3Tot += Pge3*eventweight;
    dPge3Tott += dPge3*dPge3*eventweight*eventweight;

   }
  
  std::cout << "P2Tot: " << P2Tot << " +- " << sqrt(dP2Tott) << std::endl;
  std::cout << "Pge2Tot: " << Pge2Tot << " +- " << sqrt(dPge2Tott) << std::endl;
  std::cout << "Pge3Tot: " << Pge3Tot << " +- " << sqrt(dPge3Tott) << std::endl;  

  C_P0->Write();
  C_P1->Write();
  C_P2->Write();
  C_Pge2->Write();
  C_Pge3->Write();
  C_dP0->Write();
  C_dP1->Write();
  C_dP2->Write();
  C_dPge2->Write();
  C_dPge3->Write();
  C_P0->Close();
  C_P1->Close();
  C_P2->Close();
  C_Pge2->Close();
  C_Pge3->Close();
  C_dP0->Close();
  C_dP1->Close();
  C_dP2->Close();
  C_dPge2->Close();
  C_dPge3->Close();
  fout.Write();
}
