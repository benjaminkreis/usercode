#include "TMinuit.h"
#include "TLatex.h"
#include "TFile.h"
#include "TChain.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TF1.h"
#include "TF2.h"
#include "TFormula.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TAxis.h"
#include "TMatrixT.h"
#include "TVectorT.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TROOT.h"
#include "TFitter.h"
#include "TMultiGraph.h"
#include <iostream>
#include <vector>
#include <exception>
#include <cmath> 
#include <iomanip>
#include <assert.h>

#include "analyzeFillABCDInput.h"
#include "Dfrac.C"

using namespace std;

bool bcontinue(int nbtags, TString bcut){
  bool returnTrue = false; 
  if(bcut=="eq1"){
    if(nbtags==1) returnTrue = true;
  }
  else if(bcut=="ge1"){
    if(nbtags>=1) returnTrue = true;
  }
  else if(bcut=="ge2"){
    if(nbtags>=2) returnTrue = true;
  }
  else{assert(0);}
  return returnTrue;
}

bool bcontinue(int nbtags, int min){
  
  if(nbtags==min){
    return true;
  }
  else{
    return false;
  }
}

int passbtagcut(int nbtags){//just used for Dfrac
  if(nbtags>=2){
    return 1;
  }
  else{
    return 0;
  }
}

double abError(double a, double aE, double b, double bE){

  double Err = 0;
  Err = sqrt( a*a*bE*bE + b*b*aE*aE);

  return Err;
}


double aObError(double a, double aE, double b, double bE){

  double Err = 0;
  Err = a/b*sqrt( aE*aE/(a*a)+bE*bE/(b*b) );

  return Err;
}

double *doAnalyzeFillABCD(TString joshType = "calo", TString bcont="", double borderv1a = 50, double borderv1b = 150, int fitNum = 6, bool verbose = false, TString fName = ""){
  cout << endl;
  if(verbose)cout << "Begin analyzeFillABCD" << endl;
  //gROOT->SetStyle("Plain");
  //gStyle->SetPalette(1);
  //gStyle->SetOptStat("nemruo");
  //gStyle->SetOptStat("");
  gROOT->SetStyle("CMS");
  gStyle->SetOptFit(1);

  bool josh=true;
  bool subtractSM =false;
  double SMfactor =1.0;
  bool useHTcut = false; //assumes HT>300 has already been applied (important for extrapolation aka points in D)
  double HTcut = 500.;
  bool drawLines=false;
  double singleLow=100;
  double singleHigh=150;
  
  // int fitNum = 10; //number of bins in xL region, used to fit ratio
  int extendedNum = 11; //number of bins in xR region, used in extrapolation
  TString xLabel = "MET";
  
  //LOAD WEIGHTS
  TFile finweight("/afs/cern.ch/user/k/kreis/scratch0/CMSSW_3_6_3/src/Filter/RA2Filter/test/results/weight_MG.root","READ");
  
  TH1D* HweightP = 0;
  if(!finweight.IsZombie()){
    HweightP = (TH1D*)finweight.Get("Hweight");
  }
  else{
    cout << "ERROR loading weights" << endl;
  }
  TFile fout("plots"+fName+".root", "RECREATE");
  TH1D Hweight = *HweightP;
  finweight.Close();

  TString yTitle = "#Delta #phi_{min}";
  TString yTitle_ratio = "r(E_{T}^{miss}) ";
  TString xTitle = "E_{T}^{miss} [GeV]";
  
  //double pi=4*atan(1.0);
  double pi = 3.142;


  //double borderv1a=80.0;
  //double  borderv1b=90.0;
  double borderv2a=150.0;
  double borderv2b=1000000.;
  double borderv2b_plot=450;
  double borderh1a=0.0;
  double borderh1b=0.3;
  double borderh2a=0.3;
  double borderh2b=pi;

  double ConstMETCut = 150.;
  double ConstU = 0., ConstU_e=0., ConstL=0., ConstL_e=0.;
 
  double nA=0., nB=0., nC=0., nD=0., nCextra=0.;
  double nA_e=0., nB_e=0., nC_e=0., nD_e=0., nCextra_e=0.; 
  double nOutside=0.;
  ///////////////////
  //    CANVAS     //
  ///////////////////
  TCanvas * C_reweight = new TCanvas("C_reweight", "Canvas reweight", 640, 2*480);
  C_reweight->Divide(1,2);
  C_reweight->cd(1);
  TCanvas * C_contribute = new TCanvas("C_contribute", "Canvas contributions", 2*640, 2*480);
  C_contribute->Divide(2,2);
  C_contribute->cd(1);
  TCanvas * C_Data = new TCanvas("C_Data", "Canvas Data");
  C_Data->cd();
  TCanvas * C_extrap = new TCanvas("C_extrap", "Canvas ratio", 800, 700);
  C_extrap->Divide(1,1);
  C_extrap->cd(1);
  TCanvas * C_fit = new TCanvas("C_fit", "Canvas fit", 800, 700);
  C_fit->Divide(1,1);
  C_fit->cd(1);
  TCanvas * C_ABCD = new TCanvas("C_ABCD", "Canvas ABCD", 1000, 1000);
  C_ABCD->Divide(2,2);
  C_ABCD->cd(1);
  TCanvas * C_hist1 = new TCanvas("C_hist1", "Canvas hist1");//, 800, 700);
  C_hist1->Divide(1,1);
  C_hist1->cd(1);
  TCanvas * C_dp = new TCanvas("C_dp", "Canvas dp", 640, 480);
  C_dp->cd();
  TCanvas * C_x = new TCanvas("C_x", "Canvas x", 640, 480);
  C_x->cd();
  TCanvas *C_temp = new TCanvas("C_temp", "C_temp", 640, 480);
  C_temp->cd();
  TCanvas *C_temp2 = new TCanvas("C_temp2", "C_temp2", 640, 480);
  C_temp2->cd();
  TCanvas *C_UL = new TCanvas("C_UL", "C_UL");
  C_UL->cd();
  TCanvas *C_hpy = new TCanvas("C_hpy", "C_hpy", 640, 480);
  C_hpy->cd();
  TCanvas *C_hpx = new TCanvas("C_hpx", "C_hpx", 640, 480);
  C_hpx->cd();


  /////////////////////////////
  //        HISTOGRAMS       //
  /////////////////////////////

  //for weight
  TH1D* histPtHat = new TH1D("histPtHat","Pt-Hat",8000,0,2000);
  TH1D* histPtHat_noW = new TH1D("histPtHat_noW","Pt-Hat",8000,0,2000);
  
  //prep for histograms A, B, C, D
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
  cout << endl;
  
  //double xWidthU = (borderv2b_plot-borderv2a)/(double)extendedNum;
  double xWidthU = (borderv2b_plot-borderv1b)/(double)extendedNum;
  Double_t xBinsU[extendedNum+1];
  if(verbose)cout << "xBinsU: ";
  for(int i = 0; i<=extendedNum; i++){
    //xBinsU[i]=borderv2a+i*xWidthU;
    xBinsU[i]=borderv1b+i*xWidthU;
    if(verbose)cout << xBinsU[i] << " ";
  }
  cout << endl;
  
  // histogram for A, B, C, D
  TH2D* hist1 = new TH2D("H_ABCD", "ABCD", 100,0,1000,160,0,pi);
  TH2D* hist2 = new TH2D("H_ABCD2", "ABCD2", 100,0,500,100,0,pi);
  TH2D* hist3 = new TH2D("H_ABCD3", "ABCD3", 90,50,500,100,0,pi);
  
  //essential
  TH2D* histA = new TH2D("H_A", "Region A", fitNum, xBinsL, 1, yBinsL);
  TH2D* histAm = new TH2D("H_Am", "Region Am", fitNum, xBinsL, 1, yBinsT);
  TH2D* histB = new TH2D("H_B", "Region B", fitNum, xBinsL, 1, yBinsU);
  TH2D* histAs = new TH2D("H_As", "Region A", fitNum, xBinsL, 1, yBinsL);
  TH2D* histAms = new TH2D("H_Ams", "Region Am", fitNum, xBinsL, 1, yBinsT);
  TH2D* histBs = new TH2D("H_Bs", "Region B", fitNum, xBinsL, 1, yBinsU);
  TH2D* histASM = new TH2D("H_ASM", "Region A", fitNum, xBinsL, 1, yBinsL);
  TH2D* histAmSM = new TH2D("H_AmSM", "Region Am", fitNum, xBinsL, 1, yBinsT);
  TH2D* histBSM = new TH2D("H_BSM", "Region B", fitNum, xBinsL, 1, yBinsU);
  TH2D* histAd = new TH2D("H_Ad", "Region A", fitNum, xBinsL, 1, yBinsL);
  TH2D* histAmd = new TH2D("H_Amd", "Region Am", fitNum, xBinsL, 1, yBinsT);
  TH2D* histBd = new TH2D("H_Bd", "Region B", fitNum, xBinsL, 1, yBinsU);
  
  TH2D* histC = new TH2D("H_C", "Region C", extendedNum, xBinsU, 1, yBinsU);
  TH2D* histD = new TH2D("H_D", "Region D", extendedNum, xBinsU, 1, yBinsL);
  
  TH2D*  histSU = new TH2D("histSU", "histSU", 1, singleLow, singleHigh, 1, borderh2a, borderh2b);
  TH2D*  histSL = new TH2D("histSL", "histSL", 1, singleLow, singleHigh, 1, borderh1a, borderh1b);
  const int nULbins=13;
  const Double_t ULbins[] = {0,20,40,60,80,100,120,140,160,180,200,240,280,400};
  TH1D* histU = new TH1D("histU", "histU", nULbins, ULbins);
  TH1D* histL = new TH1D("histL", "histL", nULbins, ULbins);
  TH1D* histUL = new TH1D("histUL", "histUL", nULbins, ULbins);
  //TH1D* histU = new TH1D("histU", "histU", 60,borderv1a, borderv2b_plot); 
  //TH1D* histL = new TH1D("histL", "histL", 60,borderv1a, borderv2b_plot); 
  //TH1D* histUL = new TH1D("histUL", "histUL", 60,borderv1a, borderv2b_plot); 
  TH1D* histUSM = new TH1D("histUSM", "histUSM", 60,borderv1a, borderv2b_plot); 
  TH1D* histLSM = new TH1D("histLSM", "histLSM", 60,borderv1a, borderv2b_plot); 
  TH1D* histULSM = new TH1D("histULSM", "histULSM", 60,borderv1a, borderv2b_plot); 

  TH2D* histDm = new TH2D("H_Dm", "Region Dm", extendedNum, xBinsU, 1, yBinsL);
  TH1D* histCx = new TH1D("H_Cx", "Region C - "+xLabel, extendedNum, borderv2a, borderv2b_plot);
  TH1D* histDx = new TH1D("H_Dx", "Region D - "+xLabel, extendedNum, borderv2a, borderv2b_plot);
  TH1D* histdp = new TH1D("H_dp", "dp", 50, borderh1a, borderh2b);
  TH1D* histx = new TH1D("H_x", "x", 200, 0, borderv2b_plot);
  histA->Sumw2();
  histAm->Sumw2();
  histB->Sumw2();
  histAs->Sumw2();
  histAms->Sumw2();
  histBs->Sumw2();
  histASM->Sumw2();
  histAmSM->Sumw2();
  histBSM->Sumw2();
  histAd->Sumw2();
  histAmd->Sumw2();
  histBd->Sumw2();
  histC->Sumw2();
  histD->Sumw2();
  histCx->Sumw2();
  histDx->Sumw2();
  histdp->Sumw2();
  histx->Sumw2();
  histSL->Sumw2();
  histSU->Sumw2();
  histU->Sumw2();
  histL->Sumw2();
  histUL->Sumw2();
  histUSM->Sumw2();
  histLSM->Sumw2();
  histULSM->Sumw2();
  TAxis *xaxA = histA->GetXaxis();
  TAxis *xaxB = histB->GetXaxis();
  TAxis *xaxC = histC->GetXaxis();
  TAxis *xaxD = histD->GetXaxis();
  TAxis *xaxCx = histCx->GetXaxis();
  TAxis *xaxDx = histDx->GetXaxis();
  TAxis *yaxA = histA->GetYaxis();
  TAxis *yaxB = histB->GetYaxis();
  TAxis *yaxC = histC->GetYaxis();
  TAxis *yaxD = histD->GetYaxis();
  TAxis *yaxCx = histCx->GetYaxis();
  TAxis *yaxDx = histDx->GetYaxis();
  xaxA->SetTitle(xTitle);
  xaxB->SetTitle(xTitle);
  xaxC->SetTitle(xTitle);
  xaxD->SetTitle(xTitle);
  xaxCx->SetTitle(xTitle);
  xaxDx->SetTitle(xTitle);
  yaxA->SetTitleOffset(1.4);
  yaxB->SetTitleOffset(1.15);
  yaxC->SetTitleOffset(1.15);
  yaxD->SetTitleOffset(1.4);
  yaxA->SetTitle(yTitle);
  yaxB->SetTitle(yTitle);
  yaxC->SetTitle(yTitle);
  yaxD->SetTitle(yTitle);
  yaxCx->SetTitle("N");
  yaxDx->SetTitle("N");
  
  TH1D* histWD = new TH1D("histWD", "histWD", 9, 0.5, 9.5);
  TH1D* histWC = new TH1D("histWC", "histWC", 9, 0.5, 9.5);
  TH1D* histWA = new TH1D("histWA", "histWA", 9, 0.5, 9.5);
  TH1D* histWB = new TH1D("histWB", "histWB", 9, 0.5, 9.5);

  //////////////////////////
  //LOOP over TREE to FILL//
  //////////////////////////
  
  TChain* InputChain = 0;
  TChain* InputChainSM = 0;
  //TChain* InputChain = FormChain170();
  if(josh){
    InputChain = FormChainJosh(joshType);
    if(subtractSM) InputChainSM = FormChainJoshSM(joshType);
  }
  else{
    InputChain = FormChain();
  }
  
  const int firstEntry = 0; 
  //const int numEntries = 1500000;
  const int numEntries = InputChain->GetEntries();
  int numEntriesSM = 0;
  if(subtractSM) numEntriesSM = InputChainSM->GetEntries();
  if(verbose)cout <<"numEntries: " << numEntries << endl;
  if(verbose)cout << "numEntriesSM: " << numEntriesSM << endl;
  cout << endl;

  double pass_x=0, pass_y=0, myHT=-1;
  double Dfrac=0;
  double x,y, MG=1, weightJosh;
  int nbtags=0;
  if(josh){
    if(useHTcut) InputChain->SetBranchAddress("HT",&myHT);
    InputChain->SetBranchAddress("MET",&x);
    InputChain->SetBranchAddress("minDeltaPhiMET",&y);
    InputChain->SetBranchAddress("weight",&weightJosh);
    InputChain->SetBranchAddress("nbSSVM",&nbtags);
  }
  else{
    InputChain->SetBranchAddress("MHT",&x);
    InputChain->SetBranchAddress("minDPhi",&y);
    //InputChain->SetBranchAddress("PtHat", &pthat);
    InputChain->SetBranchAddress("MG",&MG);
    InputChain->SetBranchAddress("btag", &nbtags);
  }

  double maxWeight = 0;
  double beforeAngular = 0;
  for(int i = firstEntry; i<numEntries; i++){
    InputChain->GetEvent(i);
    bool Anow=0, Bnow=0, Cnow=0, Dnow=0;

    //btag requirement
    bool bcontinueNow = bcontinue(nbtags, bcont);
   
    if(bcontinueNow){
      
      //get weight
      //int bin = Hweight.FindBin(pthat);
      int bin = Hweight.FindBin(MG);
      double weight=Hweight.GetBinContent(bin);
      if(josh) weight=weightJosh;
     
 
      //histPtHat->Fill(pthat,weight);
      histPtHat->Fill(MG,weight);
      histPtHat_noW->Fill(MG);
      
      assert(weight>0.);
      if(x>=borderv2a)beforeAngular+=weight;
      if(x>borderv2a) pass_x+=weight;
      if(x>borderv2a && y>borderh2a) pass_y+=weight;

      histx->Fill(x, weight);
      histdp->Fill(y, weight);

      //COUNT nA, nB, nC, nD
      if( (x>=borderv1a) && (x<borderv1b) && (y>=borderh1a) && (y<borderh1b) ){
	Anow=1;
	nA+=weight;
	nA_e+=weight*weight;
	histWA->Fill(bin);
      }
      else if( (x>=borderv2a) && (x<borderv2b) && (y>=borderh1a) && (y<borderh1b) ){
	Dnow=1;
	nD+=weight;
	nD_e+=weight*weight;
	histWD->Fill(bin);
	Dfrac +=((double)passbtagcut(nbtags))*weight;
	//if(weight>1.0) cout << "large weight: " << weight << " at x=" << x << endl;

      }
      else if( (x>=borderv1a) && (x<borderv1b) && (y>=borderh2a) && (y<borderh2b) ){
	Bnow=1;
	nB+=weight;
	nB_e+=weight*weight;
	histWB->Fill(bin);
      }
      else if( (x>=borderv2a) && (x<borderv2b) && (y>=borderh2a) && (y<borderh2b) ){
	Cnow=1;
	nC+=weight;
	nC_e +=weight*weight;
	histWC->Fill(bin);
	if(weight>maxWeight) maxWeight=weight;
      }
      else {
	//cout << "EVENT " << i << " with x = " << x << " and y = " << y << " not in A, B, C, or D." << endl;
	nOutside+=weight;
	//assert(0);
      }

      
      if( (x>=ConstMETCut) && (y>=borderh2a) && (y<borderh2b) ){
	ConstU += weight;
	ConstU_e += weight*weight;
      }
      if( (x>=ConstMETCut) && (y>=borderh1a) && (y<borderh1b) ){
	ConstL += weight;
	ConstL_e += weight*weight;
      }


      if( (x>=borderv2a) && (y>=borderh2a) && !Cnow ){
	nCextra+=weight;
	nCextra_e += weight*weight;
	if(!Cnow && verbose) cout << "nCextra with weight = " << weight << endl;
      }

      if(y>=borderh1a && y<borderh1b) histL->Fill(x,weight);
      if(y>=borderh2a && y<borderh2b) histU->Fill(x,weight);
      if(y>=borderh1a && y<borderh1b) histLSM->Fill(x,weight);
      if(y>=borderh2a && y<borderh2b) histUSM->Fill(x,weight);      

      //Fill histogram A, B, C, D
      hist1->Fill(x,y, weight);
      hist2->Fill(x,y, weight);
      hist3->Fill(x,y, weight);
      histC->Fill(x,y, weight);
      histD->Fill(x,y, weight);
      if(Cnow) histCx->Fill(x,weight);
      if(Dnow) histDx->Fill(x,weight);
      histSU->Fill(x,y, weight);
      histSL->Fill(x,y, weight);
      
      if(!useHTcut || myHT>HTcut){
	//histA->Fill(x,y, weight);
	//histB->Fill(x,y, weight);
	//histAm->Fill(x,y,x*weight);
	histAs->Fill(x,y, weight);
	histBs->Fill(x,y, weight);
	histAms->Fill(x,y,x*weight);
	histAd->Fill(x,y, weight);
	histBd->Fill(x,y, weight);
	histAmd->Fill(x,y,x*weight);
      }
      
      histDm->Fill(x,y,x*weight);
      
    }//end bcontinue
  }//end loop tree
  ConstU_e = sqrt(ConstU_e);
  ConstL_e = sqrt(ConstL_e);
 
  double single = histSU->GetBinContent(1,1)/histSL->GetBinContent(1,1);
  double single_err = aObError( histSU->GetBinContent(1,1), histSU->GetBinError(1,1), histSL->GetBinContent(1,1), histSL->GetBinError(1,1) );

  //SM LOOP///////////////////////////////////////
  //double xSM, ySM, weightSM;
  double subtractFromD = 0;
 if(subtractSM){
    if(josh){
      InputChainSM->SetBranchAddress("MET",&x);
      InputChainSM->SetBranchAddress("minDeltaPhiMET",&y);
      InputChainSM->SetBranchAddress("weight",&weightJosh);
      InputChainSM->SetBranchAddress("nbSSVM",&nbtags);
    }
    else{assert(0);}
    
    for(int i = 0; i<numEntriesSM; i++){
      InputChainSM->GetEvent(i);
      
      //btag requirement
      bool bcontinueNow = bcontinue(nbtags, bcont);
      if(bcontinueNow){
	
	double weight = weightJosh;
	weight=weight*SMfactor;

	if(!useHTcut || myHT>HTcut){
	  histAs->Fill(x,y, -weight);
	  histBs->Fill(x,y, -weight);
	  histAms->Fill(x,y,-x*weight);
	  
	  histASM->Fill(x,y, weight);
	  histBSM->Fill(x,y, weight);
	  histAmSM->Fill(x,y,x*weight);
	}
	if( (x>=borderv2a) && (x<borderv2b) && (y>=borderh1a) && (y<borderh1b) ){
	  subtractFromD +=weight;
	}
	
	
	if(y>=borderh1a && y<borderh1b) histLSM->Fill(x,-weight);
	if(y>=borderh2a && y<borderh2b) histUSM->Fill(x,-weight); 
	
      }//bcontinue
      
    }
    if(verbose)cout << "histAd: " << histAd->GetBinContent(3,1) << " +- "<< histAd->GetBinError(3,1)<< endl;
    if(verbose)cout << "histAs: " << histAs->GetBinContent(3,1) << " +- "<< histAs->GetBinError(3,1)<< endl;
    if(verbose)cout << "histASM: " << histASM->GetBinContent(3,1)  << " +- "<< histASM->GetBinError(3,1)<< endl;
    histA=histAs;
    histB=histBs;
    histAm=histAms;
    
  }
  else{
    histA=histAd;
    histB=histBd;
    histAm=histAmd;
  }
  C_temp->cd();
  gPad->SetLogy(1);
  TH1D* histAd_p = histAd->ProjectionX("histAd_p", 1,1,"e");
  TH1D* histAs_p = histAs->ProjectionX("histAs_p", 1,1,"e");
  TH1D* histASM_p = histASM->ProjectionX("histASM_p", 1,1,"e");
  histAd_p->SetMarkerColor(kBlack);
  histAs_p->SetMarkerColor(kBlue);
  histASM_p->SetMarkerColor(kRed);
  histAd_p->SetMarkerStyle(21);
  histAs_p->SetMarkerStyle(22);
  histASM_p->SetMarkerStyle(20);
  histAd_p->GetYaxis()->SetRangeUser(1, 550000);
  histAd_p->GetYaxis()->SetTitle("Events");
  histAd_p->GetXaxis()->SetTitle(xTitle);
  histAd_p->Draw();
  histAs_p->Draw("SAME");
  histASM_p->Draw("SAME");
 
  TLegend *legSM = new TLegend(.58,.77,.8,.88);
  legSM->AddEntry(histAd_p,"Data", "P");
  legSM->AddEntry(histAs_p,"Data-SM MC", "P");
  legSM->AddEntry(histASM_p,"SM MC", "P");
  legSM->SetFillColor(0);
  legSM->SetBorderSize(0);
  legSM->SetLineStyle(0);
  legSM->SetTextFont(42);
  legSM->SetFillStyle(0);
  legSM->SetTextSize(0.04);
  legSM->Draw();

  C_temp2->cd();
  gPad->SetLogy(1);
  TH1D* histBd_p = histBd->ProjectionX("histBd_p", 1,1,"e");
  TH1D* histBs_p = histBs->ProjectionX("histBs_p", 1,1,"e");
  TH1D* histBSM_p = histBSM->ProjectionX("histBSM_p", 1,1,"e");
  histBd_p->SetMarkerColor(kBlack);
  histBs_p->SetMarkerColor(kBlue);
  histBSM_p->SetMarkerColor(kRed);
  histBd_p->SetMarkerStyle(21);
  histBs_p->SetMarkerStyle(22);
  histBSM_p->SetMarkerStyle(20);
  histBd_p->GetYaxis()->SetRangeUser(1, 550000);
  histBd_p->GetYaxis()->SetTitle("Events");
  histBd_p->GetXaxis()->SetTitle(xTitle);
  histBd_p->Draw();
  histBs_p->Draw("SAME");
  histBSM_p->Draw("SAME");
 
  TLegend *legSM2 = new TLegend(.58,.77,.8,.88);
  legSM2->AddEntry(histBd_p,"Data", "P");
  legSM2->AddEntry(histBs_p,"Data-SM MC", "P");
  legSM2->AddEntry(histBSM_p,"SM MC", "P");
  legSM2->SetFillColor(0);
  legSM2->SetBorderSize(0);
  legSM2->SetLineStyle(0);
  legSM2->SetTextFont(42);
  legSM2->SetFillStyle(0);
  legSM2->SetTextSize(0.04);
  legSM2->Draw();



  ////////////////////////////////////////////////////////////////////
  ////////////////////////// Analysis ////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  histUL->Divide(histU, histL, 1., 1.,"");
  histULSM->Divide(histUSM, histLSM, 1., 1.,"");
  
  C_UL->cd();
  histUL->Draw();
  histULSM->Draw("SAME");
  
  C_x->cd();
  histx->Draw();
  C_dp->cd();
  histdp->Draw();

  //weight histograms
  C_reweight->cd(1);
  gPad->SetLogy(1);
  histPtHat_noW->Draw();
  C_reweight->cd(2);
  gPad->SetLogy(1);
  histPtHat->Draw();

  C_hist1->cd(1);
  TAxis* xax =  hist1->GetXaxis();
  TAxis* yax =  hist1->GetYaxis();
  xax->SetTitle(xTitle);
  xax->SetLabelSize(0.04);
  yax->SetTitle(yTitle);
  
  TLatex* text1 = new TLatex(3.570061,23.08044,"CMS Preliminary");
  text1->SetNDC();
  text1->SetTextAlign(13);
  text1->SetX(0.184);
  text1->SetY(0.928);
  //text1->SetLineWidth(2);                                                                                                                              
  text1->SetTextFont(42);
  text1->SetTextSizePixels(24);// dflt=28                                                                                                                        
  TLatex* text2 = new TLatex(3.570061,23.08044,"36.1 pb^{-1} at #sqrt{s} = 7 TeV");
  text2->SetNDC();
  text2->SetTextAlign(13);
  //text2->SetX(0.184);
  text2->SetX(0.35);
  text2->SetY(0.88);
  //text2->SetLineWidth(2);     
  text2->SetTextFont(42);
  text2->SetTextSizePixels(24);// dflt=28                  

  TLegend *leg = new TLegend(.58,.77,.8,.8);
  leg->AddEntry(hist1, "LM13 MC", "P");// leg->AddEntry(hrEb, "a*exp(b*x)+c", "l");
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetLineStyle(0);
  leg->SetTextFont(42);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.04);


  hist1->Draw("COLZ");
  text2->Draw();
  leg->Draw();
  gPad->SetRightMargin(.18);
  gPad->Modified();
  //cout << "x axis from " << xax->GetXmin() <<" to " <<  xax->GetXmax()<< endl;;
  //cout << "y axis from " << yax->GetXmin() << " to " << yax->GetXmax()<< endl;;
  TLine* lineAb = new TLine(borderv1a,borderh1a,borderv1b, borderh1a);
  TLine* lineAt = new TLine(borderv1a,borderh1b,borderv1b, borderh1b);
  TLine* lineAl = new TLine(borderv1a,borderh1a,borderv1a, borderh1b);
  TLine* lineAr = new TLine(borderv1b,borderh1a,borderv1b, borderh1b);
  lineAb->SetLineWidth(3);
  lineAt->SetLineWidth(3);
  lineAl->SetLineWidth(3);
  lineAr->SetLineWidth(3);
  lineAb->SetLineColor(kBlack);
  lineAt->SetLineColor(kBlack);
  lineAl->SetLineColor(kBlack);
  lineAr->SetLineColor(kBlack);
  if(drawLines) lineAb->Draw(); 
  if(drawLines) lineAt->Draw();
  if(drawLines) lineAl->Draw();
  if(drawLines) lineAr->Draw();
  TLine* lineBb = new TLine(borderv1a,borderh2a,borderv1b, borderh2a);
  TLine* lineBt = new TLine(borderv1a,borderh2b,borderv1b, borderh2b);
  TLine* lineBl = new TLine(borderv1a,borderh2a,borderv1a, borderh2b);
  TLine* lineBr = new TLine(borderv1b,borderh2a,borderv1b, borderh2b);
  lineBb->SetLineWidth(3);
  lineBt->SetLineWidth(3);
  lineBl->SetLineWidth(3);
  lineBr->SetLineWidth(3);
  lineBb->SetLineColor(kBlack);
  lineBt->SetLineColor(kBlack);
  lineBl->SetLineColor(kBlack);
  lineBr->SetLineColor(kBlack);
  if(drawLines) lineBb->Draw(); 
  if(drawLines) lineBt->Draw();
  if(drawLines) lineBl->Draw();
  if(drawLines) lineBr->Draw();
  TLine* lineCb = new TLine(borderv2a,borderh2a,borderv2b, borderh2a);
  TLine* lineCt = new TLine(borderv2a,borderh2b,borderv2b, borderh2b);
  TLine* lineCl = new TLine(borderv2a,borderh2a,borderv2a, borderh2b);
  TLine* lineCr = new TLine(borderv2b,borderh2a,borderv2b, borderh2b);
  lineCb->SetLineWidth(3);
  lineCt->SetLineWidth(3);
  lineCl->SetLineWidth(3);
  lineCr->SetLineWidth(3);
  lineCb->SetLineColor(kBlack);
  lineCt->SetLineColor(kBlack);
  lineCl->SetLineColor(kBlack);
  lineCr->SetLineColor(kBlack);
  if(drawLines) lineCb->Draw(); 
  if(drawLines) lineCt->Draw();
  if(drawLines) lineCl->Draw();
  if(drawLines) lineCr->Draw();
  TLine* lineDb = new TLine(borderv2a,borderh1a,borderv2b, borderh1a);
  TLine* lineDt = new TLine(borderv2a,borderh1b,borderv2b, borderh1b);
  TLine* lineDl = new TLine(borderv2a,borderh1a,borderv2a, borderh1b);
  TLine* lineDr = new TLine(borderv2b,borderh1a,borderv2b, borderh1b);
  lineDb->SetLineWidth(3);
  lineDt->SetLineWidth(3);
  lineDl->SetLineWidth(3);
  lineDr->SetLineWidth(3);
  lineDb->SetLineColor(kBlack);
  lineDt->SetLineColor(kBlack);
  lineDl->SetLineColor(kBlack);
  lineDr->SetLineColor(kBlack);
  if(drawLines) lineDb->Draw(); 
  if(drawLines) lineDt->Draw();
  if(drawLines) lineDl->Draw();
  if(drawLines) lineDr->Draw();
  C_hist1->Print("hist1.pdf");

  C_ABCD->cd(3);
  gPad->SetRightMargin(.22);
  gPad->SetLeftMargin(.14);
  gPad->Modified();
  histA->Draw("COLZ");
  C_ABCD->cd(1);
  gPad->SetRightMargin(.22);
  histB->Draw("COLZ");
  C_ABCD->cd(2);
  gPad->SetRightMargin(.22);
  histC->Draw("COLZ");
  C_ABCD->cd(4);
  gPad->SetRightMargin(.22);
  gPad->SetLeftMargin(.14);
  gPad->Modified();
  histD->Draw("COLZ");

  C_contribute->cd(1);
  histCx->Draw();
  C_contribute->cd(2);
  histDx->Draw();

  //hist2 analysis - Projection
  TString pOpt = "o"; //"e" for error
  TString firstHist = "";
  const int pNum = 8;
  TH1D* hpy[pNum];
  TH1D* hpx[pNum];

  TLegend *leghpy = new TLegend(0.5,0.6,0.65,0.89);
  leghpy->SetFillColor(0);
  leghpy->SetLineColor(0);
  leghpy->SetTextSize(0.04);
  TLegend *leghpx = new TLegend(0.5,0.6,0.65,0.89);
  leghpx->SetFillColor(0);
  leghpx->SetLineColor(0);
  leghpx->SetTextSize(0.04);
  

  hpy[0] =  hist2->ProjectionY("hpy_0",1,3);
  hpy[1] =  hist2->ProjectionY("hpy_1",4,6);
  hpy[2] =  hist2->ProjectionY("hpy_2",7,9);
  hpy[3] =  hist2->ProjectionY("hpy_3",10,13);
  hpy[4] =  hist2->ProjectionY("hpy_4",14,19);
  hpy[5] =  hist2->ProjectionY("hpy_5",20,28);
  hpy[6] =  hist2->ProjectionY("hpy_6",29,30);
  hpy[7] =  hist2->ProjectionY("hpy_7",31,50);

  hpx[0] =  hist2->ProjectionX("hpx_0",1,9);
  hpx[1] =  hist2->ProjectionX("hpx_1",10,19);
  hpx[2] =  hist2->ProjectionX("hpx_2",20,29);
  hpx[3] =  hist2->ProjectionX("hpx_3",30,39);
  hpx[4] =  hist2->ProjectionX("hpx_4",40,49);
  hpx[5] =  hist2->ProjectionX("hpx_5",50,64);
  hpx[6] =  hist2->ProjectionX("hpx_6",65,79);
  hpx[7] =  hist2->ProjectionX("hpx_7",80,100);

  for(int i =0; i<pNum; i++){
    if(hpy[i]->Integral()<0.00001) cout << "hpy[i]->Integral() of " << i << ": " << hpy[i]->Integral() << endl;
    if(hpx[i]->Integral()<0.00001) cout << "hpx[i]->Integral() of " << i << ": " << hpx[i]->Integral() << endl;
    hpy[i]->Scale(1./hpy[i]->Integral());
    hpx[i]->Scale(1./hpx[i]->Integral());
    hpy[i]->Rebin(2);
  }

  hpx[0]->SetLineColor(kBlue+4);
  hpx[1]->SetLineColor(kBlue+3);
  hpx[2]->SetLineColor(kBlue+2);
  hpx[3]->SetLineColor(kBlue+1);
  hpx[4]->SetLineColor(kBlue);
  hpx[5]->SetLineColor(kBlue-4);
  hpx[6]->SetLineColor(kBlue-7);
  hpx[7]->SetLineColor(kBlue-9);
  hpy[0]->SetLineColor(kRed+4);
  hpy[1]->SetLineColor(kRed+3);
  hpy[2]->SetLineColor(kRed+2);
  hpy[3]->SetLineColor(kRed+1);
  hpy[4]->SetLineColor(kRed);
  hpy[5]->SetLineColor(kRed-4);
  hpy[6]->SetLineColor(kRed-7);
  hpy[7]->SetLineColor(kRed-9);
  
  TString js = "";
  C_hpx->cd();
  for(int i =0; i<pNum; i++){
    js="";
    js+=i;
    leghpx->AddEntry(hpx[i], "s"+js, "L");
    if(i==0){
      firstHist = "";
      hpx[i]->GetYaxis()->SetRangeUser(0.0001, .4);
      hpx[i]->GetXaxis()->SetTitle(xTitle);
    }
    hpx[i]->Draw(firstHist);
    firstHist = "SAME";
  }
  firstHist = "";
  C_hpy->cd();
  for(int i =3; i<pNum; i++){
    js="";
    js+=i;
    leghpy->AddEntry(hpy[i], "s"+js, "L");
    if(i==3){
      firstHist = "";
      hpy[i]->GetYaxis()->SetRangeUser(0.0001, .3);
      hpy[i]->GetXaxis()->SetTitle(yTitle);
    } 
    hpy[i]->Draw(firstHist);
    firstHist = "SAME";
  }
  
  C_hpy->cd();
  leghpy->Draw();
  C_hpx->cd();
  leghpx->Draw();


  ////////////////////////////////////////////////////////////////////////////////////////////
   
  ///////////////////////////////
  //// ratio graph for all x ////
  ///////////////////////////////
  cout << endl;
  if(verbose)cout << "All points:" << endl;
  
  //RATIO for all x
  C_extrap->cd();
  double gr0x[fitNum+extendedNum];
  double gr0y[fitNum+extendedNum];
  double gr0x_error[fitNum+extendedNum];
  double gr0y_error[fitNum+extendedNum];
  for(int i=0; i<fitNum+extendedNum; i++){
    if(i<fitNum){
      //gr0x[i]= xaxA->GetBinCenter(i+1);
      gr0x[i] = (histAm->GetBinContent(i+1,1))/(histA->GetBinContent(i+1,1)+histB->GetBinContent(i+1,1));
      gr0x_error[i]=0;
      
      gr0y[i]= histB->GetBinContent(i+1,1)/histA->GetBinContent(i+1,1);
      gr0y_error[i] =  gr0y[i]*sqrt((histB->GetBinError(i+1,1)/histB->GetBinContent(i+1,1))*(histB->GetBinError(i+1,1)/histB->GetBinContent(i+1,1))+
 				    (histA->GetBinError(i+1,1)/histA->GetBinContent(i+1,1))*(histA->GetBinError(i+1,1)/histA->GetBinContent(i+1,1)));
      if(verbose)cout << "ratio: " << xaxA->GetBinCenter(i+1) << " (" << gr0x[i] << " +- " << gr0x_error[i] << ", " << gr0y[i] << " +- " << gr0y_error[i] << ")" << endl;
    }
    else if(i>=fitNum && i<(fitNum+extendedNum) ){
      gr0x[i] = (histDm->GetBinContent(i-fitNum+1,1))/(histD->GetBinContent(i-fitNum+1,1));
      gr0x[i]= xaxD->GetBinCenter(i-fitNum+1);
      gr0x_error[i]=0;
      
      gr0y[i]= histC->GetBinContent(i-fitNum+1,1)/histD->GetBinContent(i-fitNum+1,1);
      gr0y_error[i] =  gr0y[i]*sqrt((histC->GetBinError(i-fitNum+1,1)/histC->GetBinContent(i-fitNum+1,1))*(histC->GetBinError(i-fitNum+1,1)/histC->GetBinContent(i-fitNum+1,1))+
				    (histD->GetBinError(i-fitNum+1,1)/histD->GetBinContent(i-fitNum+1,1))*(histD->GetBinError(i-fitNum+1,1)/histD->GetBinContent(i-fitNum+1,1))); 
      if(verbose)cout << "ratio: (" << gr0x[i] << " +- " << gr0x_error[i] << ", " << gr0y[i] << " +- " << gr0y_error[i] << ")" << endl;
    }
    else{ assert(0); }
  }

  TGraphErrors * gr0 = new TGraphErrors(fitNum+extendedNum, gr0x, gr0y, gr0x_error, gr0y_error);
  gr0->SetTitle("Ratio");
  gr0->SetName("gr0");
  TAxis* xaxG0 = gr0->GetXaxis();
  TAxis* yaxG0 = gr0->GetYaxis();
  xaxG0->SetTitle(xTitle);
  yaxG0->SetTitle(yTitle_ratio);
  yaxG0->SetTitleOffset(1.15);
  //  xaxG0->SetRangeUser(0.001,borderv2b_plot);
  xaxG0->SetLimits(0.,borderv2b_plot);
  yaxG0->SetRangeUser(1.e-3,10);
  gr0->SetMarkerStyle(4);  
  gr0->Draw("AP");
  C_extrap->SetLogy(1);
  C_extrap->Modified();
  gPad->SetRightMargin(.18);
  gPad->Modified();
  
  TLegend *legFE = new TLegend(.19,.15,.3,.3);
  legFE->AddEntry(gr0, "QCD MC", "P");
  legFE->SetFillColor(0);
  legFE->SetBorderSize(0);
  legFE->SetLineStyle(0);
  legFE->SetTextFont(42);
  legFE->SetFillStyle(0);
  legFE->SetTextSize(0.04);
  legFE->Draw();

  TLine* line_gr0a = new TLine(borderv1a,yaxG0->GetXmin(),borderv1a, 10); // can replace 1 with yaxG0->GetXmax()
  TLine* line_gr0b = new TLine(borderv1b,yaxG0->GetXmin(),borderv1b, 10);
  line_gr0a->SetLineColor(kBlack);
  line_gr0b->SetLineColor(kBlack);
  line_gr0a->SetLineWidth(3);
  line_gr0b->SetLineWidth(3);
  //line_gr0a->Draw();
  //line_gr0b->Draw();
  
  
  ////////////////////////
  //GRAPH FOR FIT REGION//
  ////////////////////////
  cout << endl;
  if(verbose)cout << "Points for fit region:" << endl;
  double gr1x[fitNum];
  double gr1y[fitNum];
  double gr1x_error[fitNum];
  double gr1y_error[fitNum];
  double fitmax = borderv1b;
  for(int i =0; i<fitNum; i++){
    if( histB->GetBinContent(i+1,1)==0 || histA->GetBinContent(i+1,1)==0 ){
      assert(0);
      fitmax =  xaxA->GetBinCenter(i+1); 
    }
   
    gr1x[i] = (histAm->GetBinContent(i+1,1))/(histA->GetBinContent(i+1,1)+histB->GetBinContent(i+1,1));
    //gr1x[i] = xaxA->GetBinCenter(i+1);
    gr1x_error[i] = 0.;

    gr1y[i] = histB->GetBinContent(i+1,1)/histA->GetBinContent(i+1,1);
    gr1y_error[i] = gr1y[i]*sqrt((histB->GetBinError(i+1,1)/histB->GetBinContent(i+1,1))*(histB->GetBinError(i+1,1)/histB->GetBinContent(i+1,1))+
				 (histA->GetBinError(i+1,1)/histA->GetBinContent(i+1,1))*(histA->GetBinError(i+1,1)/histA->GetBinContent(i+1,1)));
    
    if(verbose)cout << "ratio: (" << gr1x[i] << " +- " << gr1x_error[i] <<", " << gr1y[i] << " +- " << gr1y_error[i] << ")" << endl;
  }
  cout << endl;
  double gr1x_raw[fitNum];
  double gr1y_raw[fitNum];
  double gr1x_error_raw[fitNum];
  double gr1y_error_raw[fitNum];
  for(int i =0; i<fitNum; i++){
    if( histBd->GetBinContent(i+1,1)==0 || histAd->GetBinContent(i+1,1)==0 ){
      assert(0);
      fitmax =  xaxA->GetBinCenter(i+1);
    }

    gr1x_raw[i] = (histAmd->GetBinContent(i+1,1))/(histAd->GetBinContent(i+1,1)+histBd->GetBinContent(i+1,1));
    gr1x_error_raw[i] = 0.;

    gr1y_raw[i] = histBd->GetBinContent(i+1,1)/histAd->GetBinContent(i+1,1);
    gr1y_error_raw[i] = gr1y[i]*sqrt((histBd->GetBinError(i+1,1)/histBd->GetBinContent(i+1,1))*(histBd->GetBinError(i+1,1)/histBd->GetBinContent(i+1,1))+
                                 (histAd->GetBinError(i+1,1)/histAd->GetBinContent(i+1,1))*(histAd->GetBinError(i+1,1)/histAd->GetBinContent(i+1,1)));

    if(verbose)cout << "ratio: (" << gr1x_raw[i] << " +- " << gr1x_error_raw[i] <<", " << gr1y_raw[i] << " +- " << gr1y_error_raw[i] << ")" << endl;
  }







  TGraphErrors * gr1 = new TGraphErrors(fitNum, gr1x, gr1y, gr1x_error, gr1y_error);
  gr1->SetTitle("Fit of N_B(x)/N_A(x)");
  TAxis* xaxG = gr1->GetXaxis();
  TAxis* yaxG = gr1->GetYaxis();
  xaxG->SetTitle(xTitle);
  yaxG->SetTitle(yTitle_ratio);
  yaxG->SetRangeUser(.1,10);
  gr1->SetMarkerStyle(22);//was 4
  gr1->SetMarkerSize(2);

  TGraphErrors * gr1_raw = new TGraphErrors(fitNum, gr1x_raw, gr1y_raw, gr1x_error_raw, gr1y_error_raw);
  gr1_raw->SetMarkerStyle(21);
  gr1_raw->SetMarkerColor(kBlue);
  gr1_raw->SetLineColor(kBlue);
  gr1_raw->SetLineWidth(2);
  gr1_raw->SetMarkerSize(2);
  
  TMultiGraph *mg = new TMultiGraph();
  mg->Add(gr1_raw, "P");
  mg->Add(gr1, "P");
  TLegend *legFR = new TLegend(.19,.15,.3,.3);
  legFR->AddEntry(gr1_raw, "Data", "P");
  legFR->AddEntry(gr1, "Data after MC subtraction", "P");
  legFR->SetFillColor(0);
  legFR->SetBorderSize(0);
  legFR->SetLineStyle(0);
  legFR->SetTextFont(42);
  legFR->SetFillStyle(0);
  legFR->SetTextSize(0.04);
  
  C_fit->cd();
  C_fit->SetLogy();
   gr1->Draw("AP");


  TLegend *legF = new TLegend(.19,.15,.3,.3);
  legF->AddEntry(gr1, "Data after MC subtraction", "P");
  legF->SetFillColor(0);
  legF->SetBorderSize(0);
  legF->SetLineStyle(0);
  legF->SetTextFont(42);
  legF->SetFillStyle(0);
  legF->SetTextSize(0.04);

  TLatex* text1F = new TLatex(3.570061,23.08044,"CMS Preliminary");
  text1F->SetNDC();
  text1F->SetTextAlign(13);
  text1F->SetX(0.184);
  text1F->SetY(0.928);
  //text1F->SetLineWidth(2);                                                                                                                                                                                                
  text1F->SetTextFont(42);
  text1F->SetTextSizePixels(24);// dflt=28                                                                                                                                                                                  

  TLatex* text2F = new TLatex(3.570061,23.08044,"36.1 pb^{-1} at #sqrt{s} = 7 TeV");
  text2F->SetNDC();
  text2F->SetTextAlign(13);
  text2F->SetX(0.184);
  text2F->SetY(0.88);
  //text2F->SetLineWidth(2);                                                                                                                                                                                              
  text2F->SetTextFont(42);
  text2F->SetTextSizePixels(24);// dflt=28   
  text2F->Draw();
  legF->Draw();
 
  for(int i =0; i<fitNum; i++){
    cout << gr0x[i] << " " << gr1x[i] << endl;
  }

  ////////////////////////////////////////////////////////////////////////////////////////
  // FIT /////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////
  
  TF1 *fexp1 = new TF1("fexp1", "[0]*exp([1]*x)", borderv1a, borderv1b);
  fexp1->SetParameters(3.0, -1.0/30.0);
  fexp1->SetParLimits(1,-1000.0,0.);
  
  bool fixConstant =false;
  TF1 *fexp2 = new TF1("fexp2", "[0]*exp([1]*x)+[2]", borderv1a, borderv1b);
  fexp2->SetParameters(10.0, -1.0/30.0, 0.001);  
  if(!fixConstant){
    fexp2->SetParLimits(2,-5,20);
  }
  else{
    //fexp2->FixParameter(2, ConstU/ConstL);
    //fexp2->FixParameter(2,0.0728706);
    fexp2->FixParameter(2,0.);
    //fexp2->SetParError(2,0.0424232);
    
  }
  
  //TF1 *fexp3 = new TF1("fexp3", "[0]*exp([1]*x)+[2]*exp([3]*x)", borderv1a, borderv1b);
  //fexp3->SetParameters(4.0, -1.0/20.0, 4.0, -1.0/200.0);  
  //fexp3->SetParLimits(1,-1000,0);
  //fexp3->SetParLimits(3,-1000,0);
  
  //TF1 *fexp3 = new TF1("fexp3", "[0]*exp([1]*x)+[2]+[3]*x", borderv1a, borderv1b);
  //fexp3->SetParameters(10.0, -1.0/30.0, 0.001,0.);  
  //fexp3->SetParLimits(2,0.,1000);
  //fexp3->SetParLimits(3,0.,1000);
  
  // TF1 *fexp3 = new TF1("fexp3", "[0]*exp([1]*x*x)+[2]", borderv1a, borderv1b);
  // fexp3->SetParameters(2., -0.0003, -.26);
  TF1 *fexp3 = new TF1("fexp3", "[0]*exp([1]*x/(1+[2]*x))", borderv1a, borderv1b);
  fexp3->SetParameters(3.,-.0004,.04);
  

  fexp1->SetLineColor(kBlue);
  fexp2->SetLineColor(kBlack);
  fexp3->SetLineColor(kRed);
  
  //FIT Exponential////////////////////////////////////////////////////////////
  assert(!( gr1->Fit("fexp1", "R0") ));
  cout << endl;
  cout << endl;
  //fexp1->Draw("SAME");   
  TString secondFitFail = "";
  //if(gr1->Fit("fexp2", "R0")) secondFitFail = "FAILED!";
  
  //FIT Exponential+c//////////////////////////////////////////////////////////
  assert(!( gr1->Fit("fexp2", "R0 E") ));
  cout << "Chisquare: " << fexp2->GetChisquare() << endl;
  cout << "NDF: " << fexp2->GetNDF() << endl;
  cout << "Chisquare/NDF: " <<  fexp2->GetChisquare()/fexp2->GetNDF() << endl;
  cout << endl;
  cout << endl;
  fexp2->Draw("SAME");
  C_fit->Print("fit.pdf");
  //FIT another one///////////////////////////////////////////////////////////
  //gr1->Fit("fexp3", "R0");
  //fexp3->Draw("SAME"); 


  C_Data->cd();
  gPad->SetLogy(1);
  gr1->SetLineWidth(2);
  fexp2->SetLineWidth(2);
  mg->Draw("A");
  mg->GetXaxis()->SetLimits(0,81);
  mg->GetYaxis()->SetLimits(0.1,10);
  mg->GetYaxis()->SetTitle(yTitle_ratio);
  mg->GetXaxis()->SetTitle(xTitle);
  fexp2->Draw("SAME");
  legFR->Draw();

  Double_t par_exp[2];
  fexp1->GetParameters(par_exp);
  Double_t par_exp2[3];  
  fexp2->GetParameters(par_exp2);
  Double_t par_exp3[3];  
  fexp3->GetParameters(par_exp3);
 
  TString goodFit = "PROBLEM";
  if(gMinuit->fLimset==0 && gMinuit->fCstatu.Contains("SUCCESSFUL") && par_exp2[2]>0.) goodFit= "";

  TF1 *fexp1b = new TF1("fexp1b", "[0]*exp([1]*x)", borderv1a, borderv2b);
  fexp1b->SetParameters(par_exp[0], par_exp[1]);
  TF1 *fexp2b = new TF1("fexp2b", "[0]*exp([1]*x)+[2]", borderv1a, borderv2b);
  fexp2b->SetParameters(par_exp2[0], par_exp2[1],par_exp2[2]);
  //TF1 *fexp3b = new TF1("fexp3b", "[0]*exp([1]*x)+[2]*exp([3]*x)", borderv1a, borderv2b);
  TF1 *fexp3b = new TF1("fexp3b", "[0]*exp([1]*x*x)+[2]", borderv1a, borderv2b);
  fexp3b->SetParameters(par_exp3[0], par_exp3[1], par_exp3[2]);
 
  fexp1b->SetLineColor(kBlue);
  fexp2b->SetLineColor(kBlack);
  fexp3b->SetLineColor(kRed);

  TPaveText *pt_exp = new TPaveText(.25, .7, .88, .88, "NDC");
  TString par_exp0 = "";
  par_exp0+=par_exp[0];
  TString par_exp1 = "";
  par_exp1+=par_exp[1];
  TString par_exp2_0 = "";
  TString par_exp2_1 = "";
  TString par_exp2_2 = "";
  par_exp2_0+=par_exp2[0];
  par_exp2_1+=par_exp2[1];
  par_exp2_2+=par_exp2[2];
  TString par_exp3_0 = "";
  TString par_exp3_1 = "";
  TString par_exp3_2 = "";
  TString par_exp3_3 = "";
  par_exp3_0+=par_exp3[0];
  par_exp3_1+=par_exp3[1];
  par_exp3_2+=par_exp3[2];
  //  par_exp3_3+=par_exp3[3];

  pt_exp->SetFillColor(0);
  pt_exp->SetTextSize(0.02);
  pt_exp->SetTextAlign(12);
  pt_exp->AddText( "Fit (blue) = "+par_exp0+" *exp( "+par_exp1+"*x)");
  pt_exp->AddText( "Fit (purple) = "+par_exp2_0+" *exp( "+par_exp2_1+"*x)+"+par_exp2_2);
  pt_exp->AddText( "Fit (red) = "+par_exp3_0+" *exp( "+par_exp3_1+"*x)+"+par_exp3_2+" *exp( "+par_exp3_3+"*x)");
  //  pt_exp->Draw();
  
  //add fit(s) to C_extrap///////
  C_extrap->cd();
  //fexp1b->Draw("SAME");
  fexp2b->Draw("SAME");
  //fexp3b->Draw("SAME");
  C_extrap->Print("extrap.pdf");


  ////////////////////////////////////////////////
  ///   Calculate estimate with uncertainty   ////
  ////////////////////////////////////////////////
  cout << endl;
  cout << endl;
  if(verbose)cout << "Running binned exponential estimate:" << endl;
  
  double gr_contx[extendedNum];
  double gr_conty[extendedNum];
  double gr_contxe[extendedNum];
  double gr_contye[extendedNum];
 
  double extendedEstimate_exp =0;
  double ext_serror_exp = 0;

  for(int i =1; i<=extendedNum; i++){
    
    double xError = 0;
    double xvalue = xaxD->GetBinCenter(i);  
    gr_contx[i-1] = xvalue;
    gr_contxe[i-1] = xError;
    
    //EXPONENTIAL
    double ratiox_exp = par_exp[0]*exp(par_exp[1]*xvalue);
    double exp_contribution = ratiox_exp*(histD->GetBinContent(i,1));
 
    //calculate error squared for exponential -- not sure this is correct since noticed mistake in december 2010
    double ext_serror_exp_t = 0;
    ext_serror_exp_t += exp(par_exp[1]*xvalue)*(histD->GetBinContent(i,1))*(fexp1->GetParError(0))   *   exp(par_exp[1]*xvalue)*(histD->GetBinContent(i,1))*(fexp1->GetParError(0));
    ext_serror_exp_t += par_exp[0]*xvalue*exp(par_exp[1]*xvalue)*(histD->GetBinContent(i,1))*(fexp1->GetParError(1))   *   par_exp[0]*xvalue*exp(par_exp[1]*xvalue)*(histD->GetBinContent(i,1))*(fexp1->GetParError(1));
    ext_serror_exp_t += par_exp[0]*par_exp[1]*exp(par_exp[1]*xvalue)*(histD->GetBinContent(i,1))*xError   *   par_exp[0]*par_exp[1]*exp(par_exp[1]*xvalue)*(histD->GetBinContent(i,1))*xError;
    ext_serror_exp_t += par_exp[0]*exp(par_exp[1]*xvalue)*(histD->GetBinError(i,1))   *   par_exp[0]*exp(par_exp[1]*xvalue)*(histD->GetBinError(i,1));
    ext_serror_exp += ext_serror_exp_t;
    
    extendedEstimate_exp += exp_contribution;
      gr_conty[i-1] = exp_contribution;
    gr_contye[i-1] = sqrt(ext_serror_exp_t);
    if(verbose)cout << extendedEstimate_exp << " +- " << sqrt(ext_serror_exp) << endl;
  }
  double ext_error_exp = sqrt(ext_serror_exp);
  double nC_error = sqrt(nC_e);
  double nD_error = sqrt(nD_e);
  double nCextra_error = sqrt(nCextra_e);
  
  TGraphErrors * grc = new TGraphErrors(extendedNum, gr_contx, gr_conty, gr_contxe, gr_contye); 
  C_contribute->cd(3);
  grc->SetTitle("Contribution");
  grc->GetXaxis()->SetTitle(xTitle);
  grc->GetYaxis()->SetTitle("N");
  grc->SetMarkerStyle(4);
  grc->Draw("AP");
  
  //////////////////////////////////////////////////////
  ///////////////// u n b i n n e d ////////////////////
  //////////////////////////////////////////////////////

  double unbinnedEstimate = 0.;
  double ext_serror_unBinexp = 0.;
  double ext_serror_exp_w = 0.;
  double ext_serror_exp_p0 = 0.;
  double ext_serror_exp_p1 = 0.;


  double unbinnedEstimate2 = 0.;
  double ext_serror_unBinexp2 = 0.;
  double ext_serror_exp2_w = 0.;
  double ext_serror_exp2_p0 = 0.;
  double ext_serror_exp2_p1 = 0.;
  double ext_serror_exp2_p2 = 0.;

  for(int i = firstEntry; i<numEntries; i++){
    InputChain->GetEvent(i);
    
//btag requirement
    bool bcontinueNow = bcontinue(nbtags, bcont);
    
    if(bcontinueNow){
 
      //get weight
      //int bin = Hweight.FindBin(pthat);
      int bin = Hweight.FindBin(MG);
      double weight=Hweight.GetBinContent(bin);
      if(josh) weight=weightJosh;


      //if in Region D
      if( (x>=borderv2a) && (x<borderv2b) && (y>=borderh1a) && (y<borderh1b)){

	double xvalue = x;
	//double  xvalue = 90;
	double xError=0.;
	double eventError = sqrt(weight*weight);
	
	unbinnedEstimate+=weight*(par_exp[0]*exp(par_exp[1]*xvalue));
	unbinnedEstimate2+=weight*(par_exp2[0]*exp(par_exp2[1]*xvalue)+par_exp2[2]);
	
	//correct exp error
	ext_serror_exp_p0 += exp(par_exp[1]*xvalue)*(weight)*(fexp1->GetParError(0)) ; 
	ext_serror_exp_p1 += par_exp[0]*xvalue*exp(par_exp[1]*xvalue)*(weight)*(fexp1->GetParError(1)) ; 
	ext_serror_exp_w  += par_exp[0]*exp(par_exp[1]*xvalue)*(eventError) *  par_exp[0]*exp(par_exp[1]*xvalue)*(eventError); 

	//correct exp+c error
	ext_serror_exp2_p0 += exp(par_exp2[1]*xvalue)*(weight)*(fexp2->GetParError(0));
	ext_serror_exp2_p1 += par_exp2[0]*xvalue*exp(par_exp2[1]*xvalue)*(weight)*(fexp2->GetParError(1));
	ext_serror_exp2_p2 += weight*(fexp2->GetParError(2));
	ext_serror_exp2_w  += (par_exp2[0]*exp(par_exp2[1]*xvalue)+par_exp2[2])*(eventError) *  (par_exp2[0]*exp(par_exp2[1]*xvalue)+par_exp2[2])*(eventError);
      }
      
      
    }//end bcontinueNow
  }//end loop

  bool dSubtract = true;
  if(subtractSM && dSubtract){
    for(int i = 0; i<numEntriesSM; i++){
      InputChainSM->GetEvent(i);
      
      //btag requirement
      bool bcontinueNow = bcontinue(nbtags, bcont);
      
      if(bcontinueNow){
	
	//get weight -- NEGATIVE FOR SUBTRACTION!!!
	//int bin = Hweight.FindBin(pthat);
	int bin = Hweight.FindBin(MG);
	double weight=-Hweight.GetBinContent(bin);
	if(josh) weight=-weightJosh;
	weight=weight*SMfactor;
	
	
	//if in Region D
	if( (x>=borderv2a) && (x<borderv2b) && (y>=borderh1a) && (y<borderh1b)){
	  
	  double xvalue = x;
	  //double  xvalue = 90;
	  double xError=0.;
	  double eventError = sqrt(weight*weight);
	  
	  unbinnedEstimate+=weight*(par_exp[0]*exp(par_exp[1]*xvalue));
	  unbinnedEstimate2+=weight*(par_exp2[0]*exp(par_exp2[1]*xvalue)+par_exp2[2]);
	  
	  //correct exp error
	  ext_serror_exp_p0 += exp(par_exp[1]*xvalue)*(weight)*(fexp1->GetParError(0)) ; 
	  ext_serror_exp_p1 += par_exp[0]*xvalue*exp(par_exp[1]*xvalue)*(weight)*(fexp1->GetParError(1)) ; 
	  ext_serror_exp_w  += par_exp[0]*exp(par_exp[1]*xvalue)*(eventError) *  par_exp[0]*exp(par_exp[1]*xvalue)*(eventError); 
	  
	  //correct exp+c error
	  ext_serror_exp2_p0 += exp(par_exp2[1]*xvalue)*(weight)*(fexp2->GetParError(0));
	  ext_serror_exp2_p1 += par_exp2[0]*xvalue*exp(par_exp2[1]*xvalue)*(weight)*(fexp2->GetParError(1));
	  ext_serror_exp2_p2 += weight*(fexp2->GetParError(2));
	  ext_serror_exp2_w  += (par_exp2[0]*exp(par_exp2[1]*xvalue)+par_exp2[2])*(eventError) *  (par_exp2[0]*exp(par_exp2[1]*xvalue)+par_exp2[2])*(eventError);
	}
	
	
      }//end bcontinueNow
    }//end loop

  }
  
  

  double ext_error_unBinexp;
  double ext_error_unBinexp2;
  ext_error_unBinexp2 = sqrt(ext_serror_exp2_p0*ext_serror_exp2_p0 + ext_serror_exp2_p1*ext_serror_exp2_p1 + ext_serror_exp2_p2*ext_serror_exp2_p2 + ext_serror_exp2_w);
  ext_error_unBinexp = sqrt(ext_serror_exp2_p0*ext_serror_exp_p0 + ext_serror_exp2_p1*ext_serror_exp_p1+ext_serror_exp_w);

  //////////////////////////////////////////////
  //////// e     /// n      /// d       ////////
  //////////////////////////////////////////////
  cout << endl;
  cout << "borderv1a: " << borderv1a << endl;
  cout << "borderv1b: " << borderv1b << endl;
  cout << "borderv2a: " << borderv2a << endl;
  cout << "borderv2b: " << borderv2b << endl;
  cout << "borderh1a: " << borderh1a << endl;
  cout << "borderh1b: " << borderh1b << endl;
  cout << "borderh2a: " << borderh2a << endl;
  cout << "borderh2b: " << borderh2b << endl;
  cout << "nOutside (normalized): "  << nOutside << endl;
  cout << "nCextra (normalized): "   << nCextra  << " +- " << nCextra_error << endl;
  cout << "pass_x:" << pass_x << endl;
  cout << "pass_y:" << pass_y << endl;
  cout << endl;

  cout << "fitNum: " << fitNum << endl;;
  cout << "extendedNum: " << extendedNum << endl;
  if(fitmax != borderv1b) {cout << "WARNING: Zero in fit region at " << fitmax << endl;}
  cout << "hC integral: " << histC->Integral() << endl; //misses stuff between borderv2b_plot and borderv2b
  cout << "hD integral: " << histD->Integral() << endl; 
  cout << "nD: " << nD << " +- " << nD_error << endl;
  cout << "nC: " << nC << " +- " << nC_error << endl;
  cout << "single: " << nD*single << " +- " << abError(nD, nD_error, single, single_err) << endl;
  cout << "Unbinned exponential+constant extended estimate for nC: " << unbinnedEstimate2 << " +- " << ext_error_unBinexp2 << " " << secondFitFail << endl; 
  //cout << "Unbinned exponential extended estimate for nC: " << unbinnedEstimate << " +- " << ext_error_unBinexp << endl; 
  //cout <<   "Binned exponential extended estimate for nC: " << extendedEstimate_exp << " +- " << ext_error_exp << endl;
  //cout << "beforeAngular: " << beforeAngular;
  cout << "Dfrac: " << Dfrac/nD << endl;
  cout << "Constant: " << ConstU/ConstL << " +- " << aObError(ConstU, ConstU_e, ConstL, ConstL_e) << endl;
  cout << "Subtract from D: " << subtractFromD << endl;
  cout << "maxWeight: " << maxWeight << endl;
  cout << goodFit << endl;
  cout << endl;


  double *result = new double[4];
  result[0] = unbinnedEstimate;
  result[1] = ext_error_unBinexp;
  result[2] = unbinnedEstimate2;
  result[3] = ext_error_unBinexp2;

  fexp1b->Write();
  fexp2b->Write();
  fexp3b->Write();
  gr0->Write();
  histWD->Write();
  histWC->Write();
  histWA->Write();
  histWB->Write();
  histx->Write();
  histdp->Write();
  hist2->Write();
  hist3->Write();
  histDm->Write();
  histUL->Write();
  histULSM->Write();
  histU->Write();
  histL->Write();

  histA->Write();
  histB->Write();
  histAm->Write();
  histAd->Write();
  histBd->Write();
  histAmd->Write();
  histAs->Write();
  histBs->Write();
  histAms->Write();
  histASM->Write();
  histBSM->Write();
  histAmSM->Write();

  C_ABCD->Write();
  C_hist1->Write();
  C_fit->Write();
  C_extrap->Write();
  C_Data->Write();
  C_reweight->Write();
  C_contribute->Write();
  C_x->Write();
  C_dp->Write();
  C_temp->Write();
  C_temp2->Write();
  C_hpy->Write();
  C_hpx->Write();
  C_UL->Write();

  C_ABCD->Close();
  C_hist1->Close();
  C_fit->Close();
  C_extrap->Close();
  C_Data->Close();
  C_reweight->Close();
  C_contribute->Close();
  C_x->Close();
  C_dp->Close();
  C_temp->Close();
  C_temp2->Close();
  C_hpy->Close();
  C_hpx->Close();
  C_UL->Close();

  fout.Close();

  return result;
}//end analyzeFillABCD()


void analyzeFillABCD(){
  
  TString type = "pfpf";

  //double *array00 = doAnalyzeFillABCD(type, "eq1", 0, 150, 15, true, "_eq1_M");
  //double *array00 = doAnalyzeFillABCD(type, "ge1", 0, 150, 15, true, "_ge1_M");
  double *array00 = doAnalyzeFillABCD(type, "ge2", 0, 150, 15, true, "_ge2_M");
  return;

  int binNumScale = 1;
  int nbs = 2;
  
  double *array0 = doAnalyzeFillABCD(type, nbs, 0, 60, binNumScale*6, false, "0");
  double *array1 = doAnalyzeFillABCD(type, nbs, 0, 70, binNumScale*7, false, "1");
  double *array2 = doAnalyzeFillABCD(type, nbs, 0, 80, binNumScale*8, false, "2");
  double *array3 = doAnalyzeFillABCD(type, nbs, 0, 90, binNumScale*9, false, "3");
  double *array4 = doAnalyzeFillABCD(type, nbs, 0, 100, binNumScale*10, false, "4");
  double *array5 = doAnalyzeFillABCD(type, nbs, 0, 110, binNumScale*11, false, "5");
  double *array6 = doAnalyzeFillABCD(type, nbs, 0, 120, binNumScale*12, false, "6");
  double *array7 = doAnalyzeFillABCD(type, nbs, 0, 130, binNumScale*13, false, "7");
  double *array8 = doAnalyzeFillABCD(type, nbs, 0, 140, binNumScale*14, false, "8");
  double *array9 = doAnalyzeFillABCD(type, nbs, 0, 150, binNumScale*15, false, "9");
  double *array10 = doAnalyzeFillABCD(type, nbs, 0, 160, binNumScale*16, false, "10");
  double *array11 = doAnalyzeFillABCD(type, nbs, 0, 170, binNumScale*17, false, "11");
  double *array12 = doAnalyzeFillABCD(type, nbs, 0, 180, binNumScale*18, false, "11");
  double *array13 = doAnalyzeFillABCD(type, nbs, 0, 190, binNumScale*19, false, "11");
  double *array14 = doAnalyzeFillABCD(type, nbs, 0, 200, binNumScale*20, false, "11");
  
  /*
  double *array0 = doAnalyzeFillABCD(type, nbs, 0, 60, 3, false, "0");
  double *array1 = doAnalyzeFillABCD(type, nbs, 0, 70, 3, false, "1");
  double *array2 = doAnalyzeFillABCD(type, nbs, 0, 80, 4, false, "2");
  double *array3 = doAnalyzeFillABCD(type, nbs, 0, 90, 4, false, "3");
  double *array4 = doAnalyzeFillABCD(type, nbs, 0, 100, 5, false, "4");
  double *array5 = doAnalyzeFillABCD(type, nbs, 0, 110, 5, false, "5");
  double *array6 = doAnalyzeFillABCD(type, nbs, 0, 120, 6, false, "6");
  double *array7 = doAnalyzeFillABCD(type, nbs, 0, 130, 6, false, "7");
  double *array8 = doAnalyzeFillABCD(type, nbs, 0, 140, 7, false, "8");
  double *array9 = doAnalyzeFillABCD(type, nbs, 0, 150, 7, false, "9");
  double *array10 = doAnalyzeFillABCD(type, nbs, 0, 160, 8, false, "10");
  double *array11 = doAnalyzeFillABCD(type, nbs, 0, 170, 8, false, "11");
  double *array12 = doAnalyzeFillABCD(type, nbs, 0, 180, 9, false, "11");
  double *array13 = doAnalyzeFillABCD(type, nbs, 0, 190, 9, false, "11");
  double *array14 = doAnalyzeFillABCD(type, nbs, 0, 200, 10, false, "11");
  */


  double *array1f = Dfrac(type,1);
  double *array2f = Dfrac(type,2);
  
  cout << endl;
  cout << "dfrac 1tag: " << array1f[0] << " " << array1f[1] << endl;
  cout << "dfrac 2tag: " << array2f[0] << " " << array2f[1] << endl;
  
  
  cout << endl;
  cout << array0[2] << " +/- " << array0[3] << endl;
  cout << array1[2] << " +/- " << array1[3] << endl;
  cout << array2[2] << " +/- " << array2[3] << endl;
  cout << array3[2] << " +/- " << array3[3] << endl;
  cout << array4[2] << " +/- " << array4[3] << endl;
  cout << array5[2] << " +/- " << array5[3] << endl;
  cout << array6[2] << " +/- " << array6[3] << endl;
  cout << array7[2] << " +/- " << array7[3] << endl;
  cout << array8[2] << " +/- " << array8[3] << endl;
  cout << array9[2] << " +/- " << array9[3] << endl;
  cout << array10[2] << " +/- " << array10[3] << endl;
  cout << array11[2] << " +/- " << array11[3] << endl;
  cout << array12[2] << " +/- " << array12[3] << endl;
  cout << array13[2] << " +/- " << array13[3] << endl;
  cout << array14[2] << " +/- " << array14[3] << endl;
  return;
  
  
  
  cout << endl;
  cout << "btagging 1 tag" << endl;
  cout << array0[2]*array1f[0] << " +/- " << sqrt(array0[2]*array1f[1]*array0[2]*array1f[1]+array0[3]*array1f[0]*array0[3]*array1f[0]) << endl;
  cout << array1[2]*array1f[0] << " +/- " << sqrt(array1[2]*array1f[1]*array1[2]*array1f[1]+array1[3]*array1f[0]*array1[3]*array1f[0]) << endl; 
  cout << array2[2]*array1f[0] << " +/- " << sqrt(array2[2]*array1f[1]*array2[2]*array1f[1]+array2[3]*array1f[0]*array2[3]*array1f[0]) << endl; 
  cout << array3[2]*array1f[0] << " +/- " << sqrt(array3[2]*array1f[1]*array3[2]*array1f[1]+array3[3]*array1f[0]*array3[3]*array1f[0]) << endl; 
  cout << array4[2]*array1f[0] << " +/- " << sqrt(array4[2]*array1f[1]*array4[2]*array1f[1]+array4[3]*array1f[0]*array4[3]*array1f[0]) << endl; 
  cout << array5[2]*array1f[0] << " +/- " << sqrt(array5[2]*array1f[1]*array5[2]*array1f[1]+array5[3]*array1f[0]*array5[3]*array1f[0]) << endl; 
  cout << array6[2]*array1f[0] << " +/- " << sqrt(array6[2]*array1f[1]*array6[2]*array1f[1]+array6[3]*array1f[0]*array6[3]*array1f[0]) << endl;
  cout << array7[2]*array1f[0] << " +/- " << sqrt(array7[2]*array1f[1]*array7[2]*array1f[1]+array7[3]*array1f[0]*array7[3]*array1f[0]) << endl; 
  cout << array8[2]*array1f[0] << " +/- " << sqrt(array8[2]*array1f[1]*array8[2]*array1f[1]+array8[3]*array1f[0]*array8[3]*array1f[0]) << endl; 
  cout << array9[2]*array1f[0] << " +/- " << sqrt(array9[2]*array1f[1]*array9[2]*array1f[1]+array9[3]*array1f[0]*array9[3]*array1f[0]) << endl; 
  cout << array10[2]*array1f[0] << " +/- " << sqrt(array10[2]*array1f[1]*array10[2]*array1f[1]+array10[3]*array1f[0]*array10[3]*array1f[0]) << endl; 
  cout << array11[2]*array1f[0] << " +/- " << sqrt(array11[2]*array1f[1]*array11[2]*array1f[1]+array11[3]*array1f[0]*array11[3]*array1f[0]) << endl; 

  cout << endl;
  cout << "btagging 2 tag" << endl;
  cout << array0[2]*array2f[0] << " +/- " << sqrt(array0[2]*array2f[1]*array0[2]*array2f[1]+array0[3]*array2f[0]*array0[3]*array2f[0]) << endl;
  cout << array1[2]*array2f[0] << " +/- " << sqrt(array1[2]*array2f[1]*array1[2]*array2f[1]+array1[3]*array2f[0]*array1[3]*array2f[0]) << endl;
  cout << array2[2]*array2f[0] << " +/- " << sqrt(array2[2]*array2f[1]*array2[2]*array2f[1]+array2[3]*array2f[0]*array2[3]*array2f[0]) << endl;
  cout << array3[2]*array2f[0] << " +/- " << sqrt(array3[2]*array2f[1]*array3[2]*array2f[1]+array3[3]*array2f[0]*array3[3]*array2f[0]) << endl;
  cout << array4[2]*array2f[0] << " +/- " << sqrt(array4[2]*array2f[1]*array4[2]*array2f[1]+array4[3]*array2f[0]*array4[3]*array2f[0]) << endl;
  cout << array5[2]*array2f[0] << " +/- " << sqrt(array5[2]*array2f[1]*array5[2]*array2f[1]+array5[3]*array2f[0]*array5[3]*array2f[0]) << endl;
  cout << array6[2]*array2f[0] << " +/- " << sqrt(array6[2]*array2f[1]*array6[2]*array2f[1]+array6[3]*array2f[0]*array6[3]*array2f[0]) << endl;
  cout << array7[2]*array2f[0] << " +/- " << sqrt(array7[2]*array2f[1]*array7[2]*array2f[1]+array7[3]*array2f[0]*array7[3]*array2f[0]) << endl;
  cout << array8[2]*array2f[0] << " +/- " << sqrt(array8[2]*array2f[1]*array8[2]*array2f[1]+array8[3]*array2f[0]*array8[3]*array2f[0]) << endl;
  cout << array9[2]*array2f[0] << " +/- " << sqrt(array9[2]*array2f[1]*array9[2]*array2f[1]+array9[3]*array2f[0]*array9[3]*array2f[0]) << endl;
  cout << array10[2]*array2f[0] << " +/- " << sqrt(array10[2]*array2f[1]*array10[2]*array2f[1]+array10[3]*array2f[0]*array10[3]*array2f[0]) << endl;
  cout << array11[2]*array2f[0] << " +/- " << sqrt(array11[2]*array2f[1]*array11[2]*array2f[1]+array11[3]*array2f[0]*array11[3]*array2f[0]) << endl;
    

 
 }
