#define eventLoop_cxx

#include <iostream>

#include "eventLoop.h"

#include <TFile.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TTree.h"

using namespace std;

void eventLoop::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L eventLoop.C
//      Root > eventLoop t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   
  if (fChain == 0) return;
  
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;

   TFile finweight("weight_QCD.root");
   TH1D* Hweight = 0;
   if(!finweight.IsZombie()){
     Hweight = (TH1D*)finweight.Get("Hweight");
   }
   else{
     cout << "ERROR loading weights" << endl;
   }
   
   TFile fout("out.root", "RECREATE");
    
   TH1D* histPtHat = new TH1D("histPtHat","Pt-Hat",8000,0,2000);
   TH1D* histPtHat_noW = new TH1D("histPtHat_noW","Pt-Hat",8000,0,2000);
   
   TTree* treePtHat = new TTree("T_PtHat", "Pt-Hat Tree");
   double treePtHat_PtHat;
   double treePtHat_eventweight;
   treePtHat->Branch("PtHat", &treePtHat_PtHat, "treePtHat_PtHat/D");
   treePtHat->Branch("eventweight", &treePtHat_eventweight, "treePtHat_eventweight/D");

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      //cout << "jentry " << jentry << ", ientry " << ientry << endl;
      if(jentry%10000==0) cout << "jentry " << jentry << endl;
      //if(jentry==10000) break; //for debugging

      //get weight
      int bin = Hweight->FindBin(processPtHat);
      double eventweight=Hweight->GetBinContent(bin);

      // cout << processPtHat << " " << eventweight << endl;
      histPtHat->Fill(processPtHat,eventweight);
      histPtHat_noW->Fill(processPtHat);
      treePtHat_PtHat = processPtHat;
      treePtHat_eventweight = eventweight;
      treePtHat->Fill();

   }
   
  
   histPtHat->Write();
   histPtHat_noW->Write();
   treePtHat->Write();

   fout.Close();
   
}
