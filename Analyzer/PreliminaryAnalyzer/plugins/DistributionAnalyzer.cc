#include <map>
#include <string>

#include "TH1.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

class DistributionAnalyzer : public edm::EDAnalyzer {

public:
  explicit DistributionAnalyzer(const edm::ParameterSet&);
  ~DistributionAnalyzer();
  
private:

  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  // simple map to contain all histograms; 
  // histograms are booked in the beginJob() 
  // method
  std::map<std::string,TH1F*> histContainer_; 
 

  // input tags  
  edm::InputTag photonSrc_;
  edm::InputTag elecSrc_;
  edm::InputTag muonSrc_;
  edm::InputTag tauSrc_;
  edm::InputTag jetSrc_;
  edm::InputTag metSrc_;
};

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

DistributionAnalyzer::DistributionAnalyzer(const edm::ParameterSet& iConfig):
  histContainer_(),
  photonSrc_(iConfig.getUntrackedParameter<edm::InputTag>("photonSrc")),
  elecSrc_(iConfig.getUntrackedParameter<edm::InputTag>("electronSrc")),
  muonSrc_(iConfig.getUntrackedParameter<edm::InputTag>("muonSrc")),
  tauSrc_(iConfig.getUntrackedParameter<edm::InputTag>("tauSrc" )),
  jetSrc_(iConfig.getUntrackedParameter<edm::InputTag>("jetSrc" )),
  metSrc_(iConfig.getUntrackedParameter<edm::InputTag>("metSrc" ))
{
}

DistributionAnalyzer::~DistributionAnalyzer()
{
}

void
DistributionAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // get electron collection
  edm::Handle<edm::View<pat::Electron> > electrons;
  iEvent.getByLabel(elecSrc_,electrons);

  // get muon collection
  edm::Handle<edm::View<pat::Muon> > muons;
  iEvent.getByLabel(muonSrc_,muons);

  // get tau collection  
  edm::Handle<edm::View<pat::Tau> > taus;
  iEvent.getByLabel(tauSrc_,taus);

  // get jet collection
  edm::Handle<edm::View<pat::Jet> > jets;
  iEvent.getByLabel(jetSrc_,jets);

  // get met collection  
  edm::Handle<edm::View<pat::MET> > mets;
  iEvent.getByLabel(metSrc_,mets);
  
  // get photon collection  
  edm::Handle<edm::View<pat::Photon> > photons;
  iEvent.getByLabel(photonSrc_,photons);
    
      
  // loop over jets
  size_t nJets30=0;
  size_t nBJets30=0;
  for(edm::View<pat::Jet>::const_iterator jet=jets->begin(); jet!=jets->end(); ++jet){
    histContainer_["jetpt" ]->Fill(jet->pt() );
    if(jet->pt()>30){
      ++nJets30;
      if(jet->bDiscriminator("simpleSecondaryVertexBJetTags")>=2.02) nBJets30++;
    }
  }
  
  //fill histograms
  //note jetpt is already filled!
  histContainer_["jets30"]->Fill(nJets30);
  histContainer_["bjets30"]->Fill(nBJets30);   //number of bjets with pt>30GeV
  histContainer_["photons"]->Fill(photons->size() );
  histContainer_["elecs" ]->Fill(electrons->size());
  histContainer_["muons"]->Fill(muons->size() );
  histContainer_["taus" ]->Fill(taus->size()  );
  histContainer_["met_et"    ]->Fill(mets->empty() ? 0 : (*mets)[0].et()); //corrected met
  histContainer_["met_uncorrectedPt" ]->Fill(mets->empty() ? 0 : (*mets)[0].uncorrectedPt()); //uncorrected met
  histContainer_["met_corSumEt"   ]->Fill(mets->empty() ? 0 : (*mets)[0].corSumEt()); //corrected et
  histContainer_["met_sumEt"  ]->Fill(mets->empty() ? 0 : (*mets)[0].sumEt()); //uncorrected et

}

void 
DistributionAnalyzer::beginJob()
{
  // register to the TFileService
  edm::Service<TFileService> fs;
  
  // book histograms:
  histContainer_["photons" ]=fs->make<TH1F>("photons", "photon multiplicity",      10, 0,  10);
  histContainer_["elecs"   ]=fs->make<TH1F>("elecs",   "electron multiplicity",    10, 0,  10);
  histContainer_["muons"   ]=fs->make<TH1F>("muons",   "muon multiplicity",        10, 0,  10);
  histContainer_["taus"    ]=fs->make<TH1F>("taus",    "tau multiplicity",         10, 0,  10);
  histContainer_["jets30"  ]=fs->make<TH1F>("jets30",  "jet>30GeV multiplicity",   10, 0,  10);
  histContainer_["bjets30" ]=fs->make<TH1F>("bjets30", "bjet>30GeV multiplicity",  10, 0,  10);
  histContainer_["jetpt"   ]=fs->make<TH1F>("jetpt",   "jet p_{T}",                100, 0, 100); 
  histContainer_["met_et"  ]=fs->make<TH1F>("met_et",  "met.et()",                 100, 0, 600);
  histContainer_["met_uncorrectedPt" ]=fs->make<TH1F>("met_uncorrectedPt", "met.uncorrectedPt()", 100, 0, 600);
  histContainer_["met_corSumEt"      ]=fs->make<TH1F>("met_corSumEt",      "met.corSumEt()",      100, 0, 1000);
  histContainer_["met_sumEt"         ]=fs->make<TH1F>("met_sumEt",         "met.sumEt()",         100, 0, 2000);

}

void 
DistributionAnalyzer::endJob() 
{
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(DistributionAnalyzer);
