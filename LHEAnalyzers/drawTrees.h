#include "TH1D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TString.h"
#include "TTree.h"
#include "TChain.h"
#include "TLegend.h"
#include <iostream>

TStyle *theStyle =0;
void initStyle() {

  //check if the style is already defined
  if (theStyle==0 && gROOT->GetStyle("CMS")==0) {
    theStyle = new TStyle("CMS","Style for P-TDR");


    // For the canvas:
    theStyle->SetCanvasBorderMode(0);
    theStyle->SetCanvasColor(kWhite);
    theStyle->SetCanvasDefH(600); //Height of canvas
    theStyle->SetCanvasDefW(600); //Width of canvas
    theStyle->SetCanvasDefX(0);   //POsition on screen
    theStyle->SetCanvasDefY(0);
    
    // For the Pad:
    theStyle->SetPadBorderMode(0);
    // theStyle->SetPadBorderSize(Width_t size = 1);
    theStyle->SetPadColor(kWhite);
    theStyle->SetPadGridX(false);
    theStyle->SetPadGridY(false);
    theStyle->SetGridColor(0);
    theStyle->SetGridStyle(3);
    theStyle->SetGridWidth(1);
    
    // For the frame:
    theStyle->SetFrameBorderMode(0);
    theStyle->SetFrameBorderSize(1);
    theStyle->SetFrameFillColor(0);
    theStyle->SetFrameFillStyle(0);
    theStyle->SetFrameLineColor(1);
    theStyle->SetFrameLineStyle(1);
    theStyle->SetFrameLineWidth(1);
    
    // For the histo:
    // theStyle->SetHistFillColor(1);
    // theStyle->SetHistFillStyle(0);
    theStyle->SetHistLineColor(1);
    theStyle->SetHistLineStyle(0);
    theStyle->SetHistLineWidth(1);
    // theStyle->SetLegoInnerR(Float_t rad = 0.5);
  // theStyle->SetNumberContours(Int_t number = 20);
    
    theStyle->SetEndErrorSize(2);
    //  theStyle->SetErrorMarker(20);
    theStyle->SetErrorX(0.);
    
    theStyle->SetMarkerStyle(20);
    
    //For the fit/function:
    theStyle->SetOptFit(1);
    theStyle->SetFitFormat("5.4g");
    theStyle->SetFuncColor(2);
    theStyle->SetFuncStyle(1);
    theStyle->SetFuncWidth(1);
    
    //For the date:
    theStyle->SetOptDate(0);
    // theStyle->SetDateX(Float_t x = 0.01);
    // theStyle->SetDateY(Float_t y = 0.01);
    
    // For the statistics box:
    theStyle->SetOptFile(0);
    theStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
    theStyle->SetStatColor(kWhite);
    theStyle->SetStatFont(42);
    theStyle->SetStatFontSize(0.025);
    theStyle->SetStatTextColor(1);
    theStyle->SetStatFormat("6.4g");
    theStyle->SetStatBorderSize(1);
    theStyle->SetStatH(0.1);
    theStyle->SetStatW(0.15);
    // theStyle->SetStatStyle(Style_t style = 1001);
    // theStyle->SetStatX(Float_t x = 0);
    // theStyle->SetStatY(Float_t y = 0);
    
    // Margins:
    theStyle->SetPadTopMargin(0.05);
    theStyle->SetPadBottomMargin(0.13);
    theStyle->SetPadLeftMargin(0.16);
    theStyle->SetPadRightMargin(0.02);
    
    // For the Global title:
    
    theStyle->SetOptTitle(0);
    theStyle->SetTitleFont(42);
    theStyle->SetTitleColor(1);
    theStyle->SetTitleTextColor(1);
    theStyle->SetTitleFillColor(10);
    theStyle->SetTitleFontSize(0.05);
    // theStyle->SetTitleH(0); // Set the height of the title box
    // theStyle->SetTitleW(0); // Set the width of the title box
    // theStyle->SetTitleX(0); // Set the position of the title box
    // theStyle->SetTitleY(0.985); // Set the position of the title box
    // theStyle->SetTitleStyle(Style_t style = 1001);
    // theStyle->SetTitleBorderSize(2);
    
    // For the axis titles:
    
    theStyle->SetTitleColor(1, "XYZ");
    theStyle->SetTitleFont(42, "XYZ");
    theStyle->SetTitleSize(0.06, "XYZ");
    // theStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
    // theStyle->SetTitleYSize(Float_t size = 0.02);
    theStyle->SetTitleXOffset(0.9);
    theStyle->SetTitleYOffset(1.25);
    // theStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset
    
    // For the axis labels:
    
    theStyle->SetLabelColor(1, "XYZ");
    theStyle->SetLabelFont(42, "XYZ");
    theStyle->SetLabelOffset(0.007, "XYZ");
    theStyle->SetLabelSize(0.05, "XYZ");
    
    // For the axis:
    
    theStyle->SetAxisColor(1, "XYZ");
    theStyle->SetStripDecimals(kTRUE);
    theStyle->SetTickLength(0.03, "XYZ");
    theStyle->SetNdivisions(510, "XYZ");
    theStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
    theStyle->SetPadTickY(1);
    
    // Change for log plots:
    theStyle->SetOptLogx(0);
    theStyle->SetOptLogy(0);
    theStyle->SetOptLogz(0);
    
    // Postscript options:
    theStyle->SetPaperSize(20.,20.);
    // theStyle->SetLineScalePS(Float_t scale = 3);
    // theStyle->SetLineStyleString(Int_t i, const char* text);
    // theStyle->SetHeaderPS(const char* header);
    // theStyle->SetTitlePS(const char* pstitle);
    
    // theStyle->SetBarOffset(Float_t baroff = 0.5);
    // theStyle->SetBarWidth(Float_t barwidth = 0.5);
    // theStyle->SetPaintTextFormat(const char* format = "g");
    // theStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
    // theStyle->SetTimeOffset(Double_t toffset);
    // theStyle->SetHistMinimumZero(kTRUE);
    
    theStyle->cd(); //what does this do?
    //end CMS style
    
  }

}



class sample {
 public:
  sample(TString);
  
  TString name;
  TString filename;
  TString humanName;
  
  UInt_t color;

  TChain* chain;
};
sample::sample(TString inputName) {
  name = inputName;
  filename = "";
  filename += name;
  filename += ".root";
  chain = new TChain("tree");
  chain->Add(filename);
  //std::cout << chain->GetEntries() << std::endl;
}



TCanvas* thecanvas = 0;
TLegend* leg = 0;
float leg_x1 = 0.67;
float leg_x2=0.9;
float leg_y1=0.4;
float leg_y2=0.2;
double plotMinimum_ = 0;

std::vector<sample> sampleVector;
std::map<TString, TH1D*> histos;


void renewHisto() {
  
  for ( std::map<TString, TH1D*>::iterator i = histos.begin(); i!=histos.end(); ++i) {
    if (i->second != 0) {
      delete  i->second;
      i->second= 0;
    }
  }
}


void renewCanvas()
{
  if (thecanvas!=0) delete thecanvas;
  thecanvas = new TCanvas("thecanvas", "thecanvas", 500, 500);

  thecanvas->cd()->SetRightMargin(0.06);
  thecanvas->cd()->SetTopMargin(0.08); //test
}


void renewLegend() {

  if (leg!=0) delete leg;
  leg = new TLegend(leg_x1, leg_y1, leg_x2, leg_y2);
  leg->SetBorderSize(0);
  leg->SetLineStyle(0);
  leg->SetTextFont(42);
  leg->SetFillStyle(0);

}


void newPlot(){
  initStyle();
  renewHisto();
  renewCanvas();
  renewLegend();
}


void drawPlots(const TString varname, const int nbins,  const float low, const float high,
	       const TString xtitle, const TString ytitle)
{
  
  newPlot();
  gROOT->SetStyle("CMS");
  
  thecanvas->cd();
  
  //Draw Histograms
  double max = 0;
  for(unsigned int isample=0; isample<sampleVector.size(); isample++) {

    //Create histogram
    TString hname = "h";
    hname+=isample;  
    histos[sampleVector.at(isample).name] = new TH1D(hname, hname, nbins, low, high);

    //Project
    (sampleVector.at(isample).chain)->Project(hname, varname);

    //Get what you want from histogram
    if( histos[sampleVector.at(isample).name]->GetMaximum() > max ) max = histos[sampleVector.at(isample).name]->GetMaximum();
    
  }

  //Second loop to draw (knowing max)
  int ndrawn = 0;
  for(unsigned int isample=0; isample<sampleVector.size(); isample++) {
    
    //Style
    histos[sampleVector.at(isample).name]->SetLineWidth(2);
    histos[sampleVector.at(isample).name]->SetLineColor(sampleVector.at(isample).color);
    histos[sampleVector.at(isample).name]->SetMarkerColor(sampleVector.at(isample).color);
    histos[sampleVector.at(isample).name]->SetXTitle(xtitle);
    histos[sampleVector.at(isample).name]->SetYTitle(ytitle);
    if(high-low < 1.0)  histos[sampleVector.at(isample).name]->SetNdivisions(506);

    //Add to legend
    leg->AddEntry(histos[sampleVector.at(isample).name], sampleVector.at(isample).humanName, "L");

    //Draw histogram
    TString drawOptions = "HIST";
    if(ndrawn>0) drawOptions += " SAME";
    if(ndrawn==0) histos[sampleVector.at(isample).name]->SetMaximum(1.1*max);
    if(ndrawn==0) histos[sampleVector.at(isample).name]->SetMinimum(plotMinimum_);
    histos[sampleVector.at(isample).name]->Draw(drawOptions);
    ndrawn++;
  }
  
  leg->Draw();

  thecanvas->SaveAs("h_"+varname+".pdf");
  

}//end of drawPlots
