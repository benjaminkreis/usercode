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
    
    //InputChain->Add("/cu1/kreis/QCD/Spring10/Nov23/trees/T_ABCD_LM0.root");
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

  bool data =false;
  bool MC =true;
  bool contamination =true;
  bool signalC =true;

  cout << type << endl;

  
  if(type=="pfpf" && MC) {
    //InputChain->Add("/cu1/kreis/ABCDtrees/ABCDtree.RA2_PF_pfMEThigh_minDP_NoInclusive_NoTrigger.ge0b.QCD.root");
    //InputChain->Add("/cu1/kreis/ABCDtrees/22/ABCDtree.RA2_PF_pfMEThigh_minDP_NoMET_NoDeltaPhi.ge0b.QCD.root");
    InputChain->Add("/cu1/kreis/ABCDtrees/36/ABCDtree.RA2_PF_pfMEThigh_minDP_NoMET_NoDeltaPhi.ge0b.QCD.root");
  }
  else if(type=="pf" && MC) {
    InputChain->Add("/cu1/kreis/ABCDtrees/ABCDtree.RA2_calo_pfMEThigh_minDP_NoInclusive_NoTrigger.ge0b.QCD.root");
  }
  else if(type=="tc" && MC){
    InputChain->Add("/cu1/kreis/ABCDtrees/ABCDtree.RA2_calo_tcMEThigh_minDP_NoInclusive_NoTrigger.ge0b.QCD.root");
  }
  else if(type=="calo" && MC){
    //InputChain->Add("/cu1/kreis/ABCDtrees/old/ABCDtree.RA2_calo_METhigh_minDP_NoInclusive_NoTrigger.ge0b.QCD.root");
    InputChain->Add("/cu1/kreis/ABCDtrees/ABCDtree.RA2_calo_METhigh_minDP_NoMET_NoDeltaPhi.ge0b.QCD.root"); //QCDb for jetID event-cut
  }
  if(MC && contamination && type=="pfpf"){
    /* InputChain->Add("/cu1/kreis/ABCDtrees/ABCDtree.RA2_PF_pfMEThigh_minDP_NoInclusive_NoTrigger.ge0b.Zinvisible.root");
    InputChain->Add("/cu1/kreis/ABCDtrees/ABCDtree.RA2_PF_pfMEThigh_minDP_NoInclusive_NoTrigger.ge0b.WJets.root");
    InputChain->Add("/cu1/kreis/ABCDtrees/ABCDtree.RA2_PF_pfMEThigh_minDP_NoInclusive_NoTrigger.ge0b.TTbarJets.root");
    if(signalC) InputChain->Add("/cu1/kreis/ABCDtrees/ABCDtree.RA2_PF_pfMEThigh_minDP_NoInclusive_NoTrigger.ge0b.LM13.root");*/
    /*if(signalC) InputChain->Add("/cu1/kreis/ABCDtrees/22/ABCDtree.RA2_PF_pfMEThigh_minDP_NoMET_NoDeltaPhi.ge0b.LM13.root");
    InputChain->Add("/cu1/kreis/ABCDtrees/22/ABCDtree.RA2_PF_pfMEThigh_minDP_NoMET_NoDeltaPhi.ge0b.TTbarJets.root");
    InputChain->Add("/cu1/kreis/ABCDtrees/22/ABCDtree.RA2_PF_pfMEThigh_minDP_NoMET_NoDeltaPhi.ge0b.WJets.root");
    InputChain->Add("/cu1/kreis/ABCDtrees/22/ABCDtree.RA2_PF_pfMEThigh_minDP_NoMET_NoDeltaPhi.ge0b.Zinvisible.root");*/
    if(signalC) InputChain->Add("/cu1/kreis/ABCDtrees/36/ABCDtree.RA2_PF_pfMEThigh_minDP_NoMET_NoDeltaPhi.ge0b.LM13.root");
    InputChain->Add("/cu1/kreis/ABCDtrees/36/ABCDtree.RA2_PF_pfMEThigh_minDP_NoMET_NoDeltaPhi.ge0b.TTbarJets.root");
    InputChain->Add("/cu1/kreis/ABCDtrees/36/ABCDtree.RA2_PF_pfMEThigh_minDP_NoMET_NoDeltaPhi.ge0b.WJets.root");
    InputChain->Add("/cu1/kreis/ABCDtrees/36/ABCDtree.RA2_PF_pfMEThigh_minDP_NoMET_NoDeltaPhi.ge0b.Zinvisible.root");
    
  }
  if(MC && contamination && type=="calo"){
    InputChain->Add("/cu1/kreis/ABCDtrees/ABCDtree.RA2_calo_METhigh_minDP_NoInclusive_NoTrigger.ge0b.Zinvisible.root");
    InputChain->Add("/cu1/kreis/ABCDtrees/ABCDtree.RA2_calo_METhigh_minDP_NoInclusive_NoTrigger.ge0b.WJets.root");
    InputChain->Add("/cu1/kreis/ABCDtrees/ABCDtree.RA2_calo_METhigh_minDP_NoInclusive_NoTrigger.ge0b.TTbarJets.root");
    if(signalC) InputChain->Add("/cu1/kreis/ABCDtrees/ABCDtree.RA2_calo_METhigh_minDP_NoInclusive_NoTrigger.ge0b.LM13.root");
  }
  

  if(type=="pfpf" && data && !MC){
    InputChain->Add("/cu1/kreis/ABCDtrees/ABCDtree.RA2_PF_pfMEThigh_minDP_NoMET_NoDeltaPhi.ge0b.data-0.root");
  }

  return InputChain;
}
