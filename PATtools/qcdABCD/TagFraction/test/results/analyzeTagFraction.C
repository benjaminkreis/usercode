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

    
  //INPUT
  TFile finweight("weight_QCD.root");
  TH1D* Hweight = 0;
  if(!finweight.IsZombie()){
    Hweight = (TH1D*)finweight.Get("Hweight");
  }
  else{
    cout << "ERROR loading weights" << endl;
  }
    

  TChain* InputChain = FormChain(); 
  int fillNum = 0;
  int numEntries = InputChain->GetEntries();
  cout <<"numEntries: " << numEntries << endl;
  
  float P2Tot = 0, dP2Tott = 0, Pge2Tot = 0,dPge2Tott = 0, Pge3Tot=0, dPge3Tott = 0;

  float pthat, P2, dP2, Pge2, dPge2, Pge3, dPge3;
  InputChain->SetBranchAddress("PtHat", &pthat);
  InputChain->SetBranchAddress("P2", &P2);
  InputChain->SetBranchAddress("dP2", &dP2);
  InputChain->SetBranchAddress("Pge2", &Pge2);
  InputChain->SetBranchAddress("dPge2", &dPge2);
  InputChain->SetBranchAddress("Pge3", &Pge3);
  InputChain->SetBranchAddress("dPge3", &dPge3);
  
  for(int i = 1; i<=numEntries; i++){
    InputChain->GetEvent(i);
    
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

}
