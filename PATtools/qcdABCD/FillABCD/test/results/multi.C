#include "TGraphErrors.h"
#include "TVectorD.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TLine.h"
#include "TLegend.h"
#include "TBox.h"
#include "TROOT.h"
#include "TStyle.h"
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

void multi(){
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  
  //pf pf
  //double trueN = 0.269125;
  //double trueN_err = 0.107335;
  
  double trueN = 0.463308;
  double trueN_err = 0.151177;

  ifstream inFile("multi_tc.dat", std::ios::in);
  if(!inFile.good()){
    cout << "Could not open inFile!" << endl;
    return;
  }
  
  TCanvas *myC = new TCanvas("myC", "myC", 640, 480);
  myC->cd();

  int size = 8;
  double vE[size];
  double vE_err[size];
  double vEb[size];
  double vEb_err[size];
  double vEx[size];
  double vEx_err[size];

  double numE, numE_err;
  double numEb, numEb_err;
  string fill1, fill2, fill3;

  int i =0;
  while(inFile>>numE>>fill1>>numE_err>>fill2>>numEb>>fill3>>numEb_err){
    //  cout << numE << " " <<numE_err<< " " <<numEb<< " " <<numEb_err<<endl;
    vE[i] = numE;
    vE_err[i] = numE_err;
    vEb[i] = numEb;
    vEb_err[i] = numEb_err;
    vEx[i] = (double)i+1;
    vEx_err[i] = 0.0;
    // cout << vE[i] << " " << vE_err[i] << " " << vEb[i] << " " << vEb_err[i] << " " << vEx[i] << " " << vEx_err[i] << endl;
    i++;
  }
  
  
  TLine *line0 = new TLine(1, trueN, size, trueN);
  TLine *lineA = new TLine(1, trueN-trueN_err, size, trueN-trueN_err);
  TLine *lineB = new TLine(1, trueN+trueN_err, size, trueN+trueN_err);
  TBox *box = new TBox();
  line0->SetLineWidth(3);
  line0->SetLineColor(kGray);
  lineA->SetLineColor(kGray);
  lineB->SetLineColor(kGray);
  

  TGraphErrors * grE = new TGraphErrors(size, vEx, vE, vEx_err, vE_err);
  TGraphErrors * grEb = new TGraphErrors(size, vEx, vEb, vEx_err, vEb_err);

  TAxis *yA = grE->GetYaxis();
  TAxis *xA = grE->GetXaxis();
  yA->SetRangeUser(0,1.);
  xA->SetRangeUser(-2.0,size+1);
  grE->SetLineWidth(2);
  grE->SetLineColor(kBlue);
  grEb->SetLineWidth(2);
  grEb->SetLineColor(kRed);
  
  grE->SetTitle("");
  yA->SetTitle("Number of QCD Events per 50/pb");
  yA->SetTitleOffset(1.2);
  

  TLegend *leg = new TLegend(.65,.75,.85,.85);
  leg->AddEntry(grE, "a*exp(b*x)", "l");
  leg->AddEntry(grEb, "a+exp(b*x)+c", "l");
  leg->SetFillColor(0);
  leg->SetLineColor(0);
  leg->SetTextSize(0.04);

  grE->Draw("AP");
  grEb->Draw("P");
  
  leg->Draw();
  line0->Draw();
  lineA->Draw();
  lineB->Draw();
  // myC->Print("multi.pdf");

}
