#include <TH1.h>
#include <TH2.h>
#include "TGraphErrors.h"
#include <TFile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include "TPaveText.h"

#include <math.h>
#include <vector>

#include <iostream> 

using namespace std;

class FiguresOfMeritEvaluator {
  
  const char *m_signalTitle; 
  const char *m_backgroundTitle; 
  double m_xmin, m_xmax, m_ymin, m_ymax;
  std::vector<const char*> m_direction;
  std::vector<TH1D*> m_signalHisto;
  std::vector<TH1D*> m_bkgHisto;
  std::vector<TString> m_names;

public: 
  void setRange(double, double, double, double);
  void setTitle(const char*, const char *);
  void addSignal(const char *, TH1D*);
  void addBackgrounds(TH1D*, TH1D*, TH1D*, TH1D*);
  void addBackgrounds(TH1D*);
  void setCutDirection(const char *);
  void drawResults(const char *, int);
  TGraphErrors* getFOM1D(const char *, int);
};

void FiguresOfMeritEvaluator::setRange(double xmin, double xmax, double ymin, double ymax) {

  m_xmin = xmin;
  m_xmax = xmax;
  m_ymin = ymin;
  m_ymax = ymax;
}

void FiguresOfMeritEvaluator::setTitle(const char* signal, const char *back) {
  
  m_signalTitle = signal;
  m_backgroundTitle = back;
}

void FiguresOfMeritEvaluator::addSignal(const char *nameVar, TH1D* sig) {

  m_signalHisto.push_back(sig);
  m_names.push_back(TString(nameVar));
}

void FiguresOfMeritEvaluator::addBackgrounds(TH1D* bkg0, TH1D* bkg1, TH1D* bkg2, TH1D* bkg3) {

  if(bkg1) bkg0->Add(bkg1);
  if(bkg2) bkg0->Add(bkg2);
  if(bkg3) bkg0->Add(bkg3);
  m_bkgHisto.push_back(bkg0);
}

void FiguresOfMeritEvaluator::addBackgrounds(TH1D* bkg0) {

  m_bkgHisto.push_back(bkg0);
}

void FiguresOfMeritEvaluator::setCutDirection(const char *dir) { 
  m_direction.push_back(dir); 
}

void FiguresOfMeritEvaluator:: drawResults(const char *fileName, int option) {

  if( m_signalHisto.size()!=m_bkgHisto.size() ) {
    std::cout << "ERROR! for some variable or signal or background histo is missing. Exiting!" << std::endl;
    return;
  }

  TPaveText *text = new TPaveText(0.15,0.90,0.77,0.98,"brNDC");
  text->SetBorderSize(0);
  text->SetFillStyle(0);
  text->SetTextAlign(12);
  text->SetTextFont(132);
  text->SetTextSize(0.04);

  TCanvas c1("c1","",600,600);
  c1.Range(-1.146789,-2319.078,5.688073,12419.95);
  c1.SetFillColor(0);
  c1.SetBorderMode(0);
  c1.SetBorderSize(2);
  c1.SetLeftMargin(0.1677852);
  c1.SetFrameBorderMode(0);
  c1.SetFrameBorderMode(0);
  c1.cd();

  float legxmin, legxmax, legymin,legymax;
  if(option==0) {
    legxmin=0.20;
    legxmax=0.40;
    legymin=0.20;
    legymax=0.30;
  } else {
    legxmin=0.50;
    legxmax=0.70;
    legymin=0.20;
    legymax=0.40;
  }

  TLegend* leg = new TLegend(legxmin,legymin,legxmax,legymax);
  leg->SetBorderSize(0);
  leg->SetFillColor (0);
  leg->SetTextAlign (12);
  leg->SetTextSize  (0.03);

  TString fullName1(fileName);
  TPaveText *textEta;
  textEta = new TPaveText(legxmin, legymin+0.18, legxmax, legymax+0.18,"brNDC");
  if(fullName1.Contains("EB")) textEta->AddText("Barrel");
  if(fullName1.Contains("EE")) textEta->AddText("Endcaps");
  textEta->SetBorderSize(0);
  textEta->SetFillStyle(0);
  textEta->SetTextAlign(12);
  textEta->SetTextFont(132);
  textEta->SetTextSize(0.04);

  std::vector<TGraphErrors*> graphs;

  // loop over 1D
  for( unsigned int ivar=0; ivar<m_signalHisto.size(); ivar++) {
    const char *name = m_names[ivar].Data();
    std::cout << "---> processing " << name << "...";
    TGraphErrors *graph = getFOM1D(name,option);
    graphs.push_back(graph);
    leg->AddEntry(graph,name,"p");
  }

  // draw the results 
  TString fullname(fileName);
  cout << "==> fullname = " << fileName << "\t" << fullname.Data() << endl;
  TString png = TString(fullname)+TString(".png");

  for(int ig=0;ig<(int)graphs.size();++ig) {
    TGraphErrors* graph = graphs[ig];
    if( graph ) {
      
      char nameg[50];
      sprintf(nameg,"graph_%d",ig);
      graph->SetName(nameg);
      graph->SetTitle("");
      if(ig==0) graph->SetMarkerStyle(20);
      else if(ig==1) graph->SetMarkerStyle(21);     
      else if(ig==2) graph->SetMarkerStyle(22);
      else if(ig==3) graph->SetMarkerStyle(23);
      else graph->SetMarkerStyle(23+ig);
      int defColor;
      if(ig==0) defColor=kRed+1;
      else if(ig==1) defColor=kAzure-6;
      else if(ig==2) defColor=kTeal+3;
      else if(ig==3) defColor=kViolet+3;
      else defColor = ig+1;
      if(defColor==5) defColor=kOrange+4;
      graph->SetMarkerColor(defColor);
      graph->SetMarkerSize(0.8);     
      graph->SetLineColor(defColor);
      graph->SetLineWidth(2);
      graph->GetXaxis()->SetRangeUser(m_xmin,m_xmax);
      graph->GetYaxis()->SetRangeUser(m_ymin,m_ymax);
      
      std::string sigSuffix = " efficiency";
      std::string xAxisName = std::string(m_backgroundTitle) + sigSuffix;
      
      std::string bkgSuffix = (option==0) ? " rejection" : " efficiency";
      std::string yAxisName = std::string(m_signalTitle) + bkgSuffix;

      graph->GetXaxis()->SetTitle(xAxisName.c_str());
      graph->GetYaxis()->SetTitle(yAxisName.c_str());
      graph->GetYaxis()->SetTitleOffset(1.5);

      if(ig==0) graph->Draw("APE2");
      else  graph->Draw("PE2");
    }
  }
  
  leg->Draw();
  text->Draw();
  textEta->Draw();
  c1.SaveAs(png);
}

TGraphErrors* FiguresOfMeritEvaluator::getFOM1D(const char *nameVar, int option) {

  TGraphErrors *outGraph = new TGraphErrors();

  int indexVar = -1;
  for(unsigned int ivar=0; ivar<m_signalHisto.size(); ivar++) {
    if (m_names[ivar].Contains(nameVar) &&
	TString(nameVar).Contains(m_names[ivar])) indexVar=ivar;
  }

  if ( indexVar==-1 ) {
    std::cout << "ERROR! The requested variable ( "
              << nameVar << " ) is not in the list of known variables!" << std::endl;
    return 0;
  }

  TH1D *signal = m_signalHisto[indexVar];
  TH1D *background = m_bkgHisto[indexVar];
  const char *cutDir = m_direction[indexVar];

  if( signal && background ) {
    std::cout << "Integral of signal histogram " << signal->GetName() << " = "
              << signal->Integral() << std::endl;
    std::cout << "Integral of background histogram " << background->GetName() << " = "
              << background->Integral() << std::endl;
    
    TAxis *axisS = signal->GetXaxis();
    TAxis *axisB = background->GetXaxis();
    int nBinsSig = axisS->GetNbins();
    int nBinsBkg = axisB->GetNbins();
    
    if( nBinsSig!=nBinsBkg ) {
      std::cout << "ERROR! signal and background histograms have different binning." << std::endl;
      return 0;
    }

    outGraph->Set(nBinsSig+2);

    double signalIntegral = signal->Integral(0,nBinsSig+1);
    double backgroundIntegral = background->Integral(0,nBinsSig+1);

    double tmpSignalIntegral=0.0;
    double tmpBackgroundIntegral=0.0;

    for ( int ibin=0; ibin<=nBinsSig+1; ibin++) {

      if( strcmp(cutDir,"<")==0 ) {
        tmpSignalIntegral = signal->Integral(0,ibin);
        tmpBackgroundIntegral = background->Integral(0,ibin);
      }
      else if( strcmp(cutDir,">")==0 ) {
        tmpSignalIntegral = signal->Integral(ibin,nBinsSig+1);
        tmpBackgroundIntegral = background->Integral(ibin,nBinsSig+1);
      } else if( strcmp(cutDir,"=")==0 ) {
	tmpSignalIntegral = signal->GetBinContent(ibin);
	tmpBackgroundIntegral = background->GetBinContent(ibin);
      } else {
	std::cout << "CONFIGURATION ERROR! direction of the cut not set." << std::endl
                  << "Please use: \">\" for var>x0 or  \"<\" for var<x0" << std::endl;
        return 0;
      }

      double signalEff = tmpSignalIntegral / signalIntegral;
      double backgroundEff = tmpBackgroundIntegral / backgroundIntegral;

      if( option == 0 ) {
        outGraph->SetPoint(ibin,signalEff,1-backgroundEff);
        outGraph->SetPointError(ibin,0,0);
      }
      else if( option == 1 ) {
        outGraph->SetPoint(ibin,backgroundEff,signalEff);
        double backgroundEffErr = sqrt(backgroundEffErr*(1-backgroundEffErr)/backgroundIntegral);
        outGraph->SetPointError(ibin,backgroundEffErr,0.);
      }
      else {
	std::cout << "unrecognized option" << std::endl;
        return 0;
      }
    }
  }
  
  else {
    std::cout << "ERROR! Cannot find signal or background histogram for variable " << nameVar << std::endl;
    return 0;
  }
  
  return outGraph;
}

void photonIdRocs() {
 
  TFile *fileS = TFile::Open("outputFileS.root");
  TFile *fileB = TFile::Open("outputFileB.root");

  // getting histos: signal
  TH1D *HSEB_sieie_0_500     = (TH1D*)fileS->Get("HEB_sieie_0_500");
  TH1D *HSEE_sieie_0_500     = (TH1D*)fileS->Get("HEE_sieie_0_500");
  TH1D *HSEB_sieie_500_1000  = (TH1D*)fileS->Get("HEB_sieie_500_1000");
  TH1D *HSEE_sieie_500_1000  = (TH1D*)fileS->Get("HEE_sieie_500_1000");
  TH1D *HSEB_sieie_1000_2000 = (TH1D*)fileS->Get("HEB_sieie_1000_2000");
  TH1D *HSEE_sieie_1000_2000 = (TH1D*)fileS->Get("HEE_sieie_1000_2000");
  
  // getting histos: background
  TH1D *HBEB_sieie_0_500     = (TH1D*)fileB->Get("HEB_sieie_0_500");
  TH1D *HBEE_sieie_0_500     = (TH1D*)fileB->Get("HEE_sieie_0_500");

  // cosmetics
  HSEB_sieie_0_500->SetLineColor(2);
  HSEE_sieie_0_500->SetLineColor(2);
  HSEB_sieie_500_1000->SetLineColor(3);
  HSEE_sieie_500_1000->SetLineColor(3);
  HSEB_sieie_1000_2000->SetLineColor(4);
  HSEE_sieie_1000_2000->SetLineColor(4);
  //
  HBEB_sieie_0_500->SetLineColor(1);
  HBEE_sieie_0_500->SetLineColor(1);
  // 
  HSEB_sieie_0_500->SetLineWidth(2);
  HSEE_sieie_0_500->SetLineWidth(2);
  HSEB_sieie_500_1000->SetLineWidth(2);
  HSEE_sieie_500_1000->SetLineWidth(2);
  HSEB_sieie_1000_2000->SetLineWidth(2);
  HSEE_sieie_1000_2000->SetLineWidth(2);
  //
  HBEB_sieie_0_500->SetLineWidth(2);
  HBEE_sieie_0_500->SetLineWidth(2);

  // ROCs
  FiguresOfMeritEvaluator roc;
  roc.setRange(0,0.4,0.5,1);
  roc.setTitle("signal","background");
  roc.addSignal("SigmaIeIe, barrel", HSEB_sieie_0_500);
  roc.addBackgrounds(HBEB_sieie_0_500);
  roc.setCutDirection("<");
  roc.drawResults("sieieRoc",1);

  // Plots
  gStyle->SetOptStat(0);

  TLegend *leg;                                                                 
  leg = new TLegend(0.1,0.65,0.35,0.90);                                        
  leg->SetFillStyle(0);                                                         
  leg->SetBorderSize(0);                                                        
  leg->SetTextSize(0.05);                                                       
  leg->SetFillColor(0);                                                         
  leg->AddEntry(HSEB_sieie_0_500,     "S, pT:0-500",     "l");                           
  leg->AddEntry(HSEB_sieie_500_1000,  "S, pT:500-1000",  "l");                           
  leg->AddEntry(HSEB_sieie_1000_2000, "S, pT:1000-2000", "l");                           
  leg->AddEntry(HBEB_sieie_0_500,     "B, pT:0-500", "l");      

  TH2F *H_sieie_EB = new TH2F("H_sieie_EB","barrel",100,0.,0.020,100,0.,0.3);
  TH2F *H_sieie_EE = new TH2F("H_sieie_EE","endcap",100,0.,0.055,100,0.,0.3);
  H_sieie_EB -> SetTitle("barrel");
  H_sieie_EE -> SetTitle("endcap");
  H_sieie_EB -> GetXaxis()->SetTitle("#sigmaI#etaI#eta");
  H_sieie_EE -> GetXaxis()->SetTitle("#sigmaI#etaI#eta");

  TCanvas c1a("c1a","c1a",1);
  H_sieie_EB ->Draw();
  HSEB_sieie_1000_2000 -> DrawNormalized("same");
  HSEB_sieie_0_500     -> DrawNormalized("same");
  HSEB_sieie_500_1000  -> DrawNormalized("same");
  HBEB_sieie_0_500     -> DrawNormalized("same");
  leg->Draw();
  c1a.SaveAs("HEB_sieie.png");

  TCanvas c1b("c1b","c1b",1);
  H_sieie_EE ->Draw();
  HSEE_sieie_1000_2000 -> DrawNormalized("same");
  HSEE_sieie_0_500     -> DrawNormalized("same");
  HSEE_sieie_500_1000  -> DrawNormalized("same");
  HBEE_sieie_0_500     -> DrawNormalized("same");
  leg->Draw();
  c1b.SaveAs("HEE_sieie.png");
}

