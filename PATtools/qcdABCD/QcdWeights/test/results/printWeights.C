{
  cout.setf(0,ios::floatfield);
  cout.precision(100);

  //INPUT
  TFile finweight("weight_QCD.root");
  TH1D* Hweight = 0;
  if(!finweight.IsZombie()){
    Hweight = (TH1D*)finweight.Get("Hweight");
  }
  else{
    cout << "ERROR loading weights" << endl;
  }
  
  int npthat = 9;
  float pthat[] = {1, 16, 31, 81, 171, 301, 471, 801, 1401};
  
  for(int i = 0; i<npthat; i++){
    int bin = Hweight->FindBin(pthat[i]);
    double eventweight=Hweight->GetBinContent(bin);
    cout << "pthat: " << pthat[i] << ", eventweight: " << eventweight << endl;
  }

}  
