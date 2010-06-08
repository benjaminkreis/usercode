TChain* FormChain(){

  const TString treedirectory = "demo/T_ABCD";
  TChain* InputChain = new TChain(treedirectory);
  
  InputChain->Add("/cu1/kreis/QCD/T_ABCD_QCD_Pt15.root");
  InputChain->Add("/cu1/kreis/QCD/T_ABCD_QCD_Pt30.root");
  InputChain->Add("/cu1/kreis/QCD/T_ABCD_QCD_Pt80.root");
  InputChain->Add("/cu1/kreis/QCD/T_ABCD_QCD_Pt170.root");
  InputChain->Add("/cu1/kreis/QCD/T_ABCD_QCD_Pt170_2.root");
  InputChain->Add("/cu1/kreis/QCD/T_ABCD_QCD_Pt300.root");
  InputChain->Add("/cu1/kreis/QCD/T_ABCD_QCD_Pt300_2.root");
  InputChain->Add("/cu1/kreis/QCD/T_ABCD_QCD_Pt470.root");
  InputChain->Add("/cu1/kreis/QCD/T_ABCD_QCD_Pt470_2.root");
  InputChain->Add("/cu1/kreis/QCD/T_ABCD_QCD_Pt800.root");
  InputChain->Add("/cu1/kreis/QCD/T_ABCD_QCD_Pt1400.root");

  cout << "Loaded " << InputChain->GetListOfFiles()->GetEntries() << " files" << endl;

  return InputChain;

}
