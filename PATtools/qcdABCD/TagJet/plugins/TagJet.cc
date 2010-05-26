// -*- C++ -*-
//
// Package:    TagJet
// Class:      TagJet
// 
/**\class TagJet TagJet.cc qcdABCD/TagJet/src/TagJet.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Ben Kreis
//         Created:  Tue May 25 22:38:50 CEST 2010
// $Id: TagJet.cc,v 1.1 2010/05/25 22:11:39 kreis Exp $
//
//


// system include files
#include <memory>

// root include files
#include "TTree.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/PatCandidates/interface/Jet.h"

//
// class decleration
//

class TagJet : public edm::EDAnalyzer {
   public:
      explicit TagJet(const edm::ParameterSet&);
      ~TagJet();


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------

  edm::InputTag jetSrc_;
  
  TTree *tree1_;
  float tree1_nAllJets_;
  float tree1_nTightJets_;
  float tree1_nLooseJets_;
  float tree1_eta_;
  float tree1_pt_;
  float tree1_btag_;
  float tree1_bjet_;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
TagJet::TagJet(const edm::ParameterSet& iConfig):
  jetSrc_(iConfig.getUntrackedParameter<edm::InputTag>("jetSrc" ))
{
   //now do what ever initialization is needed
  
}


TagJet::~TagJet()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
TagJet::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

  // get jet collection
  edm::Handle<edm::View<pat::Jet> > jets;
  iEvent.getByLabel(jetSrc_,jets);

  int nAllJets = 0;
  int nTightJets = 0;
  int nLooseJets = 0;
  //get jet multiplicity
  for(edm::View<pat::Jet>::const_iterator jet=jets->begin(); jet!=jets->end(); ++jet){
    float jet_n90Hits = jet->jetID().n90Hits;
    float jet_fHPD = jet->jetID().fHPD;
    float jet_emf = jet->emEnergyFraction();
    
    bool etaEMFcheck = true;
    if( fabs(jet->eta())<2.6 && jet_emf <= 0.01) etaEMFcheck=false; 
    
    //good jet check
    if( jet_n90Hits >1.0 && jet_fHPD<0.98 && etaEMFcheck){
      nAllJets++;
      
      //loose pT and eta cut
      if( jet->pt()>30.0 && fabs(jet->eta())<5.0){
	nLooseJets++;
	
	//tight pT and eta cut
	if( jet->pt()>50.0 && fabs(jet->eta())<2.5){
	  nTightJets++;
	}//end tight jet
	
      }//end loose jet

    }//good jet check
  }//jet loop


  for(edm::View<pat::Jet>::const_iterator jet=jets->begin(); jet!=jets->end(); ++jet){
   
    float jet_n90Hits = jet->jetID().n90Hits;
    float jet_fHPD = jet->jetID().fHPD;
    float jet_emf = jet->emEnergyFraction();
    
    bool etaEMFcheck = true;
    if( fabs(jet->eta())<2.6 && jet_emf <= 0.01) etaEMFcheck=false; 
    
    //good jet check
    if( jet_n90Hits >1.0 && jet_fHPD<0.98 && etaEMFcheck){
      

      float jetid = 0;
      //std::cout << "pf: " << jet->partonFlavour() << ", pdg: " <<  jet->genParton()->pdgId();
      jetid = jet->partonFlavour();
      
      tree1_nAllJets_ = nAllJets;
      tree1_nTightJets_ = nTightJets;
      tree1_nLooseJets_ = nLooseJets;
      tree1_eta_ = jet->eta();
      tree1_pt_ = jet->pt();
      tree1_btag_ = 0.0;
      tree1_bjet_ = 0.0;
      if(jet->bDiscriminator("simpleSecondaryVertexBJetTags")>=1.74) tree1_btag_ = 1.0;
      if(fabs(jetid)==5.0) tree1_bjet_ = 1.0;
      
      tree1_->Fill();
    
    }//good jet check
  }//end jet loop

}


// ------------ method called once each job just before starting event loop  ------------
void 
TagJet::beginJob()
{

  edm::Service<TFileService> fs;
  tree1_ = fs->make<TTree>("T_JetTag", "Tagging Efficiency Tree");
  tree1_->Branch("nAllJets", &tree1_nAllJets_, "tree1_nAllJets_/F");
  tree1_->Branch("nTightJets", &tree1_nTightJets_, "tree1_nTightJets_/F");
  tree1_->Branch("nLooseJets", &tree1_nLooseJets_, "tree1_nLooseJets_/F");
  tree1_->Branch("eta", &tree1_eta_, "tree1_eta_/F");
  tree1_->Branch("pt", &tree1_pt_, "tree1_pt_/F");
  tree1_->Branch("btag", &tree1_btag_, "tree1_btag_/F");
  tree1_->Branch("bjet", &tree1_bjet_, "tree1_bjet_/F");

  
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TagJet::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(TagJet);
