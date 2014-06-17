//Ben Kreis, based on code from Nhan 
//c++ -o hbbTreeMaker `root-config --glibs --cflags` hbbTreeMaker.cpp

#include <cmath>
#include <cassert>
#include <iostream>

#include "LHEF.h"

#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TLorentzVector.h"
#include "TTree.h"
#include "TMath.h"

using namespace std;


void computeAngles(TLorentzVector thep4H, TLorentzVector thep4Z1, TLorentzVector thep4M11, TLorentzVector thep4M12, TLorentzVector thep4Z2, TLorentzVector thep4M21, TLorentzVector thep4M22, double& costheta1, double& costheta2, double& Phi, double& costhetastar, double& Phi1);

float getMT(TLorentzVector pZ, TLorentzVector pH)
{
  //take MET from pZ
  float myMET = pZ.Pt();
  float myMETx = pZ.Px();
  float myMETy = pZ.Py();

  float Et = pH.Et();
  float px = pH.Px();
  float py = pH.Py();
  
  float MT = -88;
  float MT2 = 2*( Et * myMET - (px*myMETx + py*myMETy) ); 
  if(MT2>0.) {MT=sqrt(MT2);}
  else {MT = -99;}
  
  return MT;
}

double deltaPhi (double phi1, double phi2)
{
  return acos(cos(phi1-phi2));
  //double deltaphi=fabs(phi1-phi2);
  //if (deltaphi > 6.283185308) deltaphi -= 6.283185308;
  //if (deltaphi > 3.141592654) deltaphi = 6.283185308-deltaphi;
  //return deltaphi;
}

bool isQuark(int pdgid)
{
  if (abs (pdgid) == 1 || //PG d quark
      abs (pdgid) == 2 || //PG u quark
      abs (pdgid) == 3 || //PG s quark
      abs (pdgid) == 4 || //PG c quark
      abs (pdgid) == 5 || //PG b quark
      abs (pdgid) == 6)   //PG t quark
    {
      return true;
    }
  else return false;
}

bool isUpType(int pdgid)
{
  if (abs (pdgid) == 2 || //PG u quark
      abs (pdgid) == 4 || //PG c quark
      abs (pdgid) == 6)   //PG t quark
    {
      return true;
    }
  else return false;
}

bool isLepton(int pdgid)
{
  if (abs (pdgid) == 11 ||   //PG electron
      abs (pdgid) == 13 ||   //PG muon
      abs (pdgid) == 15 ||   //PG tau
      abs (pdgid) == 12 ||   //PG neutrino
      abs (pdgid) == 14 ||   //PG neutrino
      abs (pdgid) == 16)     //PG neutrino
    {
      return true;
    }
  else return false;
}

bool isFermion(int pdgid)
{
  if (pdgid == 1 ||    //PG d quark
      pdgid == 2 ||    //PG u qjark
      pdgid == 3 ||    //PG s quark
      pdgid == 4 ||    //PG c quark
      pdgid == 5 ||    //PG b quark
      pdgid == 6 ||    //PG t quark
      pdgid == 11 ||   //PG electron
      pdgid == 13 ||   //PG muon
      pdgid == 15 ||   //PG tau
      pdgid == 12 ||   //PG neutrino
      pdgid == 14 ||   //PG neutrino
      pdgid == 16)  
    {
      return true;
    }
  else return false;
}

bool isAntiFermion(int pdgid)
{
  if (pdgid == -1 ||    //PG d quark
      pdgid == -2 ||    //PG u quark
      pdgid == -3 ||    //PG s quark
      pdgid == -4 ||    //PG c quark
      pdgid == -5 ||    //PG b quark
      pdgid == -6 ||    //PG t quark
      pdgid == -11 ||   //PG electron
      pdgid == -13 ||   //PG muon
      pdgid == -15 ||   //PG tau
      pdgid == -12 ||   //PG neutrino
      pdgid == -14 ||   //PG neutrino
      pdgid == -16)  
    {
      return true;
    }
  else return false;
}


////////////////////////////////////
//     M      A     I     N      //
///////////////////////////////////
int main(int argc, char **argv)
{

  bool verbose = false;
  bool higgsDecay = true;

  // Define tree to fill
  TFile file(argv[2],"RECREATE");
  TTree* tree = new TTree("tree","tree");

  float mZ, mH, mVH, mT; //masses
  float costheta1, costheta2, costhetastar, phi, phi1; //angles
  float rapidityVH;
  int isLeptonic;
  int isUpTypeQuark;
  int passEta;
  int isWplus;  
  int decayMode;
  float b0_e, b0_eta, b0_phi, b0_pt;
  float b1_e, b1_eta, b1_phi, b1_pt;
  float f0_e, f0_eta, f0_phi, f0_pt;
  float f1_e, f1_eta, f1_phi, f1_pt;
  float V_e, V_eta, V_phi, V_pt;
  float H_e, H_eta, H_phi, H_pt;
  //float VH_e, VH_eta, VH_phi, VH_pt;


  tree->Branch("b0_e",   &b0_e,   "b0_e/F");
  tree->Branch("b0_eta", &b0_eta, "b0_eta/F");
  tree->Branch("b0_phi", &b0_phi, "b0_phi/F");
  tree->Branch("b0_pt",  &b0_pt,  "b0_pt/F");
  tree->Branch("b1_e",   &b1_e,   "b1_e/F");
  tree->Branch("b1_eta", &b1_eta, "b1_eta/F");
  tree->Branch("b1_phi", &b1_phi, "b1_phi/F");
  tree->Branch("b1_pt",  &b1_pt,  "b1_pt/F");
  tree->Branch("f0_e",   &f0_e,   "f0_e/F");
  tree->Branch("f0_eta", &f0_eta, "f0_eta/F");
  tree->Branch("f0_phi", &f0_phi, "f0_phi/F");
  tree->Branch("f0_pt",  &f0_pt,  "f0_pt/F");
  tree->Branch("f1_e",   &f1_e,   "f1_e/F");
  tree->Branch("f1_eta", &f1_eta, "f1_eta/F");
  tree->Branch("f1_phi", &f1_phi, "f1_phi/F");
  tree->Branch("f1_pt",  &f1_pt,  "f1_pt/F");
  tree->Branch("V_e",    &V_e,   "V_e/F");
  tree->Branch("V_eta",  &V_eta, "V_eta/F");
  tree->Branch("V_phi",  &V_phi, "V_phi/F");
  tree->Branch("V_pt",   &V_pt,  "V_pt/F");
  tree->Branch("H_e",    &H_e,   "H_e/F");
  tree->Branch("H_eta",  &H_eta, "H_eta/F");
  tree->Branch("H_phi",  &H_phi, "H_phi/F");
  tree->Branch("H_pt",   &H_pt,  "H_pt/F");
  //tree->Branch("VH_e",   &VH_e,   "VH_e/F");
  //tree->Branch("VH_eta", &VH_eta, "VH_eta/F");
  //tree->Branch("VH_phi", &VH_phi, "VH_phi/F");
  //tree->Branch("VH_pt",  &VH_pt,  "VH_pt/F");
  tree->Branch("mT",     &mT,     "mT/F");
  tree->Branch("mZ",     &mZ,     "mZ/F");
  tree->Branch("mH",     &mH,     "mH/F");
  tree->Branch("mVH",    &mVH,    "mVH/F");
  tree->Branch("costheta1",    &costheta1,    "costheta1/F");
  tree->Branch("costheta2",    &costheta2,    "costheta2/F");
  tree->Branch("costhetastar", &costhetastar, "costhetastar/F");
  tree->Branch("phi",          &phi,          "phi/F");
  tree->Branch("phi1",         &phi1,         "phi1/F");
  tree->Branch("rapidityVH",   &rapidityVH,   "rapidityVH/F");
  tree->Branch("isLeptonic",   &isLeptonic,   "isLeptonic/I");
  tree->Branch("isUpTypeQuark",&isUpTypeQuark,"isUpTypeQuark/I");
  tree->Branch("passEta",      &passEta,      "passEta/I");
  tree->Branch("isWplus",      &isWplus,      "isWplus/I");
  tree->Branch("decayMode",    &decayMode,    "decayMode/I");
  //Zee=0, Zmm=1, Ztt=2, Zuu=3, Zcc=4, Zdd=5, Zss=6, Zbb=7
  //Wen=8, Wmn=9, Wtn=10, Wud=11, Wcs=12
  //Znene=13, Znmnm=14, Zntnt=15

  // Reader object
  cout << "Creating reader object for input LHE file " << argv[1] << endl;
  std::ifstream ifsLHE (argv[1]) ;
  LHEF::Reader lheReader (ifsLHE) ;

  // Loop over events
  int eventCount = 0;
  int hadronicWCount = 0;
  int leptonicWCount = 0;
  int hadronicZCount = 0;
  int leptonicZCount = 0;
  int BR_Zee=0, BR_Zmm=0, BR_Ztt=0, BR_Znn=0, BR_Zuu=0, BR_Zcc=0, BR_Zdd=0, BR_Zss=0, BR_Zbb=0;
  int BR_Wen=0, BR_Wmn=0, BR_Wtn=0, BR_Wud=0, BR_Wcs=0;
  int BR_Znene=0, BR_Znmnm=0, BR_Zntnt=0;

  while ( lheReader.readEvent() ) 
    {
      eventCount++;
      if (eventCount % 100000 == 0 && verbose) std::cout << "Event " << eventCount << "\n" ;

      //Sanity check on number of particles
      if( higgsDecay && lheReader.hepeup.IDUP.size() != 8 ) 
	{
	  cout << "Error! Expected 8 particles in the event." << endl;
	  return 0;
	}
      /*
      else if( higgsDecay==0 && lheReader.hepeup.IDUP.size() != 6 ) 
	{
	  cout << "Error! Expected 6 particles in the event." << endl;
	  return 0;
	}
      */

      // Indices for particles of interest
      int i_f0 = -1; //final fermion particle
      int i_f1 = -1; //final fermion antiparticle 
      int i_b0 = -1; //final b particle
      int i_b1 = -1; //final b antiparticle 
      int i_H  = -1; //intermediate higgs 
      int i_V  = -1; //intermediate V  

      bool VisZ = false;
      bool VisW = false;
      decayMode = -1;
      
      std::vector<int> finalFermions ;

      // Loop over particles
      for (int iPart = 0 ; iPart < lheReader.hepeup.IDUP.size (); ++iPart){

	if( lheReader.hepeup.ISTUP.at (iPart) == -1  ) // incoming
	  {
	    //cout << "Event " << eventCount << ", incoming: " << lheReader.hepeup.IDUP.at (iPart) << endl;
	    //do nothing
	  }
	else if( lheReader.hepeup.ISTUP.at (iPart) == 2  ) // intermediate
	  {
	    
	    if( lheReader.hepeup.IDUP.at(iPart) == 25 ) //higgs
	      {
		i_H = iPart;
	      }
	    
	    if( lheReader.hepeup.IDUP.at(iPart) == 23) //Z
	      {
		i_V = iPart;
		VisZ = true;
	      }
	    
	    if( abs(lheReader.hepeup.IDUP.at(iPart)) == 24 ) //W+ or -
	      {
		i_V = iPart;
		VisW = true;
		isWplus=1;
		if(lheReader.hepeup.IDUP.at(iPart)==-24) isWplus=0;
	      }
	  }
	else if(  lheReader.hepeup.ISTUP.at (iPart) == 1 ) //outgoing 
	  {
	    //cout << "Event " << eventCount << ", outgoing: " << lheReader.hepeup.IDUP.at (iPart) << endl;

	    if ( lheReader.hepeup.IDUP.at(iPart) == 5 ) i_b0 = iPart;
	    if ( lheReader.hepeup.IDUP.at(iPart) == -5 ) i_b1 = iPart;
	    if( abs(lheReader.hepeup.IDUP.at(iPart) ) == 5) continue;
	    
	    finalFermions.push_back (iPart) ;
	    if(     isFermion(lheReader.hepeup.IDUP.at(iPart)) ) i_f0 = iPart;
	    if( isAntiFermion(lheReader.hepeup.IDUP.at(iPart)) ) i_f1 = iPart;	 
	  }//end of outgoing if statement
	else { assert(0); } //sanity check that all particles are either incoming, intermediate, or outgoing.
	
      }// End loop over particles
      
      //some sanity checks
      assert( (VisZ && !VisW) || (VisW && !VisZ) );
      assert( finalFermions.size() == 2 ); //if not the case, have to be smarter with assigning i_f0 and i_f1
      assert( (i_f0 != -1) && (i_f1 != -1) && (i_H != -1) && (i_V != -1) );

      //Add to branching fraction counters and check if leptonic
      if( VisW && isQuark(lheReader.hepeup.IDUP.at(finalFermions.at(0))) && isQuark(lheReader.hepeup.IDUP.at(finalFermions.at(1))) ) //hadronic W
	{
	  hadronicWCount++;
	  isLeptonic=0;
	  isUpTypeQuark=0;
	}
      else if( VisW && isLepton(lheReader.hepeup.IDUP.at(finalFermions.at(0))) && isLepton(lheReader.hepeup.IDUP.at(finalFermions.at(1))) ) //leptonic W
	{
	  leptonicWCount++;
	  isLeptonic=1;
	  isUpTypeQuark=0;
	}
      else if( VisZ && isQuark(lheReader.hepeup.IDUP.at(finalFermions.at(0))) && isQuark(lheReader.hepeup.IDUP.at(finalFermions.at(1))) ) //hadronic Z
	{
	  hadronicZCount++;
	  isLeptonic=0;
	  isUpTypeQuark=0;
	  if( isUpType( lheReader.hepeup.IDUP.at(finalFermions.at(0)) ) ) isUpTypeQuark=1;
	}
      else if( VisZ && isLepton(lheReader.hepeup.IDUP.at(finalFermions.at(0))) && isLepton(lheReader.hepeup.IDUP.at(finalFermions.at(1))) ) //leptonic Z
	{
	  leptonicZCount++;
	  isLeptonic=1;
	  isUpTypeQuark=0;
	}
      else {assert(0);}
      
      //More detailed
      if(VisZ)
	{
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==11 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==11  ){
	    BR_Zee++;
	    decayMode=0;
	  }
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==13 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==13  ){
	    BR_Zmm++;
	    decayMode=1;
	  }
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==15 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==15  ){
	    BR_Ztt++;
	    decayMode=2;
	  }
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==12 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==12  ){
	    BR_Znene++;
	    decayMode=13;
	  }
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==14 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==14  ){
	    BR_Znmnm++;
	    decayMode=14;
	  }
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==16 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==16  ){
	    BR_Zntnt++;
	    decayMode=15;
	  }
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==2 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==2  ){
	    BR_Zuu++;
	    decayMode=3;
	  }
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==4 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==4  ){
	    BR_Zcc++;
	    decayMode=4;
	  }
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==1 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==1  ){
	    BR_Zdd++;
	    decayMode=5;
	  }
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==3 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==3  ){
	    BR_Zss++;
	    decayMode=6;
	  }
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==5 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==5  ){
	    BR_Zbb++;
	    decayMode=7;
	  }
	}
      else if(VisW)
	{
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==11 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==12
	      || abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==12 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==11){
	    BR_Wen++;
	    decayMode=8;
	  }
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==13 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==14
	      || abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==14 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==13){
	    BR_Wmn++;
	    decayMode=9;
	  }
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==15 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==16
	      || abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==16 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==15){
	    BR_Wtn++;
	    decayMode=10;
	  }
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==2 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==1
	      || abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==1 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==2){
	    BR_Wud++;
	    decayMode=11;
	  }
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==4 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==3
	      || abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==3 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==4){
	    BR_Wcs++;
	    decayMode=12;
	  }
	}
      else {assert(0);}


      //Create the TLorentzVectors 
      TLorentzVector fs_f0
        (
         lheReader.hepeup.PUP.at(i_f0).at(0), //PG px
         lheReader.hepeup.PUP.at(i_f0).at(1), //PG py
         lheReader.hepeup.PUP.at(i_f0).at(2), //PG pz
         lheReader.hepeup.PUP.at(i_f0).at(3)  //PG E
         ) ;
      TLorentzVector fs_f1
        (
         lheReader.hepeup.PUP.at(i_f1).at(0), //PG px
         lheReader.hepeup.PUP.at(i_f1).at(1), //PG py
         lheReader.hepeup.PUP.at(i_f1).at(2), //PG pz
         lheReader.hepeup.PUP.at(i_f1).at(3)  //PG E
         ) ;
      TLorentzVector fs_H
        (
         lheReader.hepeup.PUP.at(i_H).at(0), //PG px
         lheReader.hepeup.PUP.at(i_H).at(1), //PG py
         lheReader.hepeup.PUP.at(i_H).at(2), //PG pz
         lheReader.hepeup.PUP.at(i_H).at(3)  //PG E
         ) ;
      TLorentzVector fs_b0
        (
         lheReader.hepeup.PUP.at(i_b0).at(0), //PG px
         lheReader.hepeup.PUP.at(i_b0).at(1), //PG py
         lheReader.hepeup.PUP.at(i_b0).at(2), //PG pz
         lheReader.hepeup.PUP.at(i_b0).at(3)  //PG E
         ) ;
      TLorentzVector fs_b1
        (
         lheReader.hepeup.PUP.at(i_b1).at(0), //PG px
         lheReader.hepeup.PUP.at(i_b1).at(1), //PG py
         lheReader.hepeup.PUP.at(i_b1).at(2), //PG pz
         lheReader.hepeup.PUP.at(i_b1).at(3)  //PG E
         ) ;

      //fake the H decay -- this creates problems for computeAngles.  Do i need to boost?
      //fs_b0 = (0.5)*fs_H; 
      //fs_b1 = (0.5)*fs_H;
      //cout << "Total: " << fs_H.E() << " " << fs_H.Px() << " " << fs_H.Py() << " " << fs_H.Pz() << endl; //check that this works
      //cout << "b0: " << fs_b0.E() << " " << fs_b0.Px() << " " << fs_b0.Py() << " " << fs_b0.Pz() << endl;
      //cout << "b1: " << fs_b1.E() << " " << fs_b1.Px() << " " << fs_b1.Py() << " " << fs_b1.Pz() << endl;

      //acceptance test
      passEta = 0;
      if(fabs(fs_f0.PseudoRapidity())<2.4 && fabs(fs_f1.PseudoRapidity())<2.4 && fabs(fs_b0.PseudoRapidity())<2.4 && fabs(fs_b1.PseudoRapidity())<2.4){
	passEta = 1;
      }

      TLorentzVector p4_Vff = fs_f0 + fs_f1;
      TLorentzVector p4_Hbb = fs_b0 + fs_b1;
      TLorentzVector p4_VH  = p4_Vff + p4_Hbb;
      
      //cuts
      //if(p4_Vff.Pt()<100 || p4_Hbb.Pt()<100) continue;

      double a_costheta1, a_costheta2, a_costhetastar, a_Phi, a_Phi1;
      computeAngles( p4_VH, p4_Vff, fs_f0, fs_f1, p4_Hbb, fs_b0, fs_b1, 
		     a_costheta1, a_costheta2, a_Phi, a_costhetastar, a_Phi1);
      
      mT = getMT(p4_Vff, p4_Hbb);

      //remap to convention of arXiv:1309.4819, including fix from Sinan&Alex
      costheta2     = (float) a_costheta1;
      costheta1     = (float) a_costhetastar;
      phi           = (float) a_Phi1;
      costhetastar  = (float) a_costheta2;
      phi1          = (float) a_Phi;
      mVH           = (float) p4_VH.M();
      rapidityVH    = (float) p4_VH.Rapidity();
      mZ            = (float) p4_Vff.M();
      mH            = (float) p4_Hbb.M();
      //isLeptonic, isUpTypeQuark, and decayMode already filled

      b0_e   = fs_b0.E();
      b0_phi = fs_b0.Phi();
      b0_eta = fs_b0.Eta();
      b0_pt  = fs_b0.Pt();
      b1_e   = fs_b1.E();
      b1_phi = fs_b1.Phi();
      b1_eta = fs_b1.Eta();
      b1_pt  = fs_b1.Pt();
      f0_e   = fs_f0.E();
      f0_phi = fs_f0.Phi();
      f0_eta = fs_f0.Eta();
      f0_pt  = fs_f0.Pt();
      f1_e   = fs_f1.E();
      f1_phi = fs_f1.Phi();
      f1_eta = fs_f1.Eta();
      f1_pt  = fs_f1.Pt();
      V_e    = p4_Vff.E();
      V_phi  = p4_Vff.Phi();
      V_eta  = p4_Vff.Eta();
      V_pt   = p4_Vff.Pt();
      H_e    = p4_Hbb.E();
      H_phi  = p4_Hbb.Phi();
      H_eta  = p4_Hbb.Eta();
      H_pt   = p4_Hbb.Pt();
      //VH_e   = p4_VH.E();
      //VH_phi = p4_VH.Phi();
      //VH_eta = p4_VH.Eta();
      //VH_pt  = p4_VH.Pt();


      tree->Fill();

    }// End loop over events

  BR_Znn = BR_Znene + BR_Znmnm + BR_Zntnt;
  
  if(verbose) {
    cout << "Total number of events processed: " << eventCount << endl;
    cout << leptonicWCount << " Leptonic W fraction: " << ((double)leptonicWCount)/((double)eventCount) << endl;
    cout << hadronicWCount << " Hadronic W fraction: " << ((double)hadronicWCount)/((double)eventCount) << endl;
    cout << leptonicZCount << " Leptonic Z fraction: " << ((double)leptonicZCount)/((double)eventCount) << endl;
    cout << hadronicZCount << " Hadronic Z fraction: " << ((double)hadronicZCount)/((double)eventCount) << endl;
    
    //detail
    cout << "BR_Zee " << ((double)BR_Zee)/((double)eventCount) << endl;
    cout << "BR_Zmm " << ((double)BR_Zmm)/((double)eventCount) << endl;
    cout << "BR_Ztt " << ((double)BR_Ztt)/((double)eventCount) << endl;
    cout << "BR_Znene " << ((double)BR_Znene)/((double)eventCount) << endl;
    cout << "BR_Znmnm " << ((double)BR_Znmnm)/((double)eventCount) << endl;
    cout << "BR_Zntnt " << ((double)BR_Zntnt)/((double)eventCount) << endl;
    cout << "BR_Zinv " << ((double)BR_Znn)/((double)eventCount) << endl;
    cout << "BR_Zuu " << ((double)BR_Zuu)/((double)eventCount) << endl;
    cout << "BR_Zcc " << ((double)BR_Zcc)/((double)eventCount) << endl;
    cout << "BR_Zdd " << ((double)BR_Zdd)/((double)eventCount) << endl;
    cout << "BR_Zss " << ((double)BR_Zss)/((double)eventCount) << endl;
    cout << "BR_Zbb " << ((double)BR_Zbb)/((double)eventCount) << endl;
    cout << "BR_Wen " << ((double)BR_Wen)/((double)eventCount) << endl;
    cout << "BR_Wmn " << ((double)BR_Wmn)/((double)eventCount) << endl;
    cout << "BR_Wtn " << ((double)BR_Wtn)/((double)eventCount) << endl;
    cout << "BR_Wud " << ((double)BR_Wud)/((double)eventCount) << endl;
    cout << "BR_Wcs " << ((double)BR_Wcs)/((double)eventCount) << endl;
  }

  // Write to file
  file.cd();
  tree->Write();
  file.Close();

  return 0;
}


//////////////////////////////////
//// P A P E R   4 - V E C T O R   D E F I N I T I O N   O F   P H I   A N D   P H I 1
//////////////////////////////////
void computeAngles(TLorentzVector thep4H, TLorentzVector thep4Z1, TLorentzVector thep4M11, TLorentzVector thep4M12, TLorentzVector thep4Z2, TLorentzVector thep4M21, TLorentzVector thep4M22, double& costheta1, double& costheta2, double& Phi, double& costhetastar, double& Phi1){
    
    ///////////////////////////////////////////////
    // check for z1/z2 convention, redefine all 4 vectors with convention
    ///////////////////////////////////////////////	
    TLorentzVector p4H, p4Z1, p4M11, p4M12, p4Z2, p4M21, p4M22;
    p4H = thep4H;
    
    p4Z1 = thep4Z1; p4M11 = thep4M11; p4M12 = thep4M12;
    p4Z2 = thep4Z2; p4M21 = thep4M21; p4M22 = thep4M22;
    //// costhetastar
	TVector3 boostX = -(thep4H.BoostVector());
	TLorentzVector thep4Z1inXFrame( p4Z1 );
	TLorentzVector thep4Z2inXFrame( p4Z2 );	
	thep4Z1inXFrame.Boost( boostX );
	thep4Z2inXFrame.Boost( boostX );
	TVector3 theZ1X_p3 = TVector3( thep4Z1inXFrame.X(), thep4Z1inXFrame.Y(), thep4Z1inXFrame.Z() );
	TVector3 theZ2X_p3 = TVector3( thep4Z2inXFrame.X(), thep4Z2inXFrame.Y(), thep4Z2inXFrame.Z() );    
    costhetastar = theZ1X_p3.CosTheta();
    
    //// --------------------------- costheta1
    TVector3 boostV1 = -(thep4Z1.BoostVector());
    TLorentzVector p4M11_BV1( p4M11 );
	TLorentzVector p4M12_BV1( p4M12 );	
    TLorentzVector p4M21_BV1( p4M21 );
	TLorentzVector p4M22_BV1( p4M22 );
    p4M11_BV1.Boost( boostV1 );
	p4M12_BV1.Boost( boostV1 );
	p4M21_BV1.Boost( boostV1 );
	p4M22_BV1.Boost( boostV1 );
    
    TLorentzVector p4V2_BV1 = p4M21_BV1 + p4M22_BV1;
    //// costheta1
    costheta1 = -p4V2_BV1.Vect().Dot( p4M11_BV1.Vect() )/p4V2_BV1.Vect().Mag()/p4M11_BV1.Vect().Mag();
    
    //// --------------------------- costheta2
    TVector3 boostV2 = -(thep4Z2.BoostVector());
    TLorentzVector p4M11_BV2( p4M11 );
	TLorentzVector p4M12_BV2( p4M12 );	
    TLorentzVector p4M21_BV2( p4M21 );
	TLorentzVector p4M22_BV2( p4M22 );
    p4M11_BV2.Boost( boostV2 );
	p4M12_BV2.Boost( boostV2 );
	p4M21_BV2.Boost( boostV2 );
	p4M22_BV2.Boost( boostV2 );
    
    TLorentzVector p4V1_BV2 = p4M11_BV2 + p4M12_BV2;
    //// costheta2
    costheta2 = -p4V1_BV2.Vect().Dot( p4M21_BV2.Vect() )/p4V1_BV2.Vect().Mag()/p4M21_BV2.Vect().Mag();
    
    //// --------------------------- Phi and Phi1
    //    TVector3 boostX = -(thep4H.BoostVector());
    TLorentzVector p4M11_BX( p4M11 );
	TLorentzVector p4M12_BX( p4M12 );	
    TLorentzVector p4M21_BX( p4M21 );
	TLorentzVector p4M22_BX( p4M22 );	
    
	p4M11_BX.Boost( boostX );
	p4M12_BX.Boost( boostX );
	p4M21_BX.Boost( boostX );
	p4M22_BX.Boost( boostX );
    
    TVector3 tmp1 = p4M11_BX.Vect().Cross( p4M12_BX.Vect() );
    TVector3 tmp2 = p4M21_BX.Vect().Cross( p4M22_BX.Vect() );    
    
    TVector3 normal1_BX( tmp1.X()/tmp1.Mag(), tmp1.Y()/tmp1.Mag(), tmp1.Z()/tmp1.Mag() ); 
    TVector3 normal2_BX( tmp2.X()/tmp2.Mag(), tmp2.Y()/tmp2.Mag(), tmp2.Z()/tmp2.Mag() ); 
    
    //// Phi
    TLorentzVector p4Z1_BX = p4M11_BX + p4M12_BX;    
    double tmpSgnPhi = p4Z1_BX.Vect().Dot( normal1_BX.Cross( normal2_BX) );
    double sgnPhi = tmpSgnPhi/fabs(tmpSgnPhi);
    Phi = sgnPhi * acos( -1.*normal1_BX.Dot( normal2_BX) );
    
    
    //////////////
    
    TVector3 beamAxis(0,0,1);
    TVector3 tmp3 = (p4M11_BX + p4M12_BX).Vect();
    
    TVector3 p3V1_BX( tmp3.X()/tmp3.Mag(), tmp3.Y()/tmp3.Mag(), tmp3.Z()/tmp3.Mag() );
    TVector3 tmp4 = beamAxis.Cross( p3V1_BX );
    TVector3 normalSC_BX( tmp4.X()/tmp4.Mag(), tmp4.Y()/tmp4.Mag(), tmp4.Z()/tmp4.Mag() );
    
    //// Phi1
    double tmpSgnPhi1 = p4Z1_BX.Vect().Dot( normal1_BX.Cross( normalSC_BX) );
    double sgnPhi1 = tmpSgnPhi1/fabs(tmpSgnPhi1);    
    Phi1 = sgnPhi1 * acos( normal1_BX.Dot( normalSC_BX) );    
    
    //    std::cout << "extractAngles: " << std::endl;
    //    std::cout << "costhetastar = " << costhetastar << ", costheta1 = " << costheta1 << ", costheta2 = " << costheta2 << std::endl;
    //    std::cout << "Phi = " << Phi << ", Phi1 = " << Phi1 << std::endl;    
    
}



