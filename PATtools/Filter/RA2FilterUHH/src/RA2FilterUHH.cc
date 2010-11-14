// RA2 Filter for SUSY b-tag - UHH version
// Ben Kreis

// system include files
#include <memory>
#include <cmath>
#include "assert.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Utilities/interface/InputTag.h"


//#include "FWCore/ServiceRegistry/interface/Service.h"
//#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//#include "FWCore/Framework/interface/TriggerNames.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/VertexReco/interface/Vertex.h" //code compiles without this
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

//////////////////////////////////////////////////////////////////////
class RA2FilterUHH : public edm::EDFilter {
   public:
      explicit RA2FilterUHH(const edm::ParameterSet&);
      ~RA2FilterUHH();

private:
  virtual void beginJob() ;
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
      
  // ----------member data ---------------------------
  
  unsigned int nEvents_;
  unsigned int nEvPass1_;
  unsigned int nEvPass2_;
  unsigned int nEvPass3_;
  unsigned int nEvPass4_;
  unsigned int nEvPass5_;
  unsigned int nEvPass6_;
  unsigned int nEvPass7_;
  
  std::vector<std::string> hltnames_;

  // input tags  
  edm::InputTag elecSrc_;
  edm::InputTag muonSrc_;
  edm::InputTag jetSrc_;
  edm::InputTag metSrc_;
  edm::InputTag triggerResultsTag_;
  edm::InputTag pvSrc_;
  edm::InputTag bsSrc_;
  double doABCD_;

};



// constants, enums and typedefs
// static data member definitions
// constructors and destructor
RA2FilterUHH::RA2FilterUHH(const edm::ParameterSet& iConfig):

  hltnames_(0),
  
  elecSrc_(iConfig.getUntrackedParameter<edm::InputTag>("electronSrc")),
  muonSrc_(iConfig.getUntrackedParameter<edm::InputTag>("muonSrc")),
  jetSrc_(iConfig.getUntrackedParameter<edm::InputTag>("jetSrc" )),
  metSrc_(iConfig.getUntrackedParameter<edm::InputTag>("metSrc" )),
  triggerResultsTag_(iConfig.getUntrackedParameter<edm::InputTag>("triggerResults")),
  pvSrc_(iConfig.getParameter<edm::InputTag>("pvSrc")),
  bsSrc_(iConfig.getParameter<edm::InputTag>("bsSrc")),
  doABCD_(iConfig.getUntrackedParameter<double>("doABCD"))

{
   //now do what ever initialization is needed
  
}


RA2FilterUHH::~RA2FilterUHH()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

////////////////////////////////////////////////////////////////////////
////////////////////// member functions ////////////////////////////////
////////////////////////////////////////////////////////////////////////

// ------------ method called on each new Event  ------------
bool
RA2FilterUHH::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // using namespace edm;
  nEvents_++;
 
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
  
  //reco::BeamSpot beamSpot;
  // get beam spot collection
  edm::Handle<reco::BeamSpot> bsHandle;
  iEvent.getByLabel(bsSrc_, bsHandle);

  edm::Handle<GenEventInfoProduct> genEvtInfo;
  bool hasGenEventInfo = iEvent.getByLabel("generator", genEvtInfo);
  
  float pthat;
  if(hasGenEventInfo){ pthat = ( genEvtInfo->hasBinningValues() ? (genEvtInfo->binningValues())[0] : 0.0);}
  else {pthat = -1.0;}
  
  
  
  //Trigger Cut/////////////////////////////////////////////
  //////////////////////////////////////////////////////////
  bool pass1 = false;
  //BEN edm::TriggerNames triggerNames;
  
  if(triggerResults.isValid() ){
    
    //Ben
    /*triggerNames.init(*triggerResults);
    hltnames_=triggerNames.triggerNames();
    for( int itrig=0; itrig<(int)hltnames_.size(); ++itrig){
      //std::cout << "Trigger bit:" << itrig <<", Name:" << hltnames_[itrig] << ", Fired:" << triggerResults->accept(itrig) << std::endl;
      if ( !triggerResults->wasrun(itrig)) std::cout<<"WARNING -- a trigger path was not run for this event!"<<std::endl;
      if ( triggerResults->error(itrig)) std::cout<<"WARNING -- a trigger path had an error for this event!"<<std::endl;
      if(hltnames_[itrig]=="HLT_HT200" && triggerResults->accept(itrig)) pass1=1;
      }*/

    //DON
    const edm::TriggerNames & triggerNames = iEvent.triggerNames(*triggerResults);
    unsigned int trigger_size = triggerResults->size();
    unsigned int trigger_position = triggerNames.triggerIndex("HLT_HT200");
    
    if (trigger_position < trigger_size){
      pass1= triggerResults->accept(trigger_position);
      //  if(pass1) cout << iEvent.id() << " passes Don's trigger cut" << endl;
    }
  }
  else {
    std::cout<<"ERROR -- triggerResults is invalid!"<<std::endl;
  }
  //end Trigger Cut////////////////////////////////////////  
  
  
  //Primary Vertex Cut//////////////////////////////////////
  //////////////////////////////////////////////////////////
  bool pass2 = false;
  if(pvHandle.isValid()){
    
    for(std::vector<reco::Vertex>::const_iterator pv=pvHandle->begin(); pv!=pvHandle->end(); ++pv){
      double pv_ndof = pv->ndof();
      double pv_z = pv->z();
      double pv_rho = pv->position().Rho();
      
      if(pv_ndof>4.0 && fabs(pv_z)<=15.0 && pv_rho<=2.0){
	pass2=1;
	break;
      }
      
    }// end loop over primary vertices
  }// end isValid check
  else {
    std::cout<<"ERROR -- pvHandle is invalid!"<<std::endl;
  }
  //end Primary Vertex Cut//////////////////////////////////
  
  
  //Jet Stuff///////////////////////////////////////////////
  //////////////////////////////////////////////////////////
  int nTightJets = 0;
  int nLooseJets = 0;
  
  float myHT = 0; //scalar sum 
  float myMHTx=0, myMHTy=0;
  
  float jetpt1=0, jetpt2=0, jetpt3=0;
  edm::View<pat::Jet>::const_iterator jet1=jets->end(), jet2=jets->end(),jet3=jets->end();
  
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
	
	//add jet to HT estimate
	myHT += jet->pt();
	
	//tight pT and eta cut
	if( jet->pt()>50.0 && fabs(jet->eta())<2.5){
	  
	  nTightJets++;
	  
	}// end tight pT and eta cut
      }// end loose pT and eta cut
    }// end good jet cut
  }// end jets loop
  
  bool pass3 = false;
  if(nTightJets>=3) pass3 = true;
  
  bool pass4 = false;
  if(myHT>300) pass4 = true;

  bool pass5 = false;
  float myMHT = sqrt(myMHTx*myMHTx+myMHTy*myMHTy);
  if(myMHT>150) pass5 = true;
  
  //RA2 jet-phi stuff
  bool pass7 = false;
  if(nLooseJets>=3){ 
    float jet1phi = jet1->phi();
    float jet2phi = jet2->phi();
    float jet3phi = jet3->phi();
    float myMHTphi = atan2(myMHTy,myMHTx);
    
    float jet1dPhi = acos(cos(jet1phi-myMHTphi));
    float jet2dPhi = acos(cos(jet2phi-myMHTphi));
    float jet3dPhi = acos(cos(jet3phi-myMHTphi));
  
    if(jet1dPhi>0.3 && jet2dPhi>0.5  && jet3dPhi>0.3) pass7 = true;
  }
  //end Jet Stuff//////////////////////////////////////////  

  // beam spot stuff //////////////////////////////////////
  /////////////////////////////////////////////////////////
  reco::BeamSpot beamSpot;
 
  if (bsHandle.isValid()){
    beamSpot = *bsHandle;
  }
  else{
    std::cout<<"ERROR - bsHandle is invalid!" <<std::endl;
  }
  math::XYZPoint beamSpotPoint(beamSpot.x0(), beamSpot.y0(), beamSpot.z0());

  /////////////////////////////////////////////////////////


  // isolated muon stuff //////////////////////////////////
  /////////////////////////////////////////////////////////
  bool isolatedMuon = false;
  for(edm::View<pat::Muon>::const_iterator muon=muons->begin(); muon!=muons->end(); ++muon){
    //note: can use muon->isGood("GlobalMuonPromptTight") or  muon::isGoodMuon(*muon, muon::GlobalMuonPromptTight) or muon->muonID("GlobalMuonPromptTight")
    //note: muon->track()->pt() can be different from muon->pt()
    if(muon->track().isAvailable()){
      float m_d0bs =  -1.0 * (muon->track()->dxy(beamSpotPoint));
      float m_nHits = muon->track()->numberOfValidHits();
      float m_iso = (muon->trackIso()+muon->ecalIso()+muon->hcalIso())/muon->pt();
      if( (muon->muonID("GlobalMuonPromptTight")) && (fabs(m_d0bs) < 0.2) && (m_nHits>=11.0)  && (muon->pt()>10) && (m_iso<0.1) ) isolatedMuon = true;
    }//end track available check
  }//end muon loop
  
  // end isolated muon stuff  ///////////////////////////////
  
  
  // isolated electron stuff ///////////////////////////////
  //////////////////////////////////////////////////////////
  bool isolatedElectron = false;
  for(edm::View<pat::Electron>::const_iterator electron=electrons->begin(); electron!=electrons->end(); ++electron){
    //note: electron->trackIso() same as electron->dr04TkSumPt() 
    if(electron->gsfTrack().isAvailable()){
      float e_d0bs =  -1.0 * (electron->gsfTrack()->dxy(beamSpotPoint));
      float e_iso =  (electron->dr03TkSumPt()+electron->ecalIso()+electron->hcalIso())/electron->pt();
      if( (electron->electronID("eidLoose")) && (fabs(e_d0bs)<0.2) && (e_iso<0.5) && (electron->pt()>15)  ) isolatedElectron = true;
    }//end gsfTrack available check
  }//end electron loop

  // end isolated electron stuff /////////////////////////

  // isolated lepton veto ////////////////////////////////
  ////////////////////////////////////////////////////////
  bool pass6 = false;
  if( !isolatedMuon && !isolatedElectron) pass6=true;
  ////////////////////////////////////////////////////////

  

  ///////////////////////////
  // C    T        L    W  //
  //   U         F   O     //  
  ///////////////////////////
  if(pass1){
    nEvPass1_++;
    if(pass2){
      nEvPass2_++;
      if(pass3){
	nEvPass3_++;
	if(pass4){
	  nEvPass4_++;
	  if(pass5 || (doABCD_>0.5) ){ // MHT cut
	    nEvPass5_++;
	    if(pass6){
	      nEvPass6_++;
	      if(pass7 || (doABCD_>0.5) ){ // angular cuts
		nEvPass7_++;
		return true;
	      }//end pass7
	    }//end pass6
	  }//end pass5
	}//end pass4
      }//end pass3
    }//end pass2
  }//end pass1
  
  return false;
  
}

// ------------ method called once each job just before starting event loop  ------------
void 
RA2FilterUHH::beginJob()
{
  nEvents_ = 0;
  nEvPass1_ = 0;
  nEvPass2_ = 0;
  nEvPass3_ = 0;
  nEvPass4_ = 0;
  nEvPass5_ = 0;
  nEvPass6_ = 0;
  nEvPass7_ = 0;
}

// ------------ method called once each job just after ending the event loop  ------------
void 
RA2FilterUHH::endJob() {
  std::cout << "\n2010 RA2CUT FLOW:" << std::endl;
  std::cout << "nEvPass0: " << nEvents_ << " +- " << sqrt(nEvents_) <<std::endl;
  std::cout << "nEvPass1: " << nEvPass1_ << " +- " << sqrt(nEvPass1_) << ", " << (float) nEvPass1_/nEvents_*100.0 << " %" << std::endl;
  std::cout << "nEvPass2: " << nEvPass2_ << " +- " << sqrt(nEvPass2_) << ", " << (float) nEvPass2_/nEvPass1_*100.0 << " %" << std::endl;
  std::cout << "nEvPass3: " << nEvPass3_ << " +- " << sqrt(nEvPass3_) << ", " << (float) nEvPass3_/nEvPass2_*100.0 << " %" << std::endl;
  std::cout << "nEvPass4: " << nEvPass4_ << " +- " << sqrt(nEvPass4_) << ", " << (float) nEvPass4_/nEvPass3_*100.0 << " %" << std::endl;
  std::cout << "nEvPass5: " << nEvPass5_ << " +- " << sqrt(nEvPass5_) << ", " << (float) nEvPass5_/nEvPass4_*100.0 << " %" << std::endl;
  std::cout << "nEvPass6: " << nEvPass6_ << " +- " << sqrt(nEvPass6_) << ", " << (float) nEvPass6_/nEvPass5_*100.0 << " %" << std::endl;
  std::cout << "nEvPass7: " << nEvPass7_ << " +- " << sqrt(nEvPass7_) << ", " << (float) nEvPass7_/nEvPass6_*100.0 << " %" << std::endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(RA2FilterUHH);
