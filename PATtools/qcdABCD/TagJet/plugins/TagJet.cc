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
// $Id$
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

  for(edm::View<pat::Jet>::const_iterator jet=jets->begin(); jet!=jets->end(); ++jet){
    std::cout << "START" << std::endl;
    std::cout << jet->genParton()->pdgId() << std::endl;
    float jetid = jet->genParton()->pdgId();
    std::cout << "CHECKED ID" << std::endl;
    tree1_eta_ = jet->eta();
    tree1_pt_ = jet->pt();
    tree1_btag_ = 0.0;
    tree1_bjet_ = 0.0;
    std::cout << jetid << std::endl;
    
    if(jet->bDiscriminator("simpleSecondaryVertexBJetTags")>=1.74) tree1_btag_ = 1.0;
    if(fabs(jetid)==5.0) tree1_bjet_ = 1.0;
    
    std::cout << "CHECK" << std::endl;

    tree1_->Fill();

  }//end jet loop

}


// ------------ method called once each job just before starting event loop  ------------
void 
TagJet::beginJob()
{

  edm::Service<TFileService> fs;
  tree1_ = fs->make<TTree>("T_JetTag", "Tagging Efficiency Tree");
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
