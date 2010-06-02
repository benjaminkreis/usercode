TChain* FormChain(){

  const TString treedirectory = "T_JetTag";
  TChain* InputChain = new TChain(treedirectory);

  InputChain->Add("/cu1/kreis/QCD/T_TagJet_QCD_Pt170.root");
  InputChain->Add("/cu1/kreis/QCD/T_TagJet_QCD_Pt300.root");
  InputChain->Add("/cu1/kreis/QCD/T_TagJet_QCD_Pt470.root");
  InputChain->Add("/cu1/kreis/QCD/T_TagJet_QCD_Pt800.root");
  InputChain->Add("/cu1/kreis/QCD/T_TagJet_QCD_Pt1400.root");

  cout << "Loaded " << InputChain->GetListOfFiles()->GetEntries() << " files" << endl;

  return InputChain;

}
