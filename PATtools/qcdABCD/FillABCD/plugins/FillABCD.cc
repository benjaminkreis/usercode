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
// $Id: FillABCD.cc,v 1.3 2010/08/19 11:17:48 kreis Exp $
//
//


// system include files
#include <memory>
#include <stdlib.h>

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
#include "DataFormats/PatCandidates/interface/MET.h"
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
  
  //input tags
  edm::InputTag jetSrc_;
  edm::InputTag metSrc_;
  double MG_;

  TTree *tree1_;
  double tree1_minDPhi_;
  double tree1_MHT_;
  double tree1_MET_;
  double tree1_qScale_;
  double tree1_PtHat_;
  int tree1_btag_;
  int tree1_bjet_;
  int tree1_nTightJets_;
  int tree1_nLooseJets_;
  double tree1_MG_;

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
  jetSrc_(iConfig.getUntrackedParameter<edm::InputTag>("jetSrc" )),
  metSrc_(iConfig.getUntrackedParameter<edm::InputTag>("metSrc" )),
  MG_(iConfig.getUntrackedParameter<double>("MG"))

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
   
   // get met collection
   edm::Handle<edm::View<pat::MET> > mets;
   iEvent.getByLabel(metSrc_,mets);

   edm::Handle<GenEventInfoProduct> genEvtInfo;
   bool hasGenEventInfo = iEvent.getByLabel("generator", genEvtInfo);

   double pthat = -1.0;
   double qScale = -1.0;
   if(hasGenEventInfo){ 
     pthat = ( genEvtInfo->hasBinningValues() ? (genEvtInfo->binningValues())[0] : 0.0);
     qScale = genEvtInfo->qScale();
   }
  

   //Jet Stuff///////////////////////////////////////////////
   //////////////////////////////////////////////////////////
   double dPhiMin = -1.0; 

   int nTightJets = 0;
   int nLooseJets = 0;
   int nbtag =0, nbjet =0;
  
   double myHT = 0; //scalar sum 
   double myMHTx=0, myMHTy=0;
  
   double jetpt1=0, jetpt2=0, jetpt3=0;
   edm::View<pat::Jet>::const_iterator jet1=jets->end(), jet2=jets->end(),jet3=jets->end();
  
   double jetTpt1=0, jetTpt2=0, jetTpt3=0;
   edm::View<pat::Jet>::const_iterator jetT1=jets->end(), jetT2=jets->end(),jetT3=jets->end();
   
   for(edm::View<pat::Jet>::const_iterator jet=jets->begin(); jet!=jets->end(); ++jet){
    
     int jet_n90Hits = jet->jetID().n90Hits;
     double jet_fHPD = jet->jetID().fHPD;
     float jet_emf = jet->emEnergyFraction();

     bool etaEMFcheck = true;
     if( fabs(jet->eta())<2.6 && jet_emf <= 0.01) etaEMFcheck=false; 
    
     //good jet check
     if( jet_n90Hits >1 && jet_fHPD<0.98 && etaEMFcheck){
       
       //check b
       int jetid = 0;
       jetid = jet->partonFlavour();
       if(fabs(jet->eta())<2.4 && jet->pt()>30.0){
	 if(jet->bDiscriminator("simpleSecondaryVertexBJetTags")>=1.74) nbtag++;
	 if(abs(jetid)==5) nbjet++;
       }
       

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
   
   double myMHT = sqrt(myMHTx*myMHTx+myMHTy*myMHTy);
   
   double myMET = mets->front().et();
  
   //ABCD jet-phi stuff
   double jet1phi = jetT1->phi();
   double jet2phi = jetT2->phi();
   double jet3phi = jetT3->phi();
   double myMHTphi = atan2(myMHTy,myMHTx);
   
   double jet1dPhi = acos(cos(jet1phi-myMHTphi));
   double jet2dPhi = acos(cos(jet2phi-myMHTphi));
   double jet3dPhi = acos(cos(jet3phi-myMHTphi));
   
   //mins
   if(jet1dPhi <= jet2dPhi && jet1dPhi <= jet3dPhi){ dPhiMin = jet1dPhi; }
   else if(jet2dPhi <= jet1dPhi && jet2dPhi <= jet3dPhi){ dPhiMin = jet2dPhi; }
   else if(jet3dPhi <= jet1dPhi && jet3dPhi <= jet2dPhi){ dPhiMin = jet3dPhi; }
   else{ assert(false); }
   
   

   tree1_minDPhi_ = dPhiMin;
   tree1_MHT_= myMHT;
   tree1_MET_ = myMET;
   tree1_qScale_ = qScale;
   tree1_PtHat_ = pthat;
   tree1_MG_ = MG_; 
   tree1_btag_ = nbtag;
   tree1_bjet_ = nbjet;
   tree1_nTightJets_ = nTightJets;
   tree1_nLooseJets_ = nLooseJets;
   tree1_->Fill(); 
  
   //end Jet Stuff//////////////////////////////////////////  
}


// ------------ method called once each job just before starting event loop  ------------
void 
FillABCD::beginJob()
{

  edm::Service<TFileService> fs;
  tree1_ = fs->make<TTree>("T_ABCD", "ABCD TREE");
  tree1_->Branch("minDPhi", &tree1_minDPhi_, "tree1_minDPhi_/D");
  tree1_->Branch("MHT", &tree1_MHT_, "tree1_MHT_/D");
  tree1_->Branch("MET", &tree1_MET_, "tree1_MET_/D");
  tree1_->Branch("qScale", &tree1_qScale_, "tree1_qScale_/D");
  tree1_->Branch("PtHat", &tree1_PtHat_, "tree1_PtHat_/D");
  tree1_->Branch("btag", &tree1_btag_, "tree1_btag_/I");
  tree1_->Branch("bjet", &tree1_bjet_, "tree1_bjet_/I");
  tree1_->Branch("nTightJets", &tree1_nTightJets_, "tree1_nTightJets_/I");
  tree1_->Branch("nLooseJets", &tree1_nLooseJets_, "tree1_nLooseJets_/I");
  tree1_->Branch("MG", &tree1_MG_, "tree1_MG_/D");

  nEvents_ = 0;

}

// ------------ method called once each job just after ending the event loop  ------------
void 
FillABCD::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(FillABCD);
