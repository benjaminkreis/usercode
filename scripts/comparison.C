{
gROOT->SetStyle("Plain");

TFile* f1=TFile::Open("DonResults/patHistos_zjets.root");
TFile* f2=TFile::Open("ben_combine.root");

TString name1 = "analyzeBasicPat/met_uncorrectedpt";
TString name2 = "analyzeBasicPat/uc_met";

TH1F * h1 = (TH1F*)f1.Get(name1);
TH1F * h2 = (TH1F*)f2.Get(name2);

h1->SetLineWidth(2);
h2->SetLineWidth(2);
h1->SetLineColor(3);
h2->SetLineColor(4);

h1->Draw();
h2->Draw("same");

}
