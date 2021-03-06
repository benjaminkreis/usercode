// -*- C++ -*-
//
// Package:    OptFilter
// Class:      OptFilter
// 
/**\class OptFilter OptFilter.cc Filter/OptFilter/src/OptFilter.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Ben Kreis
//         Created:  Thu Jun 10 13:24:41 CEST 2010
// $Id: OptFilter.cc,v 1.7 2010/06/14 10:20:06 kreis Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Common/interface/TriggerNames.h"
//#include "FWCore/Framework/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"

#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/VertexReco/interface/Vertex.h" 
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

//
// class declaration
//

class OptFilter : public edm::EDFilter {
   public:
      explicit OptFilter(const edm::ParameterSet&);
      ~OptFilter();

   private:
      virtual void beginJob() ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      // ----------member data ---------------------------
  edm::InputTag elecSrc_;
  edm::InputTag muonSrc_;
  edm::InputTag jetSrc_;
  edm::InputTag metSrc_;
  edm::InputTag triggerResultsTag_;
  edm::InputTag pvSrc_;

  std::vector<std::string> hltnames_;

  int nEventsUnweighted_;
  double nEvents_;
  double nPass0_;
  double nPass1_;
  double nPass2_;
  double nPass3_;
  double nPass4_;
  double nPass5_;
  double nPass6_;
  double nPass7_;
  double nPass8a_;
  double nPass8b_;
  double nPass8c_;

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
OptFilter::OptFilter(const edm::ParameterSet& iConfig):
  elecSrc_(iConfig.getUntrackedParameter<edm::InputTag>("electronSrc")),
  muonSrc_(iConfig.getUntrackedParameter<edm::InputTag>("muonSrc")),
  jetSrc_(iConfig.getUntrackedParameter<edm::InputTag>("jetSrc" )),
  metSrc_(iConfig.getUntrackedParameter<edm::InputTag>("metSrc" )),
  triggerResultsTag_(iConfig.getUntrackedParameter<edm::InputTag>("triggerResults")),
  pvSrc_(iConfig.getParameter<edm::InputTag>("pvSrc"))
{
   //now do what ever initialization is needed

}


OptFilter::~OptFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
OptFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  nEventsUnweighted_++;
 
  using namespace edm;
  using namespace std;
  
  // get electron collection
  edm::Handle<edm::View<pat::Electron> > electrons;
  iEvent.getByLabel(elecSrc_,electrons);
  
  // get muon collection
  edm::Handle<edm::View<pat::Muon> > muons;
  iEvent.getByLabel(muonSrc_,muons);
  
  // get jet collection
  edm::Handle<edm::View<pat::Jet> > jets;
  iEvent.getByLabel(jetSrc_,jets);
  
  // get met collection
  edm::Handle<edm::View<pat::MET> > mets;
  iEvent.getByLabel(metSrc_,mets);

  // get trigger results collection
  edm::Handle<edm::TriggerResults> triggerResults;
  iEvent.getByLabel(triggerResultsTag_,triggerResults);

  // get primary vertex collection
  edm::Handle<std::vector<reco::Vertex> > pvHandle;
  iEvent.getByLabel(pvSrc_, pvHandle);
  
  // get gen event info
  edm::Handle<GenEventInfoProduct> genEvtInfo;
  bool hasGenEventInfo = iEvent.getByLabel("generator", genEvtInfo);



  /////////////////////////////////////

  ////////////////////////////////////


  // WEIGHT ///////////////////////////////////////////////
  ////////////////////////////////////////////////////////
  float pthat = -1.0;
  float qScale = -1.0;
  if(hasGenEventInfo){ 
    pthat = ( genEvtInfo->hasBinningValues() ? (genEvtInfo->binningValues())[0] : 0.0);
    qScale = genEvtInfo->qScale();
  }
    
  double weight0_15 = 0.0;
  double weight15_30 = 14298.997620292488136328756809234619140625;
  double weight30_80 = 1091.53851475096462309011258184909820556640625;
  double weight80_170 =  28.207406584048921871499260305427014827728271484375;
  double weight170_300 = 0.79018790063592792005664477983373217284679412841796875;
  double weight300_470 =  0.036904883563299566151538755320871132425963878631591796875;
  double weight470_800 =  0.0036947781444590052125909185321006589219905436038970947265625;
  double weight800_1400 =  9.9209949383626135598068795928838881081901490688323974609375e-05;
  double weight1400_inf = 9.428943722732783707174484925939150770091146114282310009002685546875e-07;
  
  double weight = 1.0;
  bool isQCD = false;
  
  if(isQCD){
    if(pthat<15.0){
      weight = weight0_15;
    }
    else if(pthat>=15.0 && pthat <30.0){
      weight =  weight15_30;
    }
    else if(pthat>=30.0 && pthat <80.0){
      weight =  weight30_80;
    }
    else if(pthat>=80.0 && pthat < 170.0){
      weight =  weight80_170;
    }
    else if(pthat>=170.0 && pthat < 300.0){
      weight =  weight170_300;
    }
    else if(pthat>=300.0 && pthat < 470.0){
      weight =  weight300_470;
    }
    else if(pthat>=470.0 && pthat < 800.0){
      weight =  weight470_800;
    }
    else if(pthat>=800.0 && pthat < 1400.0){
      weight =  weight800_1400;
    }
    else if(pthat>=1400){
      weight =  weight1400_inf;
    }
    else{
      std::cout << "NO WEIGHT ERROR" << std::endl;
      weight = 0.0;
    }
  }
  nEvents_+=weight;
  //////////////////////////////////////////////////////////
  


  //Trigger Cut/////////////////////////////////////////////
  //////////////////////////////////////////////////////////
  bool pass0 = false;
  // edm::TriggerNames triggerNames;
  
  if(triggerResults.isValid() ){
    
    //BEN
    /* triggerNames.init(*triggerResults);
    hltnames_=triggerNames.triggerNames();
    for( int itrig=0; itrig<(int)hltnames_.size(); ++itrig){
      //std::cout << "Trigger bit:" << itrig <<", Name:" << hltnames_[itrig] << ", Fired:" << triggerResults->accept(itrig) << std::endl;
      if ( !triggerResults->wasrun(itrig)) std::cout<<"WARNING -- a trigger path was not run for this event!"<<std::endl;
      if ( triggerResults->error(itrig)) std::cout<<"WARNING -- a trigger path had an error for this event!"<<std::endl;
      if(hltnames_[itrig]=="HLT_HT200" && triggerResults->accept(itrig)) pass0=true;
      }*/

    //DON
    const edm::TriggerNames & triggerNames = iEvent.triggerNames(*triggerResults);
    unsigned int trigger_size = triggerResults->size();
    unsigned int trigger_position = triggerNames.triggerIndex("HLT_HT200");
    
    if (trigger_position < trigger_size){
      pass0= triggerResults->accept(trigger_position);
      //  if(pass0) cout << iEvent.id() << " passes Don's trigger cut" << endl;
    }
  }
  else {
    std::cout<<"ERROR -- triggerResults is invalid!"<<std::endl;
  }
  //end Trigger Cut////////////////////////////////////////  
  
  
  //Primary Vertex Cut//////////////////////////////////////
  //////////////////////////////////////////////////////////
  bool pass1 = false;
  if(pvHandle.isValid()){
    
    for(std::vector<reco::Vertex>::const_iterator pv=pvHandle->begin(); pv!=pvHandle->end(); ++pv){
      double pv_ndof = pv->ndof();
      double pv_z = pv->z();
      double pv_rho = pv->position().Rho();
      
      if(pv_ndof>=5.0 && fabs(pv_z)<=15.0 && pv_rho<=2.0 && !pv->isFake()){
	pass1=true;
	break;
      }
      
    }// end loop over primary vertices
  }// end isValid check
  else {
    std::cout<<"ERROR -- pvHandle is invalid!"<<std::endl;
  }
  //end Primary Vertex Cut//////////////////////////////////
  


  // jets //////////////////////////////////////////////
  //////////////////////////////////////////////////////
  int nLooseJets = 0;  
  int nbtag = 0;

  float myHT = 0; //scalar sum 
 
  float jetpt1=0, jetpt2=0, jetpt3=0;
  edm::View<pat::Jet>::const_iterator jet1=jets->end(), jet2=jets->end(),jet3=jets->end();
  
  for(edm::View<pat::Jet>::const_iterator jet=jets->begin(); jet!=jets->end(); ++jet){
  
    float jet_n90Hits = jet->jetID().n90Hits;
    //float jet_hitsInN90 = jet->jetID().hitsInN90;
    float jet_fHPD = jet->jetID().fHPD;
    float jet_emf = jet->emEnergyFraction();
    
    //good jet check
    if( jet_n90Hits >1.0 && jet_fHPD<0.98 && jet_emf>0.01 ){
      
      //loose jets
      if( jet->pt()>50.0 && fabs(jet->eta())<2.4 ){

	nLooseJets++;
	myHT += jet->pt();
	if(jet->bDiscriminator("simpleSecondaryVertexBJetTags")>=1.74) nbtag++;
	
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

      }//end loose jets
	
    }// end good jet check
  }// end jets loop
  
  bool pass2 = false;
  bool pass3 = false;
  bool pass4 = false;
  if(nLooseJets>2) pass2 = true;
  //if(jetpt1>100.0 && jetpt2>75.0 && jetpt3>50.0) pass3 = true;
  if(jetpt1>100.0) pass3 = true;
  if(myHT>400.0) pass4 = true;


  bool pass8a = false;
  bool pass8b = false;
  bool pass8c = false;
  if(nbtag>0) pass8a = true;
  if(nbtag>1) pass8b = true;
  if(nbtag>2) pass8c = true;


  // MET ////////////////////////////////////////////////
  /////////////////////////////////////////////////////////
  bool pass5 = false;
  for(edm::View<pat::MET>::const_iterator met=mets->begin(); met!=mets->end();++met){
    if (met->et()>150) pass5 = true;
  }
  
  // isolated muon stuff //////////////////////////////////
  /////////////////////////////////////////////////////////
  bool pass6 = false;
  bool isolatedMuon = false;
  for(edm::View<pat::Muon>::const_iterator muon=muons->begin(); muon!=muons->end(); ++muon){
    float m_iso = (muon->trackIso()+muon->ecalIso()+muon->hcalIso())/muon->pt();
    if( (muon->muonID("GlobalMuonPromptTight")) && (muon->numberOfValidHits()>=11.0) && (muon->pt()>10) && (fabs(muon->eta())<2.4)  && (fabs(muon->dB()) < 0.2) && (m_iso<0.1) ) isolatedMuon = true;
  }//end muon loop
  if(!isolatedMuon) pass6 = true;
  // end isolated muon stuff ///////////////////////////////

  // isolated electron stuff ///////////////////////////////
  //////////////////////////////////////////////////////////
  bool pass7 = false;
  bool isolatedElectron = false;
  for(edm::View<pat::Electron>::const_iterator electron=electrons->begin(); electron!=electrons->end(); ++electron){
    float e_iso =  (electron->trackIso()+electron->ecalIso()+electron->hcalIso())/electron->et();
    if( (electron->et()>15) && (fabs(electron->eta())<2.4) && (fabs(electron->dB())<0.2) && (e_iso<0.1) && (electron->electronID("eidLoose")) ) isolatedElectron = true;
  }//end electron loop
  if(!isolatedElectron) pass7 = true;
  // end isolated electron stuff /////////////////////////

 

  bool saveEvent = false;

  if(pass0){
    nPass0_+=weight;
    if(pass1){
      nPass1_+=weight;
      if(pass2){
	nPass2_+=weight;
	if(pass3){
	  nPass3_+=weight;
	  if(pass4){
	    nPass4_+=weight;
	    if(pass5){
	      nPass5_+=weight;
	      if(pass6){
		nPass6_+=weight;
		if(pass7){
		  nPass7_+=weight;
		  if(pass8a){
		    nPass8a_+=weight;
		    if(pass8b){
		      nPass8b_+=weight;
		      if(pass8c){
			nPass8c_+=weight;
		      }
		    }
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }

  
  if(saveEvent){
    return true;
  }
  else{
    return false;
  }


}

// ------------ method called once each job just before starting event loop  ------------
void 
OptFilter::beginJob()
{
  nEventsUnweighted_ = 0;
  nEvents_ = 0.0;
  nPass0_ = 0.0;
  nPass1_ = 0.0;
  nPass2_ = 0.0;
  nPass3_ = 0.0;
  nPass4_ = 0.0;
  nPass5_ = 0.0;
  nPass6_ = 0.0;
  nPass7_ = 0.0;
  nPass8a_ = 0.0;
  nPass8b_ = 0.0;
  nPass8c_ = 0.0;
}

// ------------ method called once each job just after ending the event loop  ------------
void 
OptFilter::endJob() {

  std::cout << "\nCU SUSY b-tag Optmized Cutflow:" << std::endl;
  std::cout << "nEvents (unweighted): " << nEventsUnweighted_ << std::endl;
  std::cout << "nEvents: " << nEvents_ << std::endl;
  std::cout << "nPass0: " << nPass0_ << " +- " << sqrt(nPass0_) << ", " << (float) nPass0_/nEvents_*100.0 << " %" << std::endl;
  std::cout << "nPass1: " << nPass1_ << " +- " << sqrt(nPass1_) << ", " << (float) nPass1_/nPass0_*100.0 << " %" << std::endl;
  std::cout << "nPass2: " << nPass2_ << " +- " << sqrt(nPass2_) << ", " << (float) nPass2_/nPass1_*100.0 << " %" << std::endl;
  std::cout << "nPass3: " << nPass3_ << " +- " << sqrt(nPass3_) << ", " << (float) nPass3_/nPass2_*100.0 << " %" << std::endl;
  std::cout << "nPass4: " << nPass4_ << " +- " << sqrt(nPass4_) << ", " << (float) nPass4_/nPass3_*100.0 << " %" << std::endl;
  std::cout << "nPass5: " << nPass5_ << " +- " << sqrt(nPass5_) << ", " << (float) nPass5_/nPass4_*100.0 << " %" << std::endl;
  std::cout << "nPass6: " << nPass6_ << " +- " << sqrt(nPass6_) << ", " << (float) nPass6_/nPass5_*100.0 << " %" << std::endl;
  std::cout << "nPass7: " << nPass7_ << " +- " << sqrt(nPass7_) << ", " << (float) nPass7_/nPass6_*100.0 << " %" << std::endl;
  std::cout << "nPass8a: " << nPass8a_ << " +- " << sqrt(nPass8a_) << ", " << (float) nPass8a_/nPass7_*100.0 << " %" << std::endl;
  std::cout << "nPass8b: " << nPass8b_ << " +- " << sqrt(nPass8b_) << ", " << (float) nPass8b_/nPass7_*100.0 << " %" << std::endl;
  std::cout << "nPass8c: " << nPass8c_ << " +- " << sqrt(nPass8c_) << ", " << (float) nPass8c_/nPass7_*100.0 << " %" << std::endl;

}

//define this as a plug-in
DEFINE_FWK_MODULE(OptFilter);
