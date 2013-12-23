// -*- C++ -*-
//
// Package:    QcdWeights
// Class:      QcdWeights
// 
/**\class QcdWeights QcdWeights.cc qcdABCD/QcdWeights/src/QcdWeights.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Ben Kreis
//         Created:  Tue May 25 20:39:19 CEST 2010
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

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"


//
// class decleration
//

class QcdWeights : public edm::EDAnalyzer {
   public:
      explicit QcdWeights(const edm::ParameterSet&);
      ~QcdWeights();


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------

  TTree *tree1_;
  float tree1_PtHat_;
  float tree1_qScale_;

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
QcdWeights::QcdWeights(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

}


QcdWeights::~QcdWeights()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
QcdWeights::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;


   edm::Handle<GenEventInfoProduct> genEvtInfo;
   bool hasGenEventInfo = iEvent.getByLabel("generator", genEvtInfo);

   float pthat = -1.0;
   float qScale = -1.0;
   if(hasGenEventInfo){ 
     pthat = ( genEvtInfo->hasBinningValues() ? (genEvtInfo->binningValues())[0] : 0.0);
     qScale = genEvtInfo->qScale();
   }
  
  
   tree1_PtHat_ = pthat;
   tree1_qScale_ = qScale;
   tree1_->Fill();

}


// ------------ method called once each job just before starting event loop  ------------
void 
QcdWeights::beginJob()
{

  edm::Service<TFileService> fs;
  tree1_ = fs->make<TTree>("T_weights", "Weight Tree");
  tree1_->Branch("qScale", &tree1_qScale_, "tree1_qScale_/F");
  tree1_->Branch("PtHat", &tree1_PtHat_, "tree1_PtHat_/F");

}

// ------------ method called once each job just after ending the event loop  ------------
void 
QcdWeights::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(QcdWeights);
