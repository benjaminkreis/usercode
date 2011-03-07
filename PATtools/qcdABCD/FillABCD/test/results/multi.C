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
 
  if(dataSet == "f10"){
    if(tagType=="pre"){
      cout << "need nums" << endl;
    }
    else if(tagType=="eq1"){
      trueN  = 3.53319;
      trueN_err = 1.37855;
    } 
    else if(tagType=="ge1"){
      trueN = 4.03775;
      trueN_err = 1.39968;
    }
    else if(tagType=="ge2"){
      trueN = 0.504563;
      trueN_err = 0.242294;
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
  
  const int size = 9;
  const int k = 9;
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
  line0->SetLineColor(kAzure);
  lineA->SetLineColor(kAzure);
  lineB->SetLineColor(kAzure);
  
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
  /*
  xA->SetBinLabel(1,"60");
  xA->SetBinLabel(2,"70");
  xA->SetBinLabel(3,"80"); 
  xA->SetBinLabel(4,"90"); 
  xA->SetBinLabel(5,"100"); 
  xA->SetBinLabel(6,"110"); 
  xA->SetBinLabel(7,"120"); 
  xA->SetBinLabel(8,"130"); 
  xA->SetBinLabel(9,"140"); 
  xA->SetBinLabel(10,"150"); 
  xA->SetBinLabel(11,"160"); 
  xA->SetBinLabel(12,"170"); 
  xA->SetBinLabel(13,"180"); 
  xA->SetBinLabel(14,"190"); 
  xA->SetBinLabel(15,"200"); 
  xA->SetLabelSize(0.04);
  xA->SetTitle("Fit range max [MET in GeV]");
  */
  xA->SetBinLabel(1,"110");
  xA->SetBinLabel(2,"115");
  xA->SetBinLabel(3,"120");
  xA->SetBinLabel(4,"125");
  xA->SetBinLabel(5,"130");
  xA->SetBinLabel(6,"135");
  xA->SetBinLabel(7,"140");
  xA->SetBinLabel(8,"145");
  xA->SetBinLabel(9,"150");
  xA->SetLabelSize(0.04);                                                                                                                                                                                                                                          
  xA->SetTitle("control region max [MET in GeV]");

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
  //doMulti("pfpf", "multi_pfpf_nob_datafix",20,"Data (w/ fixed c)", "data");
 
  //doMulti("pfpf", "ge1_Q_start0", 6, "madgraph QCD MC, >= 1 b-tags", "ge1");
  //doMulti("pfpf", "ge1_Q_start10", 6, "QCD MC", "ge1");
  //doMulti("pfpf", "ge1_Q_start20", 6, "QCD MC", "ge1");
  //doMulti("pfpf", "ge2_Q_start0", .6, "madgraph QCD MC, >= 2 b-tags", "ge2");
  //doMulti("pfpf", "ge2_Q_start10", 1, "QCD MC", "ge2");
  //doMulti("pfpf", "ge2_Q_start20", 1, "QCD MC", "ge2");
  //doMulti("pfpf", "ge1_QLM13_start0", 6, "QCD+LM13 MC", "ge1");
  //doMulti("pfpf", "ge2_QLM13_start0", 2, "QCD+LM13 MC", "ge2");
  //doMulti("pfpf", "ge1_P_start0", 6, "PYTHIA QCD MC", "ge1");
  //doMulti("pfpf", "ge1_PU_start0", 6, "PYTHIA Pileup QCD MC, >= 1 b-tags", "ge1");
  //doMulti("pfpf", "ge2_PU_start0", 1, "PYTHIA Pileup QCD MC", "ge2");
  //doMulti("pfpf", "pre_PU_start0", 30, "PYTHIA Pileup QCD MC, pretag", "pre");

  //doMulti("pfpf", "ge1_PUR_start0", 6, "PYTHIA Pileup QCD MC w/ JERbias, >=1 b-tags", "ge1");
  //doMulti("pfpf", "ge2_PUR_start0", 2, "PYTHIA Pileup QCD MC w/ JERbias, >=2 b-tags", "ge2");

  
  doMulti("pfpf","CR_eq1_PU_start60", 16, "PYTHIA PU QCD MC, =1 b-tags", "eq1");
  doMulti("pfpf","CR_ge1_PU_start60", 16, "PYTHIA PU QCD MC, >=1 b-tags", "ge1");
  doMulti("pfpf","CR_ge2_PU_start60", 5, "PYTHIA PU QCD MC, >=2 b-tags", "ge2");
  doMulti("pfpf","CR_eq1_PU_start65", 16, "PYTHIA PU QCD MC, =1 b-tags", "eq1");
  doMulti("pfpf","CR_ge1_PU_start65", 16, "PYTHIA PU QCD MC, >=1 b-tags", "ge1");
  doMulti("pfpf","CR_ge2_PU_start65", 5, "PYTHIA PU QCD MC, >=2 b-tags", "ge2");
  doMulti("pfpf","CR_eq1_PU_start70", 16, "PYTHIA PU QCD MC, =1 b-tags", "eq1");
  doMulti("pfpf","CR_ge1_PU_start70", 16, "PYTHIA PU QCD MC, >=1 b-tags", "ge1");
  doMulti("pfpf","CR_ge2_PU_start70", 5, "PYTHIA PU QCD MC, >=2 b-tags", "ge2");
  doMulti("pfpf","CR_eq1_PU_start75", 16, "PYTHIA PU QCD MC, =1 b-tags", "eq1");
  doMulti("pfpf","CR_ge1_PU_start75", 16, "PYTHIA PU QCD MC, >=1 b-tags", "ge1");
  doMulti("pfpf","CR_ge2_PU_start75", 5, "PYTHIA PU QCD MC, >=2 b-tags", "ge2");
  doMulti("pfpf","CR_eq1_PU_start80", 16, "PYTHIA PU QCD MC, =1 b-tags", "eq1");
  doMulti("pfpf","CR_ge1_PU_start80", 16, "PYTHIA PU QCD MC, >=1 b-tags", "ge1");
  doMulti("pfpf","CR_ge2_PU_start80", 5, "PYTHIA PU QCD MC, >=2 b-tags", "ge2");
  doMulti("pfpf","CR_eq1_PU_start85", 16, "PYTHIA PU QCD MC, =1 b-tags", "eq1");
  doMulti("pfpf","CR_ge1_PU_start85", 16, "PYTHIA PU QCD MC, >=1 b-tags", "ge1");
  doMulti("pfpf","CR_ge2_PU_start85", 5, "PYTHIA PU QCD MC, >=2 b-tags", "ge2");
  doMulti("pfpf","CR_eq1_PU_start90", 16, "PYTHIA PU QCD MC, =1 b-tags", "eq1");
  doMulti("pfpf","CR_ge1_PU_start90", 16, "PYTHIA PU QCD MC, >=1 b-tags", "ge1");
  doMulti("pfpf","CR_ge2_PU_start90", 5, "PYTHIA PU QCD MC, >=2 b-tags", "ge2");
  doMulti("pfpf","CR_eq1_PU_start95", 16, "PYTHIA PU QCD MC, =1 b-tags", "eq1");
  doMulti("pfpf","CR_ge1_PU_start95", 16, "PYTHIA PU QCD MC, >=1 b-tags", "ge1");
  doMulti("pfpf","CR_ge2_PU_start95", 5, "PYTHIA PU QCD MC, >=2 b-tags", "ge2");
  doMulti("pfpf","CR_eq1_PU_start100", 16, "PYTHIA PU QCD MC, =1 b-tags", "eq1");
  doMulti("pfpf","CR_ge1_PU_start100", 16, "PYTHIA PU QCD MC, >=1 b-tags", "ge1");
  doMulti("pfpf","CR_ge2_PU_start100", 5, "PYTHIA PU QCD MC, >=2 b-tags", "ge2");





}
