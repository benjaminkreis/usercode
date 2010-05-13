// -*- C++ -*-
//
// Package:    FillABCD
// Class:      FillABCD
// 
/**\class FillABCD FillABCD.cc qcdABCD/FillABCD/src/FillABCD.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Ben Kreis
//         Created:  Thu May 13 06:03:28 CEST 2010
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
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

//
// class decleration
//

class FillABCD : public edm::EDAnalyzer {
   public:
      explicit FillABCD(const edm::ParameterSet&);
      ~FillABCD();


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
  unsigned int nEvents_;
  edm::InputTag jetSrc_;
  TTree *tree1_;
  float tree1_minDPhi_;
  float tree1_MHT_;
  float tree1_qScale_;
  float tree1_PtHat_;


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
FillABCD::FillABCD(const edm::ParameterSet& iConfig):
  jetSrc_(iConfig.getUntrackedParameter<edm::InputTag>("jetSrc" ))

{
   //now do what ever initialization is needed


}


FillABCD::~FillABCD()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
FillABCD::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   
   nEvents_++;

   // get jet collection
   edm::Handle<edm::View<pat::Jet> > jets;
   iEvent.getByLabel(jetSrc_,jets);
   
   edm::Handle<GenEventInfoProduct> genEvtInfo;
   bool hasGenEventInfo = iEvent.getByLabel("generator", genEvtInfo);

   float pthat = -1.0;
   float qScale = -1.0;
   if(hasGenEventInfo){ 
     pthat = ( genEvtInfo->hasBinningValues() ? (genEvtInfo->binningValues())[0] : 0.0);
     qScale = genEvtInfo->qScale();
   }
  

   //Jet Stuff///////////////////////////////////////////////
   //////////////////////////////////////////////////////////
   float dPhiMin = -1.0; 

   int nTightJets = 0;
   int nLooseJets = 0;
  
   float myHT = 0; //scalar sum 
   float myMHTx=0, myMHTy=0;
  
   float jetpt1=0, jetpt2=0, jetpt3=0;
   edm::View<pat::Jet>::const_iterator jet1=jets->end(), jet2=jets->end(),jet3=jets->end();
  
   float jetTpt1=0, jetTpt2=0, jetTpt3=0;
   edm::View<pat::Jet>::const_iterator jetT1=jets->end(), jetT2=jets->end(),jetT3=jets->end();
  
   for(edm::View<pat::Jet>::const_iterator jet=jets->begin(); jet!=jets->end(); ++jet){
    
     float jet_n90Hits = jet->jetID().n90Hits;
     float jet_fHPD = jet->jetID().fHPD;
     float jet_emf = jet->emEnergyFraction();

     bool etaEMFcheck = true;
     if( fabs(jet->eta())<2.6 && jet_emf <= 0.01) etaEMFcheck=false; 
    
     //good jet check
     if( jet_n90Hits >1.0 && jet_fHPD<0.98 && etaEMFcheck){
      
       //loose pT and eta cut
       if( jet->pt()>30.0 && fabs(jet->eta())<5.0){
	 
	 nLooseJets++;

	 //find leading 3 jets
	 if(jet->pt() > jetpt1) {
	   jet3 = jet2;
	   jetpt3 = jetpt2;
	   jet2 = jet1;
	   jetpt2 = jetpt1;
	   jet1 = jet;
	   jetpt1 = jet->pt();
	 }
	 else if(jet->pt() > jetpt2){
	   jet3 = jet2;
	   jetpt3 = jetpt2;
	   jet2 = jet;
	   jetpt2 = jet->pt();
	 }
	 else if(jet->pt() > jetpt3){
	   jet3 = jet;
	   jetpt3 = jet->pt();
	 }
	 
	 // add jet to MHT estimate
	 myMHTx+=-jet->px();
	 myMHTy+=-jet->py();
	 
	 //tight pT and eta cut
	 if( jet->pt()>50.0 && fabs(jet->eta())<2.5){
	     
	   nTightJets++;
	     
	   //find leading 3 jets
	   if(jet->pt() > jetTpt1) {
	     jetT3 = jetT2;
	     jetTpt3 = jetTpt2;
	     jetT2 = jetT1;
	     jetTpt2 = jetTpt1;
	     jetT1 = jet;
	     jetTpt1 = jet->pt();
	   }
	   else if(jet->pt() > jetTpt2){
	     jetT3 = jetT2;
	     jetTpt3 = jetTpt2;
	     jetT2 = jet;
	     jetTpt2 = jet->pt();
	   }
	   else if(jet->pt() > jetTpt3){
	     jetT3 = jet;
	     jetTpt3 = jet->pt();
	   }  
	     
	   //add jet to HT estimate
	   myHT += jet->pt();
	     
	 }// end loose pT and eta cut
       }// end loose pT and eta cut
     }// end good jet cut
   }// end jets loop
  
   float myMHT = sqrt(myMHTx*myMHTx+myMHTy*myMHTy);

   //ABCD jet-phi stuff
   float jet1phi = jetT1->phi();
   float jet2phi = jetT2->phi();
   float jet3phi = jetT3->phi();
   float myMHTphi = atan2(myMHTy,myMHTx);
   
   float jet1dPhi = acos(cos(jet1phi-myMHTphi));
   float jet2dPhi = acos(cos(jet2phi-myMHTphi));
   float jet3dPhi = acos(cos(jet3phi-myMHTphi));
   
   //mins
   if(jet1dPhi <= jet2dPhi && jet1dPhi <= jet3dPhi){ dPhiMin = jet1dPhi; }
   else if(jet2dPhi <= jet1dPhi && jet2dPhi <= jet3dPhi){ dPhiMin = jet2dPhi; }
   else if(jet3dPhi <= jet1dPhi && jet3dPhi <= jet2dPhi){ dPhiMin = jet3dPhi; }
   else{ assert(false); }
   
   tree1_minDPhi_ = dPhiMin;
   tree1_MHT_= myMHT;
   tree1_qScale_ = qScale;
   tree1_PtHat_ = pthat;
   tree1_->Fill();
   std::cout << "FILL" << std::endl;

   //end Jet Stuff//////////////////////////////////////////  



}


// ------------ method called once each job just before starting event loop  ------------
void 
FillABCD::beginJob()
{

  edm::Service<TFileService> fs;
  tree1_ = fs->make<TTree>("T_ABCD", "ABCD TREE");
  tree1_->Branch("minDPhi", &tree1_minDPhi_, "tree1_minDPhi_/F");
  tree1_->Branch("MHT", &tree1_MHT_, "tree1_MHT_/F");
  tree1_->Branch("qScale", &tree1_qScale_, "tree1_qScale_/F");
  tree1_->Branch("PtHat", &tree1_PtHat_, "tree1_PtHat_/F");

  nEvents_ = 0;

}

// ------------ method called once each job just after ending the event loop  ------------
void 
FillABCD::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(FillABCD);
