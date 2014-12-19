#include <iostream>

#include "TChain.h"
#include "TH1D.h"

using namespace std;

void checkBF(){

  TChain* tin_signal = new TChain("tree");
  tin_signal->Add("out.root");
  cout << "total = " << tin_signal->GetEntries() << endl;
  
  double total=0; 
  double BR_Zee=0, BR_Zmm=0, BR_Ztt=0, BR_Znn=0, BR_Zuu=0, BR_Zcc=0, BR_Zdd=0, BR_Zss=0, BR_Zbb=0;
  double BR_Wen=0, BR_Wmn=0, BR_Wtn=0, BR_Wud=0, BR_Wcs=0;
  double BR_Znene=0, BR_Znmnm=0, BR_Zntnt=0;
  

  TH1D* h = new TH1D("h", "h", 1,  -2, 2);
  tin_signal->Project("h", "costheta1");
  total = h->Integral();
  cout << "total = " << total << endl;

  //Zee=0, Zmm=1, Ztt=2, Zuu=3, Zcc=4, Zdd=5, Zss=6, Zbb=7
  //Wen=8, Wmn=9, Wtn=10, Wud=11, Wcs=12
  //Znene=13, Znmnm=14, Zntnt=15

  tin_signal->Project("h", "costheta1", "decayMode==0");
  BR_Zee=h->Integral()/total;

  tin_signal->Project("h", "costheta1", "decayMode==1");
  BR_Zmm=h->Integral()/total;

  tin_signal->Project("h", "costheta1", "decayMode==2");
  BR_Ztt=h->Integral()/total;

  tin_signal->Project("h", "costheta1", "decayMode==3");
  BR_Zuu=h->Integral()/total;

  tin_signal->Project("h", "costheta1", "decayMode==4");
  BR_Zcc=h->Integral()/total;

  tin_signal->Project("h", "costheta1", "decayMode==5");
  BR_Zdd=h->Integral()/total;

  tin_signal->Project("h", "costheta1", "decayMode==6");
  BR_Zss=h->Integral()/total;

  tin_signal->Project("h", "costheta1", "decayMode==7");
  BR_Zbb=h->Integral()/total;

  tin_signal->Project("h", "costheta1", "decayMode==8");
  BR_Wen=h->Integral()/total;

  tin_signal->Project("h", "costheta1", "decayMode==9");
  BR_Wmn=h->Integral()/total;

  tin_signal->Project("h", "costheta1", "decayMode==10");
  BR_Wtn=h->Integral()/total;

  tin_signal->Project("h", "costheta1", "decayMode==11");
  BR_Wud=h->Integral()/total;

  tin_signal->Project("h", "costheta1", "decayMode==12");
  BR_Wcs=h->Integral()/total;

  tin_signal->Project("h", "costheta1", "decayMode==13");
  BR_Znene=h->Integral()/total;

  tin_signal->Project("h", "costheta1", "decayMode==14");
  BR_Znmnm=h->Integral()/total;

  tin_signal->Project("h", "costheta1", "decayMode==15");
  BR_Zntnt=h->Integral()/total;

  BR_Znn = BR_Znene + BR_Znmnm + BR_Zntnt;


  cout << "BR_Zee " << BR_Zee << endl;
  cout << "BR_Zmm " << BR_Zmm << endl;
  cout << "BR_Ztt " << BR_Ztt << endl;
  cout << "BR_Znene " << BR_Znene << endl;
  cout << "BR_Znmnm " << BR_Znmnm << endl;
  cout << "BR_Zntnt " << BR_Zntnt << endl;
  //cout << "BR_Zinv " << BR_Znn << endl;
  cout << "BR_Zuu " << BR_Zuu << endl;
  cout << "BR_Zcc " << BR_Zcc << endl;
  cout << "BR_Zdd " << BR_Zdd << endl;
  cout << "BR_Zss " << BR_Zss << endl;
  cout << "BR_Zbb " << BR_Zbb << endl;
  cout << "BR_Wen " << BR_Wen << endl;
  cout << "BR_Wmn " << BR_Wmn << endl;
  cout << "BR_Wtn " << BR_Wtn << endl;
  cout << "BR_Wud " << BR_Wud << endl;
  cout << "BR_Wcs " << BR_Wcs << endl;
  cout << endl;

  cout << "<!-- Zlep = " << BR_Zee + BR_Zmm + BR_Ztt + BR_Znn << " -->" << endl;
  cout << "<!-- Zhad = " << BR_Zuu + BR_Zcc + BR_Zdd + BR_Zss + BR_Zbb << " -->" << endl;
  cout << "<!-- Wlep = " << BR_Wen + BR_Wmn + BR_Wtn << " -->" << endl;
  cout << "<!-- Whad = " <<  BR_Wud + BR_Wcs << " -->" << endl;
  cout << " <BR> BR_Zee = " << BR_Zee << endl;
  cout << " <BR> BR_Zmm = " << BR_Zmm << endl;
  cout << " <BR> BR_Ztt = " << BR_Ztt << endl;
  cout << " <BR> BR_Znene = " << BR_Znene << endl;
  cout << " <BR> BR_Znmnm = " << BR_Znmnm << endl;
  cout << " <BR> BR_Zntnt = " << BR_Zntnt << endl;
  cout << " <BR> BR_Zuu = " << BR_Zuu << endl;
  cout << " <BR> BR_Zcc = " << BR_Zcc << endl;
  cout << " <BR> BR_Zdd = " << BR_Zdd << endl;
  cout << " <BR> BR_Zss = " << BR_Zss << endl;
  cout << " <BR> BR_Zbb = " << BR_Zbb << endl;
  cout << " <BR> BR_Wen = " << BR_Wen << endl;
  cout << " <BR> BR_Wmn = " << BR_Wmn << endl;
  cout << " <BR> BR_Wtn = " << BR_Wtn << endl;
  cout << " <BR> BR_Wud = " << BR_Wud << endl;
  cout << " <BR> BR_Wcs = " << BR_Wcs << endl;

}
