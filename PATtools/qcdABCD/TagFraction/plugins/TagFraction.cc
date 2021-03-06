// -*- C++ -*-
//
// Package:    TagFraction
// Class:      TagFraction
// 
/**\class TagFraction TagFraction.cc qcdABCD/TagFraction/src/TagFraction.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Ben Kreis
//         Created:  Mon Jun  7 13:29:42 CEST 2010
// $Id: TagFraction.cc,v 1.4 2010/06/22 11:38:35 kreis Exp $
//
//


// system include files
#include <memory>

//c++
#include <iostream>
#include <vector>

// root include files
#include "TTree.h"
#include "TH2D.h"
#include "TFile.h"

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

class TagFraction : public edm::EDAnalyzer {
   public:
      explicit TagFraction(const edm::ParameterSet&);
      ~TagFraction();


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
  edm::InputTag jetSrc_;
  unsigned int nEvents_;
  unsigned int nPassCuts_;
  double P2Tot_;
  double dP2Tott_;
  double Pge2Tot_;
  double dPge2Tott_;
  double Pge3Tot_;
  double dPge3Tott_;

  TTree *tree1_;
  double tree1_P0_;
  double tree1_dP0_;
  double tree1_P1_;
  double tree1_dP1_;
  double tree1_P2_;
  double tree1_dP2_;
  double tree1_Pge2_;
  double tree1_dPge2_;
  double tree1_Pge3_;
  double tree1_dPge3_;
  double tree1_btag_;
  double tree1_bjet_;
  double tree1_qScale_;
  double tree1_PtHat_;

  TH2D* hTagEff_;
  TH2D* hTagEffe_;

  
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
TagFraction::TagFraction(const edm::ParameterSet& iConfig):
  jetSrc_(iConfig.getUntrackedParameter<edm::InputTag>("jetSrc" ))

{
   //now do what ever initialization is needed

}


TagFraction::~TagFraction()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
TagFraction::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;
    
   nEvents_++;

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
   int nbtag =0, nbjet =0;
  
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
       double jetid = 0;
       jetid = jet->partonFlavour();
       if(fabs(jet->eta())<2.4 && jet->pt()>30.0){
	 if(jet->bDiscriminator("simpleSecondaryVertexBJetTags")>=1.74) nbtag++;
	 if(fabs(jetid)==5.0) nbjet++;
       }

       //jet b tagging efficiency
       if( jet->pt()>30.0 && fabs(jet->eta())<2.4 ){
	 double thisJetEff = 0.0;
	 double thisJetEffe = 0.0;
	 int globalBinEff = hTagEff_->FindBin( jet->pt(), jet->eta() );
	 thisJetEff = hTagEff_->GetBinContent(globalBinEff);
	 thisJetEffe =  hTagEffe_->GetBinContent(globalBinEff);
	 //cout << jet->pt() << " " << jet->eta() << " " << thisJetEff << " +- " << thisJetEffe << endl;
	 if(thisJetEff>0.0 && thisJetEffe >0.0){
	   jetEff.push_back(thisJetEff);
	   jetEffe.push_back(thisJetEffe);
	 }
	 else{
	   cout << "NEGATIVE!: " << thisJetEff << " " << thisJetEffe << endl;
	 }
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
   if(myMHT>150.0 && dPhiMin>0.3){
     nPassCuts_++;
    
     // calculate probability for 0 tags
     double P0=1;
     double dP0t=0;
     for(int i = 0; i<jetEff.size(); i++){
       if(jetEff[i]>0.0 && jetEffe[i]>0.0){
	 P0=P0*(1-jetEff[i]);
	 dP0t += (jetEffe[i]/jetEff[i])*(jetEffe[i]/jetEff[i]);
       }
     }
     double dP0 = sqrt(dP0t)*P0;
    
     // calculate probability for exactly 1 tag
     double P1 = 0;
     double dP1t = 0;
     for(int i = 0; i<jetEff.size(); i++){
       
       //second part
       double P1a = 1;
       double dP1at = 0;
       for(int j = 0; j<jetEff.size(); j++){
	 if(j!=i){
	   if(jetEff[i]>0.0 && jetEffe[i]>0.0){
	     P1a=P1a*(1-jetEff[j]);
	     dP1at += (jetEffe[i]/jetEff[i])*(jetEffe[i]/jetEff[i]);
	   }// end if eff stuff is not zero
	 }//end if j!=i
       }//end loop over j
       double dP1a = sqrt(dP1at)*P1a;

       P1+=jetEff[i]*P1a;
       dP1t += (jetEff[i]*P1a*sqrt( (jetEffe[i]/jetEff[i])*(jetEffe[i]/jetEff[i]) + (dP1a/P1a)*(dP1a/P1a)  ))*(jetEff[i]*P1a*sqrt( (jetEffe[i]/jetEff[i])*(jetEffe[i]/jetEff[i]) + (dP1a/P1a)*(dP1a/P1a) ));
     }//end loop over i
     double dP1 = sqrt(dP1t);


     // calculate probability for exactly 2 tags
     double P2 = 0;
     double dP2t = 0;
     for(int i=0; i<jetEff.size(); i++){
       for(int j=i+1; j<jetEff.size(); j++){
	 
	 double P2a = 1;
	 double dP2at = 0;
	 for(int k=0; k<jetEff.size(); k++){
	   if(k!=i && k!=j){
	     if(jetEff[i]>0.0 && jetEffe[i]>0.0){
	       P2a=P2a*(1-jetEff[k]);
	       dP2at += (jetEffe[i]/jetEff[i])*(jetEffe[i]/jetEff[i]);
	     }//end iff eff stuff is not zero
	   }//end if k is not i or j
	 }//end loop over k
	 double dP2a = sqrt(dP2at)*P2a;
	 
	 P2+=jetEff[i]*jetEff[j]*P2a;
	 dP2t += (jetEff[i]*jetEff[j]*P2a*sqrt((jetEffe[i]/jetEff[i])*(jetEffe[i]/jetEff[i]) + (jetEffe[j]/jetEff[j])*(jetEffe[j]/jetEff[j]) + (dP2a/P2a)*(dP2a/P2a) )) * (jetEff[i]*jetEff[j]*P2a*sqrt((jetEffe[i]/jetEff[i])*(jetEffe[i]/jetEff[i]) + (jetEffe[j]/jetEff[j])*(jetEffe[j]/jetEff[j]) + (dP2a/P2a)*(dP2a/P2a) ));
       }//j loop
     }// i loop
     double dP2 = sqrt(dP2t);
     
     // calculate probability for >= 2 tags and >=3 tags
     double Pge2, dPge2, Pge3, dPge3;
     Pge2 = 1 - P1 - P0;
     dPge2 = sqrt(dP1*dP1 + dP0*dP0);
     Pge3 = 1 - P2 - P1 - P0;
     dPge3 = sqrt(dP2*dP2 + dP1*dP1 + dP0*dP0);
     
     cout << P0 << " " << P1 << " " << P2  << " " << Pge2 << " " << Pge3 << endl;

     tree1_P0_=P0;
     tree1_dP0_=dP0;
     tree1_P1_=P1;
     tree1_dP1_=dP1;
     tree1_P2_=P2;
     tree1_dP2_=dP2;
     tree1_Pge2_ = Pge2;
     tree1_dPge2_ = dPge2;
     tree1_Pge3_ = Pge3;
     tree1_dPge3_ = dPge3;
     tree1_btag_ = nbtag;
     tree1_bjet_ = nbjet;
     tree1_qScale_ = qScale;
     tree1_PtHat_ = pthat;
     
     tree1_->Fill();
     
     //this part only works if events have the same weight
     // P2Tot_ += P2;
     // dP2Tott_ += dP2*dP2;
     // Pge2Tot_ += Pge2;
     // dPge2Tott_ += dPge2*dPge2;
     // Pge3Tot_ += Pge3;
     // dPge3Tott_ += dPge3*dPge3;
     
   }// end signal region cut
} 


// ------------ method called once each job just before starting event loop  ------------
void 
TagFraction::beginJob()
{
  nEvents_ = 0;
  nPassCuts_ = 0;
  P2Tot_ = 0;
  dP2Tott_ = 0;
  Pge2Tot_ = 0;
  dPge2Tott_ = 0;
  Pge3Tot_ = 0;
  dPge3Tott_ = 0;

  edm::Service<TFileService> fs;
  tree1_ = fs->make<TTree>("T_TagFraction", "Tagging Probability Tree");
  tree1_->Branch("P0", &tree1_P0_, "tree1_P0_/D");
  tree1_->Branch("dP0", &tree1_dP0_, "tree1_dP0_/D");
  tree1_->Branch("P1", &tree1_P1_, "tree1_P1_/D");
  tree1_->Branch("dP1", &tree1_dP1_, "tree1_dP1_/D");
  tree1_->Branch("P2", &tree1_P2_, "tree1_P2_/D");
  tree1_->Branch("dP2", &tree1_dP2_, "tree1_dP2_/D");
  tree1_->Branch("Pge2", &tree1_Pge2_, "tree1_Pge2_/D");
  tree1_->Branch("dPge2", &tree1_dPge2_, "tree1_dPge2_/D");
  tree1_->Branch("Pge3", &tree1_Pge3_, "tree1_Pge3_/D");
  tree1_->Branch("dPge3", &tree1_dPge3_, "tree1_dPge3_/D");
  tree1_->Branch("btag", &tree1_btag_, "tree1_btag_/D");
  tree1_->Branch("bjet", &tree1_bjet_, "tree1_bjet_/D"); 
  tree1_->Branch("qScale", &tree1_qScale_, "tree1_qScale_/D");
  tree1_->Branch("PtHat", &tree1_PtHat_, "tree1_PtHat_/D");

  TFile f("/afs/cern.ch/user/k/kreis/scratch0/PATtest/CMSSW_3_3_6/src/qcdABCD/tagjeteff_QCD_double.root");
  if(!f.IsZombie()){
    hTagEff_ = (TH2D*)f.Get("Heff_t");
    hTagEffe_ = (TH2D*)f.Get("Heff_te");
    std::cout << "hTagEff_ GetEntries(): " << hTagEff_->GetEntries()<<std::endl;
    std::cout << "hTagEffe_ GetEntries(): " << hTagEffe_->GetEntries()<<std::endl;
  }
  else{
    std::cout << "ERROR loading efficiency file." << std::endl;
  }

}

// ------------ method called once each job just after ending the event loop  ------------
void 
TagFraction::endJob() {
  std::cout << "nEvents: " << nEvents_ << std::endl;
  std::cout << "nPassCuts: " << nPassCuts_<< std::endl;
  //  std::cout << "P2Tot: " << P2Tot_ << " +- " << sqrt(dP2Tott_) << std::endl;
  // std::cout << "Pge2Tot: " << Pge2Tot_ << " +- " << sqrt(dPge2Tott_) << std::endl;
  // std::cout << "Pge3Tot: " << Pge3Tot_ << " +- " << sqrt(dPge3Tott_) << std::endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(TagFraction);
