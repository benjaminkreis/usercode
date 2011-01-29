#include "TLatex.h"
#include "TGraphErrors.h"
#include "TVectorD.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TLine.h"
#include "TFile.h"
#include "TLegend.h"
#include "TBox.h"
#include "TH1D.h"
#include "TROOT.h"
#include "TStyle.h"
#include <assert.h>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

void doMulti(TString type = "pfpf", TString fileName= "", double ymax= 50., TString legEntry = "", TString tagType = ""){
  gROOT->SetStyle("CMS");

  TString dataSet = "";
  //dataSet = "s10";
  dataSet = "f10";
  
  assert(type=="pfpf");
  
  double trueN=0.;
  double trueN_err=0.;
  double trueN_factor = 1.;
  
  bool drawLines = true;
 
  //tagType should be nob, e1, ge1, ge2, data (no lines)
  if(dataSet == "s10"){
    if(tagType=="nob"){
      trueN =8.79075;
      trueN_err =2.22547;
    }
    else if(tagType=="e1"){
      cout << "need nums" << endl;
    }
    else if(tagType=="ge1"){
      trueN = 2.35149;
      trueN_err = 1.27487;
    }
    else if(tagType=="ge2"){
      trueN =0.19454;
      trueN_err =0.0775882;
    }
    else  if(tagType=="data"){
      drawLines=false;
    }
    else{
    cout << "invalid tagType!" << endl;
    assert(0);
    }
  }
  else if(dataSet == "f10"){
    if(tagType=="nob"){
      trueN = 5.08044;
      trueN_err = 1.37063;
    }
    else if(tagType=="e1"){
      cout << "need nums" << endl;
    }
    else if(tagType=="ge1"){
     cout << "need nums" << endl;
    }
    else if(tagType=="ge2"){
      cout << "need nums" << endl;
    }
    else  if(tagType=="data"){
      drawLines=false;
    }
    else{
      cout << "invalid tagType!" << endl;
      assert(0);
    }
  }
  else{
    cout << "invalid dataset!" << endl;
    assert(0);
  }

  trueN = trueN*trueN_factor;
  trueN_err = trueN_err*trueN_factor;
  
  cout << "Opening multiResults/"+fileName+".dat" << endl;
  ifstream inFile("multiResults/"+fileName+".dat", std::ios::in);
  if(!inFile.good()){
    cout << "Could not open inFile!" << endl;
    return;
  }
  
  TCanvas *myC = new TCanvas("myC", "myC");
  myC->cd();
  
  const int size = 12;
  const int k = 10;
  TH1D *hrEb = new TH1D("hrEb", "hrEb", size, -0.5, size-0.5);

  double numEb, numEb_err, numEb_mean=0, numEb_sd=0, numEb_errT=0;
  string fill1, fill2, fill3;

  int i =0;
  while(inFile>>numEb>>fill3>>numEb_err){
    hrEb->Fill(i,numEb);
    hrEb->SetBinError(i+1, numEb_err);
    i++;
  }
  
  cout << "i: " << i << endl;
  cout << "Use first " << k << " bins for mean calculation. " << endl;
  for(int j=1; j<=k; j++){
    numEb_mean+=hrEb->GetBinContent(j);
    numEb_errT+=hrEb->GetBinError(j);
  }
  numEb_mean=numEb_mean/((double)k);

  for(int j=1; j<=k; j++){
    numEb_sd+=(hrEb->GetBinContent(j)-numEb_mean)*(hrEb->GetBinContent(j)-numEb_mean);
  }
  numEb_sd=sqrt(numEb_sd*1./((double)k));
  
  //mean and standard deviation
  cout << fileName << ":" << endl;
  cout << "numEb_mean: " << numEb_mean << " +- " << numEb_sd << endl;
  cout << "true: " << trueN << " +- " << trueN_err << endl; 
  // cout << "error prop" << endl;
  // cout << "numEb_mean: " << numEb_mean << " +- " << numEb_errT/((double)i) << endl;
  

  TLine *line0 = new TLine(-0.5, trueN, size-0.5, trueN);
  TLine *lineA = new TLine(-0.5, trueN-trueN_err, size-0.5, trueN-trueN_err);
  TLine *lineB = new TLine(-0.5, trueN+trueN_err, size-0.5, trueN+trueN_err);
  TBox *box = new TBox();
  line0->SetLineWidth(3);
  line0->SetLineColor(kGray);
  lineA->SetLineColor(kGray);
  lineB->SetLineColor(kGray);
  
  TAxis *yA;
  yA =  hrEb->GetYaxis();
  TAxis *xA; 
  xA =  hrEb->GetXaxis();
  yA->SetRangeUser(0,ymax);
  xA->SetRangeUser(-2.0,size+1);
  hrEb->SetLineWidth(2);
  //hrEb->SetLineColor(kRed);
  
  hrEb->SetTitle(type);
  yA->SetTitle("Predicted Number of QCD Events");
  //  yA->SetTitleOffset(1.2);
  xA->SetBinLabel(1,"0-90,10");
  xA->SetBinLabel(2,"10-90,10");
  xA->SetBinLabel(3,"20-90,10"); 
  xA->SetBinLabel(4,"30-90,10"); 
  xA->SetBinLabel(5,"40-90,10"); 
  xA->SetBinLabel(6,"50-90,10"); 
  xA->SetBinLabel(7,"10-70,10"); 
  xA->SetBinLabel(8,"10-80,10"); 
  xA->SetBinLabel(9,"10-100,10"); 
  xA->SetBinLabel(10,"10-110,10"); 
  xA->SetBinLabel(11,"10-90,5"); 
  xA->SetBinLabel(12,"10-90,20"); 
  xA->SetLabelSize(0.03);
  xA->SetTitle("Fit range min-max, Number of bins");
  
  TLegend *leg = new TLegend(.17,.77,.35,.8);
  leg->AddEntry(hrEb, legEntry, "P");// leg->AddEntry(hrEb, "a*exp(b*x)+c", "l");
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetLineStyle(0);
  leg->SetTextFont(42);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.04);

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
  text2->SetX(0.184);
  text2->SetY(0.88);
  //text2->SetLineWidth(2);
  text2->SetTextFont(42);
  text2->SetTextSizePixels(24);// dflt=28
  
  hrEb->Draw();

 
    
  // text1->Draw();
  text2->Draw();
  leg->Draw();
  gPad->SetRightMargin(0.05);
  if(drawLines){
    line0->Draw();
    lineA->Draw();
    lineB->Draw();
  }
  myC->Print("multiResults/"+fileName+".pdf");
  
  
  hrEb->Clear();
}

void multi(){
  
  
  //doMulti("pfpf", "multi_pfpf_nob_36", 40, "QCD MC", "nob");
  //doMulti("pfpf", "multi_pfpf_nob_contB36", 40, "QCD+SM MC", "nob");
  //doMulti("pfpf", "multi_pfpf_nob_contBS36", 40, "QCD+SM+LM13 MC", "nob");
  
  //doMulti("pfpf", "multi_pfpf_36_1", 13, "QCD MC", "ge1"); 
  //doMulti("pfpf", "multi_pfpf_contB36_1", 13, "QCD+SM MC", "ge1");      
  //doMulti("pfpf", "multi_pfpf_contBS36_1", 13, "QCD+SM+LM13 MC", "ge1");
  
  //doMulti("pfpf", "multi_pfpf_36", 2.5, "QCD MC", "ge2"); 
  //doMulti("pfpf", "multi_pfpf_contB36", 2.5, "QCC+SM MC", "ge2:); 
  //doMulti("pfpf", "multi_pfpf_contBS36", 2.5, "QCD+SM+LM13 MC", "ge2"); 
  
  // doMulti("pfpf", "multi_pfpf_nob_data", 70, "Data", "data"); 
  // doMulti("pfpf", "multi_pfpf_data_1", 18, "Data", "data");
  // doMulti("pfpf", "multi_pfpf_data", 2.5, "Data", "data"); 


  // fall10
  //doMulti("pfpf", "multi_pfpf_nob_36", 30, "QCD MC", "nob");
  // doMulti("pfpf", "multi_pfpf_nob_contB36", 30, "QCD+SM MC", "nob");
  //doMulti("pfpf", "multi_pfpf_nob_contBS36", 30, "QCD+SM+LM13 MC", "nob");
  //doMulti("pfpf", "multi_pfpf_nob_contB36sub", 30, "QCD+SM MC (w/ SM sub)", "nob");
  //  doMulti("pfpf", "multi_pfpf_nob_contBS36sub", 30, "QCD+SM+LM13 MC (w/ SM sub)", "nob");

  //doMulti("pfpf", "multi_pfpf_nob_data", 50, "Data", "data");
  //doMulti("pfpf", "multi_pfpf_nob_datasub", 50, "Data (w/ SM sub)", "data");

  //doMulti("pfpf", "multi_pfpf_nob_36fix", 20, "QCD MC (w/ fixed c)", "nob");
  //doMulti("pfpf", "multi_pfpf_nob_contB36fix", 20, "QCD+SM MC (w/ fixed c)", "nob");
  //doMulti("pfpf", "multi_pfpf_nob_contBS36fix", 20, "QCD+SM+LM13 MC (w/ fixed c)", "nob");
  doMulti("pfpf", "multi_pfpf_nob_datafix",20,"Data (w/ fixed c)", "data");
}
