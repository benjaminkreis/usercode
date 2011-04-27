// RA2 Filter for SUSY b-tag
// Ben Kreis

// system include files
#include <memory>
#include <cmath>
#include "assert.h"

//root
#include "TH1D.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Common/interface/TriggerNames.h"
//#include "FWCore/Framework/interface/TriggerNames.h"
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
class DonRA2 : public edm::EDFilter {
   public:
      explicit DonRA2(const edm::ParameterSet&);
      ~DonRA2();

private:
  virtual void beginJob() ;
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
      
  // ----------member data ---------------------------
  
  TH1D* hist1_;

  int nEventsUnweighted_;
  double nEvents_;
  double nEvPass0_;
  double nEvPass1_;
  double nEvPass2_;
  double nEvPass3_;
  double nEvPass4_;
  double nEvPass5_;
  double nEvPass6_;
  double nEvPass7_;
  double nEvPass7p5_;
  double nEvPass8a_;
  double nEvPass8b_;
  double nEvPass8c_;

  std::vector<std::string> hltnames_;

  // input tags  
  edm::InputTag elecSrc_;
  edm::InputTag muonSrc_;
  edm::InputTag jetSrc_;
  edm::InputTag metSrc_;
  edm::InputTag triggerResultsTag_;
  edm::InputTag pvSrc_;
  edm::InputTag bsSrc_;
  double useWeight_;

};



// constants, enums and typedefs
// static data member definitions
// constructors and destructor
DonRA2::DonRA2(const edm::ParameterSet& iConfig):

  hltnames_(0),
  
  elecSrc_(iConfig.getUntrackedParameter<edm::InputTag>("electronSrc")),
  muonSrc_(iConfig.getUntrackedParameter<edm::InputTag>("muonSrc")),
  jetSrc_(iConfig.getUntrackedParameter<edm::InputTag>("jetSrc" )),
  metSrc_(iConfig.getUntrackedParameter<edm::InputTag>("metSrc" )),
  triggerResultsTag_(iConfig.getUntrackedParameter<edm::InputTag>("triggerResults")),
  pvSrc_(iConfig.getParameter<edm::InputTag>("pvSrc")),
  bsSrc_(iConfig.getParameter<edm::InputTag>("bsSrc")),
  useWeight_(iConfig.getUntrackedParameter<double>("useWeight"))
  

{
   //now do what ever initialization is needed

}


DonRA2::~DonRA2()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

////////////////////////////////////////////////////////////////////////
////////////////////// member functions ////////////////////////////////
////////////////////////////////////////////////////////////////////////

// ------------ method called on each new Event  ------------
bool
DonRA2::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // using namespace edm;
  nEventsUnweighted_++;
 
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
  
  // get beam spot collection
  edm::Handle<reco::BeamSpot> bsHandle;
  iEvent.getByLabel(bsSrc_, bsHandle);

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
  //bool isQCD = false;
  //bool isQCD = true;
  
  if(useWeight_>0.0){
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
  
  
  //Jet Stuff///////////////////////////////////////////////
  //////////////////////////////////////////////////////////
  int nTightJets = 0;
  int nLooseJets = 0;
  
  int nbtag = 0;

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
	
	//tight pT and eta cut
	if( jet->pt()>50.0 && fabs(jet->eta())<2.5){

	  
	  nTightJets++;
	  
	  //add jet to HT estimate
	  myHT += jet->pt();
	  
	  //btagging (not in ra2)
	  if(jet->bDiscriminator("simpleSecondaryVertexBJetTags")>=1.74) nbtag++;
	

	}// end loose pT and eta cut
      }// end loose pT and eta cut
    }// end good jet cut
  }// end jets loop
  
  bool pass2 = false;
  if(nTightJets>=3) pass2 = true;
  
  bool pass3 = true; //no cut in Don RA2

  bool pass4 = false;
  if(myHT>300) pass4 = true;

  bool pass5 = false;
  float myMHT = sqrt(myMHTx*myMHTx+myMHTy*myMHTy);
  if(myMHT>150) pass5 = true;
 

  //RA2 jet-phi stuff
  bool pass7p5 = false;
  if(nLooseJets>=3){ 
    float jet1phi = jet1->phi();
    float jet2phi = jet2->phi();
    float jet3phi = jet3->phi();
    float myMHTphi = atan2(myMHTy,myMHTx);
    
    float jet1dPhi = acos(cos(jet1phi-myMHTphi));
    float jet2dPhi = acos(cos(jet2phi-myMHTphi));
    float jet3dPhi = acos(cos(jet3phi-myMHTphi));
  
    if(jet1dPhi>0.3 && jet2dPhi>0.5  && jet3dPhi>0.3) pass7p5 = true;
  }

  //btag stuff not in ra2
  bool pass8a = false;
  bool pass8b = false;
  bool pass8c = false;
  if(nbtag>0) pass8a = true;
  if(nbtag>1) pass8b = true;
  if(nbtag>2) pass8c = true;


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


  

  ///////////////////////////
  // C    T        L    W  //
  //   U         F   O     //  
  ///////////////////////////
  
  hist1_->Fill(0.0,weight);
  if(pass0){
    nEvPass0_+=weight;
    hist1_->Fill(1., weight);
    if(pass1){
      nEvPass1_+=weight;
      hist1_->Fill(2., weight);
      if(pass2){
	nEvPass2_+=weight;
	hist1_->Fill(3., weight);
	if(pass3){
	  nEvPass3_+=weight;
	  hist1_->Fill(4., weight);
	  if(pass4){
	    nEvPass4_+=weight;
	    hist1_->Fill(5., weight);
	    if(pass5){ // MHT cut
	      nEvPass5_+=weight;
	      hist1_->Fill(6., weight);
	      if(pass6){
		nEvPass6_+=weight;
		hist1_->Fill(7., weight);
		if(pass7){ // angular cuts
		  nEvPass7_+=weight;
		  hist1_->Fill(8., weight);
		  if(pass7p5){
		    nEvPass7p5_+=weight;
		    hist1_->Fill(9., weight);
		    if(pass8a){
		      nEvPass8a_+=weight;
		      hist1_->Fill(10., weight);
		      if(pass8b){
			nEvPass8b_+=weight;
			hist1_->Fill(11., weight);
			if(pass8c){
			  nEvPass8c_+=weight;
			  hist1_->Fill(12., weight);
			}//end pass8c
		      }//end pass8b
		    }//end pass8a
		  }//end pass7p5
		}//end pass7
	      }//end pass6
	    }//end pass5
	  }//end pass4
	}//end pass3
      }//end pass2
    }//end pass1
  }//end pass0
  return false;
  
}

// ------------ method called once each job just before starting event loop  ------------
void 
DonRA2::beginJob()
{
  nEventsUnweighted_ = 0;
  nEvents_ = 0;
  nEvPass0_ = 0;
  nEvPass1_ = 0;
  nEvPass2_ = 0;
  nEvPass3_ = 0;
  nEvPass4_ = 0;
  nEvPass5_ = 0;
  nEvPass6_ = 0;
  nEvPass7_ = 0;
  nEvPass7p5_ = 0;
  nEvPass8a_ = 0;
  nEvPass8b_ = 0;
  nEvPass8c_ = 0;

  //edm::Service<TFileService> fs;
  // hist1_ = fs->make<TH1D>("hist1", "cutflow hist",13,-0.5,12.5);
  hist1_ = new TH1D("hist1", "cutflow hist",13,-0.5,12.5);
  hist1_->Sumw2();
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DonRA2::endJob() {
  
  std::cout << "\n2010 DonRA2CUT FLOW (from counters):" << std::endl;
  std::cout << "nEvents (unweighted): " << nEventsUnweighted_ << std::endl;
  std::cout << "nEvents: " << nEvents_ << std::endl;
  std::cout << "nEvPass0: " << nEvPass0_ << " +- " << sqrt(nEvPass0_) << ", " << (double) nEvPass0_/nEventsUnweighted_*100.0 << " %" << std::endl;
  std::cout << "nEvPass1: " << nEvPass1_ << " +- " << sqrt(nEvPass1_) << ", " << (double) nEvPass1_/nEvPass0_*100.0 << " %" << std::endl;
  std::cout << "nEvPass2: " << nEvPass2_ << " +- " << sqrt(nEvPass2_) << ", " << (double) nEvPass2_/nEvPass1_*100.0 << " %" << std::endl;
  std::cout << "nEvPass3: " << nEvPass3_ << " +- " << sqrt(nEvPass3_) << ", " << (double) nEvPass3_/nEvPass2_*100.0 << " %" << std::endl;
  std::cout << "nEvPass4: " << nEvPass4_ << " +- " << sqrt(nEvPass4_) << ", " << (double) nEvPass4_/nEvPass3_*100.0 << " %" << std::endl;
  std::cout << "nEvPass5: " << nEvPass5_ << " +- " << sqrt(nEvPass5_) << ", " << (double) nEvPass5_/nEvPass4_*100.0 << " %" << std::endl;
  std::cout << "nEvPass6: " << nEvPass6_ << " +- " << sqrt(nEvPass6_) << ", " << (double) nEvPass6_/nEvPass5_*100.0 << " %" << std::endl;
  std::cout << "nEvPass7: " << nEvPass7_ << " +- " << sqrt(nEvPass7_) << ", " << (double) nEvPass7_/nEvPass6_*100.0 << " %" << std::endl;
  std::cout << "nEvPass7p5: " << nEvPass7p5_ << " +- " << sqrt(nEvPass7p5_) << ", " << (double) nEvPass7p5_/nEvPass7_*100.0 << " %" << std::endl;
  std::cout << "nEvPass8a: " << nEvPass8a_ << " +- " << sqrt(nEvPass8a_) << ", " << (double) nEvPass8a_/nEvPass7p5_*100.0 << " %" << std::endl;
  std::cout << "nEvPass8b: " << nEvPass8b_ << " +- " << sqrt(nEvPass8b_) << ", " << (double) nEvPass8b_/nEvPass8a_*100.0 << " %" << std::endl;
  std::cout << "nEvPass8c: " << nEvPass8c_ << " +- " << sqrt(nEvPass8c_) << ", " << (double) nEvPass8c_/nEvPass8b_*100.0 << " %" << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << "\n2010 DonRA2CUT FLOW (from histogram):" << std::endl;
  std::cout << "nEvents: " << hist1_->GetBinContent(1) << " +- "<< hist1_->GetBinError(1) << std::endl;
  std::cout << "nPass0_: " << hist1_->GetBinContent(2) << " +- "<< hist1_->GetBinError(2) << std::endl;
  std::cout << "nPass1_: " << hist1_->GetBinContent(3) << " +- "<< hist1_->GetBinError(3) << std::endl;
  std::cout << "nPass2_: " << hist1_->GetBinContent(4) << " +- "<< hist1_->GetBinError(4) << std::endl;
  std::cout << "nPass3_: " << hist1_->GetBinContent(5) << " +- "<< hist1_->GetBinError(5) << std::endl;
  std::cout << "nPass4_: " << hist1_->GetBinContent(6) << " +- "<< hist1_->GetBinError(6) << std::endl;
  std::cout << "nPass5_: " << hist1_->GetBinContent(7) << " +- "<< hist1_->GetBinError(7) << std::endl;
  std::cout << "nPass6_: " << hist1_->GetBinContent(8) << " +- "<< hist1_->GetBinError(8) << std::endl;
  std::cout << "nPass7_: " << hist1_->GetBinContent(9) << " +- "<< hist1_->GetBinError(9) << std::endl;
  std::cout << "nPass7p5_: " << hist1_->GetBinContent(10) << " +- "<< hist1_->GetBinError(10) << std::endl;
  std::cout << "nPass8a_: " << hist1_->GetBinContent(11) << " +- "<< hist1_->GetBinError(11) << std::endl;
  std::cout << "nPass8b_: " << hist1_->GetBinContent(12) << " +- "<< hist1_->GetBinError(12) << std::endl;
  std::cout << "nPass8c_: " << hist1_->GetBinContent(13) << " +- "<< hist1_->GetBinError(13) << std::endl;

}

//define this as a plug-in
DEFINE_FWK_MODULE(DonRA2);
