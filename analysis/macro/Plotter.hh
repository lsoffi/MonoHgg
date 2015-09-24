#ifndef _plottertools_
#define _plottertools_

#include "Style.hh"

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
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

#include <iostream>
#include <cmath>

#define nvar 30 //nvar has to be equal to NVARIABLES

struct PlotOptStruct{
public:
  Int_t		nbins;
  Double_t	xmin;
  Double_t	xmax; 
};

typedef std::map<TString,TH1D*>		TH1DMap;
typedef TH1DMap::iterator		TH1DMapIter;
typedef std::map<TString,TH2D*>		TH2DMap;
typedef TH2DMap::iterator		TH2DMapIter;


class Plotter{
public:
  Plotter(const TString inName, const TString outName, const TString inSpecies, const DblVec puweights, const Double_t lumi, Bool_t sigMC);
  ~Plotter();

  void DoPlots();  
  void FindMinAndMax(TH1F *& h, int plotLog);
  TH1D * DrawOverflowBin(const TH1D * h);

  void SetBranchAddresses();
  void SetUpPlots();
  TH1D * MakeTH1DPlot(const TString hname, const TString htitle, const Int_t nbins, const Double_t xlow, const Double_t xhigh, const TString xtitle, const TString ytitle);
  TH2D * MakeTH2DPlot(const TString hname, const TString htitle, const Int_t xnbins, const Double_t xlow, const Double_t xhigh, const Int_t ynbins, const Double_t ylow, const Double_t yhigh, const TString xtitle, const TString ytitle);
  void SavePlots(); 

  void DeleteBranches();  
  void DeleteHists();

private:
  TString 	name;
  TString 	fName;
  TString 	species;
  Bool_t	isSigMC;
  TFile * 	inFile;
  TFile * 	outFile;

  DblVec	fPUWeights;
  Double_t 	fLumi;

  TTree * 	tpho;
  Int_t		nentries;

  TH1DMap	fTH1DMap;
  TH1DMap	fTH1DNewMap; //these histos have overflow bin
  TH2DMap	fTH2DMap;

  // variables for branches
  Int_t 	nvtx;
  Float_t	weight;
  Float_t	mgg;
  Float_t	ptgg;
  Float_t	t1pfmet; 
  Float_t	t1pfmetphi; 
  Float_t	t1pfmetSumEt; 
  Float_t	pfmet; 
  Float_t	pfmetphi; 
  Float_t	pfmetSumEt; 
  Float_t	calomet; 
  Float_t	calometphi; 
  Float_t	calometSumEt; 
  Float_t	pt1;
  Float_t	pt2;
  Float_t	phi1;
  Float_t	phi2;
  Float_t	eta1;
  Float_t	eta2;
  Float_t	r91;
  Float_t	r92;
  Float_t	phoiso1;
  Float_t	phoiso2;
  Float_t	chiso1;
  Float_t	chiso2;
  Float_t	neuiso1;
  Float_t	neuiso2;
  Float_t	sieie1;
  Float_t	sieie2;
  Float_t	hoe1;
  Float_t	hoe2;
  Int_t		passCHiso1;
  Int_t		passCHiso2;
  Int_t		passNHiso1;
  Int_t		passNHiso2;
  Int_t		passPHiso1;
  Int_t		passPHiso2;
  Int_t		passSieie1;
  Int_t		passSieie2;
  Int_t		passHoe1;
  Int_t		passHoe2;
  Int_t		hltPhoton26Photon16Mass60;
  Int_t		hltPhoton36Photon22Mass15;
  Int_t		hltPhoton42Photon25Mass15;
  Int_t		hltDiphoton30Mass95;
  Int_t		hltDiphoton30Mass70;
  Int_t		hltDiphoton30Mass55;
  Int_t		hltDiphoton30Mass55PV;
  Int_t		hltDiphoton30Mass55EB;

  // branches
  TBranch 	*b_nvtx;
  TBranch	*b_weight;
  TBranch	*b_mgg;
  TBranch	*b_ptgg;
  TBranch	*b_t1pfmet;
  TBranch	*b_t1pfmetPhi;
  TBranch	*b_t1pfmetSumEt;
  TBranch	*b_pfmet;
  TBranch	*b_pfmetPhi;
  TBranch	*b_pfmetSumEt;
  TBranch	*b_calomet;
  TBranch	*b_calometPhi;
  TBranch	*b_calometSumEt;
  TBranch	*b_pt1;
  TBranch	*b_pt2;
  TBranch	*b_phi1;
  TBranch	*b_phi2;
  TBranch	*b_eta1;
  TBranch	*b_eta2;
  TBranch	*b_r91;
  TBranch	*b_r92;
  TBranch	*b_phoiso1;
  TBranch	*b_phoiso2;
  TBranch	*b_chiso1;
  TBranch	*b_chiso2;
  TBranch	*b_neuiso1;
  TBranch	*b_neuiso2;
  TBranch	*b_sieie1;
  TBranch	*b_sieie2;
  TBranch	*b_hoe1;
  TBranch	*b_hoe2;
  TBranch	*b_passCHiso1;
  TBranch	*b_passCHiso2;
  TBranch	*b_passNHiso1;
  TBranch	*b_passNHiso2;
  TBranch	*b_passPHiso1;
  TBranch	*b_passPHiso2;
  TBranch	*b_passSieie1;
  TBranch	*b_passSieie2;
  TBranch	*b_passHoe1;
  TBranch	*b_passHoe2;
  TBranch	*b_hltPhoton26Photon16Mass60;
  TBranch	*b_hltPhoton36Photon22Mass15;
  TBranch	*b_hltPhoton42Photon25Mass15;
  TBranch	*b_hltDiphoton30Mass95;
  TBranch	*b_hltDiphoton30Mass70;
  TBranch	*b_hltDiphoton30Mass55;
  TBranch	*b_hltDiphoton30Mass55PV;
  TBranch	*b_hltDiphoton30Mass55EB;

};

#endif
