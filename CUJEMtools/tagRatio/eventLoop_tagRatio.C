//Ben Kreis

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"

#include "TFile.h"
#include "TChain.h"
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

bool passGoodMCBJetCuts(JetIter jet){
  if((jet->genPartonId!=5) && (jet->genPartonId!=-5)) return false;
  if( jet->pt < 30 ) return false;
  if( fabs(jet->eta) > 2.4 ) return false;
  return true; 
}//end passGoodMCBJetCuts

bool passMCBJetCuts(JetIter jet){
  if((jet->genPartonId!=5) && (jet->genPartonId!=-5)) return false;
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
void eventLoop_tagRatio(){


  //INPUT FILES
  // fileNames.push_back(); //need to add filename
  fwlite::ChainEvent ev(fileNames);

  //OUTPUT FILE
  //TFile fout("histo_ben_FWLite_analyzer.root","recreate");
  TString outfile = "plots_";
  outfile+=sampleid_;
  // outfile+="_";
  // outfile+=reqNumBJets;
  outfile+=".root";
  TFile fout(outfile,"RECREATE");
  
  int nentries = ev.size();

  //////////////////////
  //DECLARE HISTOGRAMS//
  //////////////////////
  // HistHolder histo; 
  
  TTree *tree1 = new TTree("T_tagRatio", "Tag Ratio");
  float tree1_MET;
  float tree1_jetpt1;
  float tree1_jeteta1;
  float tree1_njets;
  float tree1_ntags;
  //  float tree1_nbjets;
  float tree1_ngoodbjets;
  tree1->Branch("MET", &tree1_MET, "tree1_MET/F");
  tree1->Branch("njets", &tree1_njets, "tree1_njets/F");
  // tree1->Branch("nbjets", &tree1_nbjets, "tree1_nbjets/F");
  tree1->Branch("ngoodbjets", &tree1_ngoodbjets, "tree1_ngoodbjets/F");
  tree1->Branch("ntags", &tree1_ntags, "tree1_ntags/F");
  tree1->Branch("jetpt1", &tree1_jetpt1, "tree1_jetpt1/F");
  tree1->Branch("jeteta1", &tree1_jeteta1, "tree1_jeteta1/F");
  
 
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

      int ngoodBJets=0;
      // int nBJets=0;
      int ngoodBTags=0;
      int nJets=0;
      float jetpt1 = 0;
      float jeteta1;
      //LOOP OVER JETS
      for( JetIter jet = jets.begin(); jet != jets.end(); ++jet ) {
	if(passJetCuts(jet)){
	  nJets++;
	  if( passBTagCuts(jet) ) ngoodBTags++;
	  if( passGoodMCBJetCuts(jet) ) ngoodBJets++;
	  if(jet->pt > jetpt1){
	    jetpt1 = jet->pt;
	    jeteta1 = jet->eta;
	  }
	  
	}
      }//end loop over jets
  
      if(nJets>0){
	tree1_njets = nJets;
	tree1_ntags = ngoodBTags;
	//tree1_nbjets = nBJets;
	tree1_ngoodbjets = ngoodBJets;
	tree1_MET = metPT;
	tree1_jetpt1 = jetpt1;
	tree1_jeteta1 = jeteta1;
	tree1->Fill();
      }
      
    }//end try
    catch(std::exception& e) {
      std::cerr << " ==> caught exception " << e.what() << std::endl;
      continue;
    }
    
    // if(cnt == 10) break;//for debugging purposes
    
  }//end loop over events
  cout << "event count: " << cnt << endl;
  tree1->Print();
  fout.Write();
  fout.Close();
}//end eventLoop_tagRatio()


