#include <iostream>
#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TString.h"
#include "TLegend.h"
#include "TPad.h"
#include "THStack.h"
#include "mkPlotsLivia/CMS_lumi.C"

#define NSPECIES 5

void combiner( char * in ){
  TString in_name = in;
  TString fIn[NSPECIES];
  fIn[0] = "DMHtoGG";
  fIn[1] = "WZHtoGG";
  fIn[2] = "QCD";
  fIn[3] = "GJets";
  fIn[4] = "GGH";
  //fIn[0] = "DMHtoGG_nosel"; //signal
  //fIn[1] = "WZHtoGG_nosel"; 
  //fIn[2] = "QCD_nosel";
  //fIn[3] = "GJets_nosel";
  //fIn[4] = "GGH_nosel";

  bool is_r9 = false;
  if (in_name=="r91" || in_name=="r92") is_r9 = true;
  
  overlay(fIn[0], fIn[1], fIn[2], fIn[3], fIn[4], in_name, is_r9);
}
void overlay( const TString In1, const TString In2, const TString In3, const TString In4, const TString In5, const TString name , bool offsetLegend ){
  TString location = "diPhoPlots/ALL/";

  gStyle->SetHistLineWidth(2);
  //gROOT->SetStyle("Plain");
  //gROOT->ProcessLine(".x ./DiphotonStyle.C");
 
  TFile *f1 = new TFile(location+In1+"/diPhotHistos_"+In1+".root");
  TFile *f2 = new TFile(location+In2+"/diPhotHistos_"+In2+".root");
  TFile *f3 = new TFile(location+In3+"/diPhotHistos_"+In3+".root");
  TFile *f4 = new TFile(location+In4+"/diPhotHistos_"+In4+".root");
  TFile *f5 = new TFile(location+In5+"/diPhotHistos_"+In5+".root");

  TH1D *h_1_1 = (TH1D*) f1->Get(name+"_"+In1);
  TH1D *h_1_2 = (TH1D*) f2->Get(name+"_"+In2);
  TH1D *h_1_3 = (TH1D*) f3->Get(name+"_"+In3);
  TH1D *h_1_4 = (TH1D*) f4->Get(name+"_"+In4);
  TH1D *h_1_5 = (TH1D*) f5->Get(name+"_"+In5);

  TH1D *h1[5];
  h1[0]=h_1_1;
  h1[1]=h_1_2;
  h1[2]=h_1_3;
  h1[3]=h_1_4;
  h1[4]=h_1_5;

  THStack *hs = new THStack("hs","stacked histo");

  double max1, max2, max3, max4, max5, realmax, tmpmax;
  max1=h1[0]->GetMaximum();
  max2=h1[1]->GetMaximum();
  max3=h1[2]->GetMaximum();
  max4=h1[3]->GetMaximum();
  max5=h1[4]->GetMaximum();

  realmax = 0.;
  tmpmax = max1;
  if (max2 > tmpmax) tmpmax=max2;
  else{
    if (max3 > tmpmax) tmpmax=max3;
    else{
      if (max4 > tmpmax) tmpmax=max4;
      else{
        if (max5 > tmpmax) tmpmax=max5;
      } 
    }
  }
  realmax=tmpmax;

  Int_t n[NSPECIES];
  Int_t plot[NSPECIES];
  for (int i=0; i<NSPECIES; i++){
    n[i] = h1[i]->GetEntries();
  }

  TCanvas *c1 = new TCanvas("c1","",1200,800);
  c1->cd();
  c1->SetLogy(1);

  h1[0]->SetTitle(name);
   
  h1[1]->SetLineColor(kYellow);
  h1[1]->DrawNormalized();
  
  h1[2]->SetLineColor(kGreen+2);
  h1[2]->DrawNormalized("same");
  
  h1[3]->SetLineColor(kBlue);
  h1[3]->DrawNormalized("same");

  h1[4]->SetLineColor(kViolet);
  h1[4]->DrawNormalized("same");

  h1[0]->SetLineColor(kRed);
  h1[0]->DrawNormalized("same");
  h1[0]->SetMaximum(10*realmax);

  
  CMS_lumi( (TPad*)c1->cd(),true,0);
  
  if( offsetLegend ) TLegend *l1 = new TLegend(0.20,0.73,0.38,0.93);
  else 	             TLegend *l1 = new TLegend(0.65,0.73,0.83,0.93);

  l1->SetTextFont(42);
  l1->SetFillColor(0);
  l1->SetBorderSize(2);
  l1->AddEntry(h1[0],"DMHgg","l");
  l1->AddEntry(h1[1],"WZHtoGG","l");
  l1->AddEntry(h1[2],"QCD","l");
  l1->AddEntry(h1[3],"GJets","l");
  l1->AddEntry(h1[4],"GGH","l");
  l1->Draw();

  c1->SaveAs(location+name+"_comb.png");

  h1[0]->SetFillColor(kRed);
  h1[1]->SetFillColor(kYellow);
  h1[2]->SetFillColor(kGreen);
  h1[3]->SetFillColor(kBlue);
  h1[4]->SetFillColor(kCyan);

  h1[0]->SetLineColor(kBlack);
  h1[1]->SetLineColor(kBlack);
  h1[2]->SetLineColor(kBlack);
  h1[3]->SetLineColor(kBlack);
  h1[4]->SetLineColor(kBlack);
 
  hs->Add(h1[1]);
  hs->Add(h1[2]);
  hs->Add(h1[3]);
  hs->Add(h1[4]);
  

  TCanvas *c2 = new TCanvas("c2","",1200,800);
  c2->cd();
  c2->SetLogy(1);
  hs->Draw();
  h1[0]->Draw("same");
  CMS_lumi( (TPad*)c2->cd(),true,0);

  l1->Clear();
  l1->AddEntry(h1[0],"DMHgg","f");
  l1->AddEntry(h1[1],"WZHtoGG","f");
  l1->AddEntry(h1[2],"QCD","f");
  l1->AddEntry(h1[3],"GJets","f");
  l1->AddEntry(h1[4],"GGH","f");
  l1->Draw();

  c2->SaveAs(location+name+"_stack.png");

}
