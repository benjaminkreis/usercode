#include "TFile.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TAxis.h"
#include "TMatrixT.h"
#include "TVectorT.h"
#include <iostream>
#include <vector>
#include <exception>
#include <cmath> 
#include <iomanip>
#include <assert.h>

void analyzeABCD(){
  
  TString histname = "H_minDPhi_MET";
  
  TFile* file1 = TFile::Open("plots_LM0_nBJetsLT2.root","READ");
  TH2F* hist1 = (TH2F*) gDirectory->Get(histname);
  
  TAxis* xax =  hist1->GetXaxis();
  TAxis* yax =  hist1->GetYaxis();
  cout << "x axis from " << xax->GetXmin() <<" to " <<  xax->GetXmax()<< endl;;
  cout << "y axis from " << yax->GetXmin() << " to " << yax->GetXmax()<< endl;;
  
  
  float pi=4*atan(1.0);
  
  int nA=0, nB=0, nC=0, nD=0;
  
  float borderv1a=0.0;
  float borderv1b=150.0;
  float borderv2a=150.0;
  float borderv2b=1000.0;
  float borderh1a=0.0;
  float borderh1b=0.3;
  float borderh2a=0.3;
  float borderh2b=pi;
    
  int nBinsX =  hist1->GetNbinsX();
  int nBinsY =  hist1->GetNbinsY();
  
  for(int xbin = 1; xbin<= nBinsX; xbin++){
    for(int ybin = 1; ybin<= nBinsY; ybin++){
      float xlow =  xax->GetBinLowEdge(xbin);
      float xup =  xax->GetBinUpEdge(xbin); 
      float ylow = yax->GetBinLowEdge(ybin);
      float yup =  yax->GetBinUpEdge(ybin);
      
      cout << "xlow " << xlow << endl;
      cout << "ylow " << ylow << endl;

      if( (xlow>=borderv1a) && (xlow<borderv1b) && (ylow>=borderh1a) && (ylow<borderh1b) ){
	nA+=hist1->GetBinContent(xbin,ybin);
      }
      else if( (xlow>=borderv2a) && (xlow<borderv2b) && (ylow>=borderh1a) && (ylow<borderh1b) ){
	nD+=hist1->GetBinContent(xbin,ybin);
      }
      else if( (xlow>=borderv1a) && (xlow<borderv1b) && (ylow>=borderh2a) && (ylow<borderh2b) ){
	nB+=hist1->GetBinContent(xbin,ybin);
      }
      else if( (xlow>=borderv2a) && (xlow<borderv2b) && (ylow>=borderh2a) && (ylow<borderh2b) ){
	nC+=hist1->GetBinContent(xbin,ybin);
      }
      else { assert(0);}
      
    }//end loop over ybin
    std::cout<<endl;
  }//end loop over xbin
  std::cout<<endl;
    

  float classicalEstimate = nD*nB/nA;

  cout << "nA: " << nA << endl;
  cout << "nB: " << nB << endl;
  cout << "nC: " << nC << endl;
  cout << "nD: " << nD << endl;
  cout << "classical estimate for nC: " << classicalEstimate << endl;

}//0
