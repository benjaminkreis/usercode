TChain* FormChain(){

  const TString treedirectory = "T_JetTag";
  TChain* InputChain = new TChain(treedirectory);

  InputChain->Add("/cu1/kreis/QCD/Spring10/trees/T_TagJet_QCD_MG1.root");
  InputChain->Add("/cu1/kreis/QCD/Spring10/trees/T_TagJet_QCD_MG1_1.root");
  InputChain->Add("/cu1/kreis/QCD/Spring10/trees/T_TagJet_QCD_MG1_2.root");
  InputChain->Add("/cu1/kreis/QCD/Spring10/trees/T_TagJet_QCD_MG2.root");
  InputChain->Add("/cu1/kreis/QCD/Spring10/trees/T_TagJet_QCD_MG2_1.root");
  InputChain->Add("/cu1/kreis/QCD/Spring10/trees/T_TagJet_QCD_MG3.root");
  InputChain->Add("/cu1/kreis/QCD/Spring10/trees/T_TagJet_QCD_MG3_1.root");
  InputChain->Add("/cu1/kreis/QCD/Spring10/trees/T_TagJet_QCD_MG4.root");

  cout << "Loaded " << InputChain->GetListOfFiles()->GetEntries() << " files" << endl;

  return InputChain;

}
