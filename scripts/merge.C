#include<iostream>
#include "TChain.h"

void merge()
{

  TChain *chain = new TChain("Events");

  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_1.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_2.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_3.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_4.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_5.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_6.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_7.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_8.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_9.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_10.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_11.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_12.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_13.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_14.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_15.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_16.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_17.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_18.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_19.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_20.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_21.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_22.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_23.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_24.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_25.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_26.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_27.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_28.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_29.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_30.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_31.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_32.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_33.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_34.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_35.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_36.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_37.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_38.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_39.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_40.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_41.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_42.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_43.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_44.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_45.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_46.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_47.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_48.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_49.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_50.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_51.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_52.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_53.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_54.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_55.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_56.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_57.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_58.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_59.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_60.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_61.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_62.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_63.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_64.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_65.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_66.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_67.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_68.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_69.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_70.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_71.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_72.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_73.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_74.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_75.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_76.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_77.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_78.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_79.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_80.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_81.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_82.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_83.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_84.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_85.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_86.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_87.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_88.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_89.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_90.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_91.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_92.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_93.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_94.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_95.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_96.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_97.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_98.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_99.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_100.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_101.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_102.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_103.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_104.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_105.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_106.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_107.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_108.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_109.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_110.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_111.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_112.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_113.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_114.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_115.root);
  chain->Add(/castor/cern.ch/user/k/kreis/myPAT_CRAB_all_cor_116.root);

  std::cout << "Start merging" << std::endl;

  chain->Merge(/castor/cern.ch/user/k/kreis/Merged.root);

  std::cout <<"Merge complete" << std::endl;


}
