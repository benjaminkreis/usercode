#include "TFile.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2F.h"
#include "TF1.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TAxis.h"
#include "TMatrixT.h"
#include "TVectorT.h"
#include "TGraph.h"
#include "TROOT.h"
#include <iostream>
#include <vector>
#include <exception>
#include <cmath> 
#include <iomanip>
#include <assert.h>

////////
//B//C//
////////
//A//D//
////////
//C is signal region

void analyzeABCD(){
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptStat("e");
  
  float pi=4*atan(1.0);

  float borderv1a=0.0;
  float borderv1b=150.0;
  float borderv2a=150.0;
  float borderv2b=1000.0;
  float borderh1a=0.0;
  float borderh1b=0.3;
  float borderh2a=0.3;
  float borderh2b=pi;

   float nA=0, nB=0, nC=0, nD=0;

   TCanvas * C_ABCD = new TCanvas("C_ABCD", "Canvas ABCD", 1000, 1000);
   C_ABCD->Divide(2,2);
   C_ABCD->cd(1);
  /////////////////////////////
  //        HISTOGRAMS       //
  /////////////////////////////

  //plot 2d histogram for visual purposes
  TString histname = "H_minDPhi_MET";
  TFile* file1 = TFile::Open("plots_LM0_nBJetsLT2.root","READ");
  TH2F* hist1 = (TH2F*) gDirectory->Get(histname);
  TAxis* xax =  hist1->GetXaxis();
  TAxis* yax =  hist1->GetYaxis();
  xax->SetTitle("MET (GeV)");
  yax->SetTitle("min Delta Phi (MET, jet 1 2 3)");
  hist1->Draw("COLZ");
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
  lineAb->Draw(); 
  lineAt->Draw();
  lineAl->Draw();
  lineAr->Draw();
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
  lineBb->Draw(); 
  lineBt->Draw();
  lineBl->Draw();
  lineBr->Draw();
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
  lineCb->Draw(); 
  lineCt->Draw();
  lineCl->Draw();
  lineCr->Draw();
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
  lineDb->Draw(); 
  lineDt->Draw();
  lineDl->Draw();
  lineDr->Draw();
  
  //prep for histograms A, B, C, D
  const Double_t yBinsL[2] = {borderh1a, borderh1b};
  const Double_t yBinsU[2] = {borderh2a, borderh2b};
  
  int fitNum = 5; //number of bins in xL region, used to fit ratio
  float xWidthL = (borderv1b-borderv1a)/fitNum;
  Double_t xBinsL[fitNum+1];
  for(int i = 0; i<=fitNum; i++){
    xBinsL[i]=borderv1a+i*xWidthL;
    cout <<"xBinsL: " << xBinsL[i] << endl;
  }
  
  int extendedNum = 20; //number of bins in xU region, used to calculate nC
  float xWidthU = (borderv2b-borderv2a)/extendedNum;
  Double_t xBinsU[extendedNum+1];
  for(int i = 0; i<=extendedNum; i++){
    xBinsU[i]=borderv2a+i*xWidthU;
    cout << "xBinsU: " << xBinsU[i] << endl;
  }
  
  // histogram for A, B, C, D
  TH2F* histA = new TH2F("H_A", "Region A", fitNum, xBinsL, 1, yBinsL);
  TH2F* histB = new TH2F("H_B", "Region B", fitNum, xBinsL, 1, yBinsU);
  TH2F* histC = new TH2F("H_C", "Region C", extendedNum, xBinsU, 1, yBinsU);
  TH2F* histD = new TH2F("H_D", "Region D", extendedNum, xBinsU, 1, yBinsL);

  //////////////////
  //LOOP over TREE//
  //////////////////
  TString treename = "T_minDPhi_MET";
  TTree* tree1 = (TTree*) gDirectory->Get(treename);
  int numEntries = tree1->GetEntries();
  cout << "numEntries: " << numEntries << endl;
  
  float x = -1;
  float y = -1;
  tree1->SetBranchAddress("MET",&x);
  tree1->SetBranchAddress("minDPhi",&y);
  
  for(int i = 1; i<= numEntries; i++){ //loop over tree
    tree1->GetEvent(i);
    
    //COUNT nA, nB, nC, nD
    if( (x>=borderv1a) && (x<borderv1b) && (y>=borderh1a) && (y<borderh1b) ){
      nA++;
    }
    else if( (x>=borderv2a) && (x<borderv2b) && (y>=borderh1a) && (y<borderh1b) ){
      nD++;
    }
    else if( (x>=borderv1a) && (x<borderv1b) && (y>=borderh2a) && (y<borderh2b) ){
      nB++;
    }
    else if( (x>=borderv2a) && (x<borderv2b) && (y>=borderh2a) && (y<borderh2b) ){
      nC++;
    }
    else { assert(0);}
    
    //Fill histogram A, B, C, D
    histA->Fill(x,y);
    histB->Fill(x,y);
    histC->Fill(x,y);
    histD->Fill(x,y);
    
  }//end loop tree
  
  C_ABCD->cd(3);
  histA->Draw("COLZ");
  C_ABCD->cd(1);
  histB->Draw("COLZ");
  C_ABCD->cd(2);
  histC->Draw("COLZ");
  C_ABCD->cd(4);
  histD->Draw("COLZ");

  float gr1x[fitNum];
  float gr1y[fitNum];
  for(int i =0; i<fitNum; i++){
    gr1x[i] = histA->GetBinCenter(i+1);
    gr1y[i] = histB->GetBinContent(i+1,1)/histA->GetBinContent(i+1,1);
    cout << "ratio: (" << gr1x[i] << ", " << gr1y[i] << ")" << endl;
  }
  TGraph * gr1 = new TGraph(fitNum, gr1x, gr1y);
  //gr1->Draw("A*");

  TF1 *flin1 = new TF1("flin1", "[0]+[1]*x",borderv1a, borderv1b );
  gr1->Fit("flin1","R");
  Double_t par[2];
  flin1->GetParameters(par);
  cout << "Fit result: ratio = " << par[0] << " + " << par[1] << "*x" << endl;

  float classicalEstimate = nD*nB/nA;

  cout << "nA: " << nA << endl;
  cout << "nB: " << nB << endl;
  cout << "nC: " << nC << endl;
  cout << "nD: " << nD << endl;
  cout << "classical estimate for nC: " << classicalEstimate << endl;

}//0
