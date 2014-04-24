#include <iostream>
#include <vector>

#include "TCanvas.h"
#include "TString.h"
#include "TCut.h"
#include "TH1D.h"
#include "THStack.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TFile.h"
#include "TPad.h"
#include "TRandom3.h"
#include "TMath.h"
#include "TPaveText.h"
#include "Math/DistFunc.h"

#include "RooGlobalFunc.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooWorkspace.h"
#include "RooPlot.h"
#include "RooAddPdf.h"
#include "RooDataSet.h"
#include "RooFitResult.h"

#include "/uscms/home/kreis/Hbb/lhe/myLHE/LHEAnalyzers/drawTrees.h"

using namespace std;
using namespace RooFit;


//Global
std::vector<sample> backgroundVector;
std::vector<sample> signalVector;

TString varname = "m_jj";
int nbins = 30;
double h_min = 0; double h_max = 1800;
  

void loadSamples_step3(TString inputPath = ""){
  
  sampleVector.clear();
  backgroundVector.clear();
  
  /*
  sample sample_ZJets(inputPath+"ZJets_boostedMG_lumiWeighted/*");
  sample_ZJets.humanName = "ZJets";
  sample_ZJets.color = kGreen-3;
  backgroundVector.push_back(sample_ZJets);
  
  sample sample_WW(inputPath+"WW_lumiWeighted/*");
  sample_WW.humanName = "WW";
  sample_WW.color = kViolet+1;
  backgroundVector.push_back(sample_WW);

  sample sample_WZ(inputPath+"WZ_lumiWeighted/*");
  sample_WZ.humanName = "WZ";
  sample_WZ.color = kViolet+2;
  backgroundVector.push_back(sample_WZ);

  sample sample_ZZ(inputPath+"ZZ_lumiWeighted/*");
  sample_ZZ.humanName = "ZZ";
  sample_ZZ.color = kViolet+3;
  backgroundVector.push_back(sample_ZZ);
  */
  sample sample_SingleTop_sChannel(inputPath+"SingleTop_sChannel_lumiWeighted/*");
  sample_SingleTop_sChannel.humanName = "SingleTop_sChannel";
  sample_SingleTop_sChannel.color = kCyan;
  backgroundVector.push_back(sample_SingleTop_sChannel);
  
  sample sample_SingleTop_tChannel(inputPath+"SingleTop_tChannel_lumiWeighted/*");
  sample_SingleTop_tChannel.humanName = "SingleTop_tChannel";
  sample_SingleTop_tChannel.color = kCyan+1;
  backgroundVector.push_back(sample_SingleTop_tChannel);
  
  sample sample_SingleTop_tW(inputPath+"SingleTop_tW_lumiWeighted/*");
  sample_SingleTop_tW.humanName = "SingleTop_tW";
  sample_SingleTop_tW.color = kCyan+2;
  backgroundVector.push_back(sample_SingleTop_tW);
  /*
  sample sample_TTbar_Hadronic(inputPath+"TTbar_Hadronic_lumiWeighted/*");
  sample_TTbar_Hadronic.humanName = "TTbar_Hadronic";
  sample_TTbar_Hadronic.color = kAzure+7;
  backgroundVector.push_back(sample_TTbar_Hadronic);

  sample sample_TTbar_FullLept(inputPath+"TTbar_FullLept_lumiWeighted/*");
  sample_TTbar_FullLept.humanName = "TTbar_FullLept";
  sample_TTbar_FullLept.color = kAzure-1;
  backgroundVector.push_back(sample_TTbar_FullLept);

  sample sample_TTbar_SemiLept(inputPath+"TTbar_SemiLept_lumiWeighted/*");
  sample_TTbar_SemiLept.humanName = "TTbar_SemiLept";
  sample_TTbar_SemiLept.color = kAzure-3;
  backgroundVector.push_back(sample_TTbar_SemiLept);
  
  sample sample_WJetsPt70To100(inputPath+"WJetsPt70To100Madgraph_lumiWeighted/*");
  sample_WJetsPt70To100.humanName = "WJetsPt70To100";
  sample_WJetsPt70To100.color = kMagenta+3;
  backgroundVector.push_back(sample_WJetsPt70To100);

  sample sample_WJetsPt100To180(inputPath+"WJetsPt100To180Madgraph_lumiWeighted/*");
  sample_WJetsPt100To180.humanName = "WJetsPt100To180";
  sample_WJetsPt100To180.color = kMagenta+2;
  backgroundVector.push_back(sample_WJetsPt100To180);
 
  sample sample_WJetsPt180(inputPath+"WJetsPt180Madgraph_lumiWeighted/*");
  sample_WJetsPt180.humanName = "WJetsPt180";
  sample_WJetsPt180.color = kMagenta+1;
  backgroundVector.push_back(sample_WJetsPt180);
  */
  
  //Signal
  sample sample_WHiggs0P(inputPath+"Step2_WHiggs0P_M-125p6_8TeV-JHUGenV4-private_000_varsBDTsAdded");
  sample_WHiggs0P.humanName = "WHiggs0P";
  sample_WHiggs0P.color = kYellow+1;
  signalVector.push_back(sample_WHiggs0P);

  sample sample_WHiggs0M(inputPath+"Step2_WHiggs0M_M-125p6_8TeV-JHUGenV4-private_000_varsBDTsAdded");
  sample_WHiggs0M.humanName = "WHiggs0M";
  sample_WHiggs0M.color = kOrange-3;
  signalVector.push_back(sample_WHiggs0M);

}


void createRooHistPdfs_step3(TCut cuts){
  initStyle();
  gROOT->SetStyle("CMS");

  renewLegend(.7, .89, .89, .6);

  /////////////////////////////////////////
  //Create total background histogram
  /////////////////////////////////////////
  TH1D* hTotalBackground = new TH1D("hTotalBackground", "hTotalBackground", nbins, h_min, h_max);
  hTotalBackground->Sumw2();
  
  THStack* thestack = new THStack("thestack", "");
  
  //Loop over background vector
  for(unsigned int i=0; i < backgroundVector.size(); i++){

    TString thisName = "h"+backgroundVector.at(i).humanName;
    
    //Project tree and add to hTotalBackground
    TH1D hTemp(thisName, thisName, nbins, h_min, h_max);
    hTemp.Sumw2();
    (backgroundVector.at(i).chain)->Project(thisName, varname, cuts);
    backgroundVector.at(i).lastHistogram = hTemp;    
    hTotalBackground->Add(&backgroundVector.at(i).lastHistogram);
    cout << "Added " << backgroundVector.at(i).lastHistogram.Integral() << " events from " 
	 << backgroundVector.at(i).humanName << ", for a total of " << hTotalBackground->Integral() << endl;
    
    backgroundVector.at(i).lastHistogram.SetFillColor(backgroundVector.at(i).color);
    thestack->Add(&backgroundVector.at(i).lastHistogram);
  }
  cout << "Total background: " << hTotalBackground->Integral() << endl;
  

  ///////////////////////////////////
  //Create signal histograms
  //////////////////////////////////
  TH1D* hSignal0 = new TH1D("hSignal0", "hSignal0", nbins, h_min, h_max);
  hSignal0->Sumw2();
  (signalVector.at(0).chain)->Project("hSignal0", varname, cuts);
  signalVector.at(0).lastHistogram = *hSignal0;
  hSignal0->SetLineColor(signalVector.at(0).color);

  TH1D* hSignal1 = new TH1D("hSignal1", "hSignal1", nbins, h_min, h_max);
  hSignal1->Sumw2();
  (signalVector.at(1).chain)->Project("hSignal1", varname, cuts);
  signalVector.at(1).lastHistogram = *hSignal1;
  hSignal1->SetLineColor(signalVector.at(1).color);


  /////////////////////////////////
  //Draw templates
  /////////////////////////////////
  
  //Style
  hTotalBackground->SetMarkerSize(0);
  hSignal0->SetMarkerSize(0);
  hSignal1->SetMarkerSize(0);
  hTotalBackground->SetLineWidth(2);
  hSignal0->SetLineWidth(2);
  hSignal1->SetLineWidth(2);
 
  TCanvas * c1 = new TCanvas("c1", "c1", 640, 480);
  c1->cd();
  thestack->Draw("hist");
  thestack->GetYaxis()->SetTitle("Events");
  thestack->GetXaxis()->SetTitle(varname);
  hTotalBackground->Draw("HIST E1 SAME");
  hSignal0->Draw("HIST E1 SAME");
  hSignal1->Draw("HIST E1 SAME");
  
  //Legend
  leg->AddEntry(hSignal0->GetName(), "WH 0P", "L");
  leg->AddEntry(hSignal1->GetName(), "WH 0M", "L");
  leg->AddEntry(hTotalBackground->GetName(), "Total background", "L");
  for(int i=backgroundVector.size()-1; i>=0; i--){
    leg->AddEntry(backgroundVector.at(i).lastHistogram.GetName(), backgroundVector.at(i).humanName, "F");
  }
  leg->Draw();
 
  gPad->SetRightMargin(0.08);
  gPad->Modified();
  c1->SaveAs("c1.png");
  thestack->SetMinimum(0.1);
  gPad->SetLogy(1);
  gPad->Modified();
  c1->SaveAs("c1_log.png");


  /////////////////////////////////////////////////////////////////////////////
  //Create RooFit objets from hTotalBackground, hSignal0, and hSignal1
  /////////////////////////////////////////////////////////////////////////////
  RooRealVar mWH(varname, varname, h_min, h_max);

  RooDataHist background_rdh("background_rdh", "background_rdh", mWH, hTotalBackground);
  RooDataHist signal0_rdh("signal0_rdh", "signal0_rdh", mWH, hSignal0);
  RooDataHist signal1_rdh("signal1_rdh", "signal1_rdh", mWH, hSignal1);

  RooHistPdf pdf_background("pdf_background", "pdf_background", mWH, background_rdh);
  RooHistPdf pdf_signal0("pdf_signal0", "pdf_signal0", mWH, signal0_rdh);
  RooHistPdf pdf_signal1("pdf_signal1", "pdf_signal1", mWH, signal1_rdh);


  ////////////////////
  //Save Output
  ////////////////////
  TFile fout("fout.root", "RECREATE");
  fout.cd();
  for(unsigned int i=0; i < backgroundVector.size(); i++){
    backgroundVector.at(i).lastHistogram.Write();
  }
  hTotalBackground->Write();
  hSignal0->Write();
  hSignal1->Write();
  thestack->Write();
  c1->Write();

  RooWorkspace ws ("workspace");
  ws.autoImportClassCode(true);
  //ws.import(background_rdh, RecycleConflictNodes());
  //ws.import(signal0_rdh, RecycleConflictNodes());
  //ws.import(signal1_rdh, RecycleConflictNodes());
  ws.import(pdf_background, RecycleConflictNodes());
  ws.import(pdf_signal0, RecycleConflictNodes());
  ws.import(pdf_signal1, RecycleConflictNodes());
  ws.Write();

  fout.Close();

}








void createRooHistPdfs_post(TString inputFileName = "plots.root"){
  initStyle();
  gROOT->SetStyle("CMS");

  std::vector<TString> backgroundsVector;
  backgroundsVector.push_back("h_mass_mjj_Vtype3_highBoost__ewk");
  backgroundsVector.push_back("h_mass_mjj_Vtype3_highBoost__top");
  backgroundsVector.push_back("h_mass_mjj_Vtype3_highBoost__other");

  TFile* inputFile = new TFile(inputFileName, "READ");
  inputFile->cd();

  TH1D* hSignal0 = (TH1D*)inputFile->Get("h_mass_mjj_Vtype3_highBoost__sig");
  hSignal0->SetName("hSignal0");
  TH1D* hSignal1 = (TH1D*)inputFile->Get("h_mass_mjj_Vtype3_highBoost__sig_ALT");
  hSignal1->SetName("hSignal1");


  TH1D* hTotalBackground=0;
  //Loop over background vector
  for(unsigned int i=0; i < backgroundsVector.size(); i++){
    
    TH1D hTemp = *( (TH1D*)inputFile->Get(backgroundsVector[i]) );
    
    if(i==0) {
      hTotalBackground = (TH1D*)hTemp.Clone("hTotalBackground");
    }
    else{
      hTotalBackground->Add(&hTemp);
    }
    cout << "Added " << backgroundsVector[i] << " with " << hTemp.Integral() << " events." << endl;
  }
  cout << "Total background: " << hTotalBackground->Integral() << endl;


  /////////////////////////////////////////////////////////////////////////////
  //Create RooFit objets from hTotalBackground, hSignal0, and hSignal1
  /////////////////////////////////////////////////////////////////////////////
  double my_min = hTotalBackground->GetXaxis()->GetXmin();
  double my_max = hTotalBackground->GetXaxis()->GetXmax();
  cout << "RooRealVar " << varname << " has range " << my_min << " to " << my_max << endl;
  
  RooRealVar myVar(varname, varname, my_min, my_max);
  
  RooDataHist background_rdh("background_rdh", "background_rdh", myVar, hTotalBackground);
  RooDataHist signal0_rdh("signal0_rdh", "signal0_rdh", myVar, hSignal0);
  RooDataHist signal1_rdh("signal1_rdh", "signal1_rdh", myVar, hSignal1);
  
  RooHistPdf pdf_background("pdf_background", "pdf_background", myVar, background_rdh);
  RooHistPdf pdf_signal0("pdf_signal0", "pdf_signal0", myVar, signal0_rdh);
  RooHistPdf pdf_signal1("pdf_signal1", "pdf_signal1", myVar, signal1_rdh);
  
  
  ////////////////////
  //Save Output
  ////////////////////
  TFile fout("fout.root", "RECREATE");
  fout.cd();
  for(unsigned int i=0; i < backgroundVector.size(); i++){
    backgroundVector.at(i).lastHistogram.Write();
  }
  hTotalBackground->Write();
  hSignal0->Write();
  hSignal1->Write();
  
  RooWorkspace ws ("workspace");
  ws.autoImportClassCode(true);
  //ws.import(background_rdh, RecycleConflictNodes());
  //ws.import(signal0_rdh, RecycleConflictNodes());
  //ws.import(signal1_rdh, RecycleConflictNodes());
  ws.import(pdf_background, RecycleConflictNodes());
  ws.import(pdf_signal0, RecycleConflictNodes());
  ws.import(pdf_signal1, RecycleConflictNodes());
  ws.Write();
  
  fout.Close();
  

  
}



//Taken from
//https://github.com/cms-analysis/HiggsAnalysis-HZZ4l_Combination/blob/master/SignalSeparation/extractSignificanceStats.C
//NOTE: SM and PS are swapped between code and plot legend!!!!! Be careful about getting order of arguments right.
int analyzeToysHZZ4l(TH1D* hSM, std::vector<float> v_SM, TH1D* hPS, std::vector<float> v_PS){

  sort(v_SM.begin(),v_SM.end());//sort in ascending order
  sort(v_PS.begin(),v_PS.end()); 
  //sort(v_Obs.begin(),v_Obs.end());
  int ntoysSM= hSM->GetEntries();
  int ntoysPS= hPS->GetEntries();

  //we assume that SM is on the right and PS on the left of zero
  if(v_PS.at(0)>v_SM.at(ntoysSM-1)){
    cout<<"Swapped distributions !!! The alternative model shouldstay on the negative side of the significance."<<endl;
    cout<<"Please edit the code and change the sign of q when filling histos and vectors in the loop on tree entries"<<endl;
    return 1;
  }
  float medianSM=v_SM.at(int(ntoysSM/2));
  float medianPS=v_PS.at(int(ntoysPS/2));
  cout<<"Toys generated "<<ntoysSM<<"\t"<<ntoysPS<<endl;
  cout<<"Mean of SM/PS hypothesis: "<<hSM->GetMean()<<"\t"<<hPS->GetMean()<<endl;
  cout<<"RMS  of SM/PS hypothesis: "<<hSM->GetRMS()<<"\t"<<hPS->GetRMS()<<endl;
  cout<<"Median of SM/PS hypothesis: "<<medianSM<<"\t"<<medianPS<<endl;

  const float step=0.05;
  float coverage=0.0;
  float diff=10.0;
  float cut=v_PS.at(0)-step;
  float crosspoint=-99.0;
  int startSM=ntoysSM-1, startPS=0;
  cout<<"Starting to loop with cut at "<<cut<<endl;

  /*
  while(cut<=v_SM.at(ntoysSM-1)+step){
    //    if(int(cut*100)%100==0)
cout<<"Cutting at "<<cut<<endl;
    for(int iSM=startSM;iSM>=0;iSM--){
      
      if(v_SM.at(iSM)<cut){
	startSM=ntoysSM-iSM;
	//break;
      }
      else cout<<"SM "<<v_SM.at(iSM)<<" > "<<cut<<endl;
    }

    for(int iPS=startPS;iPS<ntoysPS;iPS++){
      if(v_PS.at(iPS)>cut){
	startPS=iPS;
	//break;
      }
      else cout<<v_PS.at(iPS)<<" < "<<cut<<endl;
  
    }
    float fracSM=(ntoysSM-startSM)/ntoysSM;
    float fracPS=startPS/ntoysPS;
    cout<<"Frac "<<fracSM<<" "<<fracPS<<endl;
    if(fabs(fracSM-fracPS)<diff){
      diff=fabs(fracSM-fracPS);
      coverage=fabs(fracSM-fracPS)/2.0;
      crosspoint=cut;
      cout<<"New coverage="<<coverage<<" at xpoint="<<crosspoint<<"  "<<startSM<<endl;
    }
    cut+=step;
  }//end while loop
  */
  cout<<"Finished loop on vector elements, min is "<<diff<<" cut is at "<<cut<<endl;
  cout<<"q value where SM and ALT distributions have same area on opposite sides: "<<crosspoint<<endl;
  cout<<"Coverage "<<coverage<<endl;
  float separation=2*ROOT::Math::normal_quantile_c(1.0 - coverage, 1.0);
  cout<<"Separation: "<<separation<<endl<<endl<<endl;

  float integralSM=hSM->Integral();
  float integralPS=hPS->Integral();
 
  float tailSM=hSM->Integral(1,hSM->FindBin(medianPS))/integralSM;
  float tailPS=hPS->Integral(hPS->FindBin(medianSM),hPS->GetNbinsX())/integralPS;
  cout<<"Tail prob SM: "<<tailSM<<"  ("<<ROOT::Math::normal_quantile_c(tailSM,1.0) <<" sigma)"<<endl;
  cout<<"Tail prob PS: "<<tailPS<<"  ("<<ROOT::Math::normal_quantile_c(tailPS,1.0) <<" sigma)"<<endl;

  diff=10.0;
  coverage=0.0;
  for(int i=1;i<hSM->GetNbinsX();i++){
    
    float fracSM=hSM->Integral(1,i) / integralSM;
    float fracPS=hPS->Integral(i,hPS->GetNbinsX()) / integralPS;
    if(fabs(fracSM-fracPS)<diff){
      diff=fabs(fracSM-fracPS);
      coverage=(fracSM+fracPS)/2.0;
    }

  }

  float sepH= 2*ROOT::Math::normal_quantile_c(1.0 - coverage, 1.0);
  cout<<"Separation from histograms = "<<sepH<<" with coverage "<<coverage<<endl;

  /*
  cout << "OBSERVED SIGNIFICANCE" << endl;

  cout << "observation: " << v_Obs[0] << endl;
  cout << "bin: " << hObs->GetMaximumBin() << endl;
  
  cout << " --------------- " << endl;
  double obsPval_SM = 1-hSM->Integral(0,hObs->GetMaximumBin())/hSM->Integral();
  cout << "pvalue SM: " << obsPval_SM << endl;
  cout << "signif SM: " << ROOT::Math::normal_quantile_c(obsPval_SM,1.0) << endl;;
  double obsPval_PS =  hPS->Integral(0,hObs->GetMaximumBin())/hPS->Integral();
  cout << "pvalue PS: " << obsPval_PS << endl;
  cout << "signif PS: " << ROOT::Math::normal_quantile_c(obsPval_PS,1.0) << endl;;
  */

  gStyle->SetOptStat(0);
  TCanvas *c1=new TCanvas("c1","c1",500,500);
  c1->cd();
  hSM->Rebin(50);
  hPS->Rebin(50);
  float maxhSM=hSM->GetBinContent(hSM->GetMaximumBin());
  float maxhPS=hPS->GetBinContent(hPS->GetMaximumBin());
  if(maxhPS>maxhSM){
    hSM->SetMaximum(maxhPS*1.15);
    hPS->SetMaximum(maxhPS*1.15);
  }
  else{
    hSM->SetMaximum(maxhSM*1.15);
    hPS->SetMaximum(maxhSM*1.15);
  }
  hSM->SetXTitle("S = -2 #times ln(L_{1}/L_{2})");
  hSM->SetYTitle("Generated experiments");
  hPS->SetXTitle("S = -2 #times ln(L_{1}/L_{2})");
  hPS->SetYTitle("Generated experiments");
  hSM->SetLineColor(kMagenta-3);
  hSM->SetFillColor(kMagenta-3);
  hSM->SetLineWidth(2);
  hSM->SetFillStyle(3605);
  hPS->SetLineColor(kBlue+1);
  hPS->SetFillColor(kBlue+1);
  hPS->SetLineWidth(2);
  hPS->SetFillStyle(3695);
  //hObs->SetLineColor(kGreen+3);
  //hObs->SetLineWidth(2);
  hSM->Draw();
  hPS->Draw("sames");
  
  /*
  TArrow *obsArrow  = new TArrow(v_Obs[0],hSM->GetMaximum()/2.,v_Obs[0],0.0,.05,"|>");
  obsArrow->SetLineWidth(3);
  obsArrow->Draw("same");
  //hObs->Draw("sames");
  */

  TLegend *leg1 = new TLegend(0.7,0.6,0.9,0.9);
  leg1->SetFillColor(0);
  leg1->SetBorderSize(0);
  leg1->AddEntry(hSM,"  PS, 0-","f");
  leg1->AddEntry(hPS,"  SM, 0+","f");
  leg1->Draw();


  TPaveText pt(0.16,0.95,0.45,0.99,"NDC");
  pt.SetFillColor(0);
  pt.AddText("CMS Expected");
  pt.SetBorderSize(0);
  TPaveText pt2(0.55,0.95,0.99,0.99,"NDC");
  pt2.SetFillColor(0);
  //pt2.AddText(Form(" #sqrt{s} = 7 TeV, L = %.3f fb^{-1}; #sqrt{s} = 8 TeV, L = %.3f fb^{-1}",lumi7TeV,lumi8TeV));
  pt2.SetBorderSize(0);
  pt.Draw();
  pt2.Draw();
  c1->SaveAs("sigsep_combine.eps");
  c1->SaveAs("sigsep_combine.root");

  return 0;

}


void doHypothesisTest(){
  initStyle();
  gROOT->SetStyle("CMS");

  int nToys = 4000;
  bool setSBConstant = true;

  //////////////////////
  //Get input
  //////////////////////
  TFile fin("fout.root", "READ");
 
  TH1D hTotalBackground = *((TH1D*)fin.Get("hTotalBackground"));
  TH1D hSignal0 = *((TH1D*)fin.Get("hSignal0"));
  TH1D hSignal1 = *((TH1D*)fin.Get("hSignal1"));
  double nBkg_ext = hTotalBackground.Integral();
  double nSig0_ext = hSignal0.Integral();
  double nSig1_ext = hSignal1.Integral();
 
  RooWorkspace *ws = (RooWorkspace*)fin.Get("workspace");
  RooHistPdf pdf_background = *((RooHistPdf*)ws->pdf("pdf_background"));
  //RooHistPdf pdf_backgroundCopy = *(RooHistPdf*)pdf_background.Clone();
  RooHistPdf pdf_signal0 = *((RooHistPdf*)ws->pdf("pdf_signal0"));
  RooHistPdf pdf_signal1 = *((RooHistPdf*)ws->pdf("pdf_signal1"));
  fin.Close();
  cout << &pdf_background << " " << &pdf_signal0 << " " << &pdf_signal1 << endl;
  cout << "nBkg_ext " << nBkg_ext << ", nSig0_ext " << nSig0_ext << ", nSig1_ext " << nSig1_ext << endl;

  //Plot pdfs
  TCanvas *c_pdfs = new TCanvas("c_pdfs", "c_pdfs", 640, 480);
  c_pdfs->cd();
  gPad->SetRightMargin(0.08);
  RooRealVar x(varname, varname, h_min, h_max);
  RooPlot *xframe = x.frame();
  pdf_background.plotOn(xframe, Name("background"));
  pdf_signal0.plotOn(xframe,Name("signal0"), LineColor(kRed));
  pdf_signal1.plotOn(xframe,Name("signal1"), LineColor(kViolet));
  xframe->SetYTitle("Projection");
  xframe->Draw();
  renewLegend(.7, .89, .89, .6);
  leg->AddEntry("background", "background", "L");
  leg->AddEntry("signal0", "0P", "L");
  leg->AddEntry("signal1", "0M", "L");
  leg->Draw();
  
  
  ////////////////////////////////////////
  //Composite models (RooFit Manual Ch 3)
  ////////////////////////////////////////
  
  //nSig0_ext = nSig0_ext*100;//for fun
  //nSig1_ext = nSig1_ext*100;//for fun
  
  //For signal0
  RooRealVar nSig0("nSig0", "nSig0", nSig0_ext, 0.0, 10.0*nSig0_ext);
  RooRealVar nBkg0("nBkg0", "nBkg0", nBkg_ext, 0.0, 10.0*nBkg_ext);
  if(setSBConstant){
    nSig0.setConstant();
    nBkg0.setConstant();
  }
  RooAddPdf model0("model0", "model0", RooArgList(pdf_signal0, pdf_background), RooArgList(nSig0, nBkg0));
  
  //For signal1
  RooRealVar nSig1("nSig1", "nSig1", nSig1_ext, 0.0, 10.0*nSig1_ext);
  RooRealVar nBkg1("nBkg1", "nBkg1", nBkg_ext, 0.0, 10.0*nBkg_ext);
  if(setSBConstant){
    nSig1.setConstant();
    nBkg1.setConstant();
  }
  RooAddPdf model1("model1", "model1", RooArgList(pdf_signal1, pdf_background), RooArgList(nSig1, nBkg1));
  //RooAddPdf model1("model1", "model1", RooArgList(pdf_signal1, pdf_backgroundCopy), RooArgList(nSig1, nBkg1));


  //Plot composite models
  TCanvas *c_models = new TCanvas("c_models", "c_models", 640, 480);
  c_models->cd();
  gPad->SetRightMargin(0.08);
  RooPlot *xframe2 = x.frame();
  model0.plotOn(xframe2, Components(pdf_background), LineStyle(kDashed));
  model0.plotOn(xframe2);
  model1.plotOn(xframe2, LineColor(kRed));
  xframe2->Draw();


  ///////////////////////////////
  //Toys (RooFit Manual Ch 3)
  //////////////////////////////
  
  TH1D* hTestStatistic_data0 = new TH1D("hTestStatistic_data0", "hTestSTatistic_data0", 8000, -15, 15);
  TH1D* hTestStatistic_data1 = new TH1D("hTestStatistic_data1", "hTestSTatistic_data1", 8000, -15, 15);
  std::vector<float> v_data0, v_data1;

  for(int i=0; i<nToys; i++){

    RooDataSet* data0 = model0.generate(x, Extended(kTRUE));
    RooFitResult* r_data0_model0 = model0.fitTo(*data0, Extended(kTRUE), Save(kTRUE));
    RooFitResult* r_data0_model1 = model1.fitTo(*data0, Extended(kTRUE), Save(kTRUE));
    double testStatistic_data0 = 2.*(r_data0_model0->minNll()-r_data0_model1->minNll()); //Same as -2ln(L1/L0) //FIXME sign?
    hTestStatistic_data0->Fill(testStatistic_data0);
    v_data0.push_back(testStatistic_data0);

    RooDataSet* data1 = model1.generate(x, Extended(kTRUE));
    RooFitResult* r_data1_model0 = model0.fitTo(*data1, Extended(kTRUE), Save(kTRUE));
    RooFitResult* r_data1_model1 = model1.fitTo(*data1, Extended(kTRUE), Save(kTRUE));
    double testStatistic_data1 = 2.*(r_data1_model0->minNll()-r_data1_model1->minNll()); //Same as -2ln(L1/L0) //FIXME sign?
    hTestStatistic_data1->Fill(testStatistic_data1);
    v_data1.push_back(testStatistic_data1);

    cout << testStatistic_data0 << endl;
    cout << testStatistic_data1 << endl;

    /*
    //Draw one toy
    TCanvas *c_aToy = new TCanvas("c_aToy", "c_aToy", 640, 480);
    c_aToy->cd();
    gPad->SetRightMargin(0.08);
    RooPlot *xframe3 = x.frame();
    data0->plotOn(xframe3);
    model0.plotOn(xframe3);
    model1.plotOn(xframe3, LineColor(kRed));
    xframe3->Draw();
    */
  }//toy loop

  //analyzeToysHZZ4l(hTestStatistic_data0, v_data0, hTestStatistic_data1, v_data1);
  
  if(true) {
    analyzeToysHZZ4l(hTestStatistic_data1, v_data1, hTestStatistic_data0, v_data0); //swapped
  }
  else{
    hTestStatistic_data0->Rebin(50);
    hTestStatistic_data1->Rebin(50);
  }

  //Style
  hTestStatistic_data0->SetLineColor(kBlue);
  hTestStatistic_data1->SetLineColor(kMagenta);

  TCanvas *c_testStatistics = new TCanvas("c_testStatistics", "c_testStatistics", 640, 480);
  c_testStatistics->cd();
  hTestStatistic_data0->Draw("HIST");
  hTestStatistic_data1->Draw("HIST SAME");
  c_testStatistics->SaveAs("mycode.png");
  
}


void hypothesisTest(){
  initStyle();
  gROOT->SetStyle("CMS");
  
  //******************************//
  //***********STEP 3*************//
  //******************************//

  //Options
  //TString inputPath = "/eos/uscms/store/user/sethzenz/fromdcache/Ntuple_Step1V42_Step2Tag_EDMV42_Step2_V6_MC_varsAddedSummed_v19/nominal/";
  //TCut cuts = "PUweight*weightTrig2012A*(1000.0*19.6/effectiveLumi)*(H.HiggsFlag==1 && (Vtype==2||Vtype==3) && V.pt>130 && hJet_pt[0]>30 && hJet_pt[1]>30 && abs(hJet_eta[0])<2.5 && abs(hJet_eta[1])<2.5 && hJet_id[0]==1 && hJet_id[1]==1 && hJet_puJetIdL[0]>0 && hJet_puJetIdL[1]>0 && hJet_csv[0]>0 && hJet_csv[1]>0)";  
  
  //loadSamples(inputPath);
  //createRooHistPdfs(cuts); //variable and binning set in here
  //doHypothesisTest();

  //*****************************//
  //******** STEP 4 POST ********//
  //*****************************//

  createRooHistPdfs_post(); // creates hTotalBackground, hSignal0, hSignal1 in fout.root
  doHypothesisTest();

}
