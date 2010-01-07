// Preliminary Filter for SUSY b-tag
// Original Author:  "Wee Teo"
// Modified by Ben Kreis


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//
// class declaration
//

class PreliminaryFilter : public edm::EDFilter {
   public:
      explicit PreliminaryFilter(const edm::ParameterSet&);
      ~PreliminaryFilter();

private:
  virtual void beginJob() ;
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
      
  // ----------member data ---------------------------
  
  unsigned int nEvents_;
  //unsigned int nTrigs_;
  //unsigned int nPrimaryMu_;
  //unsigned int nEv1Mu_;
  unsigned int nGoodJets_;
  unsigned int nLargePtJets_;
  unsigned int nBJets_;
  unsigned int nEvJets_;
  unsigned int nEvLargePtJets_;
  unsigned int nEvMET_;
  unsigned int nEvDeltaPhi_;
  unsigned int nEvBJets_;
  unsigned int nElectrons_;
  unsigned int nMuons_;
  unsigned int nLeptons_;
  unsigned int nPositive_;
  unsigned int nNegative_;
  unsigned int nEv0Lepton_;
  unsigned int nEv1Lepton_;
  unsigned int nEv2SSLepton_;
  unsigned int nEv2OSLepton_;
  //unsigned int nEvNoLooseMu_;
  //unsigned int nLooseMu_;
  //unsigned int nEvNoLooseEle_;
  //unsigned int nLooseEle_;

  // input tags  
  //edm::InputTag photonSrc_;
  edm::InputTag elecSrc_;
  edm::InputTag muonSrc_;
  //edm::InputTag tauSrc_;
  edm::InputTag jetSrc_;
  edm::InputTag metSrc_;

};



#include "DataFormats/PatCandidates/interface/Electron.h"
//#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
//#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
PreliminaryFilter::PreliminaryFilter(const edm::ParameterSet& iConfig):
  //photonSrc_(iConfig.getUntrackedParameter<edm::InputTag>("photonSrc")),
  elecSrc_(iConfig.getUntrackedParameter<edm::InputTag>("electronSrc")),
  muonSrc_(iConfig.getUntrackedParameter<edm::InputTag>("muonSrc")),
  //tauSrc_(iConfig.getUntrackedParameter<edm::InputTag>("tauSrc" )),
  jetSrc_(iConfig.getUntrackedParameter<edm::InputTag>("jetSrc" )),
  metSrc_(iConfig.getUntrackedParameter<edm::InputTag>("metSrc" ))
{
   //now do what ever initialization is needed

}


PreliminaryFilter::~PreliminaryFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
PreliminaryFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // using namespace edm;
  nEvents_++;

  //get electron collection
  edm::Handle<edm::View<pat::Electron> > electrons;
  iEvent.getByLabel(elecSrc_,electrons);

  // get muon collection
  edm::Handle<edm::View<pat::Muon> > muons;
  iEvent.getByLabel(muonSrc_,muons);

  // get tau collection  
  //edm::Handle<edm::View<pat::Tau> > taus;
  //iEvent.getByLabel(tauSrc_,taus);

  // get jet collection
  edm::Handle<edm::View<pat::Jet> > jets;
  iEvent.getByLabel(jetSrc_,jets);

  // get met collection  
  edm::Handle<edm::View<pat::MET> > mets;
  iEvent.getByLabel(metSrc_,mets);
  
  // get photon collection  
  //edm::Handle<edm::View<pat::Photon> > photons;
  //iEvent.getByLabel(photonSrc_,photons);

  // get trigger collection
  //edm::Handle<pat::TriggerEvent> triggerEvent;
  //iEvent.getByLabel("patTriggerEvent", triggerEvent);

  //  if(triggerEvent.isValid){
  //pat::TriggerEvent const * trig = & * triggerEvent;
    
  //bool passTrig = false;
  //if(trig){
  //if ( trig->wasRun() && trig->wasAccept() ) {   
  //  pat::TriggerPath const * muPath = trig->path("HLT_Mu9");

      //STEP 1:  trigger cut
  //  if (muPath != 0 && muPath->wasAccept() ) {
  //passTrig = true;

 	    
 
  //STEP 1: At least four good jets
  nGoodJets_ = 0;
  nLargePtJets_ = 0;
  int jetNumber;
  int jetIterator=0;
  float highestJetPt=0, highestPtJetPhi;
  float secondHighestJetPt=0, secondHighestPtJetPhi;
  
  ///////////////////////////////////////////////////////////////////////
  //////////// Don's Jet Pt Ranking (without eta, pt cut) ///////////////
  ///////////////////////////////////////////////////////////////////////

  float jetpt1=0, jetpt2=0, jetpt3=0, jetpt4=0;
  float jetphi1, jetphi2, jetphi3, jetphi4;
  
  for(edm::View<pat::Jet>::const_iterator jet=jets->begin();
      jet!=jets->end(); ++jet){

    if(jet->pt() > jetpt1) {
      jetpt4 = jetpt3;
      jetphi4 = jetphi3;
      jetpt3 = jetpt2;
      jetphi3 = jetphi2;
      jetpt2 = jetpt1;
      jetphi2 = jetphi1;
      jetpt1 = jet->pt();
      jetphi1 = jet->phi();
    }
    else if(jet->pt() > jetpt2){
      jetpt4 = jetpt3;
      jetphi4 = jetphi3;
      jetpt3 = jetpt2;
      jetphi3 = jetphi2;
      jetpt2 = jet->pt();
      jetphi2 = jet->phi();
    }
    else if(jet->pt() > jetpt3){
      jetpt4 = jetpt3;
      jetphi4 = jetphi3;
      jetpt3 = jet->pt();
      jetphi3 = jet->phi();
    }
    else if(jet->pt() > jetpt4){
      jetpt4 = jet->pt();
      jetphi4 = jet->phi();
    }
    
  }
  ///////////////////////////////////////////////////////////////////////

  for(edm::View<pat::Jet>::const_iterator jet=jets->begin(); jet!=jets->end(); ++jet){
    ++jetIterator;
    if(jet->pt()>50 && fabs(jet->eta())<2.4){
      ++nGoodJets_;
      if(jet->pt()>100) ++nLargePtJets_;
      
      //highest pT jet
      if(jet->pt()>highestJetPt){
	highestJetPt=jet->pt();
	highestPtJetPhi=jet->phi(); //check this
	jetNumber = jetIterator;
      }

    }
  }

  //second highest pT jet
  jetIterator=0;
  for(edm::View<pat::Jet>::const_iterator jet=jets->begin(); jet!=jets->end(); ++jet){
    ++jetIterator;
    if(jet->pt()>50 && fabs(jet->eta())<2.4){
      if(jetIterator!=jetNumber && jet->pt()>secondHighestJetPt){
	secondHighestJetPt=jet->pt();
	secondHighestPtJetPhi=jet->phi(); //check this
      }
    }
  }


  if(nGoodJets_ >= 4) {
    nEvJets_++;

    if(nLargePtJets_ > 0) {
      nEvLargePtJets_++;

      //STEP 2: MET cut
      //std::cout << "met size = " << mets->size() << std::endl;
      bool passMETcut = false;
      float metPhi;
      for(edm::View<pat::MET>::const_iterator met=mets->begin(); met!=mets->end(); ++met){
	//      std::cout << "met->et= " << met->et() << std::endl;
	metPhi=met->phi();
	if(met->et() > 100) passMETcut = true;
      }
      
      if(passMETcut ==true){
	nEvMET_++;

	if((jetpt1 != highestJetPt) || (jetpt2 != secondHighestJetPt) || (jetphi1 != highestPtJetPhi) || (jetphi2 != secondHighestPtJetPhi)){
	  std::cout<<"DON jetpt1 = " << jetpt1 <<", jetphi1 = " << jetphi1 << std::endl;
	  std::cout<<"BEN jetpt1 = " << highestJetPt <<", jetphi1 = " << highestPtJetPhi << std::endl;
	  std::cout<<"DON jetpt2 = " << jetpt2 <<", jetphi2 = " << jetphi2 << std::endl;
	  std::cout<<"BEN jetpt2 = " << secondHighestJetPt <<", jetphi2 = " << secondHighestPtJetPhi << std::endl;
	  std::cout<<"DON jetpt3 = " << jetpt3 <<", jetphi3 = " << jetphi3 << std::endl;
	  std::cout<<"DON jetpt4 = " << jetpt4 <<", jetphi4 = " << jetphi4 << std::endl;
	}

	//if(acos(cos(fabs(jetphi1-metPhi)))>0.2 && acos(cos(fabs(jetphi2-metPhi)))>0.2){ //Don
	if(acos(cos(fabs(highestPtJetPhi-metPhi)))>0.2 && acos(cos(fabs(secondHighestPtJetPhi-metPhi)))>0.2){ //Ben
	  nEvDeltaPhi_++;
	  
	  //STEP 3: bJet cut
	  nBJets_ = 0;
	  for(edm::View<pat::Jet>::const_iterator jet=jets->begin(); jet!=jets->end(); ++jet){
	    if(jet->bDiscriminator("simpleSecondaryVertexBJetTags")>=1.74) nBJets_++;
	  }
	  if(nBJets_>=2){
	    nEvBJets_++;
	    
	    ///////////
	    //LEPTONS//
	    ///////////
	    nElectrons_ = 0;
	    nMuons_ = 0;
	    nLeptons_ = 0;
	    nPositive_ = 0;
	    nNegative_ = 0;
	    
	    //electrons
	    for(edm::View<pat::Electron>::const_iterator electron=electrons->begin(); electron!=electrons->end(); ++electron){
	      double EIso = electron->caloIso()+electron->trackIso();
	      double ERelIso = 0;
	      if(electron->pt()) ERelIso = EIso/electron->pt();
	      if(fabs(electron->eta()) < 2.5 && electron->pt()>20 && ERelIso < 0.1) {
		nElectrons_ ++;
		if(electron->charge()>0)nPositive_++;
		else nNegative_++;
	      }
	    }
	    //muons
	    for(edm::View<pat::Muon>::const_iterator muon=muons->begin(); muon!=muons->end(); ++muon){
	      double Iso = muon->caloIso()+muon->trackIso();
	      double MuRelIso = 0;
	      if(muon->pt()) MuRelIso = Iso/muon->pt();
	      if(muon->isGlobalMuon() && fabs(muon->eta()) < 2.5 && muon->pt() > 20 && MuRelIso < 0.1){
		nMuons_++;
		if(muon->charge() > 0) nPositive_++;
		else nNegative_++;
	      }
	    }
	    
	    nLeptons_ = nElectrons_ + nMuons_;
	    
	    if(nLeptons_ == 0) nEv0Lepton_++;
	    else if(nLeptons_ == 1) nEv1Lepton_++;
	    else if(nLeptons_ == 2){
	      if(nPositive_ == nNegative_) nEv2OSLepton_++;
	      else nEv2SSLepton_++;
	    }
	    
	    
	  } //bjet
	} //phi  
      } //metcut
    } //large pt jet
  }//jetcut
  
    /*	    
    //STEP 4: No loose muon
	nLooseMu_ = 0;
	for(edm::View<pat::Muon>::const_iterator muon=muons->begin(); muon!=muons->end(); ++muon){
	  //double Iso = muon->isolationR03().emEt+muon->isolationR03().hadEt+muon->isolationR03().sumPt;
	  double Iso = muon->caloIso()+muon->trackIso();
	  double RelIso = 0;
	  if(muon->pt()) RelIso = Iso/muon->pt();

	  if(muon->isGlobalMuon() && fabs(muon->eta()) < 2.5 && muon->pt() > 10 && RelIso < 0.2) {
	    nLooseMu_++;;
	    //std::cout << "found a loose muon" << std::endl;
	  }
	}
	if(nLooseMu_ < 2){
	  nEvNoLooseMu_++;

	  //STEP 5:  No loose electron
	  nLooseEle_ = 0;
	  for(edm::View<pat::Electron>::const_iterator electron=electrons->begin(); electron!=electrons->end(); ++electron){
	    double EIso = electron->caloIso()+electron->trackIso();
	    double ERelIso = 0;
	    if(electron->pt()) ERelIso = EIso/electron->pt();
		
	    if(fabs(electron->eta()) < 2.5 && electron->pt() > 15 && ERelIso < 0.2) {
	      nLooseEle_++;;
	      //std::cout << "found a loose electron" << std::endl;
	    }
	  }
	  if(nLooseEle_ < 1){
	    nEvNoLooseEle_++;

	    return true;
	  }
	    	      
	}
	    
      }//four or more jets
    }//has one good muon
    */
  //  }//contains a global muon
  //}//pass muon trigger
  //    }
  //}
    
  // loop over jets
  /*
    size_t nJets=0;
    for(edm::View<pat::Jet>::const_iterator jet=jets->begin(); jet!=jets->end(); ++jet){
    if(jet->pt()>50){
    ++nJets;
    }
    // uncomment the following line to fill the 
    // jetTowers_ histogram
    // jetTowers_->Fill(jet->getCaloConstituents().size());
    }
  */
  //histContainer_["jets"]->Fill(nJets);

  // do something similar for the other candidates
  //histContainer_["photons"]->Fill(photons->size() );
  //histContainer_["elecs" ]->Fill(electrons->size());
  //histContainer_["muons"]->Fill(muons->size() );
  //histContainer_["taus" ]->Fill(taus->size()  );
  //histContainer_["met"  ]->Fill(mets->empty() ? 0 : (*mets)[0].et());
  //}

  return false;

}

// ------------ method called once each job just before starting event loop  ------------
void 
PreliminaryFilter::beginJob()
{
  nEvents_ = 0;
  //nTrigs_ = 0;
  // nEv1Mu_ = 0;
  nEvJets_ = 0;
  nEvLargePtJets_=0;
  nEvMET_ = 0;
  nEvDeltaPhi_ = 0;
  nEvBJets_ = 0;
  //nEvNoLooseMu_ = 0;
  //nEvNoLooseEle_ = 0;

}

// ------------ method called once each job just after ending the event loop  ------------
void 
PreliminaryFilter::endJob() {
  std::cout << "\nCUT FLOW:" << std::endl;
  std::cout << "nEvents_ = "          << nEvents_        << std::endl;
  std::cout << "nEvJets_ = "          << nEvJets_        << " (" << (float) nEvJets_/nEvents_*100.0        << "%)" << std::endl;
  std::cout << "nEvLargePtJets_ = "   << nEvLargePtJets_ << " (" << (float) nEvLargePtJets_/nEvJets_*100.0 << "%)" << std::endl;
  std::cout << "nEvMET_ = "           << nEvMET_         << " (" << (float) nEvMET_/nEvLargePtJets_*100.0  << "%)" << std::endl;
  std::cout << "nEvDeltaPhi_ = "      << nEvDeltaPhi_    << " (" << (float) nEvDeltaPhi_/nEvMET_*100.0     << "%)" << std::endl;
  std::cout << "nEvBJets_ = "         << nEvBJets_       << " (" << (float) nEvBJets_/nEvDeltaPhi_*100.0   << "%)" << std::endl;
  std::cout << "\nLEPTON BREAKDOWN:" << std::endl;
  std::cout << "nEv0Lepton_ = "       << nEv0Lepton_     << " (" << (float) nEv0Lepton_/nEvBJets_*100.0    << "%)" << std::endl;
  std::cout << "nEv1Lepton_ = "     << nEv1Lepton_       << " (" << (float) nEv1Lepton_/nEvBJets_*100.0    << "%)" << std::endl;
  std::cout << "nEv2SSLepton_ = "   << nEv2SSLepton_     << " (" << (float) nEv2SSLepton_/nEvBJets_*100.0  << "%)" << std::endl;
  std::cout << "nEv2OSLepton_ = "   << nEv2OSLepton_     << " (" << (float) nEv2OSLepton_/nEvBJets_*100.0  << "%)" << std::endl;
 

}

//define this as a plug-in
DEFINE_FWK_MODULE(PreliminaryFilter);
