#include "TFile.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2F.h"
#include "TF1.h"
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
#include <iostream>
#include <vector>
#include <exception>
#include <cmath> 
#include <iomanip>
#include <assert.h>

using namespace std;

////////
//B//C//
////////
//A//D//
////////
//C is signal region

void analyzeABCD( ){
  
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(61);
  gStyle->SetOptStat("e");

  int fitNum = 4; //number of bins in xL region, used to fit ratio
  int extendedNum = 6; //number of bins in xR region, used in extrapolation

  TString extension = "rfio:/castor/cern.ch/user/k/kreis/CUSusy/RA2/ABCD/trees/";
  TString filename = "T_ABCD_QCD_Pt80";
  TString treename = "T_ABCD"; 
  TString histname = "H_minDPhi_MET";
  TString treeextension = "demo";

  TString yTitle = "min Delta Phi (MET, jet 1 2 3)";
  TString xTitle = "MET (GeV)";

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
  float nA_e=0, nB_e=0, nC_e=0, nD_e=0; 

  ///////////////////
  //    CANVAS     //
  ///////////////////
  TCanvas * C_ratio = new TCanvas("C_ratio", "Canvas ratio", 800, 700);
  C_ratio->cd(1);
  TCanvas * C_fit = new TCanvas("C_fit", "Canvas fit", 800, 700);
  C_fit->cd(1);
  TCanvas * C_ABCD = new TCanvas("C_ABCD", "Canvas ABCD", 1000, 1000);
  C_ABCD->Divide(2,2);
  C_ABCD->cd(1);
  TCanvas * C_hist1 = new TCanvas ("C_hist1", "Canvas hist1", 800, 700);
  C_hist1->cd(1);
  
  /////////////////////////////
  //        HISTOGRAMS       //
  /////////////////////////////
 
  //plot 2d histogram for visual purposes
  TFile* file1 = TFile::Open(extension+filename+".root","READ");
 
  
  //prep for histograms A, B, C, D
  const Double_t yBinsL[2] = {borderh1a, borderh1b};
  const Double_t yBinsU[2] = {borderh2a, borderh2b};
  
 
 
  float xWidthL = (borderv1b-borderv1a)/fitNum;
  Double_t xBinsL[fitNum+1];
  cout << "xBinsL: ";
  for(int i = 0; i<=fitNum; i++){
    xBinsL[i]=borderv1a+i*xWidthL;
    cout << xBinsL[i] << " ";
  }
  cout << endl;


  float xWidthU = (borderv2b-borderv2a)/extendedNum;
  Double_t xBinsU[extendedNum+1]; 
  cout << "xBinsU: ";
  for(int i = 0; i<=extendedNum; i++){
    xBinsU[i]=borderv2a+i*xWidthU;
    cout << xBinsU[i] << " ";
  }
  cout << endl;
  
  // histogram for A, B, C, D
  TH2F* hist1 = new TH2F("H_ABCD", "ABCD", 100,0,1000,160,0,pi);
  TH2F* histA = new TH2F("H_A", "Region A", fitNum, xBinsL, 1, yBinsL);
  TH2F* histB = new TH2F("H_B", "Region B", fitNum, xBinsL, 1, yBinsU);
  TH2F* histC = new TH2F("H_C", "Region C", extendedNum, xBinsU, 1, yBinsU);
  TH2F* histD = new TH2F("H_D", "Region D", extendedNum, xBinsU, 1, yBinsL);
  histA->Sumw2();
  histB->Sumw2();
  histC->Sumw2();
  histD->Sumw2();
  TAxis *xaxA = histA->GetXaxis();
  TAxis *xaxB = histB->GetXaxis();
  TAxis *xaxC = histC->GetXaxis();
  TAxis *xaxD = histD->GetXaxis();
  TAxis *yaxA = histA->GetYaxis();
  TAxis *yaxB = histB->GetYaxis();
  TAxis *yaxC = histC->GetYaxis();
  TAxis *yaxD = histD->GetYaxis();
  xaxA->SetTitle(xTitle);
  xaxB->SetTitle(xTitle);
  xaxC->SetTitle(xTitle);
  xaxD->SetTitle(xTitle);
  yaxA->SetTitleOffset(1.4);
  yaxB->SetTitleOffset(1.15);
  yaxC->SetTitleOffset(1.15);
  yaxD->SetTitleOffset(1.4);
  yaxA->SetTitle(yTitle);
  yaxB->SetTitle(yTitle);
  yaxC->SetTitle(yTitle);
  yaxD->SetTitle(yTitle);
  
  //////////////////
  //LOOP over TREE//
  //////////////////

  TTree* tree1 = (TTree*) gDirectory->Get(treeextension+"/"+treename);
  int numEntries = tree1->GetEntries();
  cout << "numEntries: " << numEntries << endl;
  
  float x = -1;
  float y = -1;
  float weight = 1;
  tree1->SetBranchAddress("MHT",&x);
  tree1->SetBranchAddress("minDPhi",&y);
  //tree1->SetBranchAddress("weight",&weight);
  
  for(int i = 1; i<= numEntries; i++){ //loop over tree
    tree1->GetEvent(i);
    
    //COUNT nA, nB, nC, nD
    if( (x>=borderv1a) && (x<borderv1b) && (y>=borderh1a) && (y<borderh1b) ){
      nA+=weight;
      nA_e+=weight*weight;
    }
    else if( (x>=borderv2a) && (x<borderv2b) && (y>=borderh1a) && (y<borderh1b) ){
      nD+=weight;
      nD_e+=weight*weight;
    }
    else if( (x>=borderv1a) && (x<borderv1b) && (y>=borderh2a) && (y<borderh2b) ){
      nB+=weight;
      nB_e+=weight*weight;
    }
    else if( (x>=borderv2a) && (x<borderv2b) && (y>=borderh2a) && (y<borderh2b) ){
      nC+=weight;
      nC_e +=weight*weight;
    }
    else {
      cout << "EVENT " << i << " with x = " << x << " and y = " << y << " not in A, B, C, or D." << endl;
      //assert(0);
    }
    
    //Fill histogram A, B, C, D
    hist1->Fill(x,y, weight);
    histA->Fill(x,y, weight);
    histB->Fill(x,y, weight);
    histC->Fill(x,y, weight);
    histD->Fill(x,y, weight);
    
  }//end loop tree
  

  TAxis* xax =  hist1->GetXaxis();
  TAxis* yax =  hist1->GetYaxis();
  xax->SetTitle(xTitle);
  yax->SetTitle(yTitle);
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
  C_hist1->Print(extension+filename+"_"+treename+"_hist1.pdf");
 
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
  C_ABCD->Print(extension+filename+"_"+treename+"_ABCD.pdf");

  /////////////////////////////////
  //EXTENDED NORMALIZATION METHOD//
  /////////////////////////////////
 
  //RATIO for all x
  C_ratio->cd();
  float gr0x[fitNum+extendedNum];
  float gr0y[fitNum+extendedNum];
  float gr0x_error[fitNum+extendedNum];
  float gr0y_error[fitNum+extendedNum];
  for(int i=0; i<fitNum+extendedNum; i++){
    if(i<fitNum){
      gr0x[i]= xaxA->GetBinCenter(i+1);
      gr0x_error[i]=0;

      gr0y[i]= histB->GetBinContent(i+1,1)/histA->GetBinContent(i+1,1);
      gr0y_error[i] =  gr0y[i]*sqrt((histB->GetBinError(i+1,1)/histB->GetBinContent(i+1,1))*(histB->GetBinError(i+1,1)/histB->GetBinContent(i+1,1))+
				    (histA->GetBinError(i+1,1)/histA->GetBinContent(i+1,1))*(histA->GetBinError(i+1,1)/histA->GetBinContent(i+1,1)));
      cout << "gr0y["<< i << "]: " << gr0y[i] << endl;
    }
    else if(i>=fitNum && i<(fitNum+extendedNum) ){
      gr0x[i]= xaxD->GetBinCenter(i-fitNum+1);
      gr0x_error[i]=0;

      gr0y[i]= histC->GetBinContent(i-fitNum+1,1)/histD->GetBinContent(i-fitNum+1,1);
      gr0y_error[i] =  gr0y[i]*sqrt((histC->GetBinError(i-fitNum+1,1)/histC->GetBinContent(i-fitNum+1,1))*(histC->GetBinError(i-fitNum+1,1)/histC->GetBinContent(i-fitNum+1,1))+
				    (histD->GetBinError(i-fitNum+1,1)/histD->GetBinContent(i-fitNum+1,1))*(histD->GetBinError(i-fitNum+1,1)/histD->GetBinContent(i-fitNum+1,1))); 
      cout << "gr0y["<< i << "]: " << gr0y[i] << endl;
   }
    else{ assert(0); }
  }
  TGraphErrors * gr0 = new TGraphErrors(fitNum+extendedNum, gr0x, gr0y, gr0x_error, gr0y_error);
  gr0->SetTitle("Ratio");
  TAxis* xaxG0 = gr0->GetXaxis();
  TAxis* yaxG0 = gr0->GetYaxis();
  xaxG0->SetTitle(xTitle);
  yaxG0->SetTitle(yTitle);
  gr0->SetMarkerStyle(4);  
  gr0->Draw("AP");
 
  TLine* line_gr0a = new TLine(borderv1a,yaxG0->GetXmin(),borderv1a, yaxG0->GetXmax());
  TLine* line_gr0b = new TLine(borderv1b,yaxG0->GetXmin(),borderv1b, yaxG0->GetXmax());
  line_gr0a->SetLineColor(kBlack);
  line_gr0b->SetLineColor(kBlack);
  line_gr0a->SetLineWidth(3);
  line_gr0b->SetLineWidth(3);
  line_gr0a->Draw();
  line_gr0b->Draw();
 

  ////////////////////////
  //GRAPH FOR FIT REGION//
  ////////////////////////
  C_fit->cd();
  float gr1x[fitNum];
  float gr1y[fitNum];
  float gr1x_error[fitNum];
  float gr1y_error[fitNum];
  float fitmax = borderv1b;
  for(int i =0; i<fitNum; i++){
    if( (histB->GetBinContent(i+1,1)/histA->GetBinContent(i+1,1) == 0) ){
      fitmax =  xaxA->GetBinCenter(i+1);
    }
    gr1x[i] = xaxA->GetBinCenter(i+1);
    gr1x_error[i] = 0;
    gr1y[i] = histB->GetBinContent(i+1,1)/histA->GetBinContent(i+1,1);
    
    gr1y_error[i] = gr1y[i]*sqrt((histB->GetBinError(i+1,1)/histB->GetBinContent(i+1,1))*(histB->GetBinError(i+1,1)/histB->GetBinContent(i+1,1))+
    			 (histA->GetBinError(i+1,1)/histA->GetBinContent(i+1,1))*(histA->GetBinError(i+1,1)/histA->GetBinContent(i+1,1)));
   
    cout << "ratio: (" << gr1x[i] << " +- " << gr1x_error[i] <<", " << gr1y[i] << " +- " << gr1y_error[i] << ")" << endl;
  }
  TGraphErrors * gr1 = new TGraphErrors(fitNum, gr1x, gr1y, gr1x_error, gr1y_error);
  gr1->SetTitle("Fit of N_B(x)/N_A(x)");
  TAxis* xaxG = gr1->GetXaxis();
  TAxis* yaxG = gr1->GetYaxis();
  xaxG->SetTitle(xTitle);
  yaxG->SetTitle(yTitle);
  gr1->SetMarkerStyle(4);
  gr1->Draw("AP");

  //////////////////////////////////
  ////////////LINEAR FIT////////////
  //////////////////////////////////
  TF1 *flin1 = new TF1("flin1", "[0]+[1]*x",borderv1a, fitmax );
  //flin1->SetParameters(2.0,-.02);
  flin1->SetParameters(4.0,-.035);
  gr1->Fit("flin1","R");
  flin1->Draw("SAME");
  Double_t par[2];
  flin1->GetParameters(par);
  cout << "Fit result: ratio = (" << par[0] << " +- " << flin1->GetParError(0) <<") + (" << par[1] << " +- " << flin1->GetParError(1) << ")*x" << endl;

  TF1 *flin1b = new TF1("flin1", "[0]+[1]*x", borderv1a, borderv2b);
  flin1b->SetParameters(par[0],par[1]);
  


  ////////////////////////////////////////////
  /////////////EXPONENTIAL FIT////////////////
  ////////////////////////////////////////////
  TF1 *fexp1 = new TF1("fexp1", "[0]*exp([1]*x)", borderv1a, borderv1b);
  //fexp1->SetParameters(2.0, -1.0/50.0);
  fexp1->SetParameters(4.0, -1.0/20.0);
 
  gr1->Fit("fexp1", "R");
  fexp1->Draw("SAME");
  Double_t par_exp[2];
  fexp1->GetParameters(par_exp);
  
  TF1 *fexp1b = new TF1("fexp1", "[0]*exp([1]*x)", borderv1a, borderv2b);
  fexp1b->SetParameters(par_exp[0], par_exp[1], par_exp[2]);

  TPaveText *pt = new TPaveText(.25, .8, .88, .88, "NDC");
  TString par0 = "";
  par0+=par[0];
  TString par1 = "";
  par1+=par[1];
  pt->SetFillColor(0);
  pt->SetTextSize(0.03);
  pt->SetTextAlign(12);
  pt->AddText( "Fit result: ratio = "+par0+" + "+par1+"*x");
  pt->Draw();
  TPaveText *pt_exp = new TPaveText(.25, .72, .88, .80, "NDC");
  TString par0_exp = "";
  par0_exp+=par_exp[0];
  TString par1_exp = "";
  par1_exp+=par_exp[1];
  pt_exp->SetFillColor(0);
  pt_exp->SetTextSize(0.03);
  pt_exp->SetTextAlign(12);
  pt_exp->AddText( "Fit result: ratio = "+par0+" *exp( "+par1+"*x)");
  pt_exp->Draw();
  
  C_fit->Print(extension+filename+"_"+treename+"_fit.pdf");

  //add fit to C_ratio
  C_ratio->cd();
  flin1b->Draw("SAME");
  fexp1b->Draw("SAME");
  C_ratio->Print(extension+filename+"_"+treename+"_ratio.pdf");

  ////////////////////////////////////////////////
  ///Calculate linear estimate with uncertainty///
  ////////////////////////////////////////////////
  float extendedEstimate=0;
  float ext_serror=0;

  float extendedEstimate_exp =0;
  float ext_serror_exp = 0;
  for(int i =1; i<=extendedNum; i++){

    float xError = 0;
    float xvalue = xaxD->GetBinCenter(i);  
    
    //LINEAR
    float ratiox = par[0] + par[1]*xvalue;
    extendedEstimate+=ratiox*(histD->GetBinContent(i,1));
    
    //calculate error squared for linear
    ext_serror += ratiox*(histD->GetBinError(i,1)) * ratiox*(histD->GetBinError(i,1));
    ext_serror += (xvalue * (histD->GetBinContent(i,1)) * (flin1->GetParError(1))) * (xvalue * (histD->GetBinContent(i,1)) * (flin1->GetParError(1)));
    ext_serror += (par[1] * (histD->GetBinContent(i,1)) * xError) * (par[1] * (histD->GetBinContent(i,1)) * xError);
    ext_serror += (histD->GetBinContent(i,1)) * (flin1->GetParError(0)) * (histD->GetBinContent(i,1)) * (flin1->GetParError(0));
 
    //EXPONENTIAL
    float ratiox_exp = par[0]*exp(par[1]*xvalue);
    extendedEstimate_exp += ratiox_exp*(histD->GetBinContent(i,1));

    //calculate error squared for exponential
    ext_serror_exp += exp(par_exp[1]*xvalue)*(histD->GetBinContent(i,1))*(fexp1->GetParError(0)) * exp(par_exp[1]*xvalue)*(histD->GetBinContent(i,1))*(fexp1->GetParError(0));
    ext_serror_exp += par_exp[0]*xvalue*exp(par_exp[1]*xvalue)*(histD->GetBinContent(i,1))*(fexp1->GetParError(1)) * par_exp[0]*xvalue*exp(par_exp[1]*xvalue)*(histD->GetBinContent(i,1))*(fexp1->GetParError(1));
    ext_serror_exp += par_exp[0]*par_exp[1]*exp(par_exp[1]*xvalue)*(histD->GetBinContent(i,1))*xError *  par_exp[0]*par_exp[1]*exp(par_exp[1]*xvalue)*(histD->GetBinContent(i,1))*xError;
    ext_serror_exp += par_exp[0]*exp(par_exp[1]*xvalue)*(histD->GetBinError(i,1)) *  par_exp[0]*exp(par_exp[1]*xvalue)*(histD->GetBinError(i,1));
  }
  float ext_error = sqrt(ext_serror);
  float ext_error_exp = sqrt(ext_serror_exp);
  
  //////////////////////
  //CLASSICAL ESTIMATE//
  //////////////////////
  float classicalEstimate = nD*nB/nA;
  float nA_error = sqrt(nA_e);
  float nB_error = sqrt(nB_e);
  float nC_error = sqrt(nC_e);
  float nD_error = sqrt(nD_e);
  float classicalEstimate_error =  classicalEstimate * sqrt( (nA_error/nA)*(nA_error/nA)+(nB_error/nB)*(nB_error/nB)+(nD_error/nD)*(nD_error/nD) );
  /////////////////////////////

  cout << "fitNum: " << fitNum << endl;;
  cout << "extendedNum: " << extendedNum << endl;
  cout << "nA: " << nA << " +- " << nA_error << endl;
  cout << "nB: " << nB << " +- " << nB_error << endl;
  cout << "nD: " << nD << " +- " << nD_error << endl;
  cout << "nC: " << nC << " +- " << nC_error << endl;
  cout << "classical estimate for nC: " << classicalEstimate << " +- " << classicalEstimate_error << endl;
  cout << "exponential extended estimate for nC: " << extendedEstimate_exp << " +- " << ext_error_exp << endl;
  cout << "linear extended estimate for nC: " << extendedEstimate << " +- " << ext_error << endl;
}//0
