#include "TMath.h"
#include "TTree.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TFile.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "mkPlotsLivia/CMS_lumi.h"
#include "mkPlotsLivia/CMS_lumi.C"

#include <iostream>

#define NSPECIES 1
#define NVARIABLES 30
#define N2DVARIABLES 12

void plotter( char * name ){
//  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1110);
  gStyle->SetStatBorderSize(0);
  gStyle->SetStatFont(42);
  gStyle->SetStatX(0.84);  
  gStyle->SetStatY(0.93);
//  gROOT->ProcessLine(".x ./DiphotonStyle.C");

  TString suffix[NSPECIES];
  suffix[0]=name;

  TString infile = "ALL";

  // open file and tree
  TFile *f = new TFile("data/"+infile+"/diPhotons_"+suffix[0]+".root");
  TTree *tev = (TTree*)f->Get("diPhoAna");
  TTree *tpho = (TTree*)f->Get("DiPhotonTree");
  // create output file
  TFile *fOut = new TFile("diPhoPlots/"+infile+"/"+suffix[0]+"/diPhotHistos_"+suffix[0]+".root","RECREATE");
  
  // variables
  float variable[NVARIABLES];
  int intvariable[NVARIABLES];
  
  // initialize the variables
  for(int i=0; i<NVARIABLES; i++){
    variable[i] = -1000.;
    intvariable[i] = -1000;
  }
  
  TString varname[NVARIABLES];
  varname[0]="mgg";
  varname[1]="pt1";
  varname[2]="r91";
  varname[3]="sieie1";
  varname[4]="hoe1";
  varname[5]="chiso1";
  varname[6]="phoiso1";
  varname[7]="neuiso1";
  varname[8]="eleveto1";
  varname[9]="pt2";
  varname[10]="r92";
  varname[11]="sieie2";
  varname[12]="hoe2";
  varname[13]="chiso2";
  varname[14]="phoiso2";
  varname[15]="neuiso2";
  varname[16]="eleveto2";
  varname[17]="t1pfmet";
  varname[18]="weight";
  varname[19]="ptgg";
  varname[20]="t1pfmetPhi";
  varname[21]="phi1";
  varname[22]="phi2";
  varname[23]="eta1";
  varname[24]="eta2";
  varname[25]="presel1";
  varname[26]="presel2";
  varname[27]="sel1";
  varname[28]="sel2";
  varname[29]="nvtx";

  int nbins[NVARIABLES];
  nbins[0]=60; 		// mgg
  nbins[1]=50; 		// pt1
  nbins[2]=100; 	// r91
  nbins[3]=300; 	// sieie1
  nbins[4]=250; 	// hoe1
  nbins[5]=150; 	// chiso1
  nbins[6]=150; 	// phoiso1
  nbins[7]=150; 	// neuiso1
  nbins[8]=100; 	// eleveto1
  nbins[9]=nbins[1];  	// pt2
  nbins[10]=nbins[2]; 	// r92
  nbins[11]=nbins[3]; 	// sieie2
  nbins[12]=nbins[4]; 	// hoe2
  nbins[13]=nbins[5]; 	// chiso2
  nbins[14]=nbins[6]; 	// phoiso2
  nbins[15]=nbins[7]; 	// neuiso2
  nbins[16]=nbins[8]; 	// eleveto2
  nbins[17]=100;     	// t1pfmet
  nbins[18]=100; 	// weight
  nbins[19]=100;	// ptgg
  nbins[20]=80;		// t1pfmetphi
  nbins[21]=nbins[20];	// phi1
  nbins[22]=nbins[20];  // phi2
  nbins[23]=100;	// eta1
  nbins[24]=nbins[23];	// eta2
  nbins[25]=2;		// presel1
  nbins[26]=nbins[25];	// presel2
  nbins[27]=nbins[25];	// sel1
  nbins[28]=nbins[25];	// sel2
  nbins[29]=60;		// nvtx

  float min[NVARIABLES];
  min[0]=0.; 		// mgg
  min[1]=0.; 		// pt1
  min[2]=0.; 		// r91
  min[3]=0.; 		// sieie1
  min[4]=0.; 		// hoe1
  min[5]=-5.; 		// chiso1
  min[6]=-5.; 		// phoiso1
  min[7]=-5.; 		// neuiso1
  min[8]=-1.; 		// eleveto1
  min[9]=min[1]; 	// pt2
  min[10]=min[2]; 	// r92
  min[11]=min[3]; 	// sieie2
  min[12]=min[4]; 	// hoe2
  min[13]=min[5]; 	// chiso2
  min[14]=min[6]; 	// phoiso2
  min[15]=min[7]; 	// neuiso2
  min[16]=min[8]; 	// eleveto2
  min[17]=0.; 		// t1pfmet
  min[18]=0.; 		// weight
  min[19]=0.;		// ptgg
  min[20]=-4.;		// t1pfmetphi
  min[21]=min[20];	// phi1
  min[22]=min[20];	// phi2
  min[23]=-5.;		// eta1
  min[24]=min[23];	// eta2
  min[25]=0.;		// presel1
  min[26]=min[25];	// presel2
  min[27]=min[25];	// sel1
  min[28]=min[25];	// sel2
  min[29]=0.;		// nvtx

  float max[NVARIABLES];
  max[0]=300.; 		// mgg
  max[1]=500.; 		// pt1
  max[2]=1.1; 		// r91
  max[3]=0.03;	 	// sieie1
  max[4]=0.025;	 	// hoe1
  max[5]=15.; 		// chiso1
  max[6]=15.; 		// phoiso1
  max[7]=15.; 		// neuiso1
  max[8]=1.; 		// eleveto1
  max[9]=max[1]; 	// pt2
  max[10]=max[2]; 	// r92
  max[11]=max[3]; 	// sieie2
  max[12]=max[4]; 	// hoe2
  max[13]=max[5]; 	// chiso2
  max[14]=max[6]; 	// phoiso2
  max[15]=max[7]; 	// neuiso2
  max[16]=max[8]; 	// eleveto2
  max[17]=1000.; 	// t1pfmet
  max[18]=100.;		// weight
  max[19]=1000.;	// ptgg
  max[20]=4.;		// t1pfmetphi
  max[21]=max[20];	// phi1
  max[22]=max[20];	// phi2
  max[23]=-5.;		// eta1
  max[24]=max[23];	// eta2
  max[25]=2.;		// presel1
  max[26]=max[25];	// presel2
  max[27]=max[25];	// sel1
  max[28]=max[25];	// sel2
  max[29]=60.;		// nvtx

  TString xaxisLabel[NVARIABLES];
  xaxisLabel[0]="m(#gamma#gamma) [GeV]";
  xaxisLabel[1]="p_{T}(#gamma1) [GeV]";
  xaxisLabel[2]="R9(#gamma1)";
  xaxisLabel[3]="#sigma_{i#eta i#eta}(#gamma1)";
  xaxisLabel[4]="H/E(#gamma1)";
  xaxisLabel[5]="CHiso(#gamma1)";
  xaxisLabel[6]="PHOiso(#gamma1)";
  xaxisLabel[7]="NEUiso(#gamma1)";
  xaxisLabel[8]="ele veto(#gamma1)";
  xaxisLabel[9]="p_{T}(#gamma2) [GeV]";
  xaxisLabel[10]="R9(#gamma2)";
  xaxisLabel[11]="#sigma_{i#eta i#eta}(#gamma2)";
  xaxisLabel[12]="H/E(#gamma2)";
  xaxisLabel[13]="CHiso(#gamma2)";
  xaxisLabel[14]="PHOiso(#gamma2)";
  xaxisLabel[15]="NEUiso(#gamma2)";
  xaxisLabel[16]="ele veto(#gamma2)";
  xaxisLabel[17]="type 1 PF MET(#gamma #gamma)";
  xaxisLabel[18]="weight";  
  xaxisLabel[19]="p_{T}(#gamma#gamma)";
  xaxisLabel[20]="type 1 PF MET #phi";
  xaxisLabel[21]="#phi(#gamma1)";
  xaxisLabel[22]="#phi(#gamma2)";
  xaxisLabel[23]="#eta(#gamma1)";
  xaxisLabel[24]="#eta(#gamma2)";
  xaxisLabel[25]="presel(#gamma1)";
  xaxisLabel[26]="presel(#gamma2)";
  xaxisLabel[27]="sel(#gamma1)";
  xaxisLabel[28]="sel(#gamma2)";
  xaxisLabel[29]="nvtx";

  // make histograms
  TH1F *h[NVARIABLES];
  for (int z=0; z<NVARIABLES; ++z){
     h[z] = new TH1F(varname[z]+"_"+suffix[0],varname[z]+"_"+suffix[0],nbins[z],min[z],max[z]);
     if(z==8 || z==16 || z==25 || z==26 || z==27 || z==28 || z==29){ //eleveto, sel, nvtx
       tpho->SetBranchAddress(varname[z],&intvariable[z]);
     }
     else{
       tpho->SetBranchAddress(varname[z],&variable[z]);
     }
  }// one histogram for each variable loop

  TString phivar[3];
  phivar[0]="higgs";
  phivar[1]="t1pfmet";
  phivar[2]="higgs_met";
  
  // extra histograms
  TH1F *hphi[3];
  for (int z=0; z<3; ++z){
    hphi[z]=new TH1F("phi_"+phivar[z]+"_"+suffix[0],"phi_"+phivar[z]+"_"+suffix[0],80,-4,4);
  }

  // 2D histograms to compare variables v. PU,pt,eta
  TH2F *hvPU[N2DVARIABLES];
  TH2F *hvPt[N2DVARIABLES];
  TH2F *hvEta[N2DVARIABLES];
  TH1F *hPHIDEffvPU;
  TH1F *hPHIDEffvPt;
  TH1F *hPHIDEffvEta;

  float evar[N2DVARIABLES];

  TString effvar[N2DVARIABLES];
  effvar[0]="hoe1";
  effvar[1]="hoe2";
  effvar[2]="r91";
  effvar[3]="r92";
  effvar[4]="sieie1";
  effvar[5]="sieie2";
  effvar[6]="chiso1";
  effvar[7]="chiso2";
  effvar[8]="neuiso1";
  effvar[9]="neuiso2";
  effvar[10]="phoiso1";
  effvar[11]="phoiso2";

  for (int z=0; z<N2DVARIABLES; ++z){
     tpho->SetBranchAddress(effvar[z],&evar[z]);
  }

  int nb[N2DVARIABLES];
  nb[0]=nbins[4];
  nb[1]=nbins[4];
  nb[2]=nbins[2];
  nb[3]=nbins[2];
  nb[4]=nbins[3];
  nb[5]=nbins[3];
  nb[6]=nbins[5];
  nb[7]=nbins[5];
  nb[8]=nbins[7];
  nb[9]=nbins[7];
  nb[10]=nbins[6];
  nb[11]=nbins[6];

  int mi[N2DVARIABLES];
  mi[0]=min[4];
  mi[1]=min[4];
  mi[2]=min[2];
  mi[3]=min[2];
  mi[4]=min[3];
  mi[5]=min[3];
  mi[6]=min[5];
  mi[7]=min[5];
  mi[8]=min[7];
  mi[9]=min[7];
  mi[10]=min[6];
  mi[11]=min[6];

  int ma[N2DVARIABLES];
  ma[0]=max[4];
  ma[1]=max[4];
  ma[2]=max[2];
  ma[3]=max[2];
  ma[4]=max[3];
  ma[5]=max[3];
  ma[6]=max[5];
  ma[7]=max[5];
  ma[8]=max[7];
  ma[9]=max[7];
  ma[10]=max[6];
  ma[11]=max[6];

  for (int z=0; z<N2DVARIABLES; ++z){
    hvPU[z]  = new TH2F(effvar[z]+"_PU_"+suffix[0],effvar[z]+"_PU_"+suffix[0],60,0,60,nb[z],mi[z],ma[z]);
    hvPt[z]  = new TH2F(effvar[z]+"_pt_"+suffix[0],effvar[z]+"_pt_"+suffix[0],50,0,500,nb[z],mi[z],ma[z]);
    hvEta[z] = new TH2F(effvar[z]+"_eta_"+suffix[0],effvar[z]+"_eta_"+suffix[0],60,-3,3,nb[z],mi[z],ma[z]);
  }
  hPHIDEffvPU  = new TH1F("PHIDEff_PU_"+suffix[0],"PHIDEff_PU_"+suffix[0],60,0,60);
  hPHIDEffvPt  = new TH1F("PHIDEff_pt_"+suffix[0],"PHIDEff_pt_"+suffix[0],60,0,600);
  hPHIDEffvEta = new TH1F("PHIDEff_eta_"+suffix[0],"PHIDEff_eta_"+suffix[0],60,-3,3);


  double phihiggs;

  int nphotons = (int)tpho->GetEntries();


  int totEffnum=0;
  int PUnum1[60]=0;
  int PTnum1[60]=0;
  int ETAnum1[60]=0;
  int PUden1[60]=0;
  int PTden1[60]=0;
  int ETAden1[60]=0;
  int PUnum2[60]=0;
  int PTnum2[60]=0;
  int ETAnum2[60]=0;
  int PUden2[60]=0;
  int PTden2[60]=0;
  int ETAden2[60]=0;

  for (int i=0; i<nphotons; i++){
    tpho->GetEntry(i);
     //if (variable[1] > variable[0]/3 && variable[9] > variable[0]/4)
    { // pt1 > mgg/3 & pt2 > mgg/4 selection

      if (intvariable[27]==1 && intvariable[28]==1) totEffnum++;

      for (int x=0; x<60; x++){
	if (variable[1] >= 10*x && variable[1] < 10*(x+1)){ // pt bins = 10GeV
	  PTden1[x]++; // number of pho1 in that pt range
          if (intvariable[27]==1) PTnum1[x]++; // num pho1 passing 
        }
	if (variable[9] >= 10*x && variable[9] < 10*(x+1)){ // pt bins = 10GeV
	  PTden2[x]++; // number of pho2 in that pt range
          if (intvariable[28]==1) PTnum2[x]++; // num pho2 passing 
        }
	if (intvariable[29] == x){ // evnts with nvtx = x 
	  PUden1[x]++;
	  PUden2[x]++;
	  if (intvariable[27]==1) PUnum1[x]++; // num pho1 passing
	  if (intvariable[28]==1) PUnum2[x]++; // num pho2 passing
        }
	if (variable[23] >= (-3+x*(1/10)) && variable[23]< (-3+(x+1)*(1/10))){ // eta bins = 1/10
	  ETAden1[x]++; // number of pho1 in that eta range
	  if (intvariable[27]==1) ETAnum1[x]++; // num pho1 passing
        }
	if (variable[24] >= (-3+x*(1/10)) && variable[24]< (-3+(x+1)*(1/10))){ // eta bins = 1/10
	  ETAden2[x]++; // number of pho2 in that eta range
	  if (intvariable[28]==1) ETAnum2[x]++; // num pho2 passing
        }
      }

      for (int z=0; z<N2DVARIABLES; z++){
	hvPU[z]->Fill(intvariable[29],evar[z],variable[18]);
	
        if (z==0 || z==2 || z==4 || z==6 || z==8 || z==10){ //plots for pho1
	  hvPt[z]->Fill(variable[1],evar[z],variable[18]);
	  hvEta[z]->Fill(variable[23],evar[z],variable[18]);
	}
	else{// plots for pho2
	  hvPt[z]->Fill(variable[9],evar[z],variable[18]);
	  hvEta[z]->Fill(variable[24],evar[z],variable[18]);
	}
      }

      phihiggs=TMath::ATan( (variable[1]*TMath::Sin(variable[21]) - variable[9]*TMath::Sin(variable[22])) / (variable[1]*TMath::Cos(variable[21]) - variable[9]*TMath::Cos(variable[22])) );
      hphi[0]->Fill(phihiggs,variable[18]);
      hphi[1]->Fill(variable[20],variable[18]);
      hphi[2]->Fill(phihiggs-variable[20],variable[18]);

      for (int z=0; z<NVARIABLES; z++){
        if (z==8 || z==16 || z==25 || z==26 || z==27 || z==28 || z==29){ // eleveto, sel, nvtx
          h[z]->Fill(intvariable[z],variable[18]);
        }
        else{
          h[z]->Fill(variable[z],variable[18]);
        }

      }// loop over variables
    }// loose pt selection
  }// loop over photons
 
  float effPU[60];
  float effPt[60];
  float effEta[60];

  for (x=0; x<60; x++){
    if(PUden1[x]+PUden2[x]==0) effPU[x]=0;
    else  effPU[x]  = (PUnum1[x]+PUnum2[x])/(PUden1[x]+PUden2[x]);
    if(PTden1[x]+PTden2[x]==0) effPt[x]=0;
    else effPt[x]  = (PTnum1[x]+PTnum2[x])/(PTden1[x]+PTden2[x]);
    if(ETAden1[x]+ETAden2[x]==0) effEta[x]=0;
    else effEta[x] = (ETAnum1[x]+ETAnum2[x])/(ETAden1[x]+ETAden2[x]);
   
    std::cout <<"x: "<<x<<" effPU: "<< effPU[x] <<" effPt: "<<effPt[x]<<" effEta[x]: "<<effEta[x]<<std::endl;
    std::cout <<"punum "<< PUnum1[x] <<" + "<< PUnum2[x] << std::endl;
    std::cout <<"puden "<< PUden1[x] <<" + "<< PUden2[x] << std::endl;
    std::cout <<"ptnum "<< PTnum1[x] <<" + "<< PTnum2[x] << std::endl;
    std::cout <<"ptden "<< PTden1[x] <<" + "<< PTden2[x] << std::endl;
    std::cout <<"etanum "<< ETAnum1[x] <<" + "<< ETAnum2[x] << std::endl;
    std::cout <<"etaden "<< ETAden1[x] <<" + "<< ETAden2[x] << std::endl;
  

  
    hPHIDEffvPU->Fill(x, effPU[x]);
    hPHIDEffvPt->Fill(x*10, effPt[x]);
    hPHIDEffvEta->Fill(-3+(x*(1/10)), effEta[x]);
  }
   
  double max1;
  double maxphi;

  fOut->cd();
 
  for (int z=0; z<3; z++){
    TCanvas* c3 = new TCanvas("c3","",1200,800);
    c3->SetLogy(0);
    hphi[z]->DrawNormalized();
    maxphi = hphi[z]->GetMaximum();
    hphi[z]->SetMaximum(10*maxphi);
    hphi[z]->GetXaxis()->SetTitle(phivar[z]);
    CMS_lumi( (TPad*)c3->cd(),true,0);

    TCanvas* c4 = new TCanvas("c4","",1200,800);
    c4->SetLogy(1);
    hphi[z]->DrawNormalized();
    maxphi = hphi[z]->GetMaximum();
    hphi[z]->SetMaximum(10*maxphi);
    hphi[z]->GetXaxis()->SetTitle(phivar[z]);
    CMS_lumi( (TPad*)c4->cd(),true,0);
  
    c3->SaveAs("diPhoPlots/"+infile+"/"+suffix[0]+"/phi_"+phivar[z]+"_"+suffix[0]+".png"); 
    c4->SaveAs("diPhoPlots/"+infile+"/"+suffix[0]+"/phi_"+phivar[z]+"_"+suffix[0]+"_log.png"); 
  }

  for (int z=0; z<NVARIABLES; z++){
    //make two TCanvas one regular one logy 
    TCanvas* c1 = new TCanvas("c1","",1200,800);
    c1->SetLogy(0);
    h[z]->DrawNormalized();
    max1 = h[z]->GetMaximum();
    h[z]->SetMaximum(10*max1);
    h[z]->GetXaxis()->SetTitle(xaxisLabel[z]);
    CMS_lumi( (TPad*)c1->cd(),true,0);   
 
    TCanvas* c2 = new TCanvas("c2","",1200,800);
    c2->SetLogy(1);
    h[z]->DrawNormalized();
    h[z]->SetMaximum(10*max1);
    h[z]->GetXaxis()->SetTitle(xaxisLabel[z]);
    CMS_lumi( (TPad*)c2->cd(),true,0);
   

    c1->SaveAs("diPhoPlots/"+infile+"/"+suffix[0]+"/"+varname[z]+"_"+suffix[0]+".png");
    c2->SaveAs("diPhoPlots/"+infile+"/"+suffix[0]+"/"+varname[z]+"_"+suffix[0]+"_log.png");
  }// loop over variables


  for (int z=0; z<N2DVARIABLES; z++){
    TCanvas* c5 = new TCanvas("c5","",1200,800);
    c5->SetLogy(0);
    c5->SetLogx(0);
    hvPU[z]->Draw("colz");
    hvPU[z]->GetXaxis()->SetTitle("nvtx"); 
    hvPU[z]->GetYaxis()->SetTitle(effvar[z]);
    CMS_lumi( (TPad*)c5->cd(),true,0);   
 
    TCanvas* c6 = new TCanvas("c6","",1200,800);
    c6->SetLogy(1);
    c6->SetLogx(1);
    hvPU[z]->Draw("colz");
    hvPU[z]->GetXaxis()->SetTitle("nvtx"); 
    hvPU[z]->GetYaxis()->SetTitle(effvar[z]);
    CMS_lumi( (TPad*)c6->cd(),true,0);

    c5->SaveAs("diPhoPlots/"+infile+"/"+suffix[0]+"/"+effvar[z]+"_PU_"+suffix[0]+".png");
    c6->SaveAs("diPhoPlots/"+infile+"/"+suffix[0]+"/"+effvar[z]+"_PU_"+suffix[0]+"_log.png");
  }// loop over 2D variables v PU


  for (int z=0; z<N2DVARIABLES; z++){
    TCanvas* c7 = new TCanvas("c7","",1200,800);
    c7->SetLogy(0);
    c7->SetLogx(0);
    hvPt[z]->Draw("colz");
    hvPt[z]->GetXaxis()->SetTitle("p_{T}"); 
    hvPt[z]->GetYaxis()->SetTitle(effvar[z]);
    CMS_lumi( (TPad*)c7->cd(),true,0);   
 
    TCanvas* c8 = new TCanvas("c8","",1200,800);
    c8->SetLogy(1);
    c8->SetLogx(1);
    hvPt[z]->Draw("colz");
    hvPt[z]->GetXaxis()->SetTitle("p_{T}");
    hvPt[z]->GetYaxis()->SetTitle(effvar[z]);
    CMS_lumi( (TPad*)c8->cd(),true,0);
   
    c7->SaveAs("diPhoPlots/"+infile+"/"+suffix[0]+"/"+effvar[z]+"_pt_"+suffix[0]+".png");
    c8->SaveAs("diPhoPlots/"+infile+"/"+suffix[0]+"/"+effvar[z]+"_pt_"+suffix[0]+"_log.png");
  }// loop over 2D variables v Pt


  for (int z=0; z<N2DVARIABLES; z++){
    TCanvas* c9 = new TCanvas("c9","",1200,800);
    c9->SetLogy(0);
    c9->SetLogx(0);
    hvEta[z]->Draw("colz");
    hvEta[z]->GetXaxis()->SetTitle("eta");
    hvEta[z]->GetYaxis()->SetTitle(effvar[z]);
    CMS_lumi( (TPad*)c9->cd(),true,0);   
 
    TCanvas* c10 = new TCanvas("c10","",1200,800);
    c10->SetLogy(1);
    c10->SetLogx(1);
    hvEta[z]->Draw("colz");
    hvEta[z]->GetXaxis()->SetTitle("eta");
    hvEta[z]->GetYaxis()->SetTitle(effvar[z]);
    CMS_lumi( (TPad*)c10->cd(),true,0);
   

    c9->SaveAs("diPhoPlots/"+infile+"/"+suffix[0]+"/"+effvar[z]+"_eta_"+suffix[0]+".png");
    c10->SaveAs("diPhoPlots/"+infile+"/"+suffix[0]+"/"+effvar[z]+"_eta_"+suffix[0]+"_log.png");
  }// loop over 2D variables v Eta 

    int maxeff;

    TCanvas* c11 = new TCanvas("c11","",1200,800);
    c11->SetLogy(0);
    hPHIDEffvPU->DrawNormalized();
    hPHIDEffvPU->SetMarkerStyle(3);
    maxeff = hPHIDEffvPt->GetMaximum();
    hPHIDEffvPU->SetMaximum(10*maxeff);
    hPHIDEffvPU->GetYaxis()->SetTitle("Efficiency");
    hPHIDEffvPU->GetXaxis()->SetTitle("PU");
    CMS_lumi( (TPad*)c11->cd(),true,0);

    TCanvas* c12 = new TCanvas("c12","",1200,800);
    c12->SetLogy(1);
    hPHIDEffvPU->DrawNormalized();
    hPHIDEffvPU->SetMaximum(10*maxeff);
    hPHIDEffvPU->GetYaxis()->SetTitle("Efficiency");
    hPHIDEffvPU->GetXaxis()->SetTitle("PU");
    CMS_lumi( (TPad*)c12->cd(),true,0);
  
    c11->SaveAs("diPhoPlots/"+infile+"/"+suffix[0]+"/PhoIDeff_PU_"+suffix[0]+".png"); 
    c12->SaveAs("diPhoPlots/"+infile+"/"+suffix[0]+"/PhoIDeff_PU_"+suffix[0]+"_log.png"); 
    
    TCanvas* c11 = new TCanvas("c11","",1200,800);
    c11->SetLogy(0);
    hPHIDEffvPt->Draw();
    hPHIDEffvPt->SetMarkerStyle(3);
    maxeff = hPHIDEffvPt->GetMaximum();
    hPHIDEffvPt->SetMaximum(10*maxeff);
    hPHIDEffvPt->GetYaxis()->SetTitle("Efficiency");
    hPHIDEffvPt->GetXaxis()->SetTitle("p_{T}");
    CMS_lumi( (TPad*)c11->cd(),true,0);

    TCanvas* c12 = new TCanvas("c12","",1200,800);
    c12->SetLogy(1);
    hPHIDEffvPt->Draw();
    hPHIDEffvPt->SetMaximum(10*maxeff);
    hPHIDEffvPt->GetYaxis()->SetTitle("Efficiency");
    hPHIDEffvPt->GetXaxis()->SetTitle("p_{T}");
    CMS_lumi( (TPad*)c12->cd(),true,0);
  
    c11->SaveAs("diPhoPlots/"+infile+"/"+suffix[0]+"/PhoIDeff_pt_"+suffix[0]+".png"); 
    c12->SaveAs("diPhoPlots/"+infile+"/"+suffix[0]+"/PhoIDeff_pt_"+suffix[0]+"_log.png"); 

    TCanvas* c11 = new TCanvas("c11","",1200,800);
    c11->SetLogy(0);
    hPHIDEffvEta->Draw();
    maxeff = hPHIDEffvEta->GetMaximum();
    hPHIDEffvEta->SetMaximum(10*maxeff);
    hPHIDEffvEta->GetYaxis()->SetTitle("Efficiency");
    hPHIDEffvEta->GetXaxis()->SetTitle("#eta");
    CMS_lumi( (TPad*)c11->cd(),true,0);

    TCanvas* c12 = new TCanvas("c12","",1200,800);
    c12->SetLogy(1);
    hPHIDEffvEta->Draw();
    hPHIDEffvEta->SetMaximum(10*maxeff);
    hPHIDEffvEta->GetYaxis()->SetTitle("Efficiency");
    hPHIDEffvEta->GetXaxis()->SetTitle("#eta");
    CMS_lumi( (TPad*)c12->cd(),true,0);
  
    c11->SaveAs("diPhoPlots/"+infile+"/"+suffix[0]+"/PhoIDeff_eta_"+suffix[0]+".png"); 
    c12->SaveAs("diPhoPlots/"+infile+"/"+suffix[0]+"/PhoIDeff_eta_"+suffix[0]+"_log.png"); 



  fOut->Write();
  fOut->Close();
}




