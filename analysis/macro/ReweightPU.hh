#ifndef _pureweight_
#define _pureweight_

#include "Style.hh"

#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TColor.h"
#include "TString.h"

#include <utility>
#include <vector>
#include <map>

class ReweightPU
{
public:

  ReweightPU( const TString MC, const TString Data, const Double_t lumi, const Int_t nBins, const TString indir, const TString outdir);
  DblVec GetPUWeights();
  ~ReweightPU();

private:
  
  TString fDataName;
  TString fMCName;
  
  Double_t fLumi;
  Int_t    fNBins;

  TString fInDir;
  TString fOutDir;
  TString fOutType;

  TH1D * fOutDataNvtx;
  TH1D * fOutMCNvtx;
  TH1D * fOutDataOverMCNvtx;

  TFile * fOutFile;

};

#endif




