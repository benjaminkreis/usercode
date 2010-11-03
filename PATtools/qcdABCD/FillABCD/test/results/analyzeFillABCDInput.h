TChain* FormChain(){

  bool contamination = false;
  bool RA2qcd = false;
  bool UHHqcd = true;

  const TString treedirectory = "demo/T_ABCD";
  TChain* InputChain = new TChain(treedirectory);
  
  if(RA2qcd){
    InputChain->Add("/cu1/kreis/QCD/Spring10/trees/T_ABCD_MG1.root");
    InputChain->Add("/cu1/kreis/QCD/Spring10/trees/T_ABCD_MG2.root");
    InputChain->Add("/cu1/kreis/QCD/Spring10/trees/T_ABCD_MG3.root");
    InputChain->Add("/cu1/kreis/QCD/Spring10/trees/T_ABCD_MG4.root");
  }
  if(UHHqcd){
    InputChain->Add("/cu1/kreis/QCD/Spring10/trees/T_ABCD_UHH_MG1.root");
    InputChain->Add("/cu1/kreis/QCD/Spring10/trees/T_ABCD_UHH_MG2.root");
    InputChain->Add("/cu1/kreis/QCD/Spring10/trees/T_ABCD_UHH_MG3.root");
    InputChain->Add("/cu1/kreis/QCD/Spring10/trees/T_ABCD_UHH_MG4.root");
  }
  if(contamination){
    InputChain->Add("/cu1/kreis/ABCD/Spring10/trees/T_ABCD_ttbar.root");
    InputChain->Add("/cu1/kreis/ABCD/Spring10/trees/T_ABCD_wjets.root");
    InputChain->Add("/cu1/kreis/ABCD/Spring10/trees/T_ABCD_zinv.root");
    InputChain->Add("/cu1/kreis/ABCD/Spring10/trees/T_ABCD_lm9.root");
  }
  cout << "Loaded " << InputChain->GetListOfFiles()->GetEntries() << " files" << endl;

  return InputChain;

}

TChain* FormChainJosh(TString type){
  const TString treedirectory = "ABCDtree";
  TChain* InputChain = new TChain(treedirectory);

  if(type=="pf") {
    InputChain->Add("/cu1/kreis/ABCDtrees/ABCDtree.RA2_PF_pfMEThigh_minDP_NoInclusive_NoTrigger.ge0b.QCD.root");
  }
  else if(type=="tc"){
    InputChain->Add("/cu1/kreis/ABCDtrees/ABCDtree.RA2_calo_tcMEThigh_minDP_NoInclusive_NoTrigger.ge0b.QCD.root");
  }
  else if(type=="calo"){
    InputChain->Add("/cu1/kreis/ABCDtrees/ABCDtree.RA2_calo_METhigh_minDP_NoInclusive_NoTrigger.ge0b.QCD.root");
  }
  else{ assert(0); }

  return InputChain;
}
