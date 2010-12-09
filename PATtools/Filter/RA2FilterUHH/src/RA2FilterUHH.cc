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

#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"

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
  
  unsigned int nEventsUnweighted_;
  double nEvents_;
  double nEvPass1_;
  double nEvPass2_;
  double nEvPass3_;
  double nEvPass4_;
  double nEvPass5_;
  double nEvPass6_;
  double nEvPass7_;
  
  double eEvPass1_;
  double eEvPass2_;
  double eEvPass3_;
  double eEvPass4_;
  double eEvPass5_;
  double eEvPass6_;
  double eEvPass7_;

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

  PFJetIDSelectionFunctor PFjetIdLoose_;

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
  //bsSrc_(iConfig.getParameter<edm::InputTag>("bsSrc")),
  doABCD_(iConfig.getUntrackedParameter<double>("doABCD")),
  PFjetIdLoose_(iConfig.getParameter<edm::ParameterSet>("pfjetIdLoose") )
  //  PFjetIdLoose_( PFJetIDSelectionFunctor::FIRSTDATA, PFJetIDSelectionFunctor::LOOSE)
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
  nEventsUnweighted_++;
 
  edm::Handle<double> myweight1;
  iEvent.getByLabel(edm::InputTag("weightProducer","weight"), myweight1);
  //std::cout << *myweight1 << std::endl;
  double myweight = *myweight1;
  nEvents_+=myweight;

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

  /*
  //reco::BeamSpot beamSpot;
  // get beam spot collection
  edm::Handle<reco::BeamSpot> bsHandle;
  iEvent.getByLabel(bsSrc_, bsHandle);
  */
  
  edm::Handle<GenEventInfoProduct> genEvtInfo;
  bool hasGenEventInfo = iEvent.getByLabel("generator", genEvtInfo);
  
  float pthat;
  if(hasGenEventInfo){ pthat = ( genEvtInfo->hasBinningValues() ? (genEvtInfo->binningValues())[0] : 0.0);}
  else {pthat = -1.0;}
  
  double pi=4*atan(1.0);
 
 
  //Trigger Cut/////////////////////////////////////////////
  //////////////////////////////////////////////////////////
 
  bool pass1 = false;
  pass1=true;
  /*
  if(triggerResults.isValid() ){
    
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
  */
  
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
  pat::strbitset retpf = PFjetIdLoose_.getBitTemplate();
  bool badJet = false;
  ///////////loop for jetID event veto
  for(edm::View<pat::Jet>::const_iterator jet=jets->begin(); jet!=jets->end(); ++jet){
    bool badJet = false;
    
    if( (fabs(jet->eta())>5.0) || (jet->pt()<30.) ) continue; 
  
    if( jet->isCaloJet() ){
      int jet_n90Hits = jet->jetID().n90Hits;
      double jet_fHPD = jet->jetID().fHPD;
      float jet_emf = jet->emEnergyFraction();
      bool etaEMFcheck = true;
      if( (fabs(jet->eta())<2.6) && (jet_emf<=0.01)) etaEMFcheck=false; 
      if( !((jet_n90Hits>1) && (jet_fHPD<0.98) && etaEMFcheck) ) badJet=true;
    }
    else if( jet->isPFJet() ){
      if( !PFjetIdLoose_(*jet,retpf) ) badJet=true;
    }
    if(badJet==true) break;   
  }//end jet loop
  
  
  int nTightJets = 0;
  int jetCounter = 0;
  double jetphi[3] = {-5.,-5.,-5.};
  double myHT = 0; //scalar sum 
  double myMHTx=0, myMHTy=0;
  if(!badJet){
    for(edm::View<pat::Jet>::const_iterator jet=jets->begin(); jet!=jets->end(); ++jet){
      jetCounter++;
      
      if( (fabs(jet->eta())>5.0) || (jet->pt()<30.) ) continue;
      
      //assuming jets are sorted by pt
      if(jetCounter<=3){
	if( (jet->pt()>50.0) && (fabs(jet->eta())<2.5)){
	  nTightJets++;
	  jetphi[jetCounter-1] = jet->phi();
	}
      }
      
      // add jet to MHT estimate
      myMHTx+=-jet->px();
      myMHTy+=-jet->py();
      
      //add jet to HT estimate
      myHT += jet->pt();
      
    }// end jets loop
  }//end badJet check
  
  bool pass3 = false;
  if(nTightJets==3) pass3 = true;
  
  bool pass5 = false;
  if(myHT>300.) pass5 = true;
  
  bool pass6 = false;
  double myMHT = sqrt(myMHTx*myMHTx+myMHTy*myMHTy);
  if(myMHT>150.) pass6 = true;
  
  //RA2 jet-phi stuff
  bool pass7 = false;
  if(nTightJets==3){ 
    double jet1phi = jetphi[0];
    double jet2phi = jetphi[1];
    double jet3phi = jetphi[2];
    assert(jet1phi>-pi);
    assert(jet2phi>-pi);
    assert(jet3phi>-pi);

    double myMHTphi = atan2(myMHTy,myMHTx);
    
    double jet1dPhi = acos(cos(jet1phi-myMHTphi));
    double jet2dPhi = acos(cos(jet2phi-myMHTphi));
    double jet3dPhi = acos(cos(jet3phi-myMHTphi));
  
    //UHH minDPhi
    double dPhiMin;
    if(jet1dPhi <= jet2dPhi && jet1dPhi <= jet3dPhi){ dPhiMin = jet1dPhi; }
    else if(jet2dPhi <= jet1dPhi && jet2dPhi <= jet3dPhi){ dPhiMin = jet2dPhi; }
    else if(jet3dPhi <= jet1dPhi && jet3dPhi <= jet2dPhi){ dPhiMin = jet3dPhi; }
    else {assert(0);}
    if(dPhiMin>0.3) pass7 = true;
  }
  //end Jet Stuff//////////////////////////////////////////  

  // beamspot/vertex stuff for lepton veto ////////////////
  /////////////////////////////////////////////////////////

  reco::Vertex myBeamSpot;
  edm::Handle <edm::View<reco::Vertex> >vertices;
  iEvent.getByLabel(pvSrc_, vertices); 
  if(!vertices->empty() && !vertices->front().isFake() ) myBeamSpot = vertices->front();

  const math::XYZPointD & myPosition = myBeamSpot.position();

  /////////////////////////////////////////////////////////


   // isolated muon stuff //////////////////////////////////
  /////////////////////////////////////////////////////////
  bool isolatedMuon = false;
  int nIsolatedMuons = 0;
  double currIso = 0;

  if ( !muons.isValid() ) {
    std::cout<<"ERROR -- muon handle is invalid!"<<std::endl;
  }
  
  for(edm::View<pat::Muon>::const_iterator im=muons->begin(); im!=muons->end(); ++im){
   
    //common for PF and Reco:
    if( im->isGood("GlobalMuonPromptTight") < 1 ) continue;
    if (im->pt() < 10.) continue;
    if (fabs(im->eta()) > 2.4) continue;
    if (fabs(im->innerTrack()->dxy(myPosition)) > 0.02) continue;
    if (fabs(im->vz() - myPosition.z()) > 1) continue;
    if (im->innerTrack()->numberOfValidHits() < 11) continue;

    //only PF
    if(im->pfCandidateRef().isNonnull()){
      for(int i =4; i<7; ++i){
	const pat::IsoDeposit* Idep = im->isoDeposit((pat::IsolationKeys)(i));
	if(Idep)
	  currIso += Idep->depositWithin(0.3)/im->pt();
	else{
	  std::cout<<"Isolation Key: "<<i<<" not found"<<std::endl;
	  continue;
	}
      }
      if(currIso > 0.2) continue;
    }// end pf muon
    else{
      std::cout<<"muon was not pf muon" << std::endl;
      assert(1);
    }
    
    nIsolatedMuons++;
  }//end muon loop
  if(nIsolatedMuons>=1) isolatedMuon =true;
  
  // end isolated muon stuff  ///////////////////////////////
  
  
  // isolated electron stuff ///////////////////////////////
  //////////////////////////////////////////////////////////
  bool isolatedElectron = false;
  int nIsolatedElectrons=0;

  if ( !electrons.isValid() ) {
    std::cout<<"ERROR -- electrons handle is invalid!"<<std::endl;
  }
  
  for(edm::View<pat::Electron>::const_iterator ie=electrons->begin(); ie!=electrons->end(); ++ie){
    
    //common for PF and Reco:
    if (ie->electronID("eidLoose") < 1) continue;
    if (ie->pt() < 10.) continue;
    if (fabs(ie->eta()) > 2.5) continue;
    if (fabs(ie->superCluster()->eta()) > 1.4442 && fabs(ie->superCluster()->eta()) < 1.566) continue;
    if (fabs(ie->gsfTrack()->dxy(myPosition)) > 0.02) continue;
    if (abs(ie->vz() - myPosition.z()) > 1) continue;
    
    //PF only:
    if(ie->pfCandidateRef().isNonnull() ){
      for(int i =4; i<7; ++i){
	const pat::IsoDeposit* Idep = ie->isoDeposit((pat::IsolationKeys)(i));   //only 4,5 and 6 filled
	if(Idep){                                                                 //charged,neutral and Photon candidates
	  currIso += Idep->depositWithin(0.3)/ie->pt();
	}
	else{
	  std::cout<<"Isolation Key: "<<i<<" not found"<<std::endl;
	  continue;
	}
      }
      if(currIso > 0.2) continue;
    }
    else {
      std::cout<<"muon was not pf muon" << std::endl;
      assert(1);
    }
    nIsolatedElectrons++;
  }//end electron loop
  if(nIsolatedElectrons>=1) isolatedElectron=true; 
  
  // end isolated electron stuff /////////////////////////

  // isolated lepton veto ////////////////////////////////
  ////////////////////////////////////////////////////////
  bool pass4 = false;
  if( !isolatedMuon && !isolatedElectron) pass4=true;
  ////////////////////////////////////////////////////////

  

  ///////////////////////////
  // C    T        L    W  //
  //   U         F   O     //  
  ///////////////////////////
  
  bool passPre = (pass1 && pass2 && pass3);
  if(passPre) eEvPass1_+=myweight;
  if(passPre && pass4) eEvPass4_+=myweight;
  if(passPre && pass5) eEvPass5_+=myweight;
  if(passPre && pass6) eEvPass6_+=myweight;
  if(passPre && pass7) eEvPass7_+=myweight;

  if(pass1){
    nEvPass1_+=myweight;
    if(pass2){
      nEvPass2_+=myweight;
      if(pass3){
	nEvPass3_+=myweight;
	if(pass4){
	  nEvPass4_+=myweight;
	  if(pass5){
	    nEvPass5_+=myweight;
	    if(pass6){
	      nEvPass6_+=myweight;
	      if(pass7){ 
		nEvPass7_+=myweight;
	      }//end pass7
	    }//end pass6
	  }//end pass5
	}//end pass4
      }//end pass3
    }//end pass2
  }//end pass1
  
  if(pass1){
    if(pass2){
      if(pass3){
	if(pass4){
	  if(pass5){
	    if(pass6 || (doABCD_>0.5) ){ // MHT cut      
              if(pass7 || (doABCD_>0.5) ){ // angular cuts
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
  nEventsUnweighted_ = 0;
  nEvents_ = 0.;
  nEvPass1_ = 0.;
  nEvPass2_ = 0.;
  nEvPass3_ = 0.;
  nEvPass4_ = 0.;
  nEvPass5_ = 0.;
  nEvPass6_ = 0.;
  nEvPass7_ = 0.;

  eEvPass1_ = 0.;
  eEvPass2_ = 0.;
  eEvPass3_ = 0.;
  eEvPass4_ = 0.;
  eEvPass5_ = 0.;
  eEvPass6_ = 0.;
  eEvPass7_ = 0.;
}

// ------------ method called once each job just after ending the event loop  ------------
void 
RA2FilterUHH::endJob() {


  //exclusive
  std::cout << std::endl;
  std::cout << "eEvPass1: " << eEvPass1_ << " +- " << sqrt(eEvPass1_) << std::endl;
  std::cout << "eEvPass4: " << eEvPass4_ << " +- " << sqrt(eEvPass4_) << std::endl;
  std::cout << "eEvPass5: " << eEvPass5_ << " +- " << sqrt(eEvPass5_) << std::endl;
  std::cout << "eEvPass6: " << eEvPass6_ << " +- " << sqrt(eEvPass6_) << std::endl;
  std::cout << "eEvPass7: " << eEvPass7_ << " +- " << sqrt(eEvPass7_) << std::endl;
  std::cout << std::endl;

  //cutflow
  std::cout << "2010 UHH RA2 CUTFLOW:" << std::endl;
  std::cout << "nEventsUnweighted: " << nEventsUnweighted_ << " +- " << sqrt(nEventsUnweighted_) <<std::endl;
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
