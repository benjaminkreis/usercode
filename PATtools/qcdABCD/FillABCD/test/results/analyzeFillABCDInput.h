TChain* FormChainJosh(TString type){
  TString version = "02-03";
  const TString treedirectory = "ABCDtree";
  TChain* InputChain = new TChain(treedirectory);

  bool data =false;
  bool MC =true;
  bool contamination=false;
  bool SM=false;
  bool signalC =true;
  
  cout << type << endl;
  
  if(version=="02-00"){
    if(type=="pfpf" && MC && !data) {
      //InputChain->Add("/afs/cern.ch/user/k/kreis/scratch0/ABCDtrees/ABCDtree.Baseline0_PF_pfMEThigh_PFLep0e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.QCD100.root");
      //InputChain->Add("/afs/cern.ch/user/k/kreis/scratch0/ABCDtrees/ABCDtree.Baseline0_PF_pfMEThigh_PFLep0e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.QCD250.root");
      //InputChain->Add("/afs/cern.ch/user/k/kreis/scratch0/ABCDtrees/ABCDtree.Baseline0_PF_pfMEThigh_PFLep0e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.QCD500.root");
      //InputChain->Add("/afs/cern.ch/user/k/kreis/scratch0/ABCDtrees/ABCDtree.Baseline0_PF_pfMEThigh_PFLep0e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.QCD1000.root");
      //InputChain->Add("/tmp/ABCDtree.Baseline0_PF_pfMEThigh_PFLep0e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.PythiaQCD.root");
      InputChain->Add("/afs/cern.ch/user/k/kreis/scratch0/ABCDtrees/ABCDtree.Baseline0_PF_pfMEThigh_PFLep0e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.PythiaPUQCDFlat.root");
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
  }
 
  
  else if(version=="02-03"){
    if(type=="pfpf" && MC && !data) {
      //InputChain->Add("/cu1/joshmt/ABCDtrees/11Mar22/ABCDtree.Baseline0_PF_JERbias_pfMEThigh_PFLepRA20e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.PythiaPUQCDFlat.root");
      //InputChain->Add("/cu1/kreis/ABCDtrees/36_Mar26/ABCDtree.Baseline0_PF_JERbias_pfMEThigh_PFLepRA20e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.PythiaPUQCD.root");
      InputChain->Add("/cu1/kreis/ABCDtrees/36_Mar26/ABCDtree.Baseline0_PF_JERbias_pfMEThigh_PFLepRA20e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.QCD.root");
      if(contamination){
        if(signalC)     
	  assert(0);
        if(SM){
	  assert(0);
        }
      }
    }//end MC                                                                                                                                                                                                                                           
    if(type=="pfpf" && data && !MC){
      InputChain->Add("/cu1/joshmt/ABCDtrees/11Mar22/ABCDtree.Baseline0_PF_pfMEThigh_PFLepRA20e0mu_minDP_MuonCleaning_NoMET_NoDeltaPhi.ge0b.data-0.root");
    }//end data  
  }
  

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
