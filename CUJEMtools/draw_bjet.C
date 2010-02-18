{//0

//TString histname = "H_DeltaPhi1"; //rebin 10
//TString histname = "H_DeltaPhi2"; //rebin 10
//TString histname = "H_DeltaPhi3"; //rebin 10
//TString histname = "H_DeltaPhi4"; //rebin 10
//TString histname = "H_HT"; //not yet
//TString histname = "H_MET"; //xmax 110 rebin 0, log has problems...
//TString histname = "H_MET_SumET"; //rebin 5
//TString histname = "H_MET_UncorrPT"; //rebin 0, xmax 85
//TString histname = "H_Meff"; //not yet
//TString histname = "H_NJets";
//TString histname = "H_Nbtags";
//TString histname = "H_NbjetsMC";
//TString histname = "H_bjetPT1";
//TString histname = "H_bjetPT2"; //not yet
//TString histname = "H_jetPT1";
//TString histname = "H_jetPT2"; //xbins 150
//TString histname = "H_jetPT3"; //xbins 100
//TString histname = "H_jetPT4";
//TString histname = "H_jetPT"; //rebin 5, xbins 70
TString histname = "H_jeteta1";//rebin 5

int xmax = 0;
int rebinNum = 5;

TString logOption = ""; //"" for linear
//TString logOption = "_log"; //"_log" for log

int logInt = 0;
if(logOption == "_log") logInt=1;

TString directory = "20100217_jets/";

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

gROOT->SetStyle("Plain");
gStyle->SetOptStat("neou");
//gStyle->SetOptStat(0);


int numSamples = 15;

TString samples[numSamples]={"LM0", "LM1","LM2","LM3","LM4","LM5","LM6","LM7","LM8","LM9","LM10","LM11","LM12","LM13", "TTBar-PYTHIA"};
//TString samples[numSamples]={"LM13", "TTBar-PYTHIA"}
//TString samples[numSamples]={"LM1"}

int numBjetCat = 3;
//TString bjetCats[numBjetCat] = {"nBTagsLT2","nBTagsEQ2","nBTagsGT2"};
TString bjetCats[numBjetCat] = {"nBJetsLT2","nBJetsEQ2","nBJetsGT2"};

TString filename[numBjetCat] = {"","",""};
TFile* file1;
TFile* file2;
TFile* file3;
TH1F* hist1;
TH1F* hist2;
TH1F* hist3;
TLegend* leg;

for(int i = 0; i<numSamples; i++){
  
  for(int j = 0; j<numBjetCat; j++){
    
    filename[j] = "plots_";
    filename[j]+=samples[i];
    filename[j]+="_";
    filename[j]+=bjetCats[j];
    filename[j]+=".root";
    cout << filename[j] << endl;
    
  }//end loop over b jet categories
  
  file1 = TFile::Open(directory+filename[0],"READ");
  hist1 = (TH1F*) gDirectory->Get(histname);
  file2 = TFile::Open(directory+filename[1],"READ");
  hist2 = (TH1F*) gDirectory->Get(histname);
  file3 = TFile::Open(directory+filename[2],"READ");
  hist3 = (TH1F*) gDirectory->Get(histname);
  
  if(rebinNum){
    hist1->Rebin(rebinNum);
    hist2->Rebin(rebinNum);
    hist3->Rebin(rebinNum);
  }
  

  hist1->Sumw2();
  hist2->Sumw2();
  hist3->Sumw2();

  float scale1 = 1.0/(hist1->Integral());
  float scale2 = 1.0/(hist2->Integral());
  float scale3 = 1.0/(hist3->Integral());
  hist1->Scale(scale1);
  hist2->Scale(scale2);
  hist3->Scale(scale3);

  float hmax;
  float max1 = hist1->GetMaximum();
  float max2 = hist2->GetMaximum();
  float max3 = hist3->GetMaximum();
  if(max1>max2 && max1>max3)hmax=max1;
  else if(max2>max1 && max2>max3)hmax=max2;
  else {hmax = max3;}
  hmax = hmax*1.05;
  hist1->SetMaximum(hmax);

  float hmin;
  float min1 = hist1->GetMinimum();
  float min2 = hist2->GetMinimum();
  float min3 = hist3->GetMinimum();
  if(min1<min2 && min1<min3)hmin=min1;
  else if(min2<min1 && min2<min3)hmin=min2;
  else {hmin = min3;}
  hmin = hmin*.95;
  hist1->SetMinimum(hmin);

  leg=new TLegend(.8,.85,.99,.99);
  leg->AddEntry(hist1, samples[i]+" "+bjetCats[0]);
  leg->AddEntry(hist2, samples[i]+" "+bjetCats[1]);
  leg->AddEntry(hist3, samples[i]+" "+bjetCats[2]);

  hist1->SetLineColor(2);
  hist2->SetLineColor(3);
  hist3->SetLineColor(4);

  hist1->SetLineWidth(2);
  hist2->SetLineWidth(2);
  hist3->SetLineWidth(2);
  leg->SetFillColor(0);

  if(xmax) hist1->GetXaxis()->SetRange(0,xmax);
  if(logInt) hist1->GetYaxis()->SetRangeUser(0.00001,hmax);
  
  hist1->Draw();
  hist2->Draw("SAMES");
  hist3->Draw("SAMES");
  leg->Draw();
 
  gPad->Update();
  TPaveStats *st1 = (TPaveStats*)hist1->FindObject("stats");
  if(st1){
    st1->SetX1NDC(.35);
    st1->SetX2NDC(.5);
  }
  TPaveStats *st2 = (TPaveStats*)hist2->FindObject("stats");
  if(st2){
    st2->SetX1NDC(.5);
    st2->SetX2NDC(.65);
  }
  TPaveStats *st3 = (TPaveStats*)hist3->FindObject("stats");
  if(st3){
    st3->SetX1NDC(.65);
    st3->SetX2NDC(.8);
  }
    
  
  gPad->SetLogy(logInt);
  gPad->Modified();
  
  c1->Print(directory+histname+"_"+samples[i]+logOption+".png"); 
   
  file1.Close();
  file2.Close();
  file3.Close();

}//end loop over samples



}//0

