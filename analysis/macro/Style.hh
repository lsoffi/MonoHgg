#ifndef _style_
#define _style_

#include "TROOT.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TSystem.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "TFile.h"
#include "TStyle.h"
#include "TColor.h"

#include <math.h>
#include <iostream>
#include <vector>
#include <utility>
#include <map>

typedef std::vector<Double_t> 		DblVec;
typedef std::map<TString,Color_t> 	ColorMap;
typedef std::vector<TString> 		TStrVec;
typedef std::pair<TString,Int_t>	SamplePair;
typedef std::vector<SamplePair>		SamplePairVec;
typedef SamplePairVec::iterator		SamplePairVecIter;
typedef std::map<TString,TString>	TStrMap;


void MakeOutDirectory(TString outdir);
void CheckValidFile(TFile *& file, const TString fname);
void CheckValidTree(TTree*& tree, const TString tname, const TString fname);
void CheckValidTH1D(TH1D*& plot, const TString pname, const TString fname);
void CheckValidTH2D(TH2D*& plot, const TString pname, const TString fname);
void CMSLumi(TCanvas *& canvas, const Int_t iPosX, const Double_t inlumi);
void setTDRStyle(); 

class Style{
public:
  Style(const Double_t inLumi);
  ~Style();

private:
  TCanvas *	canv; 
  Double_t	lumi;
};
#endif
