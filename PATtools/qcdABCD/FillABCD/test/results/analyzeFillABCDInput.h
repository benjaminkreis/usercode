TChain* FormChainJosh(TString type){
  const TString treedirectory = "ABCDtree";
  TChain* InputChain = new TChain(treedirectory);

  bool data =false;
  bool MC =true;
  bool contamination=false;
  bool SM=true;
  bool signalC =false;
  
  cout << type << endl;
  
  if(type=="pfpf" && MC && !data) {
    InputChain->Add("/afs/cern.ch/user/k/kreis/scratch0/ABCDtrees/ABCDtree.Baseline0_PF_pfMEThigh_PFLep0e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.QCD.root");
    if(contamination){
      if(signalC)InputChain->Add("/afs/cern.ch/user/k/kreis/scratch0/ABCDtrees/ABCDtree.Baseline0_PF_pfMEThigh_PFLep0e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.LM13.root");
      if(SM){
	InputChain->Add("/afs/cern.ch/user/k/kreis/scratch0/ABCDtrees/ABCDtree.Baseline0_PF_pfMEThigh_PFLep0e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.SingleTop-tWChannel.root");
	InputChain->Add("/afs/cern.ch/user/k/kreis/scratch0/ABCDtrees/ABCDtree.Baseline0_PF_pfMEThigh_PFLep0e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.SingleTop-tChannel.root");
	InputChain->Add("/afs/cern.ch/user/k/kreis/scratch0/ABCDtrees/ABCDtree.Baseline0_PF_pfMEThigh_PFLep0e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.SingleTop-sChannel.root");
	InputChain->Add("/afs/cern.ch/user/k/kreis/scratch0/ABCDtrees/ABCDtree.Baseline0_PF_pfMEThigh_PFLep0e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.Zinvisible.root");
	InputChain->Add("/afs/cern.ch/user/k/kreis/scratch0/ABCDtrees/ABCDtree.Baseline0_PF_pfMEThigh_PFLep0e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.ZJets.root");
	InputChain->Add("/afs/cern.ch/user/k/kreis/scratch0/ABCDtrees/ABCDtree.Baseline0_PF_pfMEThigh_PFLep0e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.WJets.root");
	InputChain->Add("/afs/cern.ch/user/k/kreis/scratch0/ABCDtrees/ABCDtree.Baseline0_PF_pfMEThigh_PFLep0e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.TTbarJets.root");
      }
    }
  }//end MC
  if(type=="pfpf" && data && !MC){
    InputChain->Add("/afs/cern.ch/user/k/kreis/scratch0/ABCDtrees/ABCDtree.Baseline0_PF_pfMEThigh_PFLep0e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.data-0.root");
  }//end data 
  
  return InputChain;
}


TChain* FormChainJoshSM(TString type){
  const TString treedirectory = "ABCDtree";
  TChain* InputChain = new TChain(treedirectory);
  
  InputChain->Add("/afs/cern.ch/user/k/kreis/scratch0/ABCDtrees/ABCDtree.Baseline0_PF_pfMEThigh_PFLep0e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.SingleTop-tWChannel.root");
  InputChain->Add("/afs/cern.ch/user/k/kreis/scratch0/ABCDtrees/ABCDtree.Baseline0_PF_pfMEThigh_PFLep0e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.SingleTop-tChannel.root");
  InputChain->Add("/afs/cern.ch/user/k/kreis/scratch0/ABCDtrees/ABCDtree.Baseline0_PF_pfMEThigh_PFLep0e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.SingleTop-sChannel.root");
  InputChain->Add("/afs/cern.ch/user/k/kreis/scratch0/ABCDtrees/ABCDtree.Baseline0_PF_pfMEThigh_PFLep0e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.Zinvisible.root");
  InputChain->Add("/afs/cern.ch/user/k/kreis/scratch0/ABCDtrees/ABCDtree.Baseline0_PF_pfMEThigh_PFLep0e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.ZJets.root");
  InputChain->Add("/afs/cern.ch/user/k/kreis/scratch0/ABCDtrees/ABCDtree.Baseline0_PF_pfMEThigh_PFLep0e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.WJets.root");
  InputChain->Add("/afs/cern.ch/user/k/kreis/scratch0/ABCDtrees/ABCDtree.Baseline0_PF_pfMEThigh_PFLep0e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.TTbarJets.root");
  return InputChain;
  
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
