#include "assert.h"
#include <iostream>

#include "TSystem.h"
#include "TString.h"
#include "TChain.h"
#include "TH1D.h"
#include "TFile.h"

#include "eventLoop.C"

using namespace std;

TChain* QCDChain(){
  
  const TString treedirectory="makeTopologyNtuple/tree";
  TChain* In_qcd = new TChain(treedirectory);

  In_qcd->Add("/cu1/joshmt/toptuplePAT/flattrees/Ntuple_QCD_Pt15_*.root");
  In_qcd->Add("/cu1/joshmt/toptuplePAT/flattrees/Ntuple_QCD_Pt30_*.root");
  In_qcd->Add("/cu1/joshmt/toptuplePAT/flattrees/Ntuple_QCD_Pt80_*.root");
  In_qcd->Add("/cu1/joshmt/toptuplePAT/flattrees/QCD_Pt170/Ntuple_QCD_Pt170_*.root");
  In_qcd->Add("/cu1/joshmt/toptuplePAT/flattrees/QCD_Pt300/Ntuple_QCD_Pt300_*.root");
  In_qcd->Add("/cu1/joshmt/toptuplePAT/flattrees/QCD_Pt470/Ntuple_QCD_Pt470_*.root");
  In_qcd->Add("/cu1/joshmt/toptuplePAT/flattrees/QCD_Pt800/Ntuple_QCD_Pt800_*.root");
  In_qcd->Add("/cu1/joshmt/toptuplePAT/flattrees/QCD_Pt1400/Ntuple_QCD_Pt1400_*.root");
  cout << "In_qcd: " << In_qcd->GetListOfFiles()->GetEntries() << endl;
  return In_qcd;
}

void calculateQCDWeights(){
  float intLuminosity = 100;
  
  const TString treedirectory="makeTopologyNtuple/tree";

  TChain In_qcd15(treedirectory);
  In_qcd15.Add("/cu1/joshmt/toptuplePAT/flattrees/Ntuple_QCD_Pt15_*.root");
  assert(In_qcd15.GetListOfFiles()->GetEntries()==18);

  TChain In_qcd30(treedirectory);
  In_qcd30.Add("/cu1/joshmt/toptuplePAT/flattrees/Ntuple_QCD_Pt30_*.root");
  assert(In_qcd30.GetListOfFiles()->GetEntries()==19);

  TChain In_qcd80(treedirectory);
  In_qcd80.Add("/cu1/joshmt/toptuplePAT/flattrees/Ntuple_QCD_Pt80_*.root");
  assert(In_qcd80.GetListOfFiles()->GetEntries()==16);

  TChain In_qcd170(treedirectory);
  In_qcd170.Add("/cu1/joshmt/toptuplePAT/flattrees/QCD_Pt170/Ntuple_QCD_Pt170_*.root");
  assert(In_qcd170.GetListOfFiles()->GetEntries()==313);

  TChain In_qcd300(treedirectory);
  In_qcd300.Add("/cu1/joshmt/toptuplePAT/flattrees/QCD_Pt300/Ntuple_QCD_Pt300_*.root");
  assert(In_qcd300.GetListOfFiles()->GetEntries()==65);

  TChain In_qcd470(treedirectory);
  In_qcd470.Add("/cu1/joshmt/toptuplePAT/flattrees/QCD_Pt470/Ntuple_QCD_Pt470_*.root");
  assert(In_qcd470.GetListOfFiles()->GetEntries()==43);

  TChain In_qcd800(treedirectory);
  In_qcd800.Add("/cu1/joshmt/toptuplePAT/flattrees/QCD_Pt800/Ntuple_QCD_Pt800_*.root");
  assert(In_qcd800.GetListOfFiles()->GetEntries()==43);

  TChain In_qcd1400(treedirectory);
  In_qcd1400.Add("/cu1/joshmt/toptuplePAT/flattrees/QCD_Pt1400/Ntuple_QCD_Pt1400_*.root");
  assert(In_qcd1400.GetListOfFiles()->GetEntries()==24);


  const int nptbins = 9;
  double ptbins[]={0,15,30,80,170,300,470,800,1400,1000000};
  TH1D Hsigmaqcd("Hsigma","cross section of qcd events", nptbins, ptbins);
  TH1D Hweightqcd("Hweight","weight of qcd events", nptbins, ptbins);
  TH1D Hnqcd("Hnqcd","n of qcd events", nptbins, ptbins);
  TH1D Hnqcd15("Hnqcd15","n of qcd15 events", nptbins, ptbins);
  TH1D Hnqcd30("Hnqcd30","n of qcd30 events", nptbins, ptbins);
  TH1D Hnqcd80("Hnqcd80","n of qcd80 events", nptbins, ptbins);
  TH1D Hnqcd170("Hnqcd170","n of qcd170 events", nptbins, ptbins);
  TH1D Hnqcd300("Hnqcd300","n of qcd300 events", nptbins, ptbins);
  TH1D Hnqcd470("Hnqcd470","n of qcd470 events", nptbins, ptbins);
  TH1D Hnqcd800("Hnqcd800","n of qcd800 events", nptbins, ptbins);
  TH1D Hnqcd1400("Hnqcd1400","n of qcd1400 events", nptbins, ptbins);

  //QCD CROSS SECTIONS
  const double s_qcd15 = 876215000.0;
  const double s_qcd30 = 60411000.0;
  const double s_qcd80 = 923821.0;
  const double s_qcd170 = 25474.9;
  const double s_qcd300 =  1255.87;
  const double s_qcd470 =  87.9799;
  const double s_qcd800 =  2.18608;
  const double s_qcd1400 =  0.0112233;

  Hsigmaqcd.SetBinContent(1,0);
  Hsigmaqcd.SetBinContent(2,s_qcd15-s_qcd30);
  Hsigmaqcd.SetBinContent(3,s_qcd30-s_qcd80);
  Hsigmaqcd.SetBinContent(4,s_qcd80-s_qcd170);
  Hsigmaqcd.SetBinContent(5,s_qcd170-s_qcd300);
  Hsigmaqcd.SetBinContent(6,s_qcd300-s_qcd470);
  Hsigmaqcd.SetBinContent(7,s_qcd470-s_qcd800);
  Hsigmaqcd.SetBinContent(8,s_qcd800-s_qcd1400);
  Hsigmaqcd.SetBinContent(9,s_qcd1400);
  
  In_qcd15.Project("Hnqcd15","processPtHat");
  In_qcd30.Project("Hnqcd30","processPtHat");
  In_qcd80.Project("Hnqcd80","processPtHat");
  In_qcd170.Project("Hnqcd170","processPtHat");
  In_qcd300.Project("Hnqcd300","processPtHat");
  In_qcd470.Project("Hnqcd470","processPtHat");
  In_qcd800.Project("Hnqcd800","processPtHat");
  In_qcd1400.Project("Hnqcd1400","processPtHat");
  
  Hnqcd.Add(&Hnqcd15);
  cout << "After QCD_Pt15, Hnqcd.Integral()=" <<Hnqcd.Integral() << endl;
  Hnqcd.Add(&Hnqcd30);
  cout << "After QCD_Pt30, Hnqcd.Integral()=" <<Hnqcd.Integral() << endl;
  Hnqcd.Add(&Hnqcd80);
  cout << "After QCD_Pt80, Hnqcd.Integral()=" <<Hnqcd.Integral() << endl;
  Hnqcd.Add(&Hnqcd170);
  cout << "After QCD_Pt170, Hnqcd.Integral()=" <<Hnqcd.Integral() << endl;
  Hnqcd.Add(&Hnqcd300);
  cout << "After QCD_Pt300, Hnqcd.Integral()=" <<Hnqcd.Integral() << endl;
  Hnqcd.Add(&Hnqcd470);
  cout << "After QCD_Pt470, Hnqcd.Integral()=" <<Hnqcd.Integral() << endl;
  Hnqcd.Add(&Hnqcd800);
  cout << "After QCD_Pt800, Hnqcd.Integral()=" <<Hnqcd.Integral() << endl;
  Hnqcd.Add(&Hnqcd1400);
  cout << "After QCD_Pt1400, Hnqcd.Integral()=" <<Hnqcd.Integral() << endl;

  for (int i=1;i<=nptbins; i++) {
    //cout << "i " << i << ", H.nqcd.GetBinContent(i) " << Hnqcd.GetBinContent(i) << ", Hsigmaqcd.GetBinContent(i) " << Hsigmaqcd.GetBinContent(i) << endl;
    if (Hnqcd.GetBinContent(i) !=0) {
      Hweightqcd.SetBinContent(i, intLuminosity*Hsigmaqcd.GetBinContent(i)/Hnqcd.GetBinContent(i));
    }
    else {
      Hweightqcd.SetBinContent(i,0);
    }
  }

  TFile fout("weight_QCD.root","RECREATE");
  Hweightqcd.Write();
  fout.Close();
}


void calculateWeights(){

  gSystem->Load("eventLoop_C.so");

  calculateQCDWeights();

  TChain* In_qcd = QCDChain();
 
  eventLoop Lqcd(In_qcd);
  Lqcd.Loop();
}
