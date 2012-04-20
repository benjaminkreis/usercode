{
//
// To see the output of this macro, click here.

//

#include "TMath.h"

gROOT->Reset();




 c1 = new TCanvas("c1","Illustration of central limit theorem",750,900);
 c1->Divide(2,3);
 c1->SetFillColor(0);
 c1->SetGrid();

 TRandom3 rnd;

 double two_pi=8*atan(1.0);
 double frac_sigma=0.2;

 c1->cd(1);
 TH1 *hist1 = new TH1F("h1","Gauss",100,-5.0,5.0);
 c1->cd(2);
 TH1 *hist2 = new TH1F("h2","MET",100,0.0,300.0);
 c1->cd(3);
 TH1 *hist3 = new TH1F("h3","Jet with smallest minDeltaPhiN",3,0.5,3.5);
 c1->cd(4);
 TH1 *hist4 = new TH1F("h4","minDeltaPhiN",100,0.0,5.0);

 //c2 = new TCanvas("c2","c2",2*640,480);
 //c2->Divide(2,1);
 TH1 * hp = new TH1F("hp","hp",15,0,1000);
 TH1 * hf = new TH1F("hf","hf",15,0,1000);
 TH1 * hpf = new TH1F("hpf","Npass/Nfail vs MET",15,0,1000);
 hp->Sumw2();
 hf->Sumw2();
 hpf->Sumw2();

 const int na = 7;
 TH1F* ha[na];
 for(int i=0; i<na; i++){
   TString ns = "";
   ns+=i;
   ha[i]=new TH1F("ha"+ns,"minDeltaPhiN",50,0.0,5.0);
   ha[i]->SetLineColor(i+1);
   ha[i]->SetLineWidth(2);
 }
 TFile * fin = new TFile("inputs.root","READ");
 TH1F* htdist = (TH1F*)fin->Get("HT");
 
 for (int i=0;i<100000;i++){
 //for (int i=0;i<100000000;i++){
   double sum=0.0;
   double ht_unsmeared=1000;
   // ht_unsmeared = htdist->GetRandom();
   //if(ht_unsmeared < 1000) continue;
   double pt1=rnd.Rndm();
   double phi1=two_pi*rnd.Rndm();
   double pt2=0.5*rnd.Rndm();  //Make on Jet soft
   double phi2=two_pi*rnd.Rndm();

   double p1x=pt1*cos(phi1);
   double p1y=pt1*sin(phi1);

   double p2x=pt2*cos(phi2);
   double p2y=pt2*sin(phi2);

   double p3x=-p1x-p2x;
   double p3y=-p1y-p2y;
   
   double pt3=sqrt(p3x*p3x+p3y*p3y);
   double phi3=atan2(p3y,p3x);

   double ht_scaling=ht_unsmeared/(pt1+pt2+pt3);

   pt1*=ht_scaling;
   pt2*=ht_scaling;
   pt3*=ht_scaling;

   p1x*=ht_scaling;
   p2x*=ht_scaling;
   p3x*=ht_scaling;

   p1y*=ht_scaling;
   p2y*=ht_scaling;
   p3y*=ht_scaling;

   double pt1s=pt1+frac_sigma*pt1*(rnd.Rndm()+rnd.Rndm()+rnd.Rndm()+rnd.Rndm()+rnd.Rndm()+rnd.Rndm()-3.0)*sqrt(12/6);
   double pt2s=pt2+frac_sigma*pt2*(rnd.Rndm()+rnd.Rndm()+rnd.Rndm()+rnd.Rndm()+rnd.Rndm()+rnd.Rndm()-3.0)*sqrt(12/6);
   double pt3s=pt3+frac_sigma*pt3*(rnd.Rndm()+rnd.Rndm()+rnd.Rndm()+rnd.Rndm()+rnd.Rndm()+rnd.Rndm()-3.0)*sqrt(12/6);

   double phi_smear=0.06;

   double phi1s=phi1+phi_smear*(rnd.Rndm()+rnd.Rndm()+rnd.Rndm()+rnd.Rndm()+rnd.Rndm()+rnd.Rndm()-3.0)*sqrt(12/6);
   double phi2s=phi2+phi_smear*(rnd.Rndm()+rnd.Rndm()+rnd.Rndm()+rnd.Rndm()+rnd.Rndm()+rnd.Rndm()-3.0)*sqrt(12/6);
   double phi3s=phi3+phi_smear*(rnd.Rndm()+rnd.Rndm()+rnd.Rndm()+rnd.Rndm()+rnd.Rndm()+rnd.Rndm()-3.0)*sqrt(12/6);

   double METx=-pt1s*cos(phi1s)-pt2s*cos(phi2s)-pt3s*cos(phi3s);
   double METy=-pt1s*sin(phi1s)-pt2s*sin(phi2s)-pt3s*sin(phi3s);

   double MET=sqrt(METx*METx+METy*METy);

   double phi_MET=atan2(METy,METx);



   double alpha1=phi1s-phi1s;
   double alpha2=phi2s-phi1s;
   double alpha3=phi3s-phi1s;
   
   double T1=sin(alpha1)*frac_sigma*pt1s;
   double T2=sin(alpha2)*frac_sigma*pt2s;
   double T3=sin(alpha3)*frac_sigma*pt3s;

   double T1sq=T1*T1+T2*T2+T3*T3;


   alpha1=phi1s-phi2s;
   alpha2=phi2s-phi2s;
   alpha3=phi3s-phi2s;
   
   T1=sin(alpha1)*frac_sigma*pt1s;
   T2=sin(alpha2)*frac_sigma*pt2s;
   T3=sin(alpha3)*frac_sigma*pt3s;

   double T2sq=T1*T1+T2*T2+T3*T3;

   alpha1=phi1s-phi3s;
   alpha2=phi2s-phi3s;
   alpha3=phi3s-phi3s;
   
   T1=sin(alpha1)*frac_sigma*pt1s;
   T2=sin(alpha2)*frac_sigma*pt2s;
   T3=sin(alpha3)*frac_sigma*pt3s;

   double T3sq=T1*T1+T2*T2+T3*T3;

   double deltaPhiN1=phi1s-phi_MET;
   if (deltaPhiN1>two_pi/2) deltaPhiN1-=two_pi;
   if (deltaPhiN1<-two_pi/2) deltaPhiN1+=two_pi;
   deltaPhiN1=fabs(deltaPhiN1);
   deltaPhiN1/=atan(sqrt(T1sq)/MET);

   double deltaPhiN2=phi2s-phi_MET;
   if (deltaPhiN2>two_pi/2) deltaPhiN2-=two_pi;
   if (deltaPhiN2<-two_pi/2) deltaPhiN2+=two_pi;
   deltaPhiN2=fabs(deltaPhiN2);
   deltaPhiN2/=atan(sqrt(T2sq)/MET);

   double deltaPhiN3=phi3s-phi_MET;
   if (deltaPhiN3>two_pi/2) deltaPhiN3-=two_pi;
   if (deltaPhiN3<-two_pi/2) deltaPhiN3+=two_pi;
   deltaPhiN3=fabs(deltaPhiN3);
   deltaPhiN3/=atan(sqrt(T3sq)/MET);

   int mindeltaPhiJet=1; //highest jet was the smallest deltaPhi

   double mindeltaPhiN=-1;

   if ((deltaPhiN1<deltaPhiN2)&&(deltaPhiN1<deltaPhiN3)){
     if (pt1s<pt2s) mindeltaPhiJet+=1;
     if (pt1s<pt3s) mindeltaPhiJet+=1;
     mindeltaPhiN=deltaPhiN1;
   }

   if ((deltaPhiN2<deltaPhiN1)&&(deltaPhiN2<deltaPhiN3)){
     if (pt2s<pt1s) mindeltaPhiJet+=1;
     if (pt2s<pt3s) mindeltaPhiJet+=1;
     mindeltaPhiN=deltaPhiN2;
   }

   if ((deltaPhiN3<deltaPhiN1)&&(deltaPhiN3<deltaPhiN2)){
     if (pt3s<pt1s) mindeltaPhiJet+=1;
     if (pt3s<pt2s) mindeltaPhiJet+=1;
     mindeltaPhiN=deltaPhiN3;
   }

   hist1->Fill((rnd.Rndm()+rnd.Rndm()+rnd.Rndm()+rnd.Rndm()+rnd.Rndm()+rnd.Rndm()-3.0)*sqrt(12/6));
   hist2->Fill(MET);
   if (MET>100){
     hist3->Fill(mindeltaPhiJet);
     hist4->Fill(mindeltaPhiN);
   }

   if(mindeltaPhiN>=4)hp->Fill(MET);
   if(mindeltaPhiN<4) hf->Fill(MET);
   if(MET>0 && MET<50) ha[0]->Fill(mindeltaPhiN);
   if(MET>50 && MET<100) ha[1]->Fill(mindeltaPhiN);
   if(MET>100 && MET<150) ha[2]->Fill(mindeltaPhiN);
   if(MET>150 && MET<200) ha[3]->Fill(mindeltaPhiN);
   if(MET>200 && MET<300) ha[4]->Fill(mindeltaPhiN);
   if(MET>300 && MET<400) ha[5]->Fill(mindeltaPhiN);
   if(MET>400) ha[6]->Fill(mindeltaPhiN);


 }


 c1->cd(1);
 hist1->Draw();

 c1->cd(2);
 hist2->Draw();

 c1->cd(3);
 hist3->Draw();

 c1->cd(4);
 hist4->Draw();

 c1->cd(6);
 hpf->Divide(hp,hf);
 hpf->Draw("hist e");
 c1->cd(5);
 ha[0]->DrawNormalized();
 ha[1]->DrawNormalized("SAME");
 ha[2]->DrawNormalized("SAME");
 ha[3]->DrawNormalized("SAME");
 ha[4]->DrawNormalized("SAME");
 ha[5]->DrawNormalized("SAME");
 ha[6]->DrawNormalized("SAME");

  TLegend * myLegend = new TLegend(0.53,0.88,0.81,0.6); 
  myLegend->SetFillColor(0);
  myLegend->SetBorderSize(0);
  myLegend->SetLineStyle(0);
  myLegend->SetTextFont(42);
  myLegend->SetFillStyle(0);
  myLegend->SetTextSize(0.035);
  myLegend->AddEntry(ha[0],"MET<50", "l");
  myLegend->AddEntry(ha[1],"50<MET<100", "l");
  myLegend->AddEntry(ha[2],"100<MET<150", "l");
  myLegend->AddEntry(ha[3],"150<MET<200", "l");
  myLegend->AddEntry(ha[4],"200<MET<300", "l");
  myLegend->AddEntry(ha[5],"300<MET<400", "l");
  myLegend->AddEntry(ha[6],"400<MET", "l");
  myLegend->Draw();
  
  TString hts = "";
  hts += (int) ht_unsmeared;
  c1->Print("central_limit_phis_"+hts+".pdf");
  c1->Print("central_limit_phis_"+hts+".png");

}

