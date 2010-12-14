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
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

void doMulti(TString type = "calo", TString add= "", double ymax= 50.){
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptStat("");
  
  //  TString type = "calo_nob";
  bool expc;
  bool exp;

  expc = true;
  exp =true;
  
  
  double trueN;
  double trueN_err;
  double trueN_factor = 36./50.; // the numbers below are for 50/pb

  if(type == "pfpf"){
    trueN = 0.269125;
    trueN_err = 0.107335;
    bool oneTag = false;
    if(oneTag){ //these numbers are for 36/pb
      trueN =2.34219;
      trueN_err =1.26983;
      trueN_factor=1.;
    }
   //ymax =4.;
    //expc = true;
  }
  else if( type == "pf"){
    trueN = 0.284114;
    trueN_err = 0.107509;
    //ymax = .5;
    // expc = true;
  }
  else if( type == "tc"){
    trueN = 0.463308;
    trueN_err = 0.151177;
    //ymax = 1.;
    //expc = true;
  }
  else if(type == "calo"){
    trueN = 0.604643;
    trueN_err = 0.0657204;
    //ymax = 2.;
    //  expc = true;
  }
  else if(type == "pfpf_nob"){
    trueN = 12.1611;
    trueN_err = 3.0787;
    // ymax = 50;
    //   expc = true;
  }
  else if(type == "pf_nob"){
    trueN = 9.95917;
    trueN_err = 2.53115;
    //ymax = 20;
    //  expc = true;
  }
  else if(type == "tc_nob"){
    trueN = 7.02955;
    trueN_err = 0.585675;
    //ymax = 13;
    // expc = true;
  }
  else if(type == "calo_nob"){
    trueN = 15.8387;
    trueN_err = .869234;
    //ymax = 45;
    // expc = true;
  }
  trueN = trueN*trueN_factor;
  trueN_err = trueN_err*trueN_factor;

  cout << "Opening multiResults/multi_"+type+add+".dat" << endl;
  ifstream inFile("multiResults/multi_"+type+add+".dat", std::ios::in);
  if(!inFile.good()){
    cout << "Could not open inFile!" << endl;
    return;
  }

  //  TFile fout("multiResults/multi_"+type+add+".root","RECREATE");

  TCanvas *myC = new TCanvas("myC", "myC", 640, 480);
  myC->cd();

  int size = 16;
  TH1D *hrE = new TH1D("hrE", "hrE", size, -0.5, size-0.5);
  TH1D *hrEb = new TH1D("hrEb", "hrEb", size, -0.5, size-0.5);

  double numE, numE_err, numE_mean=0, numE_sd=0, numE_errT=0;
  double numEb, numEb_err, numEb_mean=0, numEb_sd=0, numEb_errT=0;
  string fill1, fill2, fill3;

  int i =0;
  while(inFile>>numE>>fill1>>numE_err>>fill2>>numEb>>fill3>>numEb_err){
    //  cout << numE << " " <<numE_err<< " " <<numEb<< " " <<numEb_err<<endl;
    hrE->Fill(i,numE);
    hrE->SetBinError(i+1, numE_err);
    hrEb->Fill(i,numEb);
    hrEb->SetBinError(i+1, numEb_err);
    i++;
  }
  
  cout << "i: " << i << endl;
  for(int j=1; j<=i; j++){
    numE_mean+=hrE->GetBinContent(j);
    numEb_mean+=hrEb->GetBinContent(j);
    numE_errT+=hrE->GetBinError(j);
    numEb_errT+=hrEb->GetBinError(j);
  }
  numE_mean=numE_mean/((double)i);
  numEb_mean=numEb_mean/((double)i);
  for(int j=1; j<=i; j++){
    numE_sd+=(hrE->GetBinContent(j)-numE_mean)*(hrE->GetBinContent(j)-numE_mean);
    numEb_sd+=(hrEb->GetBinContent(j)-numEb_mean)*(hrEb->GetBinContent(j)-numEb_mean);
  }
  numE_sd=sqrt(1./((double)i)*numE_sd);
  numEb_sd=sqrt(1./((double)i)*numEb_sd);
  //mean and standard deviation
  // cout << "numE_mean: " << numE_mean << " +- " << numE_sd << endl;
  cout << "numEb_mean: " << numEb_mean << " +- " << numEb_sd << endl;
  cout << "true: " << trueN << " +- " << trueN_err << endl; 
  cout << type << add << endl;
  // cout << "error prop" << endl;
  // cout << "numE_mean: " << numE_mean << " +- " << numE_errT/((double)i) << endl;
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
  if(exp) yA = hrE->GetYaxis();
  if(expc) yA =  hrEb->GetYaxis();
  TAxis *xA; 
  if(exp) xA = hrE->GetXaxis();
  if(expc) xA =  hrEb->GetXaxis();
  yA->SetRangeUser(0,ymax);
  xA->SetRangeUser(-2.0,size+1);
  hrE->SetLineWidth(2);
  hrE->SetLineColor(kBlue);
  hrEb->SetLineWidth(2);
  hrEb->SetLineColor(kRed);
  
  hrE->SetTitle(type);
  hrEb->SetTitle(type);
  yA->SetTitle("Number of QCD Events per 36/pb");
  yA->SetTitleOffset(1.2);
  /* xA->SetBinLabel(1,"30-150,8");
  xA->SetBinLabel(2,"50-150,8");
  xA->SetBinLabel(3,"70-150,8");
  xA->SetBinLabel(4,"90-150,8");
  xA->SetBinLabel(5,"50-140,8");
  xA->SetBinLabel(6,"50-130,8");
  xA->SetBinLabel(7,"50-150,5");
  xA->SetBinLabel(8,"50-150,12");*/
  /*xA->SetBinLabel(1,"0-90,8");
  xA->SetBinLabel(2,"20-90,8");
  xA->SetBinLabel(3,"40-90,8");
  xA->SetBinLabel(4,"50-90,8");
  xA->SetBinLabel(5,"60-90,8");
  xA->SetBinLabel(6,"40-100,8");
  xA->SetBinLabel(7,"40-110,8");
  xA->SetBinLabel(8,"40-90,16");*/
  xA->SetBinLabel(1,"20-90,10");
  xA->SetBinLabel(2,"30-90,10");
  xA->SetBinLabel(3,"40-90,10");
  xA->SetBinLabel(4,"50-90,10");
  xA->SetBinLabel(5,"60-90,10");
  xA->SetBinLabel(6,"40-70,10");
  xA->SetBinLabel(7,"40-80,10");
  xA->SetBinLabel(8,"40-100,8");
  xA->SetBinLabel(9,"40-110,8");
  xA->SetBinLabel(10,"40-120,8");
  xA->SetBinLabel(11,"40-90,5");
  xA->SetBinLabel(12,"40-90,20");
  xA->SetBinLabel(13,"40-110,5");
  xA->SetBinLabel(14,"40-110,20");
  xA->SetBinLabel(15,"20-90,5");
  xA->SetBinLabel(16,"20-90,20");
  xA->SetTitle("Fit range min-max, Number of bins");
  


  TLegend *leg = new TLegend(.65,.75,.85,.85);
  if(exp)  leg->AddEntry(hrE, "a*exp(b*x)", "l");
  if(expc) leg->AddEntry(hrEb, "a*exp(b*x)+c", "l");
  leg->SetFillColor(0);
  leg->SetLineColor(0);
  leg->SetTextSize(0.04);

  
  if(exp && expc){
    hrEb->Draw();
    hrE->Draw("SAME");
  }
  if(exp && !expc) hrE->Draw();
  if(expc && !exp) hrEb->Draw();

 
//if(expc) hrEb->Draw("SAME");
  
  leg->Draw();
  bool drawLines =true;
  if(drawLines){
    line0->Draw();
    lineA->Draw();
    lineB->Draw();
  }
  myC->Print("multiResults/multi_"+type+add+".pdf");
 
  
  //hrE->Write();
  //fout.Close();
  hrE->Clear();
  hrEb->Clear();
}

void multi(){

  /*
  doMulti("pfpf_nob", "", 50.);
  doMulti("pfpf", "",3.);
  doMulti("pfpf_nob", "", 50.);
  doMulti("pfpf", "",3.);
 
  doMulti("calo_nob", "", 70.);
  doMulti("calo", "",4.5);
  doMulti("calo_nob", "",70.);
  doMulti("calo", "", 4.5);

  doMulti("pfpf_nob", "_contB", 50.);
  doMulti("pfpf", "_contB",3.);
  doMulti("pfpf_nob", "_contBS", 50.);
  doMulti("pfpf", "_contBS",3.);
 
  doMulti("calo_nob", "_contB", 70.);
  doMulti("calo", "_contB",4.5);
  doMulti("calo_nob", "_contBS",70.);
  doMulti("calo", "_contBS", 4.5);
  */
  
  /*
  doMulti("pfpf_nob", "_contBS_2", 20.);
  doMulti("pfpf_nob", "_2", 20.);
  doMulti("pfpf", "_2", 0.5);
  doMulti("pfpf", "_contBS_2", 0.5);
  */
  

  /*
  doMulti("pfpf", "_2L", .8);
  doMulti("pfpf", "_contBS_2L", 3);
  doMulti("pfpf", "_contB_2L",3);
  
  doMulti("pfpf_nob", "_2L", 20.);
  doMulti("pfpf_nob", "_contBS_2L", 50.);
  doMulti("pfpf_nob", "_contB_2L", 50.);
  */

  //doMulti("pfpf", "_22", 2);
  //doMulti("pfpf", "_contB22", 2);
  // doMulti("pfpf", "_data22",2);

  //doMulti("pfpf_nob", "_22", 50);
  //doMulti("pfpf_nob", "_contB22", 50);
  //doMulti("pfpf_nob", "_data22", 50);


  // doMulti("pfpf_nob", "_36", 35);
  //doMulti("pfpf_nob", "_contB36", 35);
  //doMulti("pfpf_nob", "_contBS36", 35);
  //doMulti("pfpf", "_36", 2); 
  //doMulti("pfpf", "_contB36", 2); 
  //doMulti("pfpf", "_contBS36", 2); 
  //doMulti("pfpf", "_36_1", 10);
  //doMulti("pfpf", "_contB36_1", 10);
  //doMulti("pfpf", "_contBS36_1", 10);
  
  doMulti("pfpf_nob", "_36c", 35);

}
