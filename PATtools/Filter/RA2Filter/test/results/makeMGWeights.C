//for QCD MG weights
//weight = sigma*L/N
#include "TH1D.h"
#include "TFile.h"

using namespace std;

void makeMGWeights(){

  TFile fout("weight_MG.root", "RECREATE");
  TH1D* Hweight = new TH1D("Hweight", "Hweight",5, -0.5, 4.5);
  Hweight->Fill(0.,0.);
  Hweight->Fill(1.,7000000.*100./10845132);
  Hweight->Fill(2.,171000.*100./4913036);
  Hweight->Fill(3.,5200.*100./4234762);
  Hweight->Fill(4.,83.*100./1661261);

  Hweight->Write();
  fout.Close();

}
