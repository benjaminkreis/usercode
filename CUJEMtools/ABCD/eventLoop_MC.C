//Ben Kreis

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TAxis.h"
#include "TMatrixT.h"
#include "TVectorT.h"
#include <iostream>
#include <vector>
#include <exception>
#include <cmath> 
#include <iomanip>
#include <assert.h>

//histoholder
#include "/afs/cern.ch/user/k/kreis/SUSYbtag/utils/HistHolder.cxx"

//Headers for the data items
#include "ProdTutorial/CUcollections/interface/CUelectron.h"
#include "ProdTutorial/CUcollections/interface/CUevent.h"
#include "ProdTutorial/CUcollections/interface/CUjet.h"
#include "ProdTutorial/CUcollections/interface/CUmcparticle.h"
#include "ProdTutorial/CUcollections/interface/CUmet.h"
#include "ProdTutorial/CUcollections/interface/CUmuon.h"
#include "ProdTutorial/CUcollections/interface/CUphoton.h"
#include "ProdTutorial/CUcollections/interface/CUsupercluster.h"
#include "ProdTutorial/CUcollections/interface/CUtrack.h"
#include "ProdTutorial/CUcollections/interface/CUprimaryvertex.h"
#include "ProdTutorial/CUcollections/interface/CUtrigger.h"
#include "ProdTutorial/CUcollections/interface/CUskimbits.h"

typedef CUelectronCollection::const_iterator      EleIter;
typedef CUjetCollection::const_iterator           JetIter;
typedef CUmcparticleCollection::const_iterator    MCparIter;
typedef CUmuonCollection::const_iterator          MuonIter;
typedef CUphotonCollection::const_iterator        PhotonIter;
typedef CUsuperclusterCollection::const_iterator  SCIter;
typedef CUprimaryvertexCollection::const_iterator  VtxIter;
typedef CUtrackCollection::const_iterator         TrackIter;
typedef CUtriggerCollection::const_iterator       TrigIter;

vector<string> fileNames;

TString sampleid_;

void selectData(TString sample, TString maxindex="") {

  TString path="rfio:/castor/cern.ch/user/p/puigh/CUSusy/CUJEM/Summer09/7TeV/Output/";
  sampleid_ = sample;
  
  if (sample.Contains("QCD")) path+="QCD-madgraph/";

  if (sample.Contains("LM")) {
    path+="LM/";
    path+=sampleid_;
    path += "_Summer09_7TeV_CUJEM_V09.root";
    cout<<"Adding to list of input files: "<<path<<endl;
    fileNames.push_back(string(path.Data()));
  }
  else  {
    if (maxindex!="") {
      if(sample.Contains("TTBar")) {
	for (int ind=1; ind<=maxindex.Atoi(); ind++) {
	  TString mypath = "rfio:/castor/cern.ch/user/k/kreis/CUSusy/CUJEM/Summer09/7TeV/Output/";
	  mypath+=sampleid_;
	  mypath += "_Summer09_7TeV_CUJEM_V09_";
	  mypath +=ind;
	  mypath +=".root";
	  cout<<"Adding to list of input files: "<<mypath<<endl;
	  fileNames.push_back(string(mypath.Data()));
	}
      }
      else if(sample.Contains("QCD_Pt170")) {
	for (int ind=1; ind<=maxindex.Atoi(); ind++) {
	  TString mypath = "rfio:/castor/cern.ch/user/k/kreis/CUSusy/CUJEM/Summer09/7TeV/Output/";
	  mypath+=sampleid_;
	  mypath += "_Summer09_7TeV_CUJEM_V09_";
	  mypath +=ind;
	  mypath +=".root";
	  cout<<"Adding to list of input files: "<<mypath<<endl;
	  fileNames.push_back(string(mypath.Data()));
	}
      }
      else if(sample.Contains("QCD_Pt80")) {
	for (int ind=1; ind<=maxindex.Atoi(); ind++) {
	  TString mypath = "rfio:/castor/cern.ch/user/j/joshmt/CUSusy/CUJEM/Summer09/7TeV/Output/";
	  mypath+=sampleid_;
	  mypath += "_Summer09_7TeV_CUJEM_V09_";
	  mypath +=ind;
	  mypath +=".root";
	  cout<<"Adding to list of input files: "<<mypath<<endl;
	  fileNames.push_back(string(mypath.Data()));
	}
      }
      else{
	for (int ind=1; ind<=maxindex.Atoi(); ind++) {
	  TString mypath=path;
	  mypath+=sampleid_;
	  mypath += "_Summer09_7TeV_CUJEM_V09_";
	  mypath +=ind;
	  mypath +=".root";
	  cout<<"Adding to list of input files: "<<mypath<<endl;
	  fileNames.push_back(string(mypath.Data()));
	}
      }
    }
    else {
      path+=sampleid_;
      path += "_Summer09_7TeV_CUJEM_V09.root";
      cout<<"Adding to list of input files: "<<path<<endl;
      fileNames.push_back(string(path.Data()));
    }
  }

}

//+++PASS BJET CUTS+++//
bool passBTagCuts(JetIter jet){
  if( jet->pt < 30 ) return false;
  if( jet->btagSecVertex < 1.74 ) return false;
  if( fabs(jet->eta) > 2.4 ) return false;
  return true; 
}//end passBTagCuts

bool passMCBJetCuts(JetIter jet){
  if((jet->genPartonId!=5) && (jet->genPartonId!=-5)) return false;
  if( jet->pt < 30 ) return false;
  if( fabs(jet->eta) > 2.4 ) return false;
  return true; 
}//end passMCBJetCuts

//+++PASS JET CUTS+++//
bool passJetCuts(JetIter jet){
  if( jet->pt < 30 ) return false;
  if ( fabs(jet->eta) > 2.4) return false;
  return true;
}//end passJetCuts


//+++CALCULATE DELTA PHI+++//
double getDeltaPhi(JetIter j1, CUmet cumet) {
  double metphi = cumet.phi;
  double j1_phi = j1->phi;
  return acos(cos(metphi-j1_phi));
}

double getDeltaPhi(JetIter j1, JetIter j2) {
  return acos(cos(j2->phi - j1->phi));
}
//end delta phi calculations


//+++MAIN FUNCTION+++//
void eventLoop_MC(){
  
  double pi = 4*atan(1.0);
  float borderv2a = 150.0;
  float borderv2b = 1000.0;
  float borderh2a = 0.3;
  float borderh2b = pi;

  //COUNTERS
  float P2Tot = 0.0;
  float Pge2Tot = 0.0;
  float Pge3Tot = 0.0;
  float nSignalTot = 0.0;
    
  //INPUT FILES
  // fileNames.push_back(); //need to add filename
  fwlite::ChainEvent ev(fileNames);

  //OUTPUT FILE
  //TFile fout("histo_ben_FWLite_analyzer.root","recreate");
  TString outfile = "plots_";
  outfile+=sampleid_;
  outfile+="_";
  outfile+=".root";
  TFile fout(outfile,"RECREATE");
  
  int nentries = ev.size();

  //////////////////////
  //DECLARE HISTOGRAMS//
  //////////////////////
  HistHolder histo; 
 
  int nbins_Phi = 160;
  int nbins_MET = 100;
  histo.make2("H_minDPhi_MET","min Delta Phi vs MET",nbins_MET,0,1000,nbins_Phi,0,pi);


  //DECLARE TREES//
  TTree *tree1 = new TTree("T_prob", "probabilities");
  float tree1_P2;
  float tree1_Pge2;
  float tree1_Pge3;
  float tree1_nbtags;
  float tree1_ngoodMCbjets;
  tree1->Branch("Probability for exactly 2 tags", &tree1_P2, "tree1_P2/F");
  tree1->Branch("Probability for 2 or more tags", &tree1_Pge2, "tree1_Pge2/F");
  tree1->Branch("Probability for 3 or more tags", &tree1_Pge3, "tree1_Pge3/F");
  tree1->Branch("ngoodMCbjets", &tree1_ngoodMCbjets, "tree1_ngoodMCbjets/F");
  tree1->Branch("nbtags", &tree1_nbtags, "tree1_nbtags/F");
    

  //////////////////////////////////////////////////////////////////////////////////////////

  //LOOP OVER EVENTS
  int cnt = 0;
  for( ev.toBegin(); !ev.atEnd(); ++ev) {
    //cout << "Event " << cnt << endl;
    //Counter output 
    cnt++;
    if( cnt==1 )        std::cout << "     Event " << cnt << std::endl;
    if( cnt%1000==0 && cnt!=1 ) std::cout << "           " << cnt << "\t" 
					    << int(double(cnt)/double(nentries)*100) << "% done" << std::endl;
    //Try the event
    try {

      //HANDLES
      fwlite::Handle<CUeventCollection> h_event;
      h_event.getByLabel(ev,"CUproducer");

      fwlite::Handle<CUelectronCollection> h_electrons;
      h_electrons.getByLabel(ev,"CUproducer","cleanLayer1Electrons");
      CUelectronCollection const &electrons = *h_electrons;

      fwlite::Handle<CUjetCollection> h_jets;
      h_jets.getByLabel(ev,"CUproducer","cleanLayer1JetsAK5");
      CUjetCollection const &jets = *h_jets;

      fwlite::Handle<CUmetCollection> h_met;
      h_met.getByLabel(ev,"CUproducer","layer1METsAK5");

      fwlite::Handle<CUmcparticleCollection> h_mcparticles;
      h_mcparticles.getByLabel(ev,"CUproducer","MCstatus3");
      CUmcparticleCollection const &mcparticles = *h_mcparticles;

      fwlite::Handle<CUmuonCollection> h_muons;
      h_muons.getByLabel(ev,"CUproducer","cleanLayer1Muons");
      CUmuonCollection const &muons = *h_muons;

      fwlite::Handle<CUphotonCollection> h_photons;
      h_photons.getByLabel(ev,"CUproducer","cleanLayer1Photons");
      CUphotonCollection const &photons = *h_photons;

      fwlite::Handle<CUsuperclusterCollection> h_superclusters;
      h_superclusters.getByLabel(ev,"CUproducer","corHybridSCandMulti5x5WithPreshower");
      CUsuperclusterCollection const &scs = *h_superclusters;

      fwlite::Handle<CUtrackCollection> h_tracks;
      h_tracks.getByLabel(ev,"CUproducer","generalTracks");
      CUtrackCollection const &tracks = *h_tracks;

      fwlite::Handle<CUtriggerCollection> h_hlt;
      h_hlt.getByLabel(ev,"CUproducer","HLT");
      CUtriggerCollection const &hlt = *h_hlt;

      fwlite::Handle<CUtriggerCollection> h_l1t_algo;
      h_l1t_algo.getByLabel(ev,"CUproducer","L1Talgo");
      CUtriggerCollection const &l1t_algo = *h_l1t_algo;

      fwlite::Handle<CUtriggerCollection> h_l1t_tech;
      h_l1t_tech.getByLabel(ev,"CUproducer","L1Ttech");
      CUtriggerCollection const &l1t_tech = *h_l1t_tech;

      fwlite::Handle<CUprimaryvertexCollection> h_privertex;
      h_privertex.getByLabel(ev,"CUproducer","offlinePrimaryVertices");
      CUprimaryvertexCollection const &vertices = *h_privertex;

      fwlite::Handle<CUskimbitsCollection> h_skimbits;
      h_skimbits.getByLabel(ev,"CUproducer","SkimBits");


      //FREQUENTLY USED VARIABLES
      double metPT  = h_met->front().pt;
      double metUnCorrPt = h_met->front().Upt;
      double sumET  = h_met->front().sumET;

      //LOOP OVER JETS
      float jetpt1=0, jetpt2=0, jetpt3=0, jetpt4=0;
      JetIter jet1= jets.end(), jet2= jets.end(), jet3= jets.end(), jet4= jets.end();
      float nbtags = 0;
      float ngoodMCbjets = 0;

      vector<float> jetEff;

      for( JetIter jet = jets.begin(); jet != jets.end(); ++jet ) {
	
	//Apply jet cuts
	if( passJetCuts(jet) ){
	
	  if( passBTagCuts(jet) ) nbtags+=1.0; 
	  if( passMCBJetCuts(jet) ) ngoodMCbjets+=1.0; 
	  
	  float thisJetEff = 0.1; // in future should come from some data-driven technique

	  jetEff.push_back(thisJetEff);

	  //Find leading jets
	  if(jet->pt > jetpt1) {
	    jet4 = jet3;
	    jetpt4 = jetpt3;
	    jet3 = jet2;
	    jetpt3 = jetpt2;
	    jet2 = jet1;
	    jetpt2 = jetpt1;
	    jet1 = jet;
	    jetpt1 = jet->pt;
	  }
	  else if(jet->pt > jetpt2){
	    jet4 = jet3;
	    jetpt4 = jetpt3;
	    jet3 = jet2;
	    jetpt3 = jetpt2;
	    jet2 = jet;
	    jetpt2 = jet->pt;
	  }
	  else if(jet->pt > jetpt3){
	    jet4 = jet3;
	    jetpt4 = jetpt3;
	    jet3 = jet;
	    jetpt3 = jet->pt;
	  }
	  else if(jet->pt > jetpt4){
	    jet4 = jet;
	    jetpt4 = jet->pt;
	  }
	  //end find leading jets
	  
	}//end jet cuts
	
      }//end loop over jets
      
      if(jetpt1>180 && jetpt2>150 && jetpt3>50){//cut0
	
	assert(jetEff.size()>2);
	
	//min delta phi is calculated from leading three jets
	float dPhi1 = acos(cos(fabs((h_met->front()).phi - jet1->phi)));
	float dPhi2 = acos(cos(fabs((h_met->front()).phi - jet2->phi)));
	float dPhi3 = acos(cos(fabs((h_met->front()).phi - jet3->phi)));
	float dPhiMin=-1.0;
	if(dPhi1<=dPhi2 && dPhi1<=dPhi3){ dPhiMin = dPhi1;}
	else if(dPhi2<=dPhi1 && dPhi2<=dPhi3){ dPhiMin = dPhi2;}
	else { dPhiMin = dPhi3; }
	// at this point we have MET (metPT) and minDPhi (dPhiMin) 
	
	//IF IN SIGNAL REGION - calculate probability that event passes b-tag cut
	if( (metPT>=borderv2a) && (metPT<borderv2b) && (dPhiMin>=borderh2a) && (dPhiMin<borderh2b) ){

	  nSignalTot++;
	  
	  // calculate probability for 0 tags
	  float P0=1;
	  for(int i = 0; i<jetEff.size(); i++){
	    P0=P0*(1-jetEff[i]);
	  }
	  
	  // calculate probability for exactly 1 tag
	  float P1=1;
	  for(int i = 0; i<jetEff.size(); i++){
	   
	    //second part
	    float P1a = 1;
	    for(int j = 0; j<jetEff.size(); j++){
	      if(j!=i){
		P1a=P1a*(1-jetEff[j]);
	      }
	    }
	    
	    P1+=jetEff[i]*P1a;
	  }
	  
	  // calculate probability for exactly 2 tags
	  float P2=1;
	  for(int i=0; i<jetEff.size(); i++){
	    for(int j=0; j<jetEff.size(); j++){
	     
	      float P2a=1;
	      for(int k=0; k<jetEff.size(); k++){
		if(k!=i && k!=j){
		  P2a=P2a*(1-jetEff[k]);
		}//if
	      }//k loop
	      
	      P2+=jetEff[i]*jetEff[j]*P2a;
	      
	    }//j loop
	  }// i loop


	  // calculate probability for >= 2 tags and >=3 tags
	  float Pge2, Pge3;
	  Pge2 = 1 - P1 - P0;
	  Pge3 = 1 - P2 - P1 - P0;
	  
	  tree1_P2=P2;
	  tree1_Pge2 = Pge2;
	  tree1_Pge3 = Pge3;
	  tree1_ngoodMCbjets = ngoodMCbjets;
	  tree1_nbtags = nbtags;
	  tree1->Fill();

	  P2Tot+=P2;
	  Pge2Tot+=Pge2;
	  Pge3Tot+=Pge3;

	}// end if in signal region
	
      }//cut 0
      
      
    }//end try
    catch(std::exception& e) {
      std::cerr << " ==> caught exception " << e.what() << std::endl;
      continue;
    }
    
    // if(cnt == 100000) break;//for debugging purposes
    
  }//end loop over events
  cout << "eventCount: " << cnt << endl;
  cout << "nSignalTot: " << nSignalTot << endl;
  cout << "P2Tot: " << P2Tot << endl;
  cout << "Pge2Tot: " << Pge2Tot << endl;
  cout << "Pge3Tot: " << Pge3Tot << endl;

  tree1->Print();
  fout.Write();
  fout.Close();
}//end eventLoop_MC()


