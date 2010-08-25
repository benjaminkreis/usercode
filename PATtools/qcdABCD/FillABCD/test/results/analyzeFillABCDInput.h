TChain* FormChain(){

  const TString treedirectory = "demo/T_ABCD";
  TChain* InputChain = new TChain(treedirectory);
  
  InputChain->Add("/cu1/kreis/QCD/Spring10/trees/T_ABCD_MG1.root");
  InputChain->Add("/cu1/kreis/QCD/Spring10/trees/T_ABCD_MG2.root");
  InputChain->Add("/cu1/kreis/QCD/Spring10/trees/T_ABCD_MG3.root");
  InputChain->Add("/cu1/kreis/QCD/Spring10/trees/T_ABCD_MG4.root");

  cout << "Loaded " << InputChain->GetListOfFiles()->GetEntries() << " files" << endl;

  return InputChain;

}
