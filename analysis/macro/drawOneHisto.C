//-------------------------------------------------------------------------
//  
//  To RUN: specify SAMPLE (Hgg, QCD, GJets) and VARIABLE to be plotted
//  also the NBINS, and START and STOP values for the histogram
//
//  root -l 'drawOneHisto.C("SAMPLE","VARIABLE",NBINS,START,STOP)'
//
//  Assumes that TTree file is:
//  ./data/starting/diPhotons_SAMPLE.root
//
//  Outputs a ROOT file "diPhotHistos_VARIABLE_SAMPLE.root"
//  and the variable plotted regularly and in log form sent to the 
//  directory:
//  ./diPhoPlots/SAMPLE/
//
//  Author: Margaret Zientek, mez34@cornell.edu
//
//-------------------------------------------------------------------------

#include "TMath.h"
#include "TTree.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TFile.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TPad.h"
#include "mkPlotsLivia/CMS_lumi.C"
#include <iostream>

void drawOneHisto( char * in_suffix, char * in_varname , int in_nbins, int in_min, int in_max){
  TString suffix(in_suffix);
  TString varname(in_varname);

  gROOT->SetStyle("Plain");
  gROOT->ProcessLine(".x ./DiphotonStyle.C");

  // open file and tree
  TFile *f = new TFile("data/starting/diPhotons_"+suffix+".root");
  TTree *tev = (TTree*)f->Get("diPhoAna");
  TTree *tpho = (TTree*)f->Get("diPhoAna/DiPhotonTree");
  // create output file
  TFile *fOut = new TFile("diPhotHistos_"+varname+"_"+suffix+".root","RECREATE");
  
  // variables
  float variable;
  
  // point to corresponding part of tree
  tpho->SetBranchAddress(varname,&variable);


  // make histograms
  TH1F *h = new TH1F(varname+"_"+suffix,varname+"_"+suffix,in_nbins,in_min,in_max);


  int nphotons = (int)tpho->GetEntries();
    for (int i=0; i<nphotons; i++){
       tpho->GetEntry(i);
       h->Fill(variable);
    } // loop over photons

  float realmax = h->GetMaximum();

  fOut->cd();
  //make two TCanvas one regular one logy 
  TCanvas* c1 = new TCanvas("c1","",1200,800);
  c1->SetLogy(0);
  h->DrawNormalized();
  h->SetMaximum(10*realmax);
  CMS_lumi( (TPad*)c1->cd(),true,0);
  
  TCanvas* c2 = new TCanvas("c2","",1200,800);
  c2->SetLogy(1);
  h->DrawNormalized();
  h->SetMaximum(10*realmax);
  CMS_lumi( (TPad*)c2->cd(),true,0);

  c1->SaveAs("diPhoPlots/"+suffix+"/"+varname+"_"+suffix+".png");
  c2->SaveAs("diPhoPlots/"+suffix+"/"+varname+"_"+suffix+"_log.png");

  fOut->Write();
  fOut->Close();

}




