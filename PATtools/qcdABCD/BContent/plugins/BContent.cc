// -*- C++ -*-
//
// Package:    BContent
// Class:      BContent
// 
/**\class BContent BContent.cc qcdABCD/BContent/src/BContent.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Ben Kreis,6 R-029,+41227678914,
//         Created:  Thu Jul  1 15:03:13 CEST 2010
// $Id$
//
//


// system include files
#include <memory>
#include <cstdlib>

// root include files
#include "TTree.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"



//
// class declaration
//

class BContent : public edm::EDAnalyzer {
   public:
      explicit BContent(const edm::ParameterSet&);
      ~BContent();


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
  edm::InputTag jetSrc_;
  TTree *tree1_;
  double tree1_qScale_;
  double tree1_PtHat_;
  double tree1_minDPhi_;
  double tree1_MHT_;
  int tree1_nbtag_;
  int tree1_nbjet_;
  int tree1_nlightmistag_;
  int tree1_ncmistag_;
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
BContent::BContent(const edm::ParameterSet& iConfig):
  jetSrc_(iConfig.getUntrackedParameter<edm::InputTag>("jetSrc" ))
{
   //now do what ever initialization is needed

}


BContent::~BContent()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
BContent::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;
  
  // get jet collection
  edm::Handle<edm::View<pat::Jet> > jets;
   iEvent.getByLabel(jetSrc_,jets);
   
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
   vector<double> jetEff;
   vector<double> jetEffe;
   
   double dPhiMin = -1.0; 
   
   int nTightJets = 0;
   int nLooseJets = 0;
   int nbtag1 = 0;

   double myHT = 0; //scalar sum 
   double myMHTx=0, myMHTy=0;
   
   double jetpt1=0, jetpt2=0, jetpt3=0;
   edm::View<pat::Jet>::const_iterator jet1=jets->end(), jet2=jets->end(),jet3=jets->end();
   
   double jetTpt1=0, jetTpt2=0, jetTpt3=0;
   edm::View<pat::Jet>::const_iterator jetT1=jets->end(), jetT2=jets->end(),jetT3=jets->end();
   
   for(edm::View<pat::Jet>::const_iterator jet=jets->begin(); jet!=jets->end(); ++jet){
     
     double jet_n90Hits = jet->jetID().n90Hits;
     double jet_fHPD = jet->jetID().fHPD;
     double jet_emf = jet->emEnergyFraction();
     
     bool etaEMFcheck = true;
     if( fabs(jet->eta())<2.6 && jet_emf <= 0.01) etaEMFcheck=false; 
    
     //good jet check
     if( jet_n90Hits >1.0 && jet_fHPD<0.98 && etaEMFcheck){
       
       //check b
       if(fabs(jet->eta())<2.4 && jet->pt()>30.0){
	 if(jet->bDiscriminator("simpleSecondaryVertexBJetTags")>=1.74) nbtag1++;
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
   //end Jet Stuff//////////////////////////////////////////  
   
   //if in signal region (running on skim, so only have to apply MHT and minDPhi cut)
   if( myMHT>150.0 && dPhiMin>0.3 && nbtag1>1){
     
     cout << "pass" << endl;
     
     int nbtag=0;
     int nbjet=0;
     int nlightmistag=0;
     int ncmistag=0;
     for(edm::View<pat::Jet>::const_iterator jet=jets->begin(); jet!=jets->end(); ++jet){
       
       double jet_n90Hits = jet->jetID().n90Hits;
       double jet_fHPD = jet->jetID().fHPD;
       double jet_emf = jet->emEnergyFraction();
       
       bool etaEMFcheck = true;
       if( fabs(jet->eta())<2.6 && jet_emf <= 0.01) etaEMFcheck=false; 
       
       //good jet check
       if( jet_n90Hits >1.0 && jet_fHPD<0.98 && etaEMFcheck){
	 
	 //check b
	 int jetid = 0;
	 jetid = jet->partonFlavour();
	 if(fabs(jet->eta())<2.4 && jet->pt()>30.0){
	   if(jet->bDiscriminator("simpleSecondaryVertexBJetTags")>=1.74){
	     nbtag++;
	     if( abs(jetid)==1 ||  abs(jetid)==2 ||  abs(jetid)==3 ) nlightmistag++;
	     if( abs(jetid)==4 ) ncmistag++;
	     if( abs(jetid)==5 ) nbjet++;
	   }
	 }
	 
       }//good jet check
     }//jet loop

     tree1_minDPhi_ = dPhiMin;
     tree1_MHT_ = myMHT;
     tree1_qScale_= qScale;
     tree1_PtHat_ = pthat;
     tree1_nbtag_ = nbtag;
     tree1_nbjet_ = nbjet;
     tree1_nlightmistag_ = nlightmistag;
     tree1_ncmistag_ = ncmistag;
     tree1_->Fill();

   }//end if in signal region
}


   // ------------ method called once each job just before starting event loop  ------------
   void 
BContent::beginJob()
{

  edm::Service<TFileService> fs;
  tree1_ = fs->make<TTree>("T_BContent", "b Content TREE");
  tree1_->Branch("minDPhi", &tree1_minDPhi_, "tree1_minDPhi_/D");
  tree1_->Branch("MHT", &tree1_MHT_, "tree1_MHT_/D");
  tree1_->Branch("qScale", &tree1_qScale_, "tree1_qScale_/D");
  tree1_->Branch("PtHat", &tree1_PtHat_, "tree1_PtHat_/D");
  tree1_->Branch("nbtag", &tree1_nbtag_, "tree1_nbtag_/I");
  tree1_->Branch("nbjet", &tree1_nbjet_, "tree1_nbjet_/I");
  tree1_->Branch("nlightmistag", &tree1_nlightmistag_, "tree1_nlightmistag_/I");
  tree1_->Branch("ncmistag", &tree1_ncmistag_, "tree1_ncmistag_/I");
}

// ------------ method called once each job just after ending the event loop  ------------
void 
BContent::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(BContent);
