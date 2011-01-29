
#include <iostream>
#include <iomanip>
#include <assert.h>


#include "TROOT.h"
#include "TStyle.h"

#include "TChain.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TF1.h"
#include "TGraphErrors.h"

#include "analyzeFillABCDInput.h"

using namespace std;

//make sure the following things are set: cut passb function, subtractSM bool, 


bool passb(int nbtags){
  if(nbtags>=2){
    return true;
  }
  else{
    return false;
  }
}



double *doBasicABCD(double borderv1a = 0., double borderv1b = 0., int fitNum = 0.){
  bool verbose = true;
  bool subtractSM = true;

  double SMfactor = 1.0; 
  double pi=4*atan(1.0)+.0001;
  double borderv2a=150.0;
  double borderv2b=1e10;
  double borderh1a=0.0;
  double borderh1b=0.3;
  double borderh2a=0.3;
  double borderh2b=pi;

  const Double_t yBinsL[2] = {borderh1a, borderh1b};
  const Double_t yBinsU[2] = {borderh2a, borderh2b};
  const Double_t yBinsT[2] = {0., pi};

  double xWidthL = (borderv1b-borderv1a)/(double)fitNum;
  Double_t xBinsL[fitNum+1];
  if(verbose) cout << "xBinsL: ";
  for(int i = 0; i<=fitNum; i++){
    xBinsL[i]=borderv1a+i*xWidthL;
    if(verbose)cout << xBinsL[i] << " ";
  }
  if(verbose) cout << endl;
  
  TH2D* histA = new TH2D("H_A", "Region A", fitNum, xBinsL, 1, yBinsL);
  TH2D* histAm = new TH2D("H_Am", "Region Am", fitNum, xBinsL, 1, yBinsT);
  TH2D* histB = new TH2D("H_B", "Region B", fitNum, xBinsL, 1, yBinsU);
  histA->Sumw2();
  histAm->Sumw2();
  histB->Sumw2();
 

  TChain* InputChain = FormChainJosh("pfpf");
  TChain* InputChainSM = 0;
  if(subtractSM) InputChainSM = FormChainJoshSM("pfpf");
  int numEntries = InputChain->GetEntries();
  int numEntriesSM = InputChainSM->GetEntries(); 
  if(verbose)cout <<"numEntries: " << numEntries << endl;
  if(verbose)cout <<"numEntriesSM: " << numEntriesSM << endl;

  double x, y, weight;
  double xSM, ySM, weightSM;
  int nbtags;
  int nbtagsSM;
  InputChain->SetBranchAddress("MET",&x);
  InputChain->SetBranchAddress("minDeltaPhiMET",&y);
  InputChain->SetBranchAddress("weight",&weight);
  InputChain->SetBranchAddress("nbSSVM",&nbtags);
  InputChainSM->SetBranchAddress("MET",&xSM);
  InputChainSM->SetBranchAddress("minDeltaPhiMET",&ySM);
  InputChainSM->SetBranchAddress("weight",&weightSM);
  InputChainSM->SetBranchAddress("nbSSVM",&nbtagsSM);

  for(int i = 0; i<numEntries; i++){
    InputChain->GetEvent(i);
    if(!passb(nbtags)) continue;

    histA->Fill(x,y, weight);
    histB->Fill(x,y, weight);
    histAm->Fill(x,y,x*weight);
  }//end loop over InputChain
 
  if(subtractSM){
    for(int i = 0; i<numEntriesSM; i++){
      InputChainSM->GetEvent(i);
      if(!passb(nbtagsSM)) continue;

      histA->Fill(xSM,ySM, -weightSM*SMfactor);
      histB->Fill(xSM,ySM, -weightSM*SMfactor);
      histAm->Fill(xSM,ySM,-xSM*weightSM*SMfactor);
    }
  }

  ////////////////////
  //Graph to fit
  ///////////////////
  double gr1x[fitNum];
  double gr1y[fitNum];
  double gr1x_error[fitNum];
  double gr1y_error[fitNum];
  double smallNum = 1.e-5;
  for(int i =0; i<fitNum; i++){
    assert( histB->GetBinContent(i+1,1)>smallNum && histA->GetBinContent(i+1,1)>smallNum);
    
    gr1x[i] = (histAm->GetBinContent(i+1,1))/(histA->GetBinContent(i+1,1)+histB->GetBinContent(i+1,1)); 
    gr1x_error[i] = 0.;
    
    gr1y[i] = histB->GetBinContent(i+1,1)/histA->GetBinContent(i+1,1);
    gr1y_error[i] = gr1y[i]*sqrt((histB->GetBinError(i+1,1)/histB->GetBinContent(i+1,1))*(histB->GetBinError(i+1,1)/histB->GetBinContent(i+1,1))+
				 (histA->GetBinError(i+1,1)/histA->GetBinContent(i+1,1))*(histA->GetBinError(i+1,1)/histA->GetBinContent(i+1,1)));
    
    if(verbose)cout << "ratio: (" << gr1x[i] << " +- " << gr1x_error[i] <<", " << gr1y[i] << " +- " << gr1y_error[i] << ")" << endl;
  }//end fill arrays for graph to fit
  TGraphErrors * gr1 = new TGraphErrors(fitNum, gr1x, gr1y, gr1x_error, gr1y_error);  


  ///////////////////
  //Fit
  //////////////////
  TF1 *fexp2 = new TF1("fexp2", "[0]*exp([1]*x)+[2]", borderv1a, borderv1b);
  fexp2->SetParameters(10.0, -1.0/30.0, 0.001);  
  fexp2->SetParLimits(2,0.,1000);
  assert(!( gr1->Fit("fexp2", "R0") ));
 
  Double_t par_exp2[3];  
  fexp2->GetParameters(par_exp2);
  

  ///////////////////
  //Extrap
  ///////////////////
  double unbinnedEstimate2 = 0.;
  double ext_serror_unBinexp2 = 0.;
  double ext_serror_exp2_w = 0.;
  double ext_serror_exp2_p0 = 0.;
  double ext_serror_exp2_p1 = 0.;
  double ext_serror_exp2_p2 = 0.;

  for(int i = 0; i<numEntries; i++){
    InputChain->GetEvent(i);
    if(!passb(nbtags)) continue;

    //if in Region D
    if( (x>=borderv2a) && (x<borderv2b) && (y>=borderh1a) && (y<borderh1b)){
      double xvalue = x;
      double xError=0.;
      double eventError = weight;
      
      unbinnedEstimate2+=weight*(par_exp2[0]*exp(par_exp2[1]*xvalue)+par_exp2[2]);

      //terms for error prop
      ext_serror_exp2_p0 += exp(par_exp2[1]*xvalue)*(weight)*(fexp2->GetParError(0));
      ext_serror_exp2_p1 += par_exp2[0]*xvalue*exp(par_exp2[1]*xvalue)*(weight)*(fexp2->GetParError(1));
      ext_serror_exp2_p2 += weight*(fexp2->GetParError(2));
      ext_serror_exp2_w  += (par_exp2[0]*exp(par_exp2[1]*xvalue)+par_exp2[2])*(eventError) *  (par_exp2[0]*exp(par_exp2[1]*xvalue)+par_exp2[2])*(eventError);
      
    }//end if in Region D
    assert(x<borderv2b);
  }//end loop over InputChain
  
  for(int i = 0; i<numEntriesSM; i++){
    InputChainSM->GetEvent(i);
    if(!passb(nbtagsSM)) continue;
   
    //if in Region D 
    if( (xSM>=borderv2a) && (xSM<borderv2b) && (ySM>=borderh1a) && (ySM<borderh1b)){
      double xvalue = xSM;
      double xError=0.;
      double eventError = weightSM*SMfactor;

      unbinnedEstimate2+=-weightSM*SMfactor*(par_exp2[0]*exp(par_exp2[1]*xvalue)+par_exp2[2]);

      //terms for error prop 
      ext_serror_exp2_p0 += exp(par_exp2[1]*xvalue)*(-weightSM*SMfactor)*(fexp2->GetParError(0));
      ext_serror_exp2_p1 += par_exp2[0]*xvalue*exp(par_exp2[1]*xvalue)*(-weightSM*SMfactor)*(fexp2->GetParError(1));
      ext_serror_exp2_p2 += -weightSM*SMfactor*(fexp2->GetParError(2));
      ext_serror_exp2_w  += (par_exp2[0]*exp(par_exp2[1]*xvalue)+par_exp2[2])*(eventError) *  (par_exp2[0]*exp(par_exp2[1]*xvalue)+par_exp2[2])*(eventError);

    }//end if in Region D 
    assert(x<borderv2b);
  }//end loop over InputChain      
  


  double ext_error_unBinexp2;
  ext_error_unBinexp2 = sqrt(ext_serror_exp2_p0*ext_serror_exp2_p0 + ext_serror_exp2_p1*ext_serror_exp2_p1 + ext_serror_exp2_p2*ext_serror_exp2_p2 + ext_serror_exp2_w);  
  
  cout << "Unbinned exponential+constant extended estimate for nC: " << unbinnedEstimate2 << " +- " << ext_error_unBinexp2 << endl;
  
  histA->Clear();
  histAm->Clear();
  histB->Clear();

  double *result = new double[2];
  result[0] = unbinnedEstimate2;
  result[1] = ext_error_unBinexp2;
}//end doBasicABCD




void basicABCD(){
  gROOT->SetStyle("CMS");
  gStyle->SetOptFit(1);
  cout << "begin basicABCD" << endl;

  double *a0 = doBasicABCD(0.,90.,10);
  double *a1 = doBasicABCD(10.,90.,10);
  double *a2 = doBasicABCD(20.,90.,10);
  double *a3 = doBasicABCD(30.,90.,10);
  double *a4 = doBasicABCD(40.,90.,10);
  double *a5 = doBasicABCD(50.,90.,10);
  double *a6 = doBasicABCD(10.,70.,10);
  double *a7 = doBasicABCD(10.,80.,10);
  double *a8 = doBasicABCD(10.,100.,10);
  double *a9 = doBasicABCD(10.,110.,10);
  double *a10 = doBasicABCD(10.,90.,5);
  double *a11 = doBasicABCD(10.,90.,20);
  
  cout << endl;
  cout << endl;
  cout << "nob" << endl;
  cout << a0[0] << " +/- " << a0[1] << endl;
  cout << a1[0] << " +/- " << a1[1] << endl;
  cout << a2[0] << " +/- " << a2[1] << endl;
  cout << a3[0] << " +/- " << a3[1] << endl;
  cout << a4[0] << " +/- " << a4[1] << endl;
  cout << a5[0] << " +/- " << a5[1] << endl;
  cout << a6[0] << " +/- " << a6[1] << endl;
  cout << a7[0] << " +/- " << a7[1] << endl;
  cout << a8[0] << " +/- " << a8[1] << endl;
  cout << a9[0] << " +/- " << a9[1] << endl;
  cout << a10[0] << " +/- " << a10[1] << endl;
  cout << a11[0] << " +/- " << a11[1] << endl;

}
