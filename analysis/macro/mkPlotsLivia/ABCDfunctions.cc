


#include "CMS_lumi.C"
using namespace RooFit;
using namespace RooStats ;

static const Int_t NCAT = 4;  // chiara

std::string filePOSTfix="";
double signalScaler=1.00;
std::string metvar= "corrmet";
std::string mggvar= "PhotonsMass";

void AddSigData(RooWorkspace*, Float_t);
void AddBkgData(RooWorkspace*, Float_t);
void MakePlotMETDataMC(RooWorkspace*, Float_t);
void MakePlotNVTXDataMC(RooWorkspace*, Float_t);






RooArgSet* defineVariables() {

  // define variables of the input ntuple //livia
  RooRealVar* PhotonsMass  = new RooRealVar("PhotonsMass", "M(gg)",100, 2000,"GeV");
  RooRealVar* PhotonsMassTrue  = new RooRealVar("PhotonsMassTrue", "M(gg)",100, 2000,"GeV");
  RooRealVar* ph1_eta = new RooRealVar("ph1_eta", "eta(g1)",-10,10,"");
  RooRealVar* ph2_eta = new RooRealVar("ph2_eta", "eta(g2)",-10,10,"");
  RooRealVar* ph1_r9 = new RooRealVar("ph1_r9", "R9(g1)",-10,10,"");
  RooRealVar* ph2_r9 = new RooRealVar("ph2_r9", "R9(g2)",-10,10,"");
  RooRealVar* evweight = new RooRealVar("evweight","Reweightings",0,10,"");
  RooRealVar* nvtx = new RooRealVar("nvtx", "nvtx", 0, 50, "");
  //  RooRealVar* btagCategory = new RooRealVar("btagCategory","event category",0.9,2.1,"") ;
  
  RooArgSet* ntplVars = new RooArgSet(*PhotonsMass, *PhotonsMassTrue, *ph1_eta, *ph2_eta, *ph1_r9, *ph2_r9,  *evweight, *nvtx);
  
  return ntplVars;
}

void runfits(const Float_t mass=150, Bool_t dobands = false) {


  TString fileBaseName(TString::Format("HighMass-hgg.m%.1f", mass));    
  TString fileBkgName(TString::Format("HighMass-hgg.inputbkg_8TeV", mass));
  
  TString card_name("HighMass-hgg_models_Bkg_8TeV_test.rs");
  HLFactory hlf("HLFactory", card_name, false);
  RooWorkspace* w = hlf.GetWs();
 


  Double_t MMIN = 100.; //130 //livia   //100  //180   //270
  Double_t MMAX = 1000.; //450      //livia //200 // 300  //2000
  w->var(metvar.c_str())->setMin(MMIN);
  w->var(metvar.c_str())->setMax(MMAX);

 
  // MakePlotDataMC(w, "PhotonsMass","PhotonsMass>0", 60, 100, 300); //MAKE FIG 2
  //MakePlotDataMC(w, "PhotonsMass","corrmet>100", 60, 100, 300); //MAKE FIG 2
  //MakePlotDataMC(w, "corrmet","PhotonsMass>0", 40, 0, 300); //MAKE FIG 2

  /*MakePlotMCOverlayed(w, "PhotonsMass"); 
  MakePlotMCOverlayed(w, "corrmet"); 
  MakePlotMCOverlayed(w, "ph1_pt"); 
  MakePlotMCOverlayed(w, "ph2_pt"); 
  MakePlotMCOverlayed(w, "ph1_eta"); 
  MakePlotMCOverlayed(w, "ph2_eta");*/ 

  ComputeABCDnumbers(w,"corrmet:PhotonsMass");

  /*MakeDataCard(w,"EFTscalar", "1", 0.071);
  MakeDataCard(w,"EFTscalar", "10", 0.070);
  MakeDataCard(w,"EFTscalar", "100", 0.13);
  MakeDataCard(w,"EFTscalar", "1000", 0.24);*/

  return;
}

//mass overlayed
void MakePlotMCOverlayed(RooWorkspace* w, std::string var) {

  Int_t ncat = NCAT;
  TString inDir = "";
  Int_t c = 4;
  // Luminosity:
  Float_t Lum = 19500.0;  
  RooRealVar lumi("lumi","lumi",Lum);
  w->import(lumi); 
  
  // Variables
  RooArgSet* ntplVars = defineVariables();
  RooRealVar* mgg = w->var(mggvar.c_str());
   
 // common preselection cut
  TString mainCut = "PhotonsMass>10"; 
  RooPlot*  plotPhotonsMassDataMC[NCAT];
  
  //**********DATA***************//
  // create tree
  TFile file("histograms_CMS-HGG_23022015_DATA_monoH.root");
  TTree* dataTree = (TTree*) file.Get("Data");

  //signal M1
  TChain*  sig1 = new TChain();
  sig1->Add("/afs/cern.ch/work/s/soffi/CMSSW620-MonoH/src/h2gglobe/AnalysisScripts/histograms_Scalar8TeV1GeV-HGG.root/ggh_m126_8TeV");


 //signal M10
  TChain*  sig10 = new TChain();
  sig10->Add("/afs/cern.ch/work/s/soffi/CMSSW620-MonoH/src/h2gglobe/AnalysisScripts/histograms_Scalar8TeV10GeV-HGG.root/ggh_m124_8TeV");


 //signal M100
  TChain*  sig100 = new TChain();
  sig100->Add("/afs/cern.ch/work/s/soffi/CMSSW620-MonoH/src/h2gglobe/AnalysisScripts/histograms_Scalar8TeV100-1000GeV-HGG.root/ggh_m127_8TeV");


 //signal M1000
  TChain*  sig1000 = new TChain();
  sig1000->Add("/afs/cern.ch/work/s/soffi/CMSSW620-MonoH/src/h2gglobe/AnalysisScripts/histograms_Scalar8TeV100-1000GeV-HGG.root/ggh_m128_8TeV");

  //Higgs to gamma gamma
  TChain* hggTree = new TChain();
  hggTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/ggh_m125_8TeV");
  hggTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/vbf_m125_8TeV");
  hggTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/tth_m125_8TeV");


  TChain* VhTree = new TChain();
  VhTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/wzh_m125_8TeV");
    
  //**********G+jets***************//
  // create tree

  TChain* gjTree = new TChain();
  gjTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/gjet_20_8TeV_pf");
  gjTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/gjet_40_8TeV_pf");
  // gjTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/gjet_20_8TeV_pp");
  //gjTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/gjet_40_8TeV_pp");

  TChain* qcdTree = new TChain();
  qcdTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/qcd_30_8TeV_pf");
  qcdTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/qcd_40_8TeV_pf"); 
  qcdTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/qcd_30_8TeV_ff");
  qcdTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/qcd_40_8TeV_ff"); 
 
  //**********DIPHOT***************//
  // create tree

  TChain* diphotjTree = new TChain();
  diphotjTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/diphojet_8TeV");
 
  TChain* diphotTree = new TChain();
  diphotTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/dipho_Box_25_8TeV");
  diphotTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/dipho_Box_250_8TeV");
   
  //V+gamma
  TChain* VgTree = new TChain();
  VgTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/ZGToLLG");
  VgTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/WGToLNuG");
  
  //V+jets
  TChain* VJetsTree = new TChain();
  VJetsTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/DYJetsToLL");
  VJetsTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/WJetsToLNu");

  //VV+jets
  TChain* VVJetsTree = new TChain();
  VVJetsTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/WWJetsTo2L2Nu");
  VVJetsTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/WZJetsTo3LNu");
  VVJetsTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/ZZJetsTo2L2Nu");
  VVJetsTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/ZZJetsTo2L2Q");
  VVJetsTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/ZZJetsTo4L");
 
  //V+gamma+gamma
  TChain* VggTree = new TChain();
  VggTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/Wpgg_dR02");
  VggTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/Wmgg_dR02");
  VggTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/Zgg_dR02");

  //tt bkg
  TChain* ttTree = new TChain();
  ttTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/ttgg_dR02");
  // ttTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/TTJets");
  //ttTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/TTGJets");



  TH1F* h_data[NCAT+1];
  TH1F* h_data_b[NCAT+1];
 
  TH1F*  h_gj[NCAT+1];
  TH1F*  h_qcd[NCAT+1];
  TH1F*  h_diphot[NCAT+1];
  TH1F*  h_diphotj[NCAT+1];
 
  TH1F*  h_Vg[NCAT+1];
  TH1F*  h_Vjets[NCAT+1];
  TH1F*  h_Vgg[NCAT+1];
  TH1F*  h_VVjets[NCAT+1];
  TH1F*  h_tt[NCAT+1];
  
  TH1F*  h_hgg[NCAT+1];
  TH1F*  h_Vh[NCAT+1];

  TH1F* h_sig1[NCAT+1];
  TH1F* h_sig10[NCAT+1];
  TH1F* h_sig100[NCAT+1];
  TH1F* h_sig1000[NCAT+1];


  TH1F* h_sum[NCAT+1]; 
 
  
  TCanvas* ctmp = new TCanvas("ctmp","MET Background Categories",1);
  Int_t nbin = 70;
  Double_t  min = 90;
  Double_t  max = 170;
  if(var=="PhotonsMass"){
    nbin = 70;
    min = 90;
    max = 170;
  }else if(var=="corrmet"){
    nbin = 60;
    min = 0;
    max = 300;
  }else if(var=="ph1_pt" || var== "ph2_pt"){
    nbin = 60;
    min = 40;
    max = 350;
  }else if(var=="ph1_eta" || var== "ph2_eta"){
    nbin = 60;
    min = -3;
    max = 3;
  } 
  
     h_data[c]= new TH1F(TString::Format("h_data_cat%d",c), TString::Format("h_data_cat%d",c), nbin, min, max);
     h_data_b[c]= new TH1F(TString::Format("h_data_b_cat%d",c), TString::Format("h_data_b_cat%d",c), nbin, min, max);
     h_gj[c]= new TH1F(TString::Format("h_gj_cat%d",c), TString::Format("h_gj_cat%d",c), nbin, min, max);
     h_qcd[c]= new TH1F(TString::Format("h_qcd_cat%d",c), TString::Format("h_qcd_cat%d",c), nbin, min, max);
     h_diphot[c]= new TH1F(TString::Format("h_diphot_cat%d",c), TString::Format("h_diphot_cat%d",c), nbin, min, max);
     h_diphotj[c]= new TH1F(TString::Format("h_diphotj_cat%d",c), TString::Format("h_diphotj_cat%d",c), nbin, min, max);
     h_Vg[c]= new TH1F(TString::Format("h_Vg_cat%d",c), TString::Format("h_Vg_cat%d",c), nbin, min, max);
     h_Vjets[c]= new TH1F(TString::Format("h_Vjets_cat%d",c), TString::Format("h_Vjets_cat%d",c), nbin, min, max);
     h_Vgg[c]= new TH1F(TString::Format("h_Vgg_cat%d",c), TString::Format("h_Vgg_cat%d",c), nbin, min, max);
     h_VVjets[c]= new TH1F(TString::Format("h_VVjets_cat%d",c), TString::Format("h_VVjets_cat%d",c), nbin, min, max);
     h_tt[c]= new TH1F(TString::Format("h_tt_cat%d",c), TString::Format("h_tt_cat%d",c), nbin, min, max);

     h_hgg[c]= new TH1F(TString::Format("h_hgg_cat%d",c), TString::Format("h_hgg_cat%d",c), nbin, min, max);   
     h_Vh[c]= new TH1F(TString::Format("h_Vh_cat%d",c), TString::Format("h_Vh_cat%d",c), nbin, min, max);

     h_sig1[c]= new TH1F(TString::Format("h_sig1_cat%d",c), TString::Format("h_sig1_cat%d",c), nbin, min, max);
     h_sig10[c]= new TH1F(TString::Format("h_sig10_cat%d",c), TString::Format("h_sig10_cat%d",c), nbin, min, max);
     h_sig100[c]= new TH1F(TString::Format("h_sig100_cat%d",c), TString::Format("h_sig100_cat%d",c), nbin, min, max);
     h_sig1000[c]= new TH1F(TString::Format("h_sig1000_cat%d",c), TString::Format("h_sig1000_cat%d",c), nbin, min, max);


    
     dataTree->Draw((var+">>h_data_cat4").c_str(),"("+ mainCut+")*1");
     dataTree->Draw((var+">>h_data_b_cat4").c_str(),"("+ mainCut+")*1");
     gjTree->Draw((var+">>h_gj_cat4").c_str(), "("+mainCut+")*evweight");
     qcdTree->Draw((var+">>h_qcd_cat4").c_str(), "("+mainCut+")*evweight");
     diphotTree->Draw((var+">>h_diphot_cat4").c_str(),"("+ mainCut+")*evweight");
     diphotjTree->Draw((var+">>h_diphotj_cat4").c_str(),"("+ mainCut+")*evweight");
     VgTree->Draw((var+">>h_Vg_cat4").c_str(), "("+mainCut+")*evweight");
     VJetsTree->Draw((var+">>h_Vjets_cat4").c_str(), "("+mainCut+")*evweight");
     VggTree->Draw((var+">>h_Vgg_cat4").c_str(), "("+mainCut+")*evweight");
     VVJetsTree->Draw((var+">>h_VVjets_cat4").c_str(), "("+mainCut+")*evweight");
     ttTree->Draw((var+">>h_tt_cat4").c_str(), "("+mainCut+")*evweight");

     hggTree->Draw((var+">>h_hgg_cat4").c_str(), "("+mainCut+")*evweight");
     VhTree->Draw((var+">>h_Vh_cat4").c_str(), "("+mainCut+")*evweight");

     sig1->Draw((var+">>h_sig1_cat4").c_str(), "("+mainCut+")*evweight");
     sig10->Draw((var+">>h_sig10_cat4").c_str(), "("+mainCut+")*evweight");
     sig100->Draw((var+">>h_sig100_cat4").c_str(), "("+mainCut+")*evweight");
     sig1000->Draw((var+">>h_sig1000_cat4").c_str(), "("+mainCut+")*evweight");
 
     //gj    
     h_gj[c]->SetLineColor(kAzure+8);
     h_gj[c]->SetLineWidth(2);
     h_gj[c]->Sumw2();
  
     //qcd  
     h_qcd[c]->SetLineColor(kAzure+2);
     h_qcd[c]->SetLineWidth(2);
     h_qcd[c]->Sumw2();
 

     //diphotj    
     h_diphotj[c]->SetLineColor(kTeal+9);
     h_diphotj[c]->SetLineWidth(2);
     h_diphotj[c]->Sumw2();
     
     //diphot
     h_diphot[c]->SetLineColor(kSpring+7);
     h_diphot[c]->SetLineWidth(2);
     h_diphot[c]->Sumw2();
  
     //Vg   
     h_Vg[c]->SetLineColor(kPink+7);
     h_Vg[c]->SetLineWidth(2);
     h_Vg[c]->Sumw2();

     //Vjets
     h_Vjets[c]->SetLineColor(kPink+6);
     h_Vjets[c]->SetLineWidth(2);
     h_Vjets[c]->Sumw2();

     //Vgg
     h_Vgg[c]->SetLineColor(kOrange-3);
     h_Vgg[c]->SetLineWidth(2);
     h_Vgg[c]->Sumw2();
  
     //VVjets   
     h_VVjets[c]->SetLineColor(kOrange-2);
     h_VVjets[c]->SetLineWidth(2);
     h_VVjets[c]->Sumw2();
 
     //tt    
     h_tt[c]->SetLineColor(kOrange-1);
     h_tt[c]->SetLineWidth(2);
     h_tt[c]->Sumw2();


     //higgs
     h_hgg[c]->SetLineColor(kMagenta);
     h_hgg[c]->SetLineWidth(2);
     h_hgg[c]->Sumw2();
 
     h_Vh[c]->SetLineColor(kCyan);
     h_Vh[c]->SetLineWidth(2);
     h_Vh[c]->Sumw2();
 

     //signal
     h_sig1[c]->SetLineColor(kBlack);
     h_sig1[c]->SetFillColor(kBlack);
     h_sig1[c]->SetFillStyle(3004);
     h_sig1[c]->SetLineWidth(2);
     h_sig1[c]->Sumw2();

     h_sig10[c]->SetLineColor(8);
     h_sig10[c]->SetFillColor(8);
     h_sig10[c]->SetFillStyle(3004);
     h_sig10[c]->SetLineWidth(2);
     h_sig10[c]->Sumw2();

     h_sig100[c]->SetLineColor(kRed);
     h_sig100[c]->SetFillColor(kRed);
     h_sig100[c]->SetFillStyle(3004);
     h_sig100[c]->SetLineWidth(2);
     h_sig100[c]->Sumw2();

     h_sig1000[c]->SetLineColor(kBlue);
     h_sig1000[c]->SetFillColor(kBlue);
     h_sig1000[c]->SetFillStyle(3004);
     h_sig1000[c]->SetLineWidth(2);
     h_sig1000[c]->Sumw2();
 
    
     if(var=="PhotonsMass") h_gj[c]->GetXaxis()->SetTitle("m_{#gamma #gamma} [GeV]");
     if(var=="corrmet") h_gj[c]->GetXaxis()->SetTitle("MET [GeV]");
     if(var=="corrmetphi") h_gj[c]->GetXaxis()->SetTitle("MET_{#phi} [GeV]");
     if(var=="ph1_pt") h_gj[c]->GetXaxis()->SetTitle("p_T{#gamma1} [GeV]");
     if(var=="ph2_pt") h_gj[c]->GetXaxis()->SetTitle("p_T{#gamma2} [GeV]");
     if(var=="ph1_eta") h_gj[c]->GetXaxis()->SetTitle("#eta_{#gamma1} ");
     if(var=="ph2_eta") h_gj[c]->GetXaxis()->SetTitle("#eta_{#gamma2} ");

     //
     h_gj[c]->Scale(1./ h_gj[c]->Integral());
     h_gj[c]->GetYaxis()->SetTitle("Fraction of Events");
     h_gj[c]->Draw("hist");
     h_gj[c]->GetYaxis()->SetRangeUser(0.001, 0.5); 

 
     h_Vgg[c]->Add(h_tt[c]);
     h_Vgg[c]->DrawNormalized("histsame");
     
     h_diphot[c]->Add(h_diphotj[c]);
     h_diphot[c]->DrawNormalized("histsame");
   
     h_hgg[c]->DrawNormalized("histsame");
     h_Vh[c]->DrawNormalized("histsame");

     h_sig1[c]->DrawNormalized("histsame");
     h_sig10[c]->DrawNormalized("histsame");
     h_sig100[c]->DrawNormalized("histsame");
     h_sig1000[c]->DrawNormalized("histsame");

    TLegend *leg1;
    leg1 = new TLegend(0.5075,0.576441,0.7875,0.9240678, TString::Format("",c), "brNDC");
   
    leg1->AddEntry(h_diphot[c],"prompt+prompt", "L");
    leg1->AddEntry(h_gj[c],"prompt+fake","L");
    leg1->AddEntry(h_Vgg[c],"W/Z/tt #gamma #gamma","L");
    leg1->AddEntry(h_hgg[c],"H #rightarrow #gamma #gamma","L");
    leg1->AddEntry(h_Vh[c],"Wh,Zh; H #rightarrow #gamma #gamma ","L");

    leg1->AddEntry(h_sig1[c],"#bar{#chi}#chi HH, m_{#chi} = 1 GeV","F");
    leg1->AddEntry(h_sig10[c],"#bar{#chi}#chi HH, m_{#chi} = 10 GeV","F");
    leg1->AddEntry(h_sig100[c],"#bar{#chi}#chi HH, m_{#chi} = 100 GeV","F");
    leg1->AddEntry(h_sig1000[c],"#bar{#chi}#chi HH, m_{#chi} = 1000 GeV","F");

    
    leg1->SetTextSize(0.035);
    leg1->SetTextFont(42);
    leg1->SetBorderSize(0);
    leg1->SetFillStyle(0);
    leg1->Draw("same");
   
    int iPos=11 ;
    CMS_lumi( ctmp,false,iPos );
    
    if(var=="PhotonsMass") {
    ctmp->SaveAs("~/www/plotsMonoH/Norm_MC_MGG_"+TString::Format("cat%d.png", c));
    ctmp->SaveAs("~/www/plotsMonoH/norm_MC_MGG_"+TString::Format("cat%d.pdf", c));  
    ctmp->SetLogy();
    h_gj[c]->GetYaxis()->SetRangeUser(0.001, 0.5); 
    ctmp->SaveAs("~/www/plotsMonoH/Norm_MC_MGG_"+TString::Format("cat%d_LOG.png", c));
    ctmp->SaveAs("~/www/plotsMonoH/Norm_MC_MGG_"+TString::Format("cat%d_LOG.pdf", c));
}
   if(var=="corrmet") {
    ctmp->SaveAs("~/www/plotsMonoH/Norm_MC_MET_"+TString::Format("cat%d.png", c));
    ctmp->SaveAs("~/www/plotsMonoH/norm_MC_MET_"+TString::Format("cat%d.pdf", c));  
    ctmp->SetLogy();
    h_gj[c]->GetYaxis()->SetRangeUser(0.01, 1); 
    ctmp->SaveAs("~/www/plotsMonoH/Norm_MC_MET_"+TString::Format("cat%d_LOG.png", c));
    ctmp->SaveAs("~/www/plotsMonoH/Norm_MC_MET_"+TString::Format("cat%d_LOG.pdf", c));
}
   if(var=="ph1_pt") {
    ctmp->SaveAs("~/www/plotsMonoH/Norm_MC_PT1_"+TString::Format("cat%d.png", c));
    ctmp->SaveAs("~/www/plotsMonoH/norm_MC_PT1_"+TString::Format("cat%d.pdf", c));  
    ctmp->SetLogy();
    h_gj[c]->GetYaxis()->SetRangeUser(0.01, 1); 
    ctmp->SaveAs("~/www/plotsMonoH/Norm_MC_PT1_"+TString::Format("cat%d_LOG.png", c));
    ctmp->SaveAs("~/www/plotsMonoH/Norm_MC_PT1_"+TString::Format("cat%d_LOG.pdf", c));
}
   if(var=="ph2_pt") {
    ctmp->SaveAs("~/www/plotsMonoH/Norm_MC_PT2_"+TString::Format("cat%d.png", c));
    ctmp->SaveAs("~/www/plotsMonoH/norm_MC_PT2_"+TString::Format("cat%d.pdf", c));  
    ctmp->SetLogy();
    h_gj[c]->GetYaxis()->SetRangeUser(0.01, 1); 
    ctmp->SaveAs("~/www/plotsMonoH/Norm_MC_PT2_"+TString::Format("cat%d_LOG.png", c));
    ctmp->SaveAs("~/www/plotsMonoH/Norm_MC_PT2_"+TString::Format("cat%d_LOG.pdf", c));
}
   if(var=="ph1_eta") {
    ctmp->SaveAs("~/www/plotsMonoH/Norm_MC_ETA1_"+TString::Format("cat%d.png", c));
    ctmp->SaveAs("~/www/plotsMonoH/norm_MC_ETA1_"+TString::Format("cat%d.pdf", c));  
    ctmp->SetLogy();
    h_gj[c]->GetYaxis()->SetRangeUser(0.01, 1); 
    ctmp->SaveAs("~/www/plotsMonoH/Norm_MC_ETA1_"+TString::Format("cat%d_LOG.png", c));
    ctmp->SaveAs("~/www/plotsMonoH/Norm_MC_ETA1_"+TString::Format("cat%d_LOG.pdf", c));
}
   if(var=="ph2_eta") {
    ctmp->SaveAs("~/www/plotsMonoH/Norm_MC_ETA2_"+TString::Format("cat%d.png", c));
    ctmp->SaveAs("~/www/plotsMonoH/norm_MC_ETA2_"+TString::Format("cat%d.pdf", c));  
    ctmp->SetLogy();
    h_gj[c]->GetYaxis()->SetRangeUser(0.01, 1); 
    ctmp->SaveAs("~/www/plotsMonoH/Norm_MC_ETA2_"+TString::Format("cat%d_LOG.png", c));
    ctmp->SaveAs("~/www/plotsMonoH/Norm_MC_ETA2_"+TString::Format("cat%d_LOG.pdf", c));
}

}





void MakePlotDataMC(RooWorkspace* w, std::string var,TString mainCut, int NBIN, double MINVAR, double MAXVAR) {

  Int_t ncat = NCAT;
  TString inDir = "";

  // Luminosity:
  Float_t Lum = 19500.0;  
  RooRealVar lumi("lumi","lumi",Lum);
  w->import(lumi); 
  
  // Variables
  RooArgSet* ntplVars = defineVariables();
  RooRealVar* met;
   
 // common preselection cut
 // TString mainCut = "PhotonsMass>1"; //"PhotonsMass>100&&PhotonsMass<130"; //"(PhotonsMass>=100 && PhotonsMass<=130)";   //130-2000
  RooPlot*  plotPhotonsMassDataMC[NCAT];
  
  //**********DATA***************//
  // create tree
  TFile file("histograms_CMS-HGG_23022015_DATA_monoH.root");
  TTree* dataTree = (TTree*) file.Get("Data");
 
//signal M1
  TChain*  sig1 = new TChain();
  sig1->Add("/afs/cern.ch/work/s/soffi/CMSSW620-MonoH/src/h2gglobe/AnalysisScripts/histograms_Scalar8TeV1GeV-HGG.root/ggh_m126_8TeV");


 //signal M10
  TChain*  sig10 = new TChain();
  sig10->Add("/afs/cern.ch/work/s/soffi/CMSSW620-MonoH/src/h2gglobe/AnalysisScripts/histograms_Scalar8TeV10GeV-HGG.root/ggh_m124_8TeV");


 //signal M100
  TChain*  sig100 = new TChain();
  sig100->Add("/afs/cern.ch/work/s/soffi/CMSSW620-MonoH/src/h2gglobe/AnalysisScripts/histograms_Scalar8TeV100-1000GeV-HGG.root/ggh_m127_8TeV");


 //signal M1000
  TChain*  sig1000 = new TChain();
  sig1000->Add("/afs/cern.ch/work/s/soffi/CMSSW620-MonoH/src/h2gglobe/AnalysisScripts/histograms_Scalar8TeV100-1000GeV-HGG.root/ggh_m128_8TeV");


  //Higgs to gamma gamma
  TChain* hggTree = new TChain();
  hggTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/ggh_m125_8TeV");
  hggTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/vbf_m125_8TeV");
  //hggTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/tth_m125_8TeV");


  TChain* VhTree = new TChain();
  VhTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/wzh_m125_8TeV");
  
  //**********G+jets***************//
  // create tree

  TChain* gjTree = new TChain();
  gjTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/gjet_20_8TeV_pf");
  gjTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/gjet_40_8TeV_pf");
  //gjTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/gjet_20_8TeV_pp");
  //gjTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/gjet_40_8TeV_pp");

  TChain* qcdTree = new TChain();
  qcdTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/qcd_30_8TeV_pf");
  qcdTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/qcd_40_8TeV_pf"); 
  // qcdTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/qcd_30_8TeV_ff");
  // qcdTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/qcd_40_8TeV_ff"); 
 
  //**********DIPHOT***************//
  // create tree

  TChain* diphotjTree = new TChain();
  diphotjTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/diphojet_8TeV");
 
  TChain* diphotTree = new TChain();
  diphotTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/dipho_Box_25_8TeV");
  diphotTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/dipho_Box_250_8TeV");
   
  //V+gamma
  TChain* VgTree = new TChain();
  VgTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/ZGToLLG");
  VgTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/WGToLNuG");
  
  //V+jets
  TChain* VJetsTree = new TChain();
  VJetsTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/DYJetsToLL");
  VJetsTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/WJetsToLNu");

  //VV+jets
  TChain* VVJetsTree = new TChain();
  VVJetsTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/WWJetsTo2L2Nu");
  VVJetsTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/WZJetsTo3LNu");
  VVJetsTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/ZZJetsTo2L2Nu");
  VVJetsTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/ZZJetsTo2L2Q");
  VVJetsTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/ZZJetsTo4L");
 
  //V+gamma+gamma
  TChain* VggTree = new TChain();
  VggTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/Wpgg_dR02");
  VggTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/Wmgg_dR02");
  VggTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/Zgg_dR02");

  //tt bkg
  TChain* ttTree = new TChain();
  ttTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/ttgg_dR02");
  // ttTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/TTJets");
  // ttTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/TTGJets");



  TH1F* h_data[NCAT+1];
  TH1F* h_data_b[NCAT+1];
  TH1F*  h_gj[NCAT+1];
  TH1F*  h_qcd[NCAT+1];

  TH1F*  h_diphot[NCAT+1];
  TH1F*  h_diphotj[NCAT+1];
   
  TH1F*  h_Vjets[NCAT+1];

  TH1F*  h_Vgg[NCAT+1]; 
  TH1F*  h_tt[NCAT+1];

  TH1F*  h_hgg[NCAT+1];
  TH1F*  h_Vh[NCAT+1];

  TH1F* h_sig1[NCAT+1];
  TH1F* h_sig10[NCAT+1];
  TH1F* h_sig100[NCAT+1];
  TH1F* h_sig1000[NCAT+1];


  TH1F* h_sum[NCAT+1]; 
  TH1F* h_pp[NCAT+1];
  TH1F* h_pf[NCAT+1];
  TH1F* h_Xgg[NCAT+1];
  TH1F*  h_Vg[NCAT+1];
  TH1F*  h_VVjets[NCAT+1];
 
  Int_t c =4;
  
  TCanvas* ctmp = new TCanvas("ctmp",(var+" Background Categories").c_str(),1);
  Int_t nbin = NBIN;
  Double_t  min = MINVAR;
  Double_t  max = MAXVAR;


     h_data[c]= new TH1F(TString::Format("h_data_cat%d",c), TString::Format("h_data_cat%d",c), nbin, min, max);
     h_data_b[c]= new TH1F(TString::Format("h_data_b_cat%d",c), TString::Format("h_data_b_cat%d",c), nbin, min, max);

     h_gj[c]= new TH1F(TString::Format("h_gj_cat%d",c), TString::Format("h_gj_cat%d",c), nbin, min, max);
     h_qcd[c]= new TH1F(TString::Format("h_qcd_cat%d",c), TString::Format("h_qcd_cat%d",c), nbin, min, max);
     h_diphot[c]= new TH1F(TString::Format("h_diphot_cat%d",c), TString::Format("h_diphot_cat%d",c), nbin, min, max);
     h_diphotj[c]= new TH1F(TString::Format("h_diphotj_cat%d",c), TString::Format("h_diphotj_cat%d",c), nbin, min, max);
     h_Vg[c]= new TH1F(TString::Format("h_Vg_cat%d",c), TString::Format("h_Vg_cat%d",c), nbin, min, max);
     h_Vjets[c]= new TH1F(TString::Format("h_Vjets_cat%d",c), TString::Format("h_Vjets_cat%d",c), nbin, min, max);
     h_Vgg[c]= new TH1F(TString::Format("h_Vgg_cat%d",c), TString::Format("h_Vgg_cat%d",c), nbin, min, max);
     h_VVjets[c]= new TH1F(TString::Format("h_VVjets_cat%d",c), TString::Format("h_VVjets_cat%d",c), nbin, min, max);
     h_tt[c]= new TH1F(TString::Format("h_tt_cat%d",c), TString::Format("h_tt_cat%d",c), nbin, min, max);

     h_hgg[c]= new TH1F(TString::Format("h_hgg_cat%d",c), TString::Format("h_hgg_cat%d",c), nbin, min, max);
     h_Vh[c]= new TH1F(TString::Format("h_Vh_cat%d",c), TString::Format("h_Vh_cat%d",c), nbin, min, max);

     h_sig1[c]= new TH1F(TString::Format("h_sig1_cat%d",c), TString::Format("h_sig1_cat%d",c), nbin, min, max);
     h_sig10[c]= new TH1F(TString::Format("h_sig10_cat%d",c), TString::Format("h_sig10_cat%d",c), nbin, min, max);
     h_sig100[c]= new TH1F(TString::Format("h_sig100_cat%d",c), TString::Format("h_sig100_cat%d",c), nbin, min, max);
     h_sig1000[c]= new TH1F(TString::Format("h_sig1000_cat%d",c), TString::Format("h_sig1000_cat%d",c), nbin, min, max);

 
    
     dataTree->Draw((var+">>h_data_cat4").c_str(),"("+ mainCut+")*1");
     dataTree->Draw((var+">>h_data_b_cat4").c_str(),"("+ mainCut+")*1");
  
     gjTree->Draw((var+">>h_gj_cat4").c_str(), "("+mainCut+")*evweight");
     qcdTree->Draw((var+">>h_qcd_cat4").c_str(), "("+mainCut+")*evweight");
     diphotTree->Draw((var+">>h_diphot_cat4").c_str(),"("+ mainCut+")*evweight");
     diphotjTree->Draw((var+">>h_diphotj_cat4").c_str(),"("+ mainCut+")*evweight");
     VgTree->Draw((var+">>h_Vg_cat4").c_str(), "("+mainCut+")*evweight");
     VJetsTree->Draw((var+">>h_Vjets_cat4").c_str(), "("+mainCut+")*evweight");
     VggTree->Draw((var+">>h_Vgg_cat4").c_str(), "("+mainCut+")*evweight");
     VVJetsTree->Draw((var+">>h_VVjets_cat4").c_str(), "("+mainCut+")*evweight");
     ttTree->Draw((var+">>h_tt_cat4").c_str(), "("+mainCut+")*evweight");

     hggTree->Draw((var+">>h_hgg_cat4").c_str(), "("+mainCut+")*evweight");
     VhTree->Draw((var+">>h_Vh_cat4").c_str(), "("+mainCut+")*evweight");

     sig1->Draw((var+">>h_sig1_cat4").c_str(), "("+mainCut+")*evweight");
     sig10->Draw((var+">>h_sig10_cat4").c_str(), "("+mainCut+")*evweight");
     sig100->Draw((var+">>h_sig100_cat4").c_str(), "("+mainCut+")*evweight");
     sig1000->Draw((var+">>h_sig1000_cat4").c_str(), "("+mainCut+")*evweight");

 
     //fisso la normalizzazione di gjets a quella gj+qcd xkè la shape di qcd _pf fa schifo, quindi prenidamoq uella di gj _pf
     Double_t qcdInt = h_qcd[c]->Integral();
     Double_t gJetIntFixed = qcdInt+h_gj[c]->Integral();
     h_gj[c]->Scale(gJetIntFixed/h_gj[c]->Integral());  
     h_gj[c]->SetFillColor(kAzure+8);
     h_gj[c]->Sumw2();
    
     h_qcd[c]->SetFillColor(kAzure+2);
     h_qcd[c]->Sumw2();
     
     h_diphotj[c]->SetFillColor(kTeal+9);
     h_diphotj[c]->Sumw2();
     
     h_diphot[c]->SetFillColor(kSpring+7);
     h_diphot[c]->Sumw2();
     
     h_Vg[c]->SetFillColor(kPink+7);
     h_Vg[c]->Sumw2();

     h_Vjets[c]->SetFillColor(kPink+6);
     h_Vjets[c]->Sumw2();

     h_Vgg[c]->SetFillColor(kOrange-3);
     h_Vgg[c]->Sumw2();
     
     h_VVjets[c]->SetFillColor(kOrange-2);;
     h_VVjets[c]->Sumw2();
     
     h_tt[c]->SetFillColor(kOrange-1);
     h_tt[c]->Sumw2();
 
     h_hgg[c]->SetFillColor(kMagenta);
     h_hgg[c]->Sumw2();
 
     h_Vh[c]->SetFillColor(kCyan);
     h_Vh[c]->Sumw2();


     //signal
     h_sig1[c]->SetLineColor(kBlack);
     h_sig1[c]->SetFillColor(kBlack);
     h_sig1[c]->SetFillStyle(3004);
     h_sig1[c]->SetLineWidth(2);
     h_sig1[c]->Sumw2();

     h_sig10[c]->SetLineColor(8);
     h_sig10[c]->SetFillColor(8);
     h_sig10[c]->SetFillStyle(3004);
     h_sig10[c]->SetLineWidth(2);
     h_sig10[c]->Sumw2();

     h_sig100[c]->SetLineColor(kRed);
     h_sig100[c]->SetFillColor(kRed);
     h_sig100[c]->SetFillStyle(3004);
     h_sig100[c]->SetLineWidth(2);
     h_sig100[c]->Sumw2();

     h_sig1000[c]->SetLineColor(kBlue);
     h_sig1000[c]->SetFillColor(kBlue);
     h_sig1000[c]->SetFillStyle(3004);
     h_sig1000[c]->SetLineWidth(2);
     h_sig1000[c]->Sumw2();


     //scale signals to 10 fb xsec
     h_sig1[c]->Scale(0.01/13.65);
     h_sig10[c]->Scale(0.01/13.65);
     h_sig100[c]->Scale(0.01/13.65);
     h_sig1000[c]->Scale(0.01/13.65);
     
     //prompt+fake (gj+qcd)
     h_pf[c] = (TH1F*) h_gj[c]->Clone();
     //prompt+prompt (diphot+diphotj)
     h_pp[c] = (TH1F*) h_diphot[c]->Clone();
     h_pp[c]->Add(h_diphotj[c]);
     //Xgg (W/Z/tt+gg)
     h_Xgg[c] = (TH1F*) h_Vgg[c]->Clone();
     h_Xgg[c]->Add(h_tt[c]);
     
     h_sum[c] = (TH1F*) h_Vh[c]->Clone();
     h_sum[c]->Add(h_hgg[c]);
     h_sum[c]->Add(h_pp[c]);
     if(h_pf[c]->GetEntries()!=0)  h_sum[c]->Add(h_pf[c]);
     h_sum[c]->Add(h_Xgg[c]);
  


     h_sum[c]->Sumw2();
     h_sum[c]->SetFillColor(kBlack);
     h_sum[c]->SetFillStyle(3003);
     h_sum[c]->SetMarkerSize(0);

     //make kolmogorov test between data and MC
     /* Double_t CHI2ndf = h_data[c]->Chi2Test(h_sum[c], "UWPCHI2/NDF");
     TPaveText* label = new TPaveText(0.6, 0.67, 0.85, 0.7, "brNDC" );
     label->SetFillColor(kWhite);
     label->SetBorderSize(0.);
     label->SetTextSize(0.038);
     label->SetTextAlign(11);
     label->SetTextFont(42);
     label->AddText(TString::Format("#chi^{2}/NDF: %.3f", CHI2ndf));*/
     
     THStack hs("hs","hs");
   
     hs.Add(h_Vh[c]);
     hs.Add(h_hgg[c]);
     hs.Add(h_Xgg[c]);  
     hs.Add(h_pp[c]);
     hs.Add(h_pf[c]); 
   
    
 


    // prepare table latex
    double ZggIntErr;
    double ZggInt; 
    double ppIntErr;
    double ppInt;  
    double pfIntErr;
    double pfInt; 
    double hggIntErr;
    double hggInt;  
    double VhIntErr;
    double VhInt;
    double sumIntErr;
    double sumInt;


    double sig1IntErr;
    double sig1Int;
    double sig10IntErr;
    double sig10Int;
    double sig100IntErr;
    double sig100Int;
    double sig1000IntErr;
    double sig1000Int;
  
    if(var=="corrmet"){
      ZggInt = h_Xgg[c]->IntegralAndError(h_Xgg[c]->FindBin(100),h_Xgg[c]->FindBin(300), ZggIntErr, "");
      ppInt = h_pp[c]->IntegralAndError(h_pp[c]->FindBin(100),h_pp[c]->FindBin(300), ppIntErr, "");
      pfInt = h_pf[c]->IntegralAndError(h_pf[c]->FindBin(100),h_pf[c]->FindBin(300), pfIntErr, ""); 
      hggInt = h_hgg[c]->IntegralAndError(h_hgg[c]->FindBin(100),h_hgg[c]->FindBin(300), hggIntErr, "");
      VhInt = h_Vh[c]->IntegralAndError(h_Vh[c]->FindBin(100),h_Vh[c]->FindBin(300), VhIntErr, ""); 
      sumInt = h_sum[c]->IntegralAndError(h_sum[c]->FindBin(100),h_sum[c]->FindBin(300), sumIntErr, "");   
      sig1Int = h_sig1[c]->IntegralAndError(h_sig1[c]->FindBin(100),h_sig1[c]->FindBin(300), sig1IntErr, "");   
      sig10Int = h_sig10[c]->IntegralAndError(h_sig10[c]->FindBin(100),h_sig10[c]->FindBin(300), sig10IntErr, "");   
      sig100Int = h_sig100[c]->IntegralAndError(h_sig100[c]->FindBin(100),h_sig100[c]->FindBin(300), sig100IntErr, "");   
      sig1000Int = h_sig1000[c]->IntegralAndError(h_sig1000[c]->FindBin(100),h_sig1000[c]->FindBin(300), sig1000IntErr, "");
    }else if(var=="PhotonsMass"){
      ZggInt = h_Xgg[c]->IntegralAndError(h_Xgg[c]->FindBin(110),h_Xgg[c]->FindBin(130), ZggIntErr, "");
      ppInt = h_pp[c]->IntegralAndError(h_pp[c]->FindBin(110),h_pp[c]->FindBin(130), ppIntErr, "");
      pfInt = h_pf[c]->IntegralAndError(h_pf[c]->FindBin(110),h_pf[c]->FindBin(130), pfIntErr, ""); 
      hggInt = h_hgg[c]->IntegralAndError(h_hgg[c]->FindBin(110),h_hgg[c]->FindBin(130), hggIntErr, "");
      VhInt = h_Vh[c]->IntegralAndError(h_Vh[c]->FindBin(110),h_Vh[c]->FindBin(130), VhIntErr, ""); 
      sumInt = h_sum[c]->IntegralAndError(h_sum[c]->FindBin(110),h_sum[c]->FindBin(130), sumIntErr, "");   
      sig1Int = h_sig1[c]->IntegralAndError(h_sig1[c]->FindBin(110),h_sig1[c]->FindBin(130), sig1IntErr, "");   
      sig10Int = h_sig10[c]->IntegralAndError(h_sig10[c]->FindBin(110),h_sig10[c]->FindBin(130), sig10IntErr, "");   
      sig100Int = h_sig100[c]->IntegralAndError(h_sig100[c]->FindBin(110),h_sig100[c]->FindBin(130), sig100IntErr, "");   
      sig1000Int = h_sig1000[c]->IntegralAndError(h_sig1000[c]->FindBin(110),h_sig1000[c]->FindBin(130), sig1000IntErr, "");
    } 
  
    if(h_pf[c]->GetEntries()==0)pfInt = 0;
    if(h_pf[c]->GetEntries()==0)pfIntErr = 0;
    RooRealVar Zgg("Zgg","Zgg",ZggInt, "");
    Zgg.setError(ZggIntErr);

    RooRealVar pp("pp","pp",ppInt, "");
    pp.setError(ppIntErr);

    RooRealVar pf("pf","pf",pfInt, "");
    pf.setError(pfIntErr);
  
    RooRealVar hgg("hgg","hgg",hggInt, "");
    hgg.setError(hggIntErr);

    RooRealVar Vh("Vh","Vh",VhInt, "");
    Vh.setError(VhIntErr);

    RooRealVar tot("tot","tot",sumInt, "");
    tot.setError(sumIntErr);

    RooRealVar s1("s1","s1",sig1Int, "");
    s1.setError(sig1IntErr);
    RooRealVar s10("s10","s10",sig10Int, "");
    s10.setError(sig10IntErr);
    RooRealVar s100("s100","s100",sig100Int, "");
    s100.setError(sig100IntErr);
    RooRealVar s1000("s1000","s1000",sig1000Int, "");
    s1000.setError(sig1000IntErr);



  
    std::cout<<"\\begin{table}[bthp]" <<std::endl;
    std::cout<<"\\begin{tabular}{cc}" <<std::endl;
    std::cout<<"\\hline \\hline" <<std::endl;
    std::cout<<"$\\sqrt{s}$ = 8 TeV; L = 19.5 $fb^{-1}$"<<" \\\\" <<std::endl;
    if(mainCut=="corrmet>100")  std::cout<<"MET $>$ 100 GeV"<<" \\\\" <<std::endl;
    if(mainCut=="PhotonsMass>110&&PhotonsMass<130")  std::cout<<"$m_{\\gamma \\gamma}$ in $[110-130]$ GeV"<<" \\\\" <<std::endl;
    if(mainCut=="PhotonsMass>110&&PhotonsMass<130&&corrmet>100")  std::cout<<"$m_{\\gamma \\gamma}$ in $[110-130]$ and MET $>$ 100 GeV"<<" \\\\" <<std::endl;
    std::cout<<"\\hline" <<std::endl;
    std::cout<<"prompt+prompt &  "<<*(pp.format(2,"EXPF"))<<" \\\\"<<std::endl;
    std::cout<<"prompt+fake &  "<<*(pf.format(2,"EXPF"))<<" \\\\"<<std::endl;
    std::cout<<"W/Z/tt $\\gamma \\gamma$ &  "<<*(Zgg.format(2,"EXPF"))<<" \\\\"<<std::endl;
    std::cout<<"$h \\rightarrow  \\gamma \\gamma$  (ggH, VBF)&  "<<*(hgg.format(2,"EXPF"))<<" \\\\"<<std::endl;
    std::cout<<"Vh,$h \\rightarrow  \\gamma \\gamma$ &  "<<*(Vh.format(2,"EXPF"))<<" \\\\"<<std::endl;
    std::cout<<"\\hline" <<std::endl;
    std::cout<<"Total Background &  "<<*(tot.format(2,"EXPF"))<<" \\\\"<<std::endl;
    std::cout<<"$\\bar{\\chi}\\chi HH, m_{\\chi}$ = 1 GeV (10 fb) &  "<<*(s1.format(2,"EXPF"))<<" \\\\"<<std::endl;
    std::cout<<"$\\bar{\\chi}\\chi HH, m_{\\chi}$ = 10 GeV (10 fb) &  "<<*(s10.format(2,"EXPF"))<<" \\\\"<<std::endl;
    std::cout<<"$\\bar{\\chi}\\chi HH, m_{\\chi}$ = 100 GeV (10 fb) &  "<<*(s100.format(2,"EXPF"))<<" \\\\"<<std::endl;
    std::cout<<"$\\bar{\\chi}\\chi HH, m_{\\chi}$ = 1000 GeV (10 fb) &  "<<*(s1000.format(2,"EXPF"))<<" \\\\"<<std::endl;
    std::cout<<"\\hline \\hline" <<std::endl;
    std::cout<<"\\end{tabular}" <<std::endl;
    std::cout<<"\\end{table}" <<std::endl;

    
    //     std::cout<<h_sum[c]->Integral()<<"   <- MC --- DATA -> " << h_data[c]->Integral()<<std::endl;
      

     std::cout<<"----------------------------------------------------------"<<std::endl;

   
    
   
 
    ctmp->cd();
    h_data[c]->Sumw2();
    h_data_b[c]->Sumw2();
    h_sum[c]->Sumw2();

    TH1F* h1_ratio1 = (TH1F*)h_data_b[c]->Clone();
    TH1F* h1_ratio1_unblind = (TH1F*)h_data[c]->Clone();
    TH1F* h1_ratio2 = (TH1F*)h_sum[c]->Clone();

    //   for(int i = 0;i < h1_ratio1->GetNbinsX();i++) std::cout<<" ratio1: "<<h1_ratio1->GetBinContent(i)<<std::endl;//if(h1_ratio1->GetBinContent(i)==0)h1_ratio1->SetBinContent(i, -1);
   
  
    ctmp->Clear();
    //-------pad 1-------//
    TPad * pad1 = new TPad("pad1", "pad1",0.01,0.13,0.75,1.);  
    
   
    pad1->SetRightMargin(0.1);
    
    pad1->SetLogy();
    pad1->Draw();
    pad1->cd();

    h_data_b[c]->SetMarkerSize(0.7);
    //    h_data_b[c]->GetYaxis()->SetTitle(TString::Format("Events /%.2f GeV", (max-min)/nbin));
    // if(var=="MET")h_data[c]->GetXaxis()->SetTitle("MET [GeV]");
    //else if(var=="PHI")h_data[c]->GetXaxis()->SetTitle("MET #phi");
   
    h_sum[c]->Draw("E2");
    h_sum[c]->GetYaxis()->SetRangeUser(0.001,100000000);
    hs.Draw("histsame");
    h_sum[c]->GetYaxis()->SetTitle(TString::Format("Events /%.2f GeV", (max-min)/nbin));
    h_sum[c]->GetYaxis()->SetRangeUser(0.001,100000000);
    if(var=="PhotonsMass")h_sum[c]->GetYaxis()->SetRangeUser(0.001,10000000000);
    if(var=="corrmet")h_sum[c]->GetXaxis()->SetTitle("MET [GeV]");
    if(var=="PhotonsMass")h_sum[c]->GetXaxis()->SetTitle("m_{#gamma #gamma} [GeV]");
    h_sum[c]->Draw("E2same");
    h_sig1[c]->Draw("histsame");
    h_sig10[c]->Draw("histsame");
    h_sig100[c]->Draw("histsame");
    h_sig1000[c]->Draw("histsame");
  

    TLegend *leg1;
    leg1 = new TLegend(0.5075,0.56441,0.7875,0.9040678, TString::Format("",c), "brNDC");
    // leg1->AddEntry(h_data_b[c],"Data","PE");
    
    leg1->AddEntry(h_pf[c],"prompt+fake", "F");
    leg1->AddEntry(h_pp[c],"prompt+prompt","F");
    leg1->AddEntry(h_Xgg[c],"W/Z/tt #gamma #gamma","F");
    
    leg1->AddEntry(h_hgg[c],"h #rightarrow #gamma #gamma (ggH, VBF)","F");
    leg1->AddEntry(h_Vh[c],"Wh,Zh; h #rightarrow #gamma #gamma","F");
    leg1->AddEntry(h_sum[c], "Bkg uncertainty", "F");
    
    leg1->AddEntry(h_sig1[c],"#bar{#chi}#chi HH, m_{#chi} = 1 GeV","F");
    leg1->AddEntry(h_sig10[c],"#bar{#chi}#chi HH, m_{#chi} = 10 GeV","F");
    leg1->AddEntry(h_sig100[c],"#bar{#chi}#chi HH, m_{#chi} = 100 GeV","F");
    leg1->AddEntry(h_sig1000[c],"#bar{#chi}#chi HH, m_{#chi} = 1000 GeV","F");

    
    leg1->SetTextSize(0.035);
    leg1->SetTextFont(42);
    leg1->SetBorderSize(0);
    leg1->SetFillStyle(0);
    leg1->Draw("same");

  
   
    int iPos=11 ;
    CMS_lumi( pad1,false,iPos );
    pad1->RedrawAxis();

    ctmp->cd(); 

    if(var=="PhotonsMass") {
      ctmp->SaveAs("~/www/plotsMonoH/DataMc_MC_MGG_"+TString::Format("cat%d.png", c));
      ctmp->SaveAs("~/www/plotsMonoH/DataMc_MC_MGG_"+TString::Format("cat%d.pdf", c));  
      ctmp->SetLogy();
      
      ctmp->SaveAs("~/www/plotsMonoH/DataMc_MC_MGG_"+TString::Format("cat%d_LOG.png", c));
      ctmp->SaveAs("~/www/plotsMonoH/DataMc_MC_MGG_"+TString::Format("cat%d_LOG.pdf", c));
    }
    if(var=="corrmet") {
      ctmp->SaveAs("~/www/plotsMonoH/DataMc_MC_MET_"+TString::Format("cat%d.png", c));
      ctmp->SaveAs("~/www/plotsMonoH/DataMc_MC_MET_"+TString::Format("cat%d.pdf", c));  
      pad1->SetLogy(1);
      hs.GetYaxis()->SetRangeUser(0.001, h_sum[c]->GetMaximum()*50);
      ctmp->SaveAs("~/www/plotsMonoH/DataMc_MC_MET_"+TString::Format("cat%d_LOG.png", c));
      ctmp->SaveAs("~/www/plotsMonoH/DataMc_MC_MET_"+TString::Format("cat%d_LOG.pdf", c));
    }
    
}




void ComputeABCDnumbers(RooWorkspace* w, std::string var ) {

  Int_t ncat = NCAT;
  TString inDir = "";

  // Luminosity:
  Float_t Lum = 19500.0;  
  RooRealVar lumi("lumi","lumi",Lum);
  w->import(lumi); 
  
  // Variables
  RooArgSet* ntplVars = defineVariables();
  RooRealVar* met;
   
 // common preselection cut
  TString mainCut = "PhotonsMass>1"; //"PhotonsMass>100&&PhotonsMass<130"; //"(PhotonsMass>=100 && PhotonsMass<=130)";   //130-2000
  RooPlot*  plotPhotonsMassDataMC[NCAT];
  
  //**********DATA***************//
  // create tree
  TFile file("histograms_CMS-HGG_23022015_DATA_monoH.root");
  TTree* dataTree = (TTree*) file.Get("Data");
 
//signal M1
  TChain*  sig1 = new TChain();
  sig1->Add("/afs/cern.ch/work/s/soffi/CMSSW620-MonoH/src/h2gglobe/AnalysisScripts/histograms_Scalar8TeV1GeV-HGG.root/ggh_m126_8TeV");


 //signal M10
  TChain*  sig10 = new TChain();
  sig10->Add("/afs/cern.ch/work/s/soffi/CMSSW620-MonoH/src/h2gglobe/AnalysisScripts/histograms_Scalar8TeV10GeV-HGG.root/ggh_m124_8TeV");


 //signal M100
  TChain*  sig100 = new TChain();
  sig100->Add("/afs/cern.ch/work/s/soffi/CMSSW620-MonoH/src/h2gglobe/AnalysisScripts/histograms_Scalar8TeV100-1000GeV-HGG.root/ggh_m127_8TeV");


 //signal M1000
  TChain*  sig1000 = new TChain();
  sig1000->Add("/afs/cern.ch/work/s/soffi/CMSSW620-MonoH/src/h2gglobe/AnalysisScripts/histograms_Scalar8TeV100-1000GeV-HGG.root/ggh_m128_8TeV");


  //Higgs to gamma gamma
  TChain* hggTree = new TChain();
  hggTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/ggh_m125_8TeV");
  hggTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/vbf_m125_8TeV");
  //hggTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/tth_m125_8TeV");


  TChain* VhTree = new TChain();
  VhTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/wzh_m125_8TeV");
  
  //**********G+jets***************//
  // create tree

  TChain* gjTree = new TChain();
  gjTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/gjet_20_8TeV_pf");
  gjTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/gjet_40_8TeV_pf");
  //gjTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/gjet_20_8TeV_pp");
  //gjTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/gjet_40_8TeV_pp");

  TChain* qcdTree = new TChain();
  qcdTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/qcd_30_8TeV_pf");
  qcdTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/qcd_40_8TeV_pf"); 
  // qcdTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/qcd_30_8TeV_ff");
  // qcdTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/qcd_40_8TeV_ff"); 
 
  //**********DIPHOT***************//
  // create tree

  TChain* diphotjTree = new TChain();
  diphotjTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/diphojet_8TeV");
 
  TChain* diphotTree = new TChain();
  diphotTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/dipho_Box_25_8TeV");
  diphotTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/dipho_Box_250_8TeV");
   
  //V+gamma
  TChain* VgTree = new TChain();
  VgTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/ZGToLLG");
  VgTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/WGToLNuG");
  
  //V+jets
  TChain* VJetsTree = new TChain();
  VJetsTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/DYJetsToLL");
  VJetsTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/WJetsToLNu");

  //VV+jets
  TChain* VVJetsTree = new TChain();
  VVJetsTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/WWJetsTo2L2Nu");
  VVJetsTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/WZJetsTo3LNu");
  VVJetsTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/ZZJetsTo2L2Nu");
  VVJetsTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/ZZJetsTo2L2Q");
  VVJetsTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/ZZJetsTo4L");
 
  //V+gamma+gamma
  TChain* VggTree = new TChain();
  VggTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/Wpgg_dR02");
  VggTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/Wmgg_dR02");
  VggTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/Zgg_dR02");

  //tt bkg
  TChain* ttTree = new TChain();
  ttTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/ttgg_dR02");
  // ttTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/TTJets");
  // ttTree->Add("histograms_CMS-HGG_23022015_MC_monoH.root/TTGJets");



  TH2F* h_data[NCAT+1];
  TH2F* h_data_b[NCAT+1];
  TH2F*  h_gj[NCAT+1];
  TH2F*  h_qcd[NCAT+1];

  TH2F*  h_diphot[NCAT+1];
  TH2F*  h_diphotj[NCAT+1];
   
  TH2F*  h_Vjets[NCAT+1];

  TH2F*  h_Vgg[NCAT+1]; 
  TH2F*  h_tt[NCAT+1];

  TH2F*  h_hgg[NCAT+1];
  TH2F*  h_Vh[NCAT+1];

  TH2F* h_sig1[NCAT+1];
  TH2F* h_sig10[NCAT+1];
  TH2F* h_sig100[NCAT+1];
  TH2F* h_sig1000[NCAT+1];


  TH2F* h_sum[NCAT+1]; 
  TH2F* h_pp[NCAT+1];
  TH2F* h_pf[NCAT+1];
  TH2F* h_Xgg[NCAT+1];
  TH2F*  h_Vg[NCAT+1];
  TH2F*  h_VVjets[NCAT+1];
 
  Int_t c =4;
  
  TCanvas* ctmp = new TCanvas("ctmp",(var+" Background Categories").c_str(),1);
  TLegend *leg1;
  leg1 = new TLegend(0.585,0.8441,0.8275,0.9040678, TString::Format("",c), "brNDC");
  
  int iPos=11 ;
  CMS_lumi( ctmp,false,iPos );
  ctmp->cd(); 
  
  Int_t xnbin = 120;
  Double_t  xmin = 100;
  Double_t  xmax = 300;
  Int_t ynbin = 180;
  Double_t  ymin = 0;
  Double_t  ymax = 300;


     h_data[c]= new TH2F(TString::Format("h_data_cat%d",c), TString::Format("h_data_cat%d",c), xnbin, xmin, xmax, ynbin, ymin, ymax);
     h_data_b[c]= new TH2F(TString::Format("h_data_b_cat%d",c), TString::Format("h_data_b_cat%d",c), xnbin, xmin, xmax, ynbin, ymin, ymax);

     h_gj[c]= new TH2F(TString::Format("h_gj_cat%d",c), TString::Format("h_gj_cat%d",c), xnbin, xmin, xmax, ynbin, ymin, ymax);
     h_qcd[c]= new TH2F(TString::Format("h_qcd_cat%d",c), TString::Format("h_qcd_cat%d",c), xnbin, xmin, xmax, ynbin, ymin, ymax);
     h_diphot[c]= new TH2F(TString::Format("h_diphot_cat%d",c), TString::Format("h_diphot_cat%d",c), xnbin, xmin, xmax, ynbin, ymin, ymax);
     h_diphotj[c]= new TH2F(TString::Format("h_diphotj_cat%d",c), TString::Format("h_diphotj_cat%d",c), xnbin, xmin, xmax, ynbin, ymin, ymax);
     h_Vg[c]= new TH2F(TString::Format("h_Vg_cat%d",c), TString::Format("h_Vg_cat%d",c), xnbin, xmin, xmax, ynbin, ymin, ymax);
     h_Vjets[c]= new TH2F(TString::Format("h_Vjets_cat%d",c), TString::Format("h_Vjets_cat%d",c), xnbin, xmin, xmax, ynbin, ymin, ymax);
     h_Vgg[c]= new TH2F(TString::Format("h_Vgg_cat%d",c), TString::Format("h_Vgg_cat%d",c), xnbin, xmin, xmax, ynbin, ymin, ymax);
     h_VVjets[c]= new TH2F(TString::Format("h_VVjets_cat%d",c), TString::Format("h_VVjets_cat%d",c), xnbin, xmin, xmax, ynbin, ymin, ymax);
     h_tt[c]= new TH2F(TString::Format("h_tt_cat%d",c), TString::Format("h_tt_cat%d",c), xnbin, xmin, xmax, ynbin, ymin, ymax);

     h_hgg[c]= new TH2F(TString::Format("h_hgg_cat%d",c), TString::Format("h_hgg_cat%d",c), xnbin, xmin, xmax, ynbin, ymin, ymax);
     h_Vh[c]= new TH2F(TString::Format("h_Vh_cat%d",c), TString::Format("h_Vh_cat%d",c), xnbin, xmin, xmax, ynbin, ymin, ymax);

     h_sig1[c]= new TH2F(TString::Format("h_sig1_cat%d",c), TString::Format("h_sig1_cat%d",c), xnbin, xmin, xmax, ynbin, ymin, ymax);
     h_sig10[c]= new TH2F(TString::Format("h_sig10_cat%d",c), TString::Format("h_sig10_cat%d",c), xnbin, xmin, xmax, ynbin, ymin, ymax);
     h_sig100[c]= new TH2F(TString::Format("h_sig100_cat%d",c), TString::Format("h_sig100_cat%d",c), xnbin, xmin, xmax, ynbin, ymin, ymax);
     h_sig1000[c]= new TH2F(TString::Format("h_sig1000_cat%d",c), TString::Format("h_sig1000_cat%d",c), xnbin, xmin, xmax, ynbin, ymin, ymax);

 
    
    
     dataTree->Draw((var+">>h_data_cat4").c_str(),"("+ mainCut+")*1");
     dataTree->Draw((var+">>h_data_b_cat4").c_str(),"("+ mainCut+")*1");
  
     gjTree->Draw((var+">>h_gj_cat4").c_str(), "("+mainCut+")*evweight");
     qcdTree->Draw((var+">>h_qcd_cat4").c_str(), "("+mainCut+")*evweight");
     diphotTree->Draw((var+">>h_diphot_cat4").c_str(),"("+ mainCut+")*evweight");
     diphotjTree->Draw((var+">>h_diphotj_cat4").c_str(),"("+ mainCut+")*evweight");
     VgTree->Draw((var+">>h_Vg_cat4").c_str(), "("+mainCut+")*evweight");
     VJetsTree->Draw((var+">>h_Vjets_cat4").c_str(), "("+mainCut+")*evweight");
     VggTree->Draw((var+">>h_Vgg_cat4").c_str(), "("+mainCut+")*evweight");
     VVJetsTree->Draw((var+">>h_VVjets_cat4").c_str(), "("+mainCut+")*evweight");
     ttTree->Draw((var+">>h_tt_cat4").c_str(), "("+mainCut+")*evweight");

     hggTree->Draw((var+">>h_hgg_cat4").c_str(), "("+mainCut+")*evweight");
     VhTree->Draw((var+">>h_Vh_cat4").c_str(), "("+mainCut+")*evweight");

     sig1->Draw((var+">>h_sig1_cat4").c_str(), "("+mainCut+")*evweight");
     sig10->Draw((var+">>h_sig10_cat4").c_str(), "("+mainCut+")*evweight");
     sig100->Draw((var+">>h_sig100_cat4").c_str(), "("+mainCut+")*evweight");
     sig1000->Draw((var+">>h_sig1000_cat4").c_str(), "("+mainCut+")*evweight");

   
     //fisso la normalizzazione di gjets a quella gj+qcd xkè la shape di qcd _pf fa schifo, quindi prenidamoq uella di gj _pf
     Double_t qcdInt = h_qcd[c]->Integral();
     Double_t gJetIntFixed = qcdInt+h_gj[c]->Integral();
     h_gj[c]->Scale(gJetIntFixed/h_gj[c]->Integral());  
     h_gj[c]->SetFillColor(kAzure+8);
     h_gj[c]->Sumw2();
    
     h_qcd[c]->SetFillColor(kAzure+2);
     h_qcd[c]->Sumw2();
     
     h_diphotj[c]->SetFillColor(kTeal+9);
     h_diphotj[c]->Sumw2();
     
     h_diphot[c]->SetFillColor(kSpring+7);
     h_diphot[c]->Sumw2();
     
     h_Vg[c]->SetFillColor(kPink+7);
     h_Vg[c]->Sumw2();

     h_Vjets[c]->SetFillColor(kPink+6);
     h_Vjets[c]->Sumw2();

     h_Vgg[c]->SetFillColor(kOrange-3);
     h_Vgg[c]->Sumw2();
     
     h_VVjets[c]->SetFillColor(kOrange-2);;
     h_VVjets[c]->Sumw2();
     
     h_tt[c]->SetFillColor(kOrange-1);
     h_tt[c]->Sumw2();
 
     h_hgg[c]->SetFillColor(kMagenta);
     h_hgg[c]->Sumw2();
 
     h_Vh[c]->SetFillColor(kCyan);
     h_Vh[c]->Sumw2();


     //signal
     h_sig1[c]->SetLineColor(kBlack);
     h_sig1[c]->SetFillColor(kBlack);
     h_sig1[c]->SetFillStyle(3004);
     h_sig1[c]->SetLineWidth(2);
     h_sig1[c]->Sumw2();

     h_sig10[c]->SetLineColor(8);
     h_sig10[c]->SetFillColor(8);
     h_sig10[c]->SetFillStyle(3004);
     h_sig10[c]->SetLineWidth(2);
     h_sig10[c]->Sumw2();

     h_sig100[c]->SetLineColor(kRed);
     h_sig100[c]->SetFillColor(kRed);
     h_sig100[c]->SetFillStyle(3004);
     h_sig100[c]->SetLineWidth(2);
     h_sig100[c]->Sumw2();

     h_sig1000[c]->SetLineColor(kBlue);
     h_sig1000[c]->SetFillColor(kBlue);
     h_sig1000[c]->SetFillStyle(3004);
     h_sig1000[c]->SetLineWidth(2);
     h_sig1000[c]->Sumw2();


     //scale signals to 10 fb xsec
     h_sig1[c]->Scale(0.01/13.65);
     h_sig10[c]->Scale(0.01/13.65);
     h_sig100[c]->Scale(0.01/13.65);
     h_sig1000[c]->Scale(0.01/13.65);
     
     //prompt+fake (gj+qcd)
     h_pf[c] = (TH2F*) h_gj[c]->Clone();
     //prompt+prompt (diphot+diphotj)
     h_pp[c] = (TH2F*) h_diphot[c]->Clone();
     h_pp[c]->Add(h_diphotj[c]);
     //Xgg (W/Z/tt+gg)
     h_Xgg[c] = (TH2F*) h_Vgg[c]->Clone();
     h_Xgg[c]->Add(h_tt[c]);
     
     h_sum[c] = (TH2F*) h_Xgg[c]->Clone();
     //h_sum[c]->Add(h_hgg[c]);
     //h_sum[c]->Add(h_Vh[c]);
     if(h_pf[c]->GetEntries()!=0)  h_sum[c]->Add(h_pf[c]);
     h_sum[c]->Add(h_pp[c]);
  


     h_sum[c]->Sumw2();
     h_sum[c]->SetFillColor(kBlack);
     h_sum[c]->SetFillStyle(3003);
     h_sum[c]->SetMarkerSize(0);
 
     std::cout<<h_sum[c]->Integral()<<std::endl;

 
    ctmp->cd();  
    
    h_sum[c]->GetYaxis()->SetTitle("MET GeV");
    h_sum[c]->GetXaxis()->SetTitle("m_{#gamma #gamma} [GeV]");
    h_sum[c]->Draw("COLZ"); 
    leg1->SetHeader("Background");
    leg1->Draw("same");
    ctmp->SaveAs("~/www/plotsMonoH/ABCD_2D_BKG.png");  

    h_sig100[c]->GetYaxis()->SetTitle("MET GeV");
    h_sig100[c]->GetXaxis()->SetTitle("m_{#gamma #gamma} [GeV]");
    h_sig100[c]->Draw("COLZ"); 
    leg1->SetHeader("Signal");
    leg1->Draw("same");
    ctmp->SaveAs("~/www/plotsMonoH/ABCD_2D_SIG100.png");  


    int mggA1min = h_sum[c]->GetXaxis()->FindBin(100);
    int mggA1max = h_sum[c]->GetXaxis()->FindBin(110)-1;
    int mggB1min = h_sum[c]->GetXaxis()->FindBin(100);
    int mggB1max = h_sum[c]->GetXaxis()->FindBin(110)-1;
    int mggA2min = h_sum[c]->GetXaxis()->FindBin(130)+1;
    int mggA2max = h_sum[c]->GetXaxis()->FindBin(180);
    int mggB2min = h_sum[c]->GetXaxis()->FindBin(130)+1;
    int mggB2max = h_sum[c]->GetXaxis()->FindBin(180);
   
    int mggDCmin = h_sum[c]->GetXaxis()->FindBin(110);
    int mggDCmax = h_sum[c]->GetXaxis()->FindBin(130);
    
    int metCmin = h_sum[c]->GetYaxis()->FindBin(0);
    int metCmax = h_sum[c]->GetYaxis()->FindBin(100)-1;
    int metDmin = h_sum[c]->GetYaxis()->FindBin(100);
    int metDmax = h_sum[c]->GetYaxis()->FindBin(400);


    double ppNCerr;
    double ppNDerr;
    double ppNA1err;
    double ppNA2err;
    double ppNB1err;
    double ppNB2err;
    double ppNAerr;
    double ppNBerr;
  
    double ppNC = h_pp[c]->IntegralAndError(mggDCmin, mggDCmax,metCmin, metCmax, ppNCerr, "");    
    double ppND = h_pp[c]->IntegralAndError(mggDCmin, mggDCmax,metDmin, metDmax, ppNDerr, "");
    double ppNA1 = h_pp[c]->IntegralAndError(mggA1min, mggA1max,metDmin, metDmax, ppNA1err, "");
    double ppNB1 = h_pp[c]->IntegralAndError(mggB1min, mggB1max,metCmin, metCmax, ppNB1err, "");
    double ppNA2 = h_pp[c]->IntegralAndError(mggA2min, mggA2max,metDmin, metDmax, ppNA2err, "");
    double ppNB2 = h_pp[c]->IntegralAndError(mggB2min, mggB2max,metCmin, metCmax, ppNB2err, "");
    ppNAerr=sqrt(ppNA1err*ppNA1err+ppNA2err*ppNA2err);
    ppNBerr=sqrt(ppNB1err*ppNB1err+ppNB2err*ppNB2err);
    double ppNA=ppNA1+ppNA2;
    double ppNB=ppNB1+ppNB2;
    double ppcorr = h_pp[c]->GetCorrelationFactor(1,2);

    double ppExpND = ppNC*ppNA/ppNB;
    double ppExpNDerr = sqrt((ppNCerr*ppNCerr*ppNA*ppNA/(ppNB*ppNB))+(ppNAerr*ppNAerr*ppNC*ppNC/(ppNB*ppNB))+(ppNBerr*ppNBerr*ppNC*ppNC*ppNA*ppNA/(pow(ppNB,4))));
    double ppDiff = TMath::Abs((ppNC*ppNA/ppNB-ppND)/(ppNC*ppNA/ppNB));
    RooRealVar ppA("ppA","ppA",ppNA,"");
    ppA.setError(ppNAerr);
    RooRealVar ppB("ppB","ppB",ppNB,"");
    ppB.setError(ppNBerr);
    RooRealVar ppC("ppC","ppC",ppNC,"");
    ppC.setError(ppNCerr);
    RooRealVar ppD("ppD","ppD",ppND,"");
    ppD.setError(ppNDerr);
    RooRealVar ppDexp("ppDexp","ppDexp",ppExpND,"");
    ppDexp.setError(ppExpNDerr);
    std::cout<<"prompt+prompt & "<<ppcorr<<" & "<<*ppA.format(2,"EXPP")<<"&"<<*ppB.format(2,"EXPP")<<"&"<<*ppC.format(2,"EXPP")<<"&"<<*ppD.format(2,"EXPP")<<"&"<<*ppDexp.format(2,"EXPP")<<"&"<<ppDiff<<"\\\\"<<std::endl;
 






  
    double pfNCerr;
    double pfNDerr;
    double pfNA1err;
    double pfNA2err;
    double pfNB1err;
    double pfNB2err;
    double pfNAerr;
    double pfNBerr;
  

    double pfNC = h_pf[c]->IntegralAndError(mggDCmin, mggDCmax,metCmin, metCmax, pfNCerr, "");    
    double pfND = h_pf[c]->IntegralAndError(mggDCmin, mggDCmax,metDmin, metDmax, pfNDerr, "");
    double pfNA1 = h_pf[c]->IntegralAndError(mggA1min, mggA1max,metDmin, metDmax, pfNA1err, "");
    double pfNB1 = h_pf[c]->IntegralAndError(mggB1min, mggB1max,metCmin, metCmax, pfNB1err, "");
    double pfNA2 = h_pf[c]->IntegralAndError(mggA2min, mggA2max,metDmin, metDmax, pfNA2err, "");
    double pfNB2 = h_pf[c]->IntegralAndError(mggB2min, mggB2max,metCmin, metCmax, pfNB2err, "");
    pfNAerr=sqrt(pfNA1err*pfNA1err+pfNA2err*pfNA2err);
    pfNBerr=sqrt(pfNB1err*pfNB1err+pfNB2err*pfNB2err);
    double pfNA=pfNA1+pfNA2;
    double pfNB=pfNB1+pfNB2;
    double pfcorr = h_pf[c]->GetCorrelationFactor(1,2); 

    double pfExpND = pfNC*pfNA/pfNB;
    double pfExpNDerr = sqrt((pfNCerr*pfNCerr*pfNA*pfNA/(pfNB*pfNB))+(pfNAerr*pfNAerr*pfNC*pfNC/(pfNB*pfNB))+(pfNBerr*pfNBerr*pfNC*pfNC*pfNA*pfNA/(pow(pfNB,4))));
    double pfDiff =   TMath::Abs((pfNC*pfNA/pfNB-pfND)/(pfNC*pfNA/pfNB));
    RooRealVar pfA("pfA","pfA",pfNA,"");
    pfA.setError(pfNAerr);
    RooRealVar pfB("pfB","pfB",pfNB,"");
    pfB.setError(pfNBerr);
    RooRealVar pfC("pfC","pfC",pfNC,"");
    pfC.setError(pfNCerr);
    RooRealVar pfD("pfD","pfD",pfND,"");
    pfD.setError(pfNDerr);
    RooRealVar pfDexp("pfDexp","pfDexp",pfExpND,"");
    pfDexp.setError(pfExpNDerr);
    std::cout<<"prompt+fake & "<<pfcorr<<" & "<<*pfA.format(2,"EXPF")<<"&"<<*pfB.format(2,"EXPF")<<"&"<<*pfC.format(2,"EXPF")<<"&"<<*pfD.format(2,"EXPF")<<"&"<<*pfDexp.format(2,"EXPF")<<"&"<<pfDiff<<"\\\\"<<std::endl;
 



  

    double XggNCerr;
    double XggNDerr;
    double XggNA1err;
    double XggNA2err;
    double XggNB1err;
    double XggNB2err;
    double XggNAerr;
    double XggNBerr;
  

    double XggNC = h_Xgg[c]->IntegralAndError(mggDCmin, mggDCmax,metCmin, metCmax, XggNCerr, "");    
    double XggND = h_Xgg[c]->IntegralAndError(mggDCmin, mggDCmax,metDmin, metDmax, XggNDerr, "");
    double XggNA1 = h_Xgg[c]->IntegralAndError(mggA1min, mggA1max,metDmin, metDmax, XggNA1err, "");
    double XggNB1 = h_Xgg[c]->IntegralAndError(mggB1min, mggB1max,metCmin, metCmax, XggNB1err, "");
    double XggNA2 = h_Xgg[c]->IntegralAndError(mggA2min, mggA2max,metDmin, metDmax, XggNA2err, "");
    double XggNB2 = h_Xgg[c]->IntegralAndError(mggB2min, mggB2max,metCmin, metCmax, XggNB2err, "");
    XggNAerr=sqrt(XggNA1err*XggNA1err+XggNA2err*XggNA2err);
    XggNBerr=sqrt(XggNB1err*XggNB1err+XggNB2err*XggNB2err);
    double XggNA=XggNA1+XggNA2;
    double XggNB=XggNB1+XggNB2;
    double Xggcorr = h_Xgg[c]->GetCorrelationFactor(1,2);

    double XggExpND = XggNC*XggNA/XggNB;
    double XggExpNDerr = sqrt((XggNCerr*XggNCerr*XggNA*XggNA/(XggNB*XggNB))+(XggNAerr*XggNAerr*XggNC*XggNC/(XggNB*XggNB))+(XggNBerr*XggNBerr*XggNC*XggNC*XggNA*XggNA/(pow(XggNB,4))));
    double XggDiff = TMath::Abs((XggNC*XggNA/XggNB-XggND)/(XggNC*XggNA/XggNB));
    RooRealVar XggA("XggA","XggA",XggNA,"");
    XggA.setError(XggNAerr);
    RooRealVar XggB("XggB","XggB",XggNB,"");
    XggB.setError(XggNBerr);
    RooRealVar XggC("XggC","XggC",XggNC,"");
    XggC.setError(XggNCerr);
    RooRealVar XggD("XggD","XggD",XggND,"");
    XggD.setError(XggNDerr);
    RooRealVar XggDexp("XggDexp","XggDexp",XggExpND,"");
    XggDexp.setError(XggExpNDerr);
    std::cout<<"W,Z,tt + $\\gamma \\gamma$ & "<<Xggcorr<<" & "<<*XggA.format(2,"EXPF")<<"&"<<*XggB.format(2,"EXPF")<<"&"<<*XggC.format(2,"EXPF")<<"&"<<*XggD.format(2,"EXPF")<<"&"<<*XggDexp.format(2,"EXPF")<<"&"<<XggDiff<<"\\\\"<<std::endl;






 
   double sumNCerr;
   double sumNDerr;
   double sumNA1err;
   double sumNA2err;
   double sumNB1err;
   double sumNB2err;
   double sumNAerr;
   double sumNBerr;
  
   
    std::cout<<"\\hline"<<std::endl;
    double sumNC = h_sum[c]->IntegralAndError(mggDCmin, mggDCmax,metCmin, metCmax, sumNCerr, "");    
    double sumND = h_sum[c]->IntegralAndError(mggDCmin, mggDCmax,metDmin, metDmax, sumNDerr, "");
    double sumNA1 = h_sum[c]->IntegralAndError(mggA1min, mggA1max,metDmin, metDmax, sumNA1err, "");
    double sumNB1 = h_sum[c]->IntegralAndError(mggB1min, mggB1max,metCmin, metCmax, sumNB1err, "");
    double sumNA2 = h_sum[c]->IntegralAndError(mggA2min, mggA2max,metDmin, metDmax, sumNA2err, "");
    double sumNB2 = h_sum[c]->IntegralAndError(mggB2min, mggB2max,metCmin, metCmax, sumNB2err, "");
    sumNAerr=sqrt(sumNA1err*sumNA1err+sumNA2err*sumNA2err);
    sumNBerr=sqrt(sumNB1err*sumNB1err+sumNB2err*sumNB2err);
    double sumNA=sumNA1+sumNA2;
    double sumNB=sumNB1+sumNB2;
    double sumcorr = h_sum[c]->GetCorrelationFactor(1,2);
    
    double sumExpND = sumNC*sumNA/sumNB;
    double sumExpNDerr = sqrt((sumNCerr*sumNCerr*sumNA*sumNA/(sumNB*sumNB))+(sumNAerr*sumNAerr*sumNC*sumNC/(sumNB*sumNB))+(sumNBerr*sumNBerr*sumNC*sumNC*sumNA*sumNA/(pow(sumNB,4))));
    double sumDiff = TMath::Abs((sumNC*sumNA/sumNB-sumND)/(sumNC*sumNA/sumNB));

    RooRealVar sumA("sumA","sumA",sumNA,"");
    sumA.setError(sumNAerr);
    RooRealVar sumB("sumB","sumB",sumNB,"");
    sumB.setError(sumNBerr);
    RooRealVar sumC("sumC","sumC",sumNC,"");
    sumC.setError(sumNCerr);
    RooRealVar sumD("sumD","sumD",sumND,"");
    sumD.setError(sumNDerr);
    RooRealVar sumDexp("sumDexp","sumDexp",sumExpND,"");
    sumDexp.setError(sumExpNDerr);
    std::cout<<"Total Background & "<<sumcorr<<" & "<<*sumA.format(2,"EXPF")<<"&"<<*sumB.format(2,"EXPF")<<"&"<<*sumC.format(2,"EXPF")<<"&"<<*sumD.format(2,"EXPF")<<"&"<<*sumDexp.format(2,"EXPF")<<"&"<<sumDiff<<"\\\\"<<std::endl;
 


    
}



void MakeDataCard(RooWorkspace* w,TString model, TString mass, double sigrate) {

  TString cardDir = "./datacard/";
  
 
  // *****************************
  // Printdata Data Card int file
  TString filename(cardDir+"datacard_8TeV_monoHgg_"+model+"_DMmass_"+mass+".txt");
  ofstream outFile(filename);

  outFile << "#MonoHgg DataCard for C&C Limit Setting, 19500 pb-1 " << endl;
  outFile << "#Run with:combine -M Asymptotic cardname.txt --run blind " << endl;
  outFile << "# Lumi =  19500 pb-1" << endl;
  outFile << "imax 1" << endl;
  outFile << "jmax *" << endl;
  outFile << "kmax *" << endl;
  outFile << "---------------" << endl;
 
  outFile << "bin 1"<< endl;
  outFile <<  "observation  0 "  << endl;
  outFile << "------------------------------" << endl;
  outFile << "bin     1             1            1           1            1               1 "<< endl;
  outFile << "process DM            Xgg          pp          pf           hgg             Vh " << endl;
  outFile << "process 0             1            2           3            4               5 " << endl;
  outFile << Form("rate %.3f          0.57         1.98        0.13         0.043           1.2 ",sigrate)<< endl;
  outFile << "--------------------------------" << endl;
  outFile << "#signal related" << endl;
  outFile << "lumi_8TeV     lnN     1.026000      -          -          -       1.02600   1.02600" << endl;
  outFile << "eff_trig      lnN     1.010000      -          -          -       1.01000   1.01000" << endl;
  outFile << "id_eff_eb     lnN     1.02000       -          -          -       1.02000   1.02000   " << endl;    
  outFile << "vtxEff        lnN   0.996/1.008     -          -          -     0.996/1.008  0.996/1.008" << endl; 
  outFile << "#background related" << endl;
  outFile << "abcd_estimate lnN       -         1.26000    1.26000    1.26000      -         -  " << endl;
  outFile << "Xgg_norm      gmN 19 -         0.03         -          -          -         -  " << endl;
  outFile << "pp_norm       gmN 36617 -           -        0.000054     -          -         -  " << endl;
  outFile << "pf_norm       gmN 24408 -           -          -       0.0000053     -         -  " << endl;
  outFile.close();

  cout << "Write data card in: " << filename << " file" << endl;

  return;
}

