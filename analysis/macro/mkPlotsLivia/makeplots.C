#include"TPaveText.h"
#include "TChain.h"
#include "TH1F.h"
#include <vector>
#include <cmath>
#include "TFile.h"
#include "TLegend.h"
#include "TPad.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TStyle.h"
#include <stdio.h>
#include "TCanvas.h"
#include "TPad.h"
#include "TH1F.h"
#include "THStack.h"
#include "TProfile.h"
#include "TH2D.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TFile.h"
#include "TTree.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TColor.h"
#include "TLatex.h"
#include "/afs/cern.ch/user/s/soffi/www/CMS_lumi.C"
using namespace std;


void hdensity( TH1F* hout){

   
  for(int i = 1; i<=hout->GetNbinsX(); i++){
   
      double area = hout->GetBinWidth(i);
      
      hout->SetBinContent(i,(double)hout->GetBinContent(i)/area);
      hout->SetBinError(i,(double)hout->GetBinError(i)/area);

      // std::cout << "i: " << i << " j: " << j <<
	//	" val: " << hout->GetBinContent(i,j) << " area: " << area <<std::endl; 

  }

}



void MakePlotDATAMC(std::string var, std::string xlabel){

  TCanvas* c1 = new TCanvas("c1", "c1", 1);
  c1->cd();
  c1->Divide(1,2);
  c1->cd(1);
  TFile* fin_data = TFile::Open("/afs/cern.ch/work/s/soffi/CMSSW_5_3_26-MonoJet/src/GJet/histos_GJets_DATA.root");
  TFile* fin_bkg = TFile::Open("/afs/cern.ch/work/s/soffi/CMSSW_5_3_26-MonoJet/src/GJet/histos_GJets_MC.root");
 
  TH1F* h_data  =  (TH1F*) fin_data->Get(("h_"+var).c_str());
  TH1F* h_bkg  =  (TH1F*) fin_bkg->Get(("h_"+var).c_str());
 
 
  h_data->Sumw2();
  h_bkg->Sumw2();
  double data_norm = h_data->Integral();
  double bkg_norm = h_bkg->Integral()*19.7*1.33;
  std::cout<<data_norm<< " "<<bkg_norm<< std::endl;
  //h_bkg->Scale( h_data->Integral()/h_bkg->Integral());
  h_bkg->Scale(bkg_norm/h_bkg->Integral());
 
  if(var ==  "phmet" || var ==  "phmetW"){
    hdensity(h_data);
    hdensity(h_bkg);
  std::cout<<"\\begin{table}[bthp]"<<std::endl;
  std::cout<<"\\begin{tabular}{||cc||}"<<std::endl;
  std::cout<<"\\hline"<<std::endl;
  // std::cout<<"MET range  \& $\\gamma$+jets events \\\\"<<td::endl;
  std::cout<<"MET range  \& $Z\\rightarrow \\nu\\nu$ events \\\\"<<td::endl;
  std::cout<<"\\hline"<<std::endl;
  std::cout<<"250--300 \&"<<h_data->Integral(h_data->FindBin(250),h_data->FindBin(300))<<" \\\\"<<std::endl;
  std::cout<<"300--350 \&"<<h_data->Integral(h_data->FindBin(300)+1,h_data->FindBin(350))<<" \\\\"<<std::endl;
  std::cout<<"350--400 \&"<<h_data->Integral(h_data->FindBin(350)+1,h_data->FindBin(400))<<" \\\\"<<std::endl;
  std::cout<<"400--500 \&"<<h_data->Integral(h_data->FindBin(400)+1,h_data->FindBin(500))<<" \\\\"<<std::endl;
    std::cout<<"500--1000 \&"<<h_data->Integral(h_data->FindBin(500)+1,h_data->FindBin(1000))<<" \\\\"<<std::endl;
  std::cout<<"\\hline"<<std::endl;
  std::cout<<"\\end{tabular}\\end{table}"<<std::endl;
  }
  h_bkg->SetLineColor(kAzure+7);
  h_bkg->SetFillColor(kAzure+6);
  h_bkg->SetLineWidth(2);
 
  h_data->Draw("PE");
  h_bkg->Draw("histsame");
  h_bkg->GetXaxis()->SetTitle(xlabel.c_str());
  h_data->SetMarkerSize(0.7);
  h_data->Draw("PEsame");

  TLegend* legmc;
  legmc = new TLegend(0.6, 0.55, 0.82, 0.89, "", "brNDC");
 
  legmc->SetTextFont(42);
  legmc->SetBorderSize(0);
  legmc->SetFillStyle(0);
 
  legmc->AddEntry(h_data, "Data", "L");
  legmc->AddEntry(h_bkg, "#gamma + jet", "L");
  legmc->Draw("same");

  CMS_lumi( (TPad*)c1->cd(1),true,0);

  c1->cd(2);

  TH1F* hs = (TH1F*) h_data->Clone();
  hs->Divide(h_bkg);
  hs->Draw("pe");
  hs->GetXaxis()->SetTitle(xlabel.c_str()); 
  hs->GetYaxis()->SetTitle("Data/MC");
  hs->GetYaxis()->SetRangeUser(0., 2.);
 
  TLine line(hs->GetBinCenter(1),1,hs->GetBinCenter(hs->GetNbinsX()+1),1);
  line.SetLineColor(kRed);
  line.Draw("same");
  
  int iPos=0 ;
  CMS_lumi( (TPad*)c1->cd(2),true,iPos );
 
  c1->SaveAs(("~/www/MonoJet/MC-DATA_"+var+"_cat.png").c_str());
  c1->SaveAs(("~/www/MonoJet/MC-DATA_"+var+"_cat.pdf").c_str());
  c1->cd(1)->SetLogy();
  c1->SaveAs(("~/www/MonoJet/MC-DATA_"+var+"_LOG_cat.png").c_str());
  c1->SaveAs(("~/www/MonoJet/MC-DATA_"+var+"_LOG_cat.pdf").c_str());

}

void MakeAllPlotDATAMC(){
  MakePlotDATAMC("phpt", "P_{T} [GeV]");
  MakePlotDATAMC("nvtx", "Number of primary vertices");
  /*
  MakePlotDATAMC("nph", "Number of photons");
  MakePlotDATAMC("njet", "Number of jets");
  MakePlotDATAMC("phmet", "Photon MET [GeV]");*/
  MakePlotDATAMC("phmet", "PF MET [GeV]");
  MakePlotDATAMC("phmetW", "PF MET Weighted [GeV]");
  
}


void Make2DEffAcc(bool isEff){

TCanvas* c1 = new TCanvas("c1", "c1", 1);
  c1->cd();
  TFile* fin = TFile::Open("/afs/cern.ch/work/s/soffi/CMSSW_5_3_26-MonoJet/src/GJet/histos_GJets_MC.root");
 
  TH2F* h_den;
  if(isEff)h_den  =  (TH2F*) fin->Get("h2_Acc");
  else  h_den  =  (TH2F*) fin->Get("h2_den");
  TH2F* h_num;
  if(isEff)h_num  =  (TH2F*) fin->Get("h2_Eff");
  else  h_num  =  (TH2F*) fin->Get("h2_Acc");
  TH2F* h_ratio = (TH2F*) h_num->Clone();
  h_ratio->Divide(h_den);
  h_ratio->GetXaxis()->SetTitle("#eta (#gamma)");
  h_ratio->GetYaxis()->SetTitle("p_{T} (#gamma) [GeV]");
  // h_ratio->GetZaxis()->SetRangeUser(0.6, 1.);
  TLegend* legmc;
  legmc = new TLegend(0.6, 0.55, 0.82, 0.89, "", "brNDC"); 
  legmc->SetTextFont(42);
  legmc->SetBorderSize(0);
  legmc->SetFillStyle(0); 

 
  gStyle->SetPaintTextFormat("4.2f");
  h_ratio->Draw("COLZTEXT");
 CMS_lumi(c1,true,0);
 if(isEff){
   c1->SaveAs("~/www/MonoJet/2DEff.png");
   c1->SaveAs("~/www/MonoJet/2DEff.pdf");
}else{
  c1->SaveAs("~/www/MonoJet/2DAcc.png");
  c1->SaveAs("~/www/MonoJet/2DAcc.pdf");


 }
}
