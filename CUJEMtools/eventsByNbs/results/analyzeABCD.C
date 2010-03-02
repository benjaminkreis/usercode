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

void analyzeABCD(){
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptStat("e");
  
  TString histname = "H_minDPhi_MET";
  
  TFile* file1 = TFile::Open("plots_QCD_Pt170_nBJetsLTEQGTmerged.root","READ");
  TH2F* hist1 = (TH2F*) gDirectory->Get(histname);
  
  TAxis* xax =  hist1->GetXaxis();
  TAxis* yax =  hist1->GetYaxis();
  cout << "x axis from " << xax->GetXmin() <<" to " <<  xax->GetXmax()<< endl;;
  cout << "y axis from " << yax->GetXmin() << " to " << yax->GetXmax()<< endl;;

  xax->SetTitle("MET (GeV)");
  yax->SetTitle("min Delta Phi (MET, jet 1 2 3)");
  hist1->Draw("COLZ");
  gPad->SetRightMargin(.18);
  gPad->Modified();
 
  float pi=4*atan(1.0);
  float nA=0, nB=0, nC=0, nD=0;
  
  float borderv1a=0.0;
  float borderv1b=150.0;
  float borderv2a=150.0;
  float borderv2b=1000.0;
  float borderh1a=0.0;
  float borderh1b=0.3;
  float borderh2a=0.3;
  float borderh2b=pi;


  // HISTOGRAM for fit 
  const Double_t yBins[4] = {borderh1a, borderh1b, borderh2a, borderh2b};
 
  int fitNum = 5;
  float f_xWidth = (borderv1b-borderv1a)/fitNum;
  Double_t f_xBins[fitNum+1];
  for(int i = 0; i<=fitNum; i++){
    f_xBins[i]=borderv1a+i*f_xWidth;
    cout <<"histf " << f_xBins[i] << endl;
  }
  TH2F* histf = new TH2F("H_ratio_setup","Ratio Setup",fitNum, f_xBins, 3, yBins);
  
  // HISTOGRAM for extension
  int extendedNum = 20;
  float e_xWidth = (borderv2b-borderv2a)/extendedNum;
  Double_t e_xBins[extendedNum+1];
  for(int i = 0; i<=extendedNum; i++){
    e_xBins[i]=borderv2a+i*e_xWidth;
    cout << "histe " << e_xBins[i] << endl;
  }

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

  int nBinsX =  hist1->GetNbinsX();
  int nBinsY =  hist1->GetNbinsY();
  
  for(int xbin = 1; xbin<= nBinsX; xbin++){
   
    for(int ybin = 1; ybin<= nBinsY; ybin++){
      float xlow =  xax->GetBinLowEdge(xbin);
      float xup =  xax->GetBinUpEdge(xbin); 
      float ylow = yax->GetBinLowEdge(ybin);
      float yup =  yax->GetBinUpEdge(ybin);
      //cout << "xlow " << xlow << endl;
      //cout << "ylow " << ylow << endl;

      histf->Fill(xlow,ylow,hist1->GetBinContent(xbin,ybin));

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
  }//end loop over xbin
  
  //histf->Draw("COLZ");
  
  float gr1x[fitNum];
  float gr1y[fitNum];
  for(int i =0; i<fitNum; i++){
    gr1x[i] = histf->GetBinCenter(i+1);
    gr1y[i] = histf->GetBinContent(i+1,3)/histf->GetBinContent(i+1,1);
    cout << "ratio: (" << gr1x[i] << ", " << gr1y[i] << ")" << endl;
  }
  TGraph * gr1 = new TGraph(fitNum, gr1x, gr1y);
  // gr1->Draw("A*");

  TF1 *flin1 = new TF1("flin1", "[0]+[1]*x",borderv1a, borderv1b );
  gr1->Fit("flin1","R");
  Double_t par[2];
  flin1->GetParameters(par);
  cout << "Fit result: ratio = " << par[0] << " + " << par[1] << "*x" << endl;

  //for(int i = 
  cout << "bin = " << int(borderv2a/(f_xWidth)) << ", hist " << histf->GetBinCenter(int(borderv2a/(f_xWidth)))<<endl;



  float classicalEstimate = nD*nB/nA;

  cout << "nA: " << nA << endl;
  cout << "nB: " << nB << endl;
  cout << "nC: " << nC << endl;
  cout << "nD: " << nD << endl;
  cout << "classical estimate for nC: " << classicalEstimate << endl;

}//0
