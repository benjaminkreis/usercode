#include "TString.h"
#include "TFile.h"
#include "TKey.h"
#include "TH1.h"
#include "TH2.h"
#include "TCollection.h"

#include <iostream>

using namespace std;

void mergeLTEQGT()
{

  TString baseName = "plots_QCD_Pt170_nBJets";

  vector<TString> names;
  names.push_back(baseName+"LT2.root");
  names.push_back(baseName+"EQ2.root");
  names.push_back(baseName+"GT2.root");
  
  vector<TFile*> files;
  
  for (unsigned int i=0; i< names.size(); i++){
    TFile* ftmp = new TFile(names.at(i));
    if (ftmp->IsZombie()) cout <<"Problem loading file with name="<<names.at(i)<< endl;
    files.push_back( ftmp);
  }
  
  TString outname = baseName;
  outname+="LTEQGTmerged.root";
  TFile fout(outname, "RECREATE");

  TIter nextkey(files[0]->GetListOfKeys());
  TKey *key;

  while (key = (TKey*) nextkey() ) {
    cout <<"-- new key --"<< endl;
    TH1F* th1=0;
    TH2F* th2=0;
    TH1F* h1=0;
    TH2F* h2=0;
    
    if( key->ReadObj()->InheritsFrom("TH2F")){
      th2=(TH2F*) key->ReadObj();
    }
    else if ( key->ReadObj()->InheritsFrom("TH1F")){
      th1=(TH1F*) key->ReadObj();
    }
    else {
      cout << "Problem determing type of key+" << key->ReadObj()->GetName()<<endl;
      continue;
    }
    
    fout.cd();

    if(th1!=0){
      h1=(TH1F*) th1->Clone();
    }
    else if (th2!=0){
      h2=(TH2F*) th2->Clone();
    }

    for (unsigned int i=1; i<files.size(); i++){
      
      if (h1!=0){
	th1 = (TH1F*) files.at(i)->Get(th1->GetName());
	h1->Add(th1);
      }
      else if (h2!=0){
	th2 = (TH2F*) files.at(i)->Get(th2->GetName());
	h2->Add(th2);
      }

    }//end for 

    fout.cd();
    if (h1!=0) h1->Write();
    else if (h2!=0) h2->Write();
    
  }//end while over keys
  
  cout << "Finished writing file " << outname << endl;
  fout.Close();

}//end mergeLTEQGT()
