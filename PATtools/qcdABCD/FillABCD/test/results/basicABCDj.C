
#include <iostream>
#include <iomanip>
#include <assert.h>


#include "TROOT.h"
#include "TStyle.h"

#include "TMinuit.h"

#include "TFile.h"
#include "TString.h"
#include "TCanvas.h"

#include "TChain.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TF1.h"
#include "TGraphErrors.h"

#include "analyzeFillABCDInput.h"

using namespace std;

//make sure the following things are set: cut passb function, subtractSM bool, SMfactor,  

const TString bcut = "eq1b";

bool passb(int nbtags){
  if (bcut == "eq1b") {
    if (nbtags==1) return true;
    else return false;
  }
  else if (bcut == "ge1b") {
    if (nbtags>=1) return true;
    else return false;
  }
  else if (bcut == "ge2b") {
    if (nbtags>=2) return true;
    else return false;
  }
  
  assert(0);
  return false;
}

double *doBasicABCD(double borderv1a = 0., double borderv1b = 0., int fitNum = 0., double SMfactor=1, bool floatp2=true) {
  bool verbose = true;
  bool subtractSM =false;
  bool doPicture=true;
  double HTcut = 300.0;
  //  double SMfactor = 1.0; 

  double NregionD = 0;
  double NregionDafterSub = 0;

  double pi=4*atan(1.0)+.0001;
  double borderv2a=150.0;
  double borderv2b=1e10;
  double borderh1a=0.0;
  double borderh1b=0.3;
  double borderh2a=0.3;
  double borderh2b=pi;

  //special setting for getting the 100<MET<150 contribution
//   borderv2a=100;
//   borderv2b=150;
//   cout<<"WARNING -- I'm using region D defined by "<<borderv2a<<" < MET < "<<borderv2b<<endl;

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
  if(verbose) cout << endl;

  //jmt
  TH1D Hpass("Hpass","pass",fitNum*3,0,300); Hpass.Sumw2();
  TH1D Hfail("Hfail","fail",fitNum*3,0,300); Hfail.Sumw2();
  TH1D Hratio("Hratio","r",fitNum*3,0,300); Hratio.Sumw2();
  
  TH2D* histA = new TH2D("H_A", "Region A", fitNum, xBinsL, 1, yBinsL);
  TH2D* histAm = new TH2D("H_Am", "Region Am", fitNum, xBinsL, 1, yBinsT);
  TH2D* histB = new TH2D("H_B", "Region B", fitNum, xBinsL, 1, yBinsU);
  histA->Sumw2();
  histAm->Sumw2();
  histB->Sumw2();
 

  TChain* InputChain = FormChainJosh("pfpf");
  TChain* InputChainSM = 0;
  if(subtractSM) InputChainSM = FormChainJoshSM("pfpf");
  int numEntries = InputChain->GetEntries();
  int numEntriesSM = 0;
  if(subtractSM) numEntriesSM = InputChainSM->GetEntries(); 
  if(verbose)cout <<"numEntries: " << numEntries << endl;
  if(verbose)cout <<"numEntriesSM: " << numEntriesSM << endl;

  double x, y, weight, HT;
  double xSM, ySM, weightSM, HTSM;
  int nbtags;
  int nbtagsSM;
  InputChain->SetBranchAddress("MET",&x);
  InputChain->SetBranchAddress("minDeltaPhiMET",&y);
  InputChain->SetBranchAddress("weight",&weight);
  InputChain->SetBranchAddress("nbSSVM",&nbtags);
  InputChain->SetBranchAddress("HT",&HT);
  if(subtractSM){
    InputChainSM->SetBranchAddress("MET",&xSM);
    InputChainSM->SetBranchAddress("minDeltaPhiMET",&ySM);
    InputChainSM->SetBranchAddress("weight",&weightSM);
    InputChainSM->SetBranchAddress("nbSSVM",&nbtagsSM);
    InputChain->SetBranchAddress("HT",&HT);
  }

  for(int i = 0; i<numEntries; i++){
    InputChain->GetEvent(i);
    if(!passb(nbtags)) continue;
    if(!(HT>HTcut)) continue;

    histA->Fill(x,y, weight);
    histB->Fill(x,y, weight);
    histAm->Fill(x,y,x*weight);

    if (y>0.3) Hpass.Fill(x,weight); //jmt
    else Hfail.Fill(x,weight);
  }//end loop over InputChain
 
  if(subtractSM){
    for(int i = 0; i<numEntriesSM; i++){
      InputChainSM->GetEvent(i);
      if(!passb(nbtagsSM)) continue;
      if(!(HT>HTcut)) continue;

      histA->Fill(xSM,ySM, -weightSM*SMfactor);
      histB->Fill(xSM,ySM, -weightSM*SMfactor);
      histAm->Fill(xSM,ySM,-xSM*weightSM*SMfactor);

      //this code won't be run anyway for what i have in mind
      if (ySM>0.3) Hpass.Fill(xSM,-weightSM*SMfactor); //jmt
      else Hfail.Fill(xSM,-weightSM*SMfactor); //jmt
    }
  }

  ////////////////////
  //Graph to fit
  ///////////////////
  double gr1x[fitNum];
  double gr1y[fitNum];
  double gr1x_error[fitNum];
  double gr1y_error[fitNum];
  double smallNum = 1.e-5;
  for(int i =0; i<fitNum; i++){
    assert( histB->GetBinContent(i+1,1)>smallNum && histA->GetBinContent(i+1,1)>smallNum);
    
    gr1x[i] = (histAm->GetBinContent(i+1,1))/(histA->GetBinContent(i+1,1)+histB->GetBinContent(i+1,1)); 
    gr1x_error[i] = 0.;
    
    gr1y[i] = histB->GetBinContent(i+1,1)/histA->GetBinContent(i+1,1);
    gr1y_error[i] = gr1y[i]*sqrt((histB->GetBinError(i+1,1)/histB->GetBinContent(i+1,1))*(histB->GetBinError(i+1,1)/histB->GetBinContent(i+1,1))+
				 (histA->GetBinError(i+1,1)/histA->GetBinContent(i+1,1))*(histA->GetBinError(i+1,1)/histA->GetBinContent(i+1,1)));
    
    if(verbose)cout << "ratio: (" << gr1x[i] << " +- " << gr1x_error[i] <<", " << gr1y[i] << " +- " << gr1y_error[i] << ")" << endl;
  }//end fill arrays for graph to fit
  TGraphErrors * gr1 = new TGraphErrors(fitNum, gr1x, gr1y, gr1x_error, gr1y_error);  


  ///////////////////
  //Fit
  //////////////////
//     TString fitopt="R";
//     TCanvas* finalPicture=0;
//   if (subtractSM) {
//     finalPicture =new TCanvas("finalPicture","fit results",500,500);
//     fitopt+="0 E"; // tried " E" and failed
//   }
//   else {
//     fitopt+="0 E";
//   }

  TF1 *fexp2 = new TF1("fexp2", "[0]*exp([1]*x)+[2]", borderv1a, borderv1b);
  fexp2->SetParameters(10.0, -1.0/30.0, 0.001);  
  if (floatp2) fexp2->SetParLimits(2,-5,20);
  else         fexp2->FixParameter(2,0);
  assert(!( gr1->Fit("fexp2", "R 0E") ));
  cout<<"MINUIT info: Is at limit? "<< gMinuit->fLimset<<" ; STATUS="<<gMinuit->fCstatu<<endl;
  Double_t par_exp2[3];  
  fexp2->GetParameters(par_exp2);
  
  //=== kludge to save fit picture ===
  if (doPicture) {
//   if (subtractSM) {
//     TString outname="fitResults_"; 
//     outname+=int(borderv1a); outname+="_"; outname+=int(borderv1b); outname+="_"; outname+= fitNum;
//     outname+=".root";
//     TFile fout(outname); //does it work to open the file way down here?
//     gr1->SetMarkerColor(kBlue);    gr1->SetLineColor(kBlue); 
//     fexp2->SetLineColor(kBlue); fexp2->SetLineWidth(fexp2->GetLineWidth()*1.75);
//     gr1->GetHistogram()->SetXTitle("E_{T}^{miss} (GeV)");
//     gr1->GetHistogram()->SetYTitle("r");
//     gr1->Draw("AP"); 
//     fexp2->Draw("SAME");
//     TGraphErrors* unsubgraph = (TGraphErrors*) fout.Get("Graph");
//     unsubgraph->SetMarkerColor(1);
//     unsubgraph->Draw("same p");

//     finalPicture->cd()->SetRightMargin(0.04);
//     finalPicture->SetLogy();

//     TString epsname=outname;
//     epsname.ReplaceAll(".root",".eps");
//     finalPicture->SaveAs(epsname);
//     fout.Close();
//   }
//   else {
    Hratio.Divide(&Hpass,&Hfail);
    TString outname = subtractSM ? "fitResults_SubSM_" : "fitResults_noSubSM_";
    outname+=bcut; outname+="_";
    outname+=int(borderv1a); outname+="_"; outname+=int(borderv1b); outname+="_"; outname+= fitNum;
    outname+=".root";
    TFile fout(outname,"recreate"); //does it work to open the file way down here?
    fexp2->Write();
    gr1->Write();
    Hratio.Write();
    fout.Close();
    //  }
  }
  //==================================
    

  ///////////////////
  //Extrap
  ///////////////////
  double unbinnedEstimate2 = 0.;
  double ext_serror_unBinexp2 = 0.;
  double ext_serror_exp2_w = 0.;
  double ext_serror_exp2_p0 = 0.;
  double ext_serror_exp2_p1 = 0.;
  double ext_serror_exp2_p2 = 0.;

  for(int i = 0; i<numEntries; i++){
    InputChain->GetEvent(i);
    if(!passb(nbtags)) continue;

    //if in Region D
    if( (x>=borderv2a) && (x<borderv2b) && (y>=borderh1a) && (y<borderh1b)){
      double xvalue = x;
      double xError=0.;
      double eventError = weight;

      NregionDafterSub += weight; //jmt -- adding this to get the number in regionD
      
      unbinnedEstimate2+=weight*(par_exp2[0]*exp(par_exp2[1]*xvalue)+par_exp2[2]);

      //terms for error prop
      ext_serror_exp2_p0 += exp(par_exp2[1]*xvalue)*(weight)*(fexp2->GetParError(0));
      ext_serror_exp2_p1 += par_exp2[0]*xvalue*exp(par_exp2[1]*xvalue)*(weight)*(fexp2->GetParError(1));
      ext_serror_exp2_p2 += weight*(fexp2->GetParError(2));
      ext_serror_exp2_w  += (par_exp2[0]*exp(par_exp2[1]*xvalue)+par_exp2[2])*(eventError) *  (par_exp2[0]*exp(par_exp2[1]*xvalue)+par_exp2[2])*(eventError);
      
    }//end if in Region D
    assert(x<borderv2b); //jmt: need to comment this out in case we're doing a medium MET estimate
  }//end loop over InputChain
  
  NregionD = NregionDafterSub; //jmt

  for(int i = 0; i<numEntriesSM; i++){
    InputChainSM->GetEvent(i);
    if(!passb(nbtagsSM)) continue;
   
    //if in Region D 
    if( (xSM>=borderv2a) && (xSM<borderv2b) && (ySM>=borderh1a) && (ySM<borderh1b)){
      double xvalue = xSM;
      double xError=0.;
      double eventError = weightSM*SMfactor;

      NregionDafterSub -= weightSM*SMfactor;

      unbinnedEstimate2+=-weightSM*SMfactor*(par_exp2[0]*exp(par_exp2[1]*xvalue)+par_exp2[2]);

      //terms for error prop 
      ext_serror_exp2_p0 += exp(par_exp2[1]*xvalue)*(-weightSM*SMfactor)*(fexp2->GetParError(0));
      ext_serror_exp2_p1 += par_exp2[0]*xvalue*exp(par_exp2[1]*xvalue)*(-weightSM*SMfactor)*(fexp2->GetParError(1));
      ext_serror_exp2_p2 += -weightSM*SMfactor*(fexp2->GetParError(2));
      ext_serror_exp2_w  += (par_exp2[0]*exp(par_exp2[1]*xvalue)+par_exp2[2])*(eventError) *  (par_exp2[0]*exp(par_exp2[1]*xvalue)+par_exp2[2])*(eventError);

    }//end if in Region D 
    assert(x<borderv2b);
  }//end loop over InputChain      
  


  double ext_error_unBinexp2;
  ext_error_unBinexp2 = sqrt(ext_serror_exp2_p0*ext_serror_exp2_p0 + ext_serror_exp2_p1*ext_serror_exp2_p1 + ext_serror_exp2_p2*ext_serror_exp2_p2 + ext_serror_exp2_w);  
  
  cout << "Unbinned exponential+constant extended estimate for nC: " << unbinnedEstimate2 << " +- " << ext_error_unBinexp2 << endl;
  
  histA->Clear();
  histAm->Clear();
  histB->Clear();

  /* jmt -- let's pass more stuff back:
0 -- estimate
1 -- error on estimate
2 -- p0
3 -- p0 error
4 -- p1
5 -- p1 error
6 -- p2
7 -- p2 error
8 -- fit is at limit or not
9 -- minuit STATUS flag
10 -- chi^2
11 -- ndof

12 -- n region D
13 -- n region D after subtraction
  */

  double *result = new double[14];
  result[0] = unbinnedEstimate2;
  result[1] = ext_error_unBinexp2;
  result[2] = fexp2->GetParameter(0);
  result[3] = fexp2->GetParError(0);
  result[4] = fexp2->GetParameter(1);
  result[5] = fexp2->GetParError(1);
  result[6] = fexp2->GetParameter(2);
  result[7] = fexp2->GetParError(2);
  result[8] = gMinuit->fLimset;
  result[9] = TString(gMinuit->fCstatu).Contains("SUCCESS") ? 1 : 0;
  result[10] = fexp2->GetChisquare();
  result[11] = fexp2->GetNDF();
  result[12] = NregionD;
  result[13] = NregionDafterSub;
  return result; //this was missing but somehow root handled it anyway!
}//end doBasicABCD


void printResult(double* result) {
  cout << result[0] << " +/- " << result[1] 
       <<" ; p0 = "<< result[2] << " +/- " << result[3] 
       <<" ; p1 = "<< result[4] << " +/- " << result[5] 
       <<" ; p2 = "<< result[6] << " +/- " << result[7] 
       <<" ; at limit="<<result[8]
       <<" ; STATUS=SUCCESSFUL? "<<result[9]
       <<" ; chi2/ndof = "<<result[10]<<" / "<<result[11]
       <<" ; ND_raw = "<<result[12]
       <<" , ND_sub = "<<result[13]
       <<endl;
}

void basicABCDj(){
  gROOT->SetStyle("CMS");
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);
  cout << "begin basicABCD" << endl;

  double nominal=0;   double nominal_err=0;
  double high=-1e9,low=1e9;

  unsigned int p2neg=0;

  for (double SMfactor = 0.7; SMfactor <=1.3001 ; SMfactor +=0.3) {
    for (int lowborder=0; lowborder<=20; lowborder+=10) {
      for (int highborder=70; highborder<=90; highborder+=10) {
	double *a0 = doBasicABCD(lowborder,highborder,10,SMfactor,true);
	printResult(a0);
	if (a0[6]<=0) {
	  p2neg++;
	  cout<<"Redoing fit with p2 fixed!"<<endl;
	  a0 = doBasicABCD(lowborder,highborder,10,SMfactor,false);
	}
	if (lowborder==0 && highborder==80 && SMfactor==1) {
	  nominal = a0[0];
	  nominal_err = a0[1];
	}
	if (a0[0] > high) high = a0[0];
	if (a0[0] < low)  low = a0[0];
      }
    }
  }

  cout<<"Final: "<<nominal<<" +/-  "<<nominal_err<<" + "<<high-nominal <<" - "<<nominal-low<<endl;
  cout<<p2neg<<endl;
  //double *a0 = doBasicABCD(0.,70.,10,SMfactor);
//   double *a1 = doBasicABCD(0.,80.,10,SMfactor);
//   double *a2 = doBasicABCD(0.,90.,10,SMfactor);
  
//   double *a3 = doBasicABCD(10.,70.,10,SMfactor);
//   double *a4 = doBasicABCD(10.,80.,10,SMfactor);
//   double *a5 = doBasicABCD(10.,90.,10,SMfactor);
  
//   double *a6 = doBasicABCD(20.,70.,10,SMfactor);
//   double *a7 = doBasicABCD(20.,80.,10,SMfactor);
//   double *a8 = doBasicABCD(20.,90.,10,SMfactor);

//    cout << endl;
//    cout << endl;
//    printResult(a0);
//    printResult(a1);
//    printResult(a2);
//    printResult(a3);
//    printResult(a4);
//    printResult(a5);
//    printResult(a6);
//    printResult(a7);
//    printResult(a8);

  

}
