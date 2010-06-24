TChain* FormChain(){

  const TString treedirectory = "demo/T_TagFraction";
  TChain* InputChain = new TChain(treedirectory);

  /*
  InputChain->Add("/cu1/kreis/QCD/T_TagFraction_QCD_Pt15.root");
  InputChain->Add("/cu1/kreis/QCD/T_TagFraction_QCD_Pt30.root");
  InputChain->Add("/cu1/kreis/QCD/T_TagFraction_QCD_Pt80.root");
  InputChain->Add("/cu1/kreis/QCD/T_TagFraction_QCD_Pt170.root");
  InputChain->Add("/cu1/kreis/QCD/T_TagFraction_QCD_Pt170_2.root");
  InputChain->Add("/cu1/kreis/QCD/T_TagFraction_QCD_Pt300.root");
  InputChain->Add("/cu1/kreis/QCD/T_TagFraction_QCD_Pt300_2.root");
  InputChain->Add("/cu1/kreis/QCD/T_TagFraction_QCD_Pt470.root");
  InputChain->Add("/cu1/kreis/QCD/T_TagFraction_QCD_Pt470_2.root");*/
  // InputChain->Add("/cu1/kreis/QCD/T_TagFraction_QCD_Pt800.root"); 
  // InputChain->Add("/cu1/kreis/QCD/T_TagFraction_QCD_Pt1400.root");
 

 
  InputChain->Add("/cu1/kreis/QCD/T_TagFraction_QCD_Pt15_double.root");  
  InputChain->Add("/cu1/kreis/QCD/T_TagFraction_QCD_Pt30_double.root");
  InputChain->Add("/cu1/kreis/QCD/T_TagFraction_QCD_Pt80_double.root");
  InputChain->Add("/cu1/kreis/QCD/T_TagFraction_QCD_Pt170_double.root");
  InputChain->Add("/cu1/kreis/QCD/T_TagFraction_QCD_Pt170_2_double.root");
  InputChain->Add("/cu1/kreis/QCD/T_TagFraction_QCD_Pt300_double.root");
  InputChain->Add("/cu1/kreis/QCD/T_TagFraction_QCD_Pt300_2_double.root");
  InputChain->Add("/cu1/kreis/QCD/T_TagFraction_QCD_Pt470_double.root");
  InputChain->Add("/cu1/kreis/QCD/T_TagFraction_QCD_Pt470_2_double.root");
  InputChain->Add("/cu1/kreis/QCD/T_TagFraction_QCD_Pt800_double.root");
  InputChain->Add("/cu1/kreis/QCD/T_TagFraction_QCD_Pt1400_double.root");
  

  cout << "Loaded " << InputChain->GetListOfFiles()->GetEntries() << " files" << endl;

  return InputChain;

}

