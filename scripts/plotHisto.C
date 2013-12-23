{
gROOT->SetStyle("Plain");
gStyle->SetOptStat(0);

/*
jets30: number of jets with pt>30GeV
bjets30: number of bjets with pt>30GeV
photons: photons->size()
elecs: electrons->size()
muons: muons->size()
taus: taus->size()
met: *mets.et()
uc_met: *mets.uncorrectedPt()
c_et: *mets.corSumEt()
uc_et: *mets.sumEt() 
*/


/*
TString hname = "jets30";
TString title = "Unit normalized multiplicity of jets with p_{T}<30 GeV";
TString printName = "jets30";
bool xRange = 0;
int minXBin = 0;
int maxXBin = 30;
*/

/*
TString hname = "bjets30";
TString title = "Unit normalized multiplicity of b-jets with p_{T}<30 GeV";
TString printName = "bjets30";
bool xRange = 0;
int minXBin = 0;
int maxXBin = 30;
*/

/*
TString hname = "met";
TString title = "Unit normalized met.pt()";
TString printName = "met_pt";
bool xRange = 1;
int minXBin = 0;
int maxXBin = 67;
*/

/*
TString hname = "uc_met";
TString title = "Unit normalized met.uncorrectedPt()";
TString printName = "met_uncorrectedPt";
bool xRange = 1;
int minXBin = 0;
int maxXBin = 67;
*/


TString hname = "c_et";
TString title = "Unit Normalized met.corSumEt()";
TString printName = "met_corSumEt";
bool xRange = 1;
int minXBin = 0;
int maxXBin = 30;


/*
TString hname = "uc_et";
TString title = "Unit Normalized met.sumEt()";
TString printName = "met_sumEt";
bool xRange = 1;
int minXBin = 0;
int maxXBin = 70;
*/

///////////////////////////////
//***************************//
///////////////////////////////

TString name = "analyzeBasicPat/"+hname;
TFile* fFile1=TFile::Open("/afs/cern.ch/user/k/kreis/scratch0/20091208_diboson/ZW/ZWdata/plotsOfPAT_ZW.root","READ");
TFile* fFile2=TFile::Open("/afs/cern.ch/user/k/kreis/scratch0/20091208_diboson/ZZ/ZZdata/plotsOfPAT_ZZ.root","READ");
TFile* fFile3=TFile::Open("/afs/cern.ch/user/k/kreis/scratch0/20091208_diboson/WW/WWdata/plotsOfPAT_WW.root","READ");

TH1F *hist1 = fFile1->Get(name);
TH1F *hist2 = fFile2->Get(name);
TH1F *hist3 = fFile3->Get(name);

hist2->SetTitle(title);

//combine results
TH1F *hist4=(TH1F*)hist1->Clone();
hist4->SetName("Diboson");
hist4->Add(hist2);
hist4->Add(hist3);

TAxis* xax = hist2->GetXaxis();
TAxis* yax = hist1->GetYaxis();
xax->SetTitle("GeV");
if(xRange)xax->SetRange(minXBin,maxXBin);
//yax->SetTitle("")


hist1->SetLineWidth(2);
hist2->SetLineWidth(2);
hist3->SetLineWidth(2);
hist4->SetLineWidth(2);
hist1->SetLineColor(2);
hist2->SetLineColor(3);
hist3->SetLineColor(4);
hist4->SetLineColor(6);

/*
hist4->Draw();
hist1->Draw("same");
hist2->Draw("same");
hist3->Draw("same");
*/

hist2->DrawNormalized();
hist3->DrawNormalized("same");
hist4->DrawNormalized("same");  
hist1->DrawNormalized("same");  

leg = new TLegend(0.75,0.70,0.89,0.89); 
leg->AddEntry(hist1,"ZW", "l"); 
leg->AddEntry(hist2,"ZZ", "l"); 
leg->AddEntry(hist3,"WW", "l"); 
leg->AddEntry(hist4,"Diboson", "l"); 
leg->SetFillColor(0);
leg->SetLineColor(0);
leg->Draw();

c1->Print("diboson/"+printName+".pdf");

gPad->SetLogy(1);
gPad->Modified();

c1->Print("diboson/"+printName+"_log.pdf");
}
