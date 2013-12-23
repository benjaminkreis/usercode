//Ben Kreis
//c++ -o hbbTreeMaker `root-config --glibs --cflags` hbbTreeMaker.cpp

#include "LHEF.h"
#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TLorentzVector.h"
#include <cmath>
#include "TTree.h"
#include <cassert>
#include <iostream>

using namespace std;


void computeAngles(TLorentzVector thep4H, TLorentzVector thep4Z1, TLorentzVector thep4M11, TLorentzVector thep4M12, TLorentzVector thep4Z2, TLorentzVector thep4M21, TLorentzVector thep4M22, double& costheta1, double& costheta2, double& Phi, double& costhetastar, double& Phi1);


double deltaPhi (double phi1, double phi2)
{
    double deltaphi=fabs(phi1-phi2);
    if (deltaphi > 6.283185308) deltaphi -= 6.283185308;
    if (deltaphi > 3.141592654) deltaphi = 6.283185308-deltaphi;
    return deltaphi;
}

bool isQuark(int pdgid)
{
  if (abs (pdgid) == 1 ||
      abs (pdgid) == 2 ||
      abs (pdgid) == 3 ||
      abs (pdgid) == 4 ||
      abs (pdgid) == 5 ||
      abs (pdgid) == 6)
    {
      return true;
    }
  else return false;
}

bool isUpType(int pdgid)
{
  if (abs (pdgid) == 2 ||
      abs (pdgid) == 4 ||
      abs (pdgid) == 6)
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
  if (pdgid == 1 ||
      pdgid == 2 ||
      pdgid == 3 ||
      pdgid == 4 ||
      pdgid == 5 ||
      pdgid == 6 ||
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
  if (pdgid == -1 ||
      pdgid == -2 ||
      pdgid == -3 ||
      pdgid == -4 ||
      pdgid == -5 ||
      pdgid == -6 ||
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

  // Define tree to fill
  TFile file(argv[2],"RECREATE");
  TTree* tree = new TTree("tree","tree");

  float mZ, mH, mVH; //masses
  float costheta1, costheta2, costhetastar, phi, phi1; //angles
  float rapidityVH;
  int isLeptonic;
  int isUpType;

  tree->Branch("mZ",  &mZ,  "mZ/F");
  tree->Branch("mH",  &mH,  "mH/F");
  tree->Branch("mVH", &mVH, "mVH/F");
  tree->Branch("costheta1",&costheta1,"costheta1/F");
  tree->Branch("costheta2",&costheta2,"costheta2/F");
  tree->Branch("costhetastar",&costhetastar,"costhetastar/F");
  tree->Branch("phi",&phi,"phi/F");
  tree->Branch("phi1",&phi1,"phi1/F");
  tree->Branch("rapidityVH",&rapidityVH,"rapidityVH/F");
  tree->Branch("isLeptonic",&isLeptonic,"isLeptonic/I");
  tree->Branch("isUpType",&isUpType,"isUpType/I");
  
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
      if (eventCount % 100000 == 0) std::cout << "Event " << eventCount << "\n" ;

      // Assuming HV from JHUGen, without H decay 
      if( lheReader.hepeup.IDUP.size() != 6 ) //two incoming, two intermediate (V,H), two outgoing (V decay)
	{
	  cout << "Error! Expected 6 particles in the event." << endl;
	  return 0;
	}
      
      // Indices for particles of interest
      int i_f0 = -1; //final fermion particle
      int i_f1 = -1; //final fermion antiparticle 
      int i_H  = -1; //intermediate higgs 
      int i_V  = -1; //intermediate V  

      bool VisZ = false;
      bool VisW = false;

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
	      }
	  }
	else if(  lheReader.hepeup.ISTUP.at (iPart) == 1 ) //outgoing 
	  {
	    //cout << "Event " << eventCount << ", outgoing: " << lheReader.hepeup.IDUP.at (iPart) << endl;
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
	  isUpType=0;
	}
      else if( VisW && isLepton(lheReader.hepeup.IDUP.at(finalFermions.at(0))) && isLepton(lheReader.hepeup.IDUP.at(finalFermions.at(1))) ) //leptonic W
	{
	  leptonicWCount++;
	  isLeptonic=1;
	  isUpType=0;
	}
      else if( VisZ && isQuark(lheReader.hepeup.IDUP.at(finalFermions.at(0))) && isQuark(lheReader.hepeup.IDUP.at(finalFermions.at(1))) ) //hadronic Z
	{
	  hadronicZCount++;
	  isLeptonic=0;
	  isUpType=0;
	  if( isUpType( lheReader.hepeup.IDUP.at(finalFermions.at(0)) ) ) isUpType=1;
	}
      else if( VisZ && isLepton(lheReader.hepeup.IDUP.at(finalFermions.at(0))) && isLepton(lheReader.hepeup.IDUP.at(finalFermions.at(1))) ) //leptonic Z
	{
	  leptonicZCount++;
	  isLeptonic=1;
	  isUpType=0;
	}
      else {assert(0);}
      
      //More detailed
      if(VisZ)
	{
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==11 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==11  ) BR_Zee++;
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==13 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==13  ) BR_Zmm++;
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==15 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==15  ) BR_Ztt++;
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==12 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==12  ) BR_Znene++;
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==14 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==14  ) BR_Znmnm++;
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==16 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==16  ) BR_Zntnt++;
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==2 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==2  ) BR_Zuu++;
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==4 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==4  ) BR_Zcc++;
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==1 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==1  ) BR_Zdd++;
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==3 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==3  ) BR_Zss++;
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==5 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==5  ) BR_Zbb++;
	}
      else if(VisW)
	{
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==11 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==12
	      || abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==12 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==11) BR_Wen++;
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==13 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==14
	      || abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==14 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==13) BR_Wmn++;
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==15 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==16
	      || abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==16 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==15) BR_Wtn++;
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==2 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==1
	      || abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==1 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==2) BR_Wud++;
	  if( abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==4 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==3
	      || abs(lheReader.hepeup.IDUP.at(finalFermions.at(0)))==3 && abs(lheReader.hepeup.IDUP.at(finalFermions.at(1)))==4) BR_Wcs++;
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

      //fake the H decay
      TLorentzVector fs_b0 = (0.5)*fs_H; 
      TLorentzVector fs_b1 = (0.5)*fs_H;
      //cout << "Total: " << fs_H.E() << " " << fs_H.Px() << " " << fs_H.Py() << " " << fs_H.Pz() << endl; //check that this works
      //cout << "b0: " << fs_b0.E() << " " << fs_b0.Px() << " " << fs_b0.Py() << " " << fs_b0.Pz() << endl;
      //cout << "b1: " << fs_b1.E() << " " << fs_b1.Px() << " " << fs_b1.Py() << " " << fs_b1.Pz() << endl;

      TLorentzVector p4_Vff = fs_f0 + fs_f1;
      TLorentzVector p4_Hbb = fs_b0 + fs_b1;
      TLorentzVector p4_VH = p4_Vff + p4_Hbb;
      
      double a_costheta1, a_costheta2, a_costhetastar, a_Phi, a_Phi1;
      computeAngles( p4_VH, p4_Vff, fs_f0, fs_f1, p4_Hbb, fs_b0, fs_b1, 
		     a_costheta1, a_costheta2, a_Phi, a_costhetastar, a_Phi1);
      
      mVH = (float) p4_VH.M();
      rapidityVH = (float) p4_VH.Rapidity();
      mZ = (float) p4_Vff.M();
      mH = (float) p4_Hbb.M();        
      costheta1 = (float) a_costheta1;                
      costheta2 = (float) a_costheta2;
      phi = (float) a_Phi;
      costhetastar = (float) a_costhetastar;
      phi1 = (float) a_Phi1;
      //isLeptonic, isUpType already filled

      tree->Fill();

    }// End loop over events

  BR_Znn = BR_Znene + BR_Znmnm + BR_Zntnt;
  
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

  // Write to file
  file.cd();
  tree->Write();
  file.Close();

  return 0;
}




/*

//OLD code for ZllHbb

int main(int argc, char **argv)
{

  // Define tree to fill
  TFile file(argv[2],"RECREATE");
  TTree* tree = new TTree("tree","tree");

  float mZ, mH, mZH; //masses
  float costheta1, costheta2, costhetastar, phi, phi1; //angles
  float rapidityZH;

  tree->Branch("mZ",  &mZ,  "mZ/F");
  tree->Branch("mH",  &mH,  "mH/F");
  tree->Branch("mZH", &mZH, "mZH/F");
  tree->Branch("costheta1",&costheta1,"costheta1/F");
  tree->Branch("costheta2",&costheta2,"costheta2/F");
  tree->Branch("costhetastar",&costhetastar,"costhetastar/F");
  tree->Branch("phi",&phi,"phi/F");
  tree->Branch("phi1",&phi1,"phi1/F");
  tree->Branch("rapidityZH",&rapidityZH,"rapidityZH/F");

  // Reader object
  cout << "Creating reader object for input LHE file " << argv[1] << endl;
  std::ifstream ifsLHE (argv[1]) ;
  LHEF::Reader lheReader (ifsLHE) ;
  
  // Loop over events
  int eventCount = 0;
  while ( lheReader.readEvent() ) 
    {
      eventCount++;

      // Assuming Z(ll)H(bb) decay from JHUGen 
      // -- I think this means I don't need to bother with quark assigment in anaPhantom.
      if( lheReader.hepeup.IDUP.size() != 8 )
	{
	  cout << "Error! Expected 8 particles in the event." << endl;
	  return 0;
	}
      
      // Indices for final state particles
      int i_lep0 = -1; //lep particle
      int i_lep1 = -1; //lep antiparticle 
      int i_b0   = -1; //b particle
      int i_b1   = -1; //b anti particle
      
      // Loop over particles
      for (int iPart = 0 ; iPart < lheReader.hepeup.IDUP.size (); ++iPart){

	if( lheReader.hepeup.ISTUP.at (iPart) == -1  ) // incoming
	  {
	    //cout << "Event " << eventCount << ", incoming: " << lheReader.hepeup.IDUP.at (iPart) << endl;
	    //do nothing
	  }
	else if( lheReader.hepeup.ISTUP.at (iPart) == 2  ) // intermediate
	  {
	    //cout << "Event " << eventCount << ", intermediate: " << lheReader.hepeup.IDUP.at (iPart) << endl;
	    //do nothing
	  }
	else if(  lheReader.hepeup.ISTUP.at (iPart) == 1 ) //outgoing
	  {
	    //cout << "Event " << eventCount << ", outgoing: " << lheReader.hepeup.IDUP.at (iPart) << endl;
	    
	    if( lheReader.hepeup.IDUP.at(iPart) == 11 || //e 
		lheReader.hepeup.IDUP.at(iPart) == 13 || //mu
		lheReader.hepeup.IDUP.at(iPart) == 15)   //tau
	      {
		i_lep0 = iPart;
	      }
	    else if( lheReader.hepeup.IDUP.at(iPart) == -11 || //e anti
		     lheReader.hepeup.IDUP.at(iPart) == -13 || //mu anti
		     lheReader.hepeup.IDUP.at(iPart) == -15)   //tau anti
	      {
		i_lep1 = iPart;
	      }
	    else if( lheReader.hepeup.IDUP.at(iPart) == 5 ) //b
	      {
		i_b0 = iPart;
	      }
	    else if( lheReader.hepeup.IDUP.at(iPart) == -5 ) //b anti
	      {
		i_b1 = iPart;
	      }
	  }//end of outgoing if statement
	else { assert(0); } //sanity check that all particles are either incoming, intermediate, or outgoing.
	
      }// End loop over particles
      assert( (i_lep0 != -1) && (i_lep1 != -1) && (i_b0 != -1) && (i_b1 != -1) );// sanity check that we found all the outgoing particles
	    
      //Create the TLorentzVectors 
      TLorentzVector fs_lep0
        (
         lheReader.hepeup.PUP.at(i_lep0).at(0), //PG px
         lheReader.hepeup.PUP.at(i_lep0).at(1), //PG py
         lheReader.hepeup.PUP.at(i_lep0).at(2), //PG pz
         lheReader.hepeup.PUP.at(i_lep0).at(3)  //PG E
         ) ;
      TLorentzVector fs_lep1
        (
         lheReader.hepeup.PUP.at(i_lep1).at(0), //PG px
         lheReader.hepeup.PUP.at(i_lep1).at(1), //PG py
         lheReader.hepeup.PUP.at(i_lep1).at(2), //PG pz
         lheReader.hepeup.PUP.at(i_lep1).at(3)  //PG E
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

      TLorentzVector p4_Zll = fs_lep0 + fs_lep1;
      TLorentzVector p4_Hbb = fs_b0 + fs_b1;
      TLorentzVector p4_ZH = p4_Zll + p4_Hbb;
      
      double a_costheta1, a_costheta2, a_costhetastar, a_Phi, a_Phi1;
      computeAngles( p4_ZH, p4_Zll, fs_lep0, fs_lep1, p4_Hbb, fs_b0, fs_b1, 
		     a_costheta1, a_costheta2, a_Phi, a_costhetastar, a_Phi1);
      
      mZH = (float) p4_ZH.M();
      rapidityZH = (float) p4_ZH.Rapidity();
      mZ = (float) p4_Zll.M();
      mH = (float) p4_Hbb.M();        
      costheta1 = (float) a_costheta1;                
      costheta2 = (float) a_costheta2;
      phi = (float) a_Phi;
      costhetastar = (float) a_costhetastar;
      phi1 = (float) a_Phi1;
      
      tree->Fill();

    }// End loop over events
  
  cout << "Total number of events processed: " << eventCount << endl;


  // Write to file
  file.cd();
  tree->Write();
  file.Close();

  return 0;
}



*/


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



