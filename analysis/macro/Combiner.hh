#ifndef _combinertools_
#define _combinertools_

#include "Style.hh"

#include "TFile.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TString.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TROOT.h"
#include "TMath.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TDirectory.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TLatex.h"
#include "THStack.h"
#include "TLine.h"

#include <iostream>
#include <cmath>

typedef std::vector<TFile*>   TFileVec;
typedef std::vector<TH1D*>    TH1DVec;
typedef std::vector<TH1DVec>  TH1DVecVec;
typedef std::vector<THStack*> THStackVec;
typedef std::vector<TPad*>    TPadVec;
typedef std::vector<TLegend*> TLegVec;
typedef std::vector<TCanvas*> TCanvVec;
typedef std::vector<TLine*>   TLineVec;

class Combiner{
public:
  Combiner(const SamplePairVec Samples, const Double_t inLumi, const ColorMap colorMap, const TString outname, const Bool_t doNmin1);
  void InitCanvAndHists();
  void InitTH1DNames();
  void DoComb();
  void MakeEffPlots();
  void OverlayPlots();
  void StackPlots();
  void MakeOutputCanvas();
  void DrawCanvasStack(const UInt_t th1d, const Bool_t isLogY);
  void DrawCanvasOverlay(const UInt_t th1d, const Bool_t isLogY);
  void MakeRatioPlots();
  void MakeRatioLine(const UInt_t th1d);
  Double_t GetMaximum(const UInt_t th1d, const Bool_t stack);
  Double_t GetMinimum(const UInt_t th1d, const Bool_t stack);
  ~Combiner();

private:
  TString	addText;
  Double_t 	lumi;

  UInt_t	fNData;
  UInt_t	fNBkg;
  UInt_t	fNSig;

  UInt_t	fIndexEff;

  UInt_t	fNTH1D;
  TStrVec	fTH1DNames;

  TStrVec 	fDataNames; 
  TStrVec	fBkgNames;
  TStrVec	fSigNames;

  TPadVec	fOutTH1DStackPads;
  TPadVec	fOutTH1DRatioPads;

  TFileVec	fDataFiles;
  TFileVec	fBkgFiles;
  TFileVec	fSigFiles;

  ColorMap	fColorMap;
  TStrMap	fSampleTitleMap;

  TH1DVecVec	fInDataTH1DHists;
  TH1DVecVec	fInBkgTH1DHists;
  TH1DVecVec	fInSigTH1DHists;

  TH1DVec	fOutBkgTH1DHists;
  TH1DVec	fOutSigTH1DHists;
  TH1DVec	fOutDataTH1DHists;
  TH1DVec	fOutRatioTH1DHists;
  TLineVec	fOutTH1DRatioLines;

  TH1DVec	fOutTH1DComb;
  THStackVec    fOutBkgTH1DStacks;
  TLegVec	fTH1DLegends;
  TCanvVec	fOutTH1DCanvases;

  TString	fOutDir;
  TString	fOutName;
  TFile *	fOutFile;


};
#endif
