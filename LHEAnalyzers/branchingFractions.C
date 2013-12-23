
#include <iostream>

using namespace std;

void branchingFractions(){
  
  double percent = 1.0/100.0;

  double Br_Z_ll   = 10.10*percent;                             // leptonic Z branching
  double Br_Z_hadr = 69.91*percent;                             // hadronic Z branching
  double Br_Z_inv  = 100.*percent - Br_Z_ll - Br_Z_hadr;         // invisible Z branching
  double Br_Z_uu   = 11.6*percent;                              // up upbar Z branching
  double Br_Z_cc   = 11.6*percent;                              // chm chmbar Z branching
  double Br_Z_dd   = 15.6*percent;                              // dn dnbar Z branching
  double Br_Z_ss   = 15.6*percent;                              // str strbar Z branching
  double Br_Z_bb   = Br_Z_hadr - Br_Z_uu - Br_Z_dd - Br_Z_cc - Br_Z_ss;
  double Br_W_ll   = 32.40*percent;                             // leptonic W branching
  double Br_W_hadr = 100.*percent - Br_W_ll;                     // hadronic W branching

  // derived branching fractions (assuming CKM=1)
  double Br_Z_ee   = 1./3.*Br_Z_ll;                             // electron Z branching
  double Br_Z_mm   = 1./3.*Br_Z_ll;                             // muon Z branching
  double Br_Z_tt   = 1./3.*Br_Z_ll;                             // tau Z branching
  double Br_Z_nn   = 1./3.*Br_Z_inv;                            // neutrino Z branching
  double Br_W_en   = 1./3.*Br_W_ll;                             // electron W branching
  double Br_W_mn   = 1./3.*Br_W_ll;                             // muon W branching
  double Br_W_tn   = 1./3.*Br_W_ll;                             // electron W branching
  double Br_W_ud   = 1./2.*Br_W_hadr;                           // u-d W branching
  double Br_W_cs   = 1./2.*Br_W_hadr;                           // c-s W branching

  double Brlept_Z_ee = Br_Z_ee/Br_Z_ll;                           // Z branching fraction Ga(el)/Ga(leptonic)
  double Brlept_Z_mm = Br_Z_mm/Br_Z_ll;                           // Z branching fraction Ga(mu)/Ga(leptonic)
  double Brlept_Z_tt = Br_Z_tt/Br_Z_ll;                           // Z branching fraction Ga(tau)/Ga(leptonic)
  double Brlept_Z_nn = Br_Z_nn/Br_Z_inv;                          // Z branching fraction Ga(neu)/Ga(invisible)
  double Brlept_W_en = Br_W_en/Br_W_ll;                           // W branching fraction Ga(el)/Ga(leptonic)
  double Brlept_W_mn = Br_W_mn/Br_W_ll;                           // W branching fraction Ga(mu)/Ga(leptonic)
  double Brlept_W_tn = Br_W_tn/Br_W_ll;                           // W branching fraction Ga(tau)/Ga(leptonic)

  double Brhadr_Z_uu = Br_Z_uu/Br_Z_hadr;                         // Z branching fraction Ga(up)/Ga(hadronic)
  double Brhadr_Z_cc = Br_Z_cc/Br_Z_hadr;                         // Z branching fraction Ga(chm)/Ga(hadronic)
  double Brhadr_Z_dd = Br_Z_dd/Br_Z_hadr;                         // Z branching fraction Ga(don)/Ga(hadronic)
  double Brhadr_Z_ss = Br_Z_ss/Br_Z_hadr;                         // Z branching fraction Ga(str)/Ga(hadronic)
  double Brhadr_Z_bb = Br_Z_bb/Br_Z_hadr;                         // Z branching fraction Ga(bot)/Ga(hadronic)
  double Brhadr_W_ud = Br_W_ud/Br_W_hadr;                         // W branching fraction Ga(up)/Ga(hadronic)
  double Brhadr_W_cs = Br_W_cs/Br_W_hadr;                         // W branching fraction Ga(chm)/Ga(hadronic)


  //PRINT

  cout << "Br_Z_ll "  << Br_Z_ll<< endl;                             // leptonic Z branching
  cout << "Br_Z_hadr "<< Br_Z_hadr<< endl;                             // hadronic Z branching
  cout << "Br_Z_inv " << Br_Z_inv<< endl;         // invisible Z branching
  cout << "Br_Z_uu "  << Br_Z_uu<< endl;                              // up upbar Z branching
  cout << "Br_Z_cc "  << Br_Z_cc<< endl;                              // chm chmbar Z branching
  cout << "Br_Z_dd "  << Br_Z_dd<< endl;                              // dn dnbar Z branching
  cout << "Br_Z_ss "  << Br_Z_ss<< endl;                              // str strbar Z branching
  cout << "Br_Z_bb "  << Br_Z_bb<< endl;
  cout << "Br_W_ll "  << Br_W_ll<< endl;                             // leptonic W branching
  cout << "Br_W_hadr "<< Br_W_hadr<< endl;                     // hadronic W branching

  // derived branching fractions (assuming CKM=1)
  cout << "Br_Z_ee "  << Br_Z_ee<< endl;                             // electron Z branching
  cout << "Br_Z_mm "  << Br_Z_mm<< endl;                             // muon Z branching
  cout << "Br_Z_tt "  << Br_Z_tt<< endl;                             // tau Z branching
  cout << "Br_Z_nn "  << Br_Z_nn<< endl;                            // neutrino Z branching
  cout << "Br_W_en "  << Br_W_en<< endl;                             // electron W branching
  cout << "Br_W_mn "  << Br_W_mn<< endl;                             // muon W branching
  cout << "Br_W_tn "  << Br_W_tn<< endl;                             // electron W branching
  cout << "Br_W_ud "  << Br_W_ud<< endl;                           // u-d W branching
  cout << "Br_W_cs "  << Br_W_cs<< endl;                           // c-s W branching

  cout << "Brlept_Z_ee "<< Brlept_Z_ee<< endl;                           // Z branching fraction Ga(el)/Ga(leptonic)
  cout << "Brlept_Z_mm "<< Brlept_Z_mm<< endl;                           // Z branching fraction Ga(mu)/Ga(leptonic)
  cout << "Brlept_Z_tt "<< Brlept_Z_tt<< endl;                           // Z branching fraction Ga(tau)/Ga(leptonic)
  cout << "Brlept_Z_nn "<< Brlept_Z_nn << endl;                          // Z branching fraction Ga(neu)/Ga(invisible)
  cout << "Brlept_W_en "<< Brlept_W_en<< endl;                           // W branching fraction Ga(el)/Ga(leptonic)
  cout << "Brlept_W_mn "<< Brlept_W_mn<< endl;                           // W branching fraction Ga(mu)/Ga(leptonic)
  cout << "Brlept_W_tn "<< Brlept_W_tn<< endl;                           // W branching fraction Ga(tau)/Ga(leptonic)

  cout << "Brhadr_Z_uu "<< Brhadr_Z_uu<< endl;                         // Z branching fraction Ga(up)/Ga(hadronic)
  cout << "Brhadr_Z_cc "<< Brhadr_Z_cc<< endl;                         // Z branching fraction Ga(chm)/Ga(hadronic)
  cout << "Brhadr_Z_dd "<< Brhadr_Z_dd<< endl;                         // Z branching fraction Ga(don)/Ga(hadronic)
  cout << "Brhadr_Z_ss "<< Brhadr_Z_ss<< endl;                         // Z branching fraction Ga(str)/Ga(hadronic)
  cout << "Brhadr_Z_bb "<< Brhadr_Z_bb<< endl;                         // Z branching fraction Ga(bot)/Ga(hadronic)
  cout << "Brhadr_W_ud "<< Brhadr_W_ud<< endl;                         // W branching fraction Ga(up)/Ga(hadronic)
  cout << "Brhadr_W_cs "<< Brhadr_W_cs<< endl;                         // W branching fraction Ga(chm)/Ga(hadronic)





}

