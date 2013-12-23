#include "drawTrees.h"

#include <iostream>

using namespace std;
using namespace RooFit;

void hbb_zll_lheDistributions(){
  
  sampleVector.clear();
  
  sample sample0("g4eq0_g1eq1");
  sample0.humanName = "g_{1}=1, g_{4}=0";
  sample0.color = kAzure-3;
  sampleVector.push_back(sample0);

  sample sample1("g4eq1_g1eq0");
  sample1.humanName = "g_{1}=0, g_{4}=1";
  sample1.color = kRed;
  sampleVector.push_back(sample1);

  sample sample3("g4eq0p142_g1eq1");
  sample3.humanName = "g_{1}=1, g_{4}=0.142";
  sample3.color = kViolet+2;
  sampleVector.push_back(sample3);


  drawPlots("mZ", 20, 70, 110, "m_{Z}", "Events");
  drawPlots("mH", 20, 125.4, 125.8, "m_{H}", "Events");
  drawPlots("mZH", 20, 150, 900, "m_{ZH}", "Events");

  drawPlots("costheta1", 20, -1, 1, "cos#theta_{1}", "Events");
  drawPlots("costheta2", 20, -1, 1, "cos#theta_{2}", "Events");
  drawPlots("costhetastar", 20, -1, 1, "cos#theta*", "Events");
  drawPlots("phi", 20, -3.142, 3.142, "#Phi", "Events");
  drawPlots("phi1", 20, -3.142, 3.142, "#Phi_{1}", "Events");

}


void hbb_zll_lhePDFOverlay(){


  ///////////////////////////////////////
  //    ANALYTIC
  ///////////////////////////////////////
  



  //////////////////////////////////////
  //    LHE DATA POINTS
  //////////////////////////////////////


  sampleVector.clear();
  
  sample sample0("g4eq0_g1eq1");
  sample0.humanName = "g_{1}=1, g_{4}=0";
  sample0.color = kAzure-3;
  sampleVector.push_back(sample0);

  sample sample1("g4eq1_g1eq0");
  sample1.humanName = "g_{1}=0, g_{4}=1";
  sample1.color = kRed;
  sampleVector.push_back(sample1);

  sample sample3("g4eq0p142_g1eq1");
  sample3.humanName = "g_{1}=1, g_{4}=0.142";
  sample3.color = kViolet+2;
  sampleVector.push_back(sample3);

  drawPlots("mZH", 20, 150, 900, "m_{ZH}", "Events");
  
  TCanvas * c_mZH = new TCanvas("c_mZH", "c_mZH", 500, 500);
  c_mZH->cd();
  TH1D* h_mZH_g1 = histos["g4eq0_g1eq1"];
  h_mZH_g1->Draw("P0 E1");


}
