 
#include "CMS_lumi.C"
using namespace RooFit;
using namespace RooStats ;

static const Int_t NCAT = 4;  // chiara

std::string filePOSTfix="";
double signalScaler=1.00;

void AddSigData(RooWorkspace*, Float_t);
void AddBkgData(RooWorkspace*, Float_t);
void MakePlotMassDataMC(RooWorkspace*);
void MakePlotNVTXDataMC(RooWorkspace*, Float_t);



RooArgSet* defineVariables() {

  // define variables of the input ntuple //livia
  RooRealVar* mgg  = new RooRealVar("mgg", "M(gg)",100, 2000,"GeV");
  RooRealVar* weight = new RooRealVar("weight","Reweightings",0,10,"");
  RooRealVar* nvtx = new RooRealVar("nvtx", "nvtx", 0, 50, "");
  RooArgSet* ntplVars = new RooArgSet(*mgg, *weight, *nvtx);
  
  return ntplVars;
}

void runfits() {
 //*******************************************************************//
  cout << "Now plot Data vs MC bkg" << endl;
  TString card_name("HighMass-hgg_models_Bkg_8TeV_test.rs");
  HLFactory hlf("HLFactory", card_name, false);
  RooWorkspace* w = hlf.GetWs();
 
  Double_t MMIN = 100.; //130 //livia   //100  //180   //270
  Double_t MMAX = 800.; //450      //livia //200 // 300  //2000
  w->var("mgg")->setMin(MMIN);
  w->var("mgg")->setMax(MMAX);

 
  cout << "Now plot Data vs MC bkg" << endl;
  
  //MakePlotNVTXDataMC(w);
  MakePlotMassDataMC(w);
  //MakePlotCutFlowDataMC(w);
  //MakePlotMETDataMC(w);
 

  return;
}




void MakePlotMassDataMC(RooWorkspace* w ) {
  TString inDir = "";

  // Luminosity:
  Float_t Lum = 40.0;  
  RooRealVar lumi("lumi","lumi",Lum);
  w->import(lumi); 
  
  // Variables
  RooArgSet* ntplVars = defineVariables();
  RooRealVar* mgg = w->var("mgg");
  // common preselection cut
  TString mainCut = "(mgg>=100 && mgg<200)&& hltDiphoton30Mass95";   //130-2000 abs(sceta1)>1.4442&&abs(sceta2)>1.4442&&
  RooPlot*  plotPhotonsMassDataMC;
  
  //**********DATA***************//
  TFile file("data/50ns_betaV4/DoubleEG.root");
  TTree* dataTree = (TTree*) file.Get("DiPhotonTree");
   
  //**********G+jets***************//
  TChain* gjTree = new TChain();
  gjTree->Add("data/50ns_betaV4/GJets.root/DiPhotonTree");

  //**********QCD***************//
  TChain* qcdTree = new TChain();
  qcdTree->Add("data/50ns_betaV4/QCD.root/DiPhotonTree");
  
  //**********DIPHOT***************//
  TChain* diphotTree = new TChain();
  diphotTree->Add("data/50ns_betaV4/DiPhoton.root/DiPhotonTree");

  //**********DY***************//
  TChain* dyTree = new TChain();
  dyTree->Add("data/50ns_betaV4/DYJetsToLL.root/DiPhotonTree");

  //**********GGH***************//
  TChain* gghTree = new TChain();
  gghTree->Add("data/50ns_betaV4/GluGluHToGG.root/DiPhotonTree");
  
  //**********VH***************//
  TChain* vhTree = new TChain();
  vhTree->Add("data/50ns_betaV4/VH.root/DiPhotonTree");
 
  //**********M1***************//
  TChain* m1Tree = new TChain();
  m1Tree->Add("data/50ns_betaV4/DMHtoGG_M1.root/DiPhotonTree");
  ///**********M10***************//
  TChain* m10Tree = new TChain();
  m10Tree->Add("data/50ns_betaV4/DMHtoGG_M10.root/DiPhotonTree");
  //**********M100***************//
  TChain* m100Tree = new TChain();
  m100Tree->Add("data/50ns_betaV4/DMHtoGG_M100.root/DiPhotonTree");
  //**********M1000***************//
  TChain* m1000Tree = new TChain();
  m1000Tree->Add("data/50ns_betaV4/DMHtoGG_M1000.root/DiPhotonTree");


  TH1F* h_data;
 
  TH1F*  h_gj;
  TH1F*  h_qcd;
  TH1F*  h_diphot;
  TH1F*  h_dy;
  TH1F*  h_ggh;
  TH1F*  h_vh;
  TH1F*  h_m1;
  TH1F*  h_m10;
  TH1F*  h_m100;
  TH1F*  h_m1000;

  TH1F* h_sum; 
 
  
  TCanvas* ctmp = new TCanvas("ctmp","PhotonsMass Background Categories",1);
  Int_t nbin = 60;
  Double_t  min = 100;
  Double_t  max = 200;

  int c = 0;
  h_data= new TH1F(TString::Format("h_data_cat%d",c), TString::Format("h_data_cat%d",c), nbin, min, max);
  h_gj= new TH1F(TString::Format("h_gj_cat%d",c), TString::Format("h_gj_cat%d",c), nbin, min, max);
  h_qcd= new TH1F(TString::Format("h_qcd_cat%d",c), TString::Format("h_qcd_cat%d",c), nbin, min, max);
  h_diphot= new TH1F(TString::Format("h_diphot_cat%d",c), TString::Format("h_diphot_cat%d",c), nbin, min, max);
  h_dy= new TH1F(TString::Format("h_dy_cat%d",c), TString::Format("h_dy_cat%d",c), nbin, min, max);
  h_ggh= new TH1F(TString::Format("h_ggh_cat%d",c), TString::Format("h_ggh_cat%d",c), nbin, min, max);
  h_vh= new TH1F(TString::Format("h_vh_cat%d",c), TString::Format("h_vh_cat%d",c), nbin, min, max);
  h_m1= new TH1F(TString::Format("h_m1_cat%d",c), TString::Format("h_m1_cat%d",c), nbin, min, max);
  h_m10= new TH1F(TString::Format("h_m10_cat%d",c), TString::Format("h_m10_cat%d",c), nbin, min, max);
  h_m100= new TH1F(TString::Format("h_m100_cat%d",c), TString::Format("h_m100_cat%d",c), nbin, min, max);
  h_m1000= new TH1F(TString::Format("h_m1000_cat%d",c), TString::Format("h_m1000_cat%d",c), nbin, min, max);

  
  
  dataTree->Draw("mgg>>h_data_cat0", "("+mainCut+")*1");
  gjTree->Draw("mgg>>h_gj_cat0", "("+mainCut+")*weight");
  qcdTree->Draw("mgg>>h_qcd_cat0", "("+mainCut+")*weight");
  diphotTree->Draw("mgg>>h_diphot_cat0","("+ mainCut+")*weight");
  dyTree->Draw("mgg>>h_dy_cat0","("+ mainCut+")*weight");
  gghTree->Draw("mgg>>h_ggh_cat0","("+ mainCut+")*weight*0.002");
  vhTree->Draw("mgg>>h_vh_cat0","("+ mainCut+")*weight*0.002");
  m1Tree->Draw("mgg>>h_m1_cat0","("+ mainCut+")*weight");
  m10Tree->Draw("mgg>>h_m10_cat0","("+ mainCut+")*weight");
  m100Tree->Draw("mgg>>h_m100_cat0","("+ mainCut+")*weight");
  m1000Tree->Draw("mgg>>h_m1000_cat0","("+ mainCut+")*weight");
 
  
  h_gj->SetFillColor(kAzure+8);
  h_gj->Sumw2();
  h_qcd->SetFillColor(kYellow+8);
  h_qcd->Sumw2();  
  h_diphot->SetFillColor(kSpring+7);
  h_diphot->Sumw2();
  h_dy->SetFillColor(kOrange+7);
  h_dy->Sumw2();
  h_ggh->SetFillColor(kMagenta+7);
  h_ggh->Sumw2();
  h_vh->SetFillColor(kPink+7);
  h_vh->Sumw2();
  
   
  h_sum = (TH1F*) h_gj->Clone();
  h_sum->Add(h_qcd);
  h_sum->Add(h_diphot);
  h_sum->Add(h_dy);
  h_sum->Add(h_ggh);
  h_sum->Add(h_vh);
  h_sum->Sumw2();
  h_sum->SetFillColor(kBlack);
  h_sum->SetFillStyle(3003);
  h_sum->SetMarkerSize(0);

  //make kolmogorov test between data and MC
  Double_t CHI2ndf = h_data->Chi2Test(h_sum, "UWPCHI2/NDF");
  TPaveText* label = new TPaveText(0.6, 0.67, 0.85, 0.7, "brNDC" );
  label->SetFillColor(kWhite);
  label->SetBorderSize(0.);
  label->SetTextSize(0.038);
  label->SetTextAlign(11);
  label->SetTextFont(42);
  // label->AddText(TString::Format("#chi^{2}/NDF: %.3f", CHI2ndf));
     
  THStack hs("hs","hs");
  hs.Add(h_vh);
  hs.Add(h_ggh); 
  hs.Add(h_diphot); 
  hs.Add(h_dy); 
  hs.Add(h_qcd); 
  hs.Add(h_gj); 
  
  std::cout<<h_sum->Integral()<<std::endl;
  std::cout<<h_data->Integral()<<std::endl;
  std::cout<<h_gj->Integral()<<" %: "<<h_gj->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<h_qcd->Integral()<<" %: "<<h_qcd->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<h_diphot->Integral()<<" %: "<<h_diphot->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<h_dy->Integral()<<" %: "<<h_dy->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<h_ggh->Integral()<<" %: "<<h_ggh->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<h_vh->Integral()<<" %: "<<h_vh->Integral()/h_sum->Integral()<<std::endl;
  
   
  std::cout<<"gj "<<h_gj->Integral()<<" %: "<<h_gj->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<"qcd "<<h_qcd->Integral()<<" %: "<<h_qcd->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<"diphot "<<h_diphot->Integral()<<" %: "<<h_diphot->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<"dy "<<h_dy->Integral()<<" %: "<<h_dy->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<"ggh "<<h_ggh->Integral()<<" %: "<<h_ggh->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<"vh "<<h_vh->Integral()<<" %: "<<h_vh->Integral()/h_sum->Integral()<<std::endl;
 
  ctmp->cd();
  h_data->Sumw2();
  h_sum->Sumw2();

  TH1F* h1_ratio1 = (TH1F*)h_data->Clone();
  TH1F* h1_ratio2 = (TH1F*)h_sum->Clone();

  ctmp->Clear();
  //-------pad 1-------//
  TPad * pad1 = new TPad("pad1", "pad1",0.01,0.13,0.75,1.);  
  
  
  pad1->SetRightMargin(0.1);
  
  pad1->SetLogy();
  pad1->Draw();
  pad1->cd();
  
  h_data->SetMarkerSize(0.6);
  h_data->Draw("pe");
  
  h_data->GetYaxis()->SetTitle(TString::Format("Events /10 GeV", (max-min)/nbin));
  h_data->GetXaxis()->SetTitle("m_{#gamma #gamma} [GeV]");
  h_data->GetYaxis()->SetRangeUser(0.1, h_data->GetMaximum()*2);
  hs.Draw("histsame");
  h_data->Draw("pesame");
  h_sum->Draw("E2same");
   
  
  
  TLegend *leg1;
  if(c!=4)leg1 = new TLegend(0.6075,0.6536441,0.8575,0.9340678, TString::Format("Category %d",c), "brNDC");
  else leg1 = new TLegend(0.5075,0.6536441,0.8075,0.9340678, TString::Format("All classes combined",c), "brNDC");
  leg1->AddEntry(h_data  ,"Data","PE");
  leg1->AddEntry(h_diphot,"#gamma + #gamma", "F");
  leg1->AddEntry(h_gj,"#gamma + jet","F");
  leg1->AddEntry(h_qcd,"QCD","F");   
  leg1->AddEntry(h_ggh,"GGH","F");   
  leg1->AddEntry(h_vh,"VH","F");   
  leg1->AddEntry(h_sum, "Bkg uncertainty", "F");
  
  
  
  leg1->SetTextSize(0.035);
  leg1->SetTextFont(42);
  leg1->SetBorderSize(0);
  leg1->SetFillStyle(0);
  leg1->Draw("same");
  
  int iPos=11 ;
  CMS_lumi( pad1,false,iPos );
  pad1->SetLogy(0);
  pad1->RedrawAxis();
  
  ctmp->cd();
  
  //-------pad 2------//
  TPad * pad2 = new TPad("pad2", "pad2",0.01,0.001,0.75,0.2);
  pad2->SetGrid();
  
  //pad2->SetTopMargin(0.01);
  pad2->SetBottomMargin(0.4);
  pad2->SetRightMargin(0.1);
  pad2->Draw();
  pad2->cd();
  
  Double_t xmax = h1_ratio1->GetXaxis()->GetXmax();
  Double_t xmin = h1_ratio1->GetXaxis()->GetXmin();
  TLine* line = new TLine(xmin,1.,xmax,1.);
  
  
  h1_ratio1->SetStats(0);
  
  h1_ratio1->Divide(h_sum);
  h1_ratio2->Divide(h_sum);
  h1_ratio1->SetMarkerStyle(20);
  h1_ratio1->SetMarkerSize(0.6);
  h1_ratio1->GetYaxis()->SetRangeUser(0., 2.); // cwr zoom
  h1_ratio1->GetYaxis()->SetNdivisions(2,false);
  h1_ratio1->GetYaxis()->SetTitle("Data/Bkg");
  h1_ratio1->GetYaxis()->SetTitleFont(42);
  h1_ratio1->GetXaxis()->SetTitleFont(42);
  h1_ratio1->GetXaxis()->SetTitle("m_{#gamma#gamma} [GeV]");
  
  h1_ratio1->GetXaxis()->SetTitleSize(0.2);
  h1_ratio1->GetXaxis()->SetLabelSize(0.16);
  h1_ratio1->GetXaxis()->SetLabelOffset(5.16);
  h1_ratio1->GetYaxis()->SetLabelSize(0.16);
  h1_ratio1->GetYaxis()->SetTitleSize(0.15);
  h1_ratio1->GetYaxis()->SetTitleOffset(0.45);
  h1_ratio1->GetXaxis()->SetTitleOffset(0.8);
  
  
  
  for(int j = 0;j<=h1_ratio1->GetNbinsX();j++){
    
    if(h_sum->GetBinContent(j))  h1_ratio1->SetBinError(j,sqrt(pow(h_data->GetBinError(j)/h_sum->GetBinContent(j), 2)+ pow(h_data->GetBinContent(j)*h_sum->GetBinError(j)/(h_sum->GetBinContent(j)*h_sum->GetBinContent(j)),2)));
    else h1_ratio1->SetBinError(j,0.);
  }
  h1_ratio1->Draw("PEX0");
  

  
  for(int j = 0;j<=h1_ratio2->GetNbinsX();j++){
      if(h_sum->GetBinContent(j)) h1_ratio2->SetBinError(j,h_sum->GetBinError(j)/h_sum->GetBinContent(j));
      else h1_ratio2->SetBinError(j,0.);
  }
  h1_ratio2->Draw("E2same");
  
  line->SetLineWidth(1.);
  line->Draw("same");
  
  ctmp->SaveAs("~/www/plotsMonoH/13TeV/DATA_MC_MASS_"+TString::Format("cat%d_EEEE.png", c));
  ctmp->SaveAs("~/www/plotsMonoH/13TeV/DATA_MC_MASS_"+TString::Format("cat%d_EEEE.pdf", c));
  
   pad1->SetLogy(1);
   ctmp->SaveAs("~/www/plotsMonoH/13TeV/DATA_MC_MASS_"+TString::Format("cat%d_LOG_EEEE.png", c));
   ctmp->SaveAs("~/www/plotsMonoH/13TeV/DATA_MC_MASS_"+TString::Format("cat%d_LOG_EEEE.pdf", c));

}



void MakePlotMETDataMC(RooWorkspace* w ) {
  TString inDir = "";

  // Luminosity:
  Float_t Lum = 40.0;  
  RooRealVar lumi("lumi","lumi",Lum);
  w->import(lumi); 
  
  // Variables
  RooArgSet* ntplVars = defineVariables();

  // common preselection cut
  TString mainCut = "(mgg>=100 && mgg<1000)&& hltDiphoton30Mass95";   //130-2000
  RooPlot*  plotPhotonsMassDataMC;
  
  //**********DATA***************//
  TFile file("data/50ns_betaV4/DoubleEG.root");
  TTree* dataTree = (TTree*) file.Get("DiPhotonTree");
   
  //**********G+jets***************//
  TChain* gjTree = new TChain();
  gjTree->Add("data/50ns_betaV4/GJets.root/DiPhotonTree");

  //**********QCD***************//
  TChain* qcdTree = new TChain();
  qcdTree->Add("data/50ns_betaV4/QCD.root/DiPhotonTree");
  
  //**********DIPHOT***************//
  TChain* diphotTree = new TChain();
  diphotTree->Add("data/50ns_betaV4/DiPhoton.root/DiPhotonTree");

  //**********GGH***************//
  TChain* gghTree = new TChain();
  gghTree->Add("data/50ns_betaV4/GluGluHToGG.root/DiPhotonTree");
  
  //**********VH***************//
  TChain* vhTree = new TChain();
  vhTree->Add("data/50ns_betaV4/VH.root/DiPhotonTree");
 
  //**********M1***************//
  TChain* m1Tree = new TChain();
  m1Tree->Add("data/50ns_betaV4/DMHtoGG_M1.root/DiPhotonTree");
  ///**********M10***************//
  TChain* m10Tree = new TChain();
  m10Tree->Add("data/50ns_betaV4/DMHtoGG_M10.root/DiPhotonTree");
  //**********M100***************//
  TChain* m100Tree = new TChain();
  m100Tree->Add("data/50ns_betaV4/DMHtoGG_M100.root/DiPhotonTree");
  //**********M1000***************//
  TChain* m1000Tree = new TChain();
  m1000Tree->Add("data/50ns_betaV4/DMHtoGG_M1000.root/DiPhotonTree");


  TH1F* h_data;
 
  TH1F*  h_gj;
  TH1F*  h_qcd;
  TH1F*  h_diphot;
  TH1F*  h_ggh;
  TH1F*  h_vh;
  TH1F*  h_m1;
  TH1F*  h_m10;
  TH1F*  h_m100;
  TH1F*  h_m1000;

  TH1F* h_sum; 
 
  
  TCanvas* ctmp = new TCanvas("ctmp","PhotonsMass Background Categories",1);
  Int_t nbin = 60;
  Double_t  min = 0;
  Double_t  max = 300;

  int c = 0;
  h_data= new TH1F(TString::Format("h_data_cat%d",c), TString::Format("h_data_cat%d",c), nbin, min, max);
  h_gj= new TH1F(TString::Format("h_gj_cat%d",c), TString::Format("h_gj_cat%d",c), nbin, min, max);
  h_qcd= new TH1F(TString::Format("h_qcd_cat%d",c), TString::Format("h_qcd_cat%d",c), nbin, min, max);
  h_diphot= new TH1F(TString::Format("h_diphot_cat%d",c), TString::Format("h_diphot_cat%d",c), nbin, min, max);
  h_ggh= new TH1F(TString::Format("h_ggh_cat%d",c), TString::Format("h_ggh_cat%d",c), nbin, min, max);
  h_vh= new TH1F(TString::Format("h_vh_cat%d",c), TString::Format("h_vh_cat%d",c), nbin, min, max);
  h_m1= new TH1F(TString::Format("h_m1_cat%d",c), TString::Format("h_m1_cat%d",c), nbin, min, max);
  h_m10= new TH1F(TString::Format("h_m10_cat%d",c), TString::Format("h_m10_cat%d",c), nbin, min, max);
  h_m100= new TH1F(TString::Format("h_m100_cat%d",c), TString::Format("h_m100_cat%d",c), nbin, min, max);
  h_m1000= new TH1F(TString::Format("h_m1000_cat%d",c), TString::Format("h_m1000_cat%d",c), nbin, min, max);

  
  
  dataTree->Draw("t1pfmet>>h_data_cat0", "("+mainCut+")*1");
  gjTree->Draw("t1pfmet>>h_gj_cat0", "("+mainCut+")*weight");
  qcdTree->Draw("t1pfmet>>h_qcd_cat0", "("+mainCut+")*weight");
  diphotTree->Draw("t1pfmet>>h_diphot_cat0","("+ mainCut+")*weight");
  gghTree->Draw("t1pfmet>>h_ggh_cat0","("+ mainCut+")*weight*0.002");
  vhTree->Draw("t1pfmet>>h_vh_cat0","("+ mainCut+")*weight*0.002");
  m1Tree->Draw("t1pfmet>>h_m1_cat0","("+ mainCut+")*weight");
  m10Tree->Draw("t1pfmet>>h_m10_cat0","("+ mainCut+")*weight");
  m100Tree->Draw("t1pfmet>>h_m100_cat0","("+ mainCut+")*weight");
  m1000Tree->Draw("t1pfmet>>h_m1000_cat0","("+ mainCut+")*weight");
 
  
  h_gj->SetFillColor(kAzure+8);
  h_gj->Sumw2();
  h_qcd->SetFillColor(kYellow+8);
  h_qcd->Sumw2();  
  h_diphot->SetFillColor(kSpring+7);
  h_diphot->Sumw2();
  h_ggh->SetFillColor(kMagenta+7);
  h_ggh->Sumw2();
  h_vh->SetFillColor(kPink+7);
  h_vh->Sumw2();
  
   
  h_sum = (TH1F*) h_gj->Clone();
  h_sum->Add(h_qcd);
  h_sum->Add(h_diphot);
  h_sum->Add(h_ggh);
  h_sum->Add(h_vh);
  h_sum->Sumw2();
  h_sum->SetFillColor(kBlack);
  h_sum->SetFillStyle(3003);
  h_sum->SetMarkerSize(0);

  //make kolmogorov test between data and MC
  Double_t CHI2ndf = h_data->Chi2Test(h_sum, "UWPCHI2/NDF");
  TPaveText* label = new TPaveText(0.6, 0.67, 0.85, 0.7, "brNDC" );
  label->SetFillColor(kWhite);
  label->SetBorderSize(0.);
  label->SetTextSize(0.038);
  label->SetTextAlign(11);
  label->SetTextFont(42);
  // label->AddText(TString::Format("#chi^{2}/NDF: %.3f", CHI2ndf));
     
  THStack hs("hs","hs");
  hs.Add(h_vh);
  hs.Add(h_ggh); 
  hs.Add(h_diphot); 
  hs.Add(h_qcd); 
  hs.Add(h_gj); 
  
  std::cout<<h_sum->Integral()<<std::endl;
  std::cout<<h_data->Integral()<<std::endl;
  std::cout<<"gj"<<h_gj->Integral()<<" %: "<<h_gj->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<"qcd "<<h_qcd->Integral()<<" %: "<<h_qcd->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<"diphot "<<h_diphot->Integral()<<" %: "<<h_diphot->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<"ggh "<<h_ggh->Integral()<<" %: "<<h_ggh->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<"vh "<<h_vh->Integral()<<" %: "<<h_vh->Integral()/h_sum->Integral()<<std::endl;
  
   
 
  ctmp->cd();
  h_data->Sumw2();
  h_sum->Sumw2();

  TH1F* h1_ratio1 = (TH1F*)h_data->Clone();
  TH1F* h1_ratio2 = (TH1F*)h_sum->Clone();

  ctmp->Clear();
  //-------pad 1-------//
  TPad * pad1 = new TPad("pad1", "pad1",0.01,0.13,0.75,1.);  
  
  
  pad1->SetRightMargin(0.1);
  
  pad1->SetLogy();
  pad1->Draw();
  pad1->cd();
  
  h_data->SetMarkerSize(0.6);
  h_data->Draw("pe");
  
  h_data->GetYaxis()->SetTitle(TString::Format("Events /10 GeV", (max-min)/nbin));
  h_data->GetXaxis()->SetTitle("m_{#gamma #gamma}NET [GeV]");
  h_data->GetYaxis()->SetRangeUser(0.1, h_data->GetMaximum()*2);
  hs.Draw("histsame");
  h_data->Draw("pesame");
  h_sum->Draw("E2same");
   
  
  
  TLegend *leg1;
  if(c!=4)leg1 = new TLegend(0.6075,0.6536441,0.8575,0.9340678, TString::Format("Category %d",c), "brNDC");
  else leg1 = new TLegend(0.5075,0.6536441,0.8075,0.9340678, TString::Format("All classes combined",c), "brNDC");
  leg1->AddEntry(h_data  ,"Data","PE");
  leg1->AddEntry(h_diphot,"#gamma + #gamma", "F");
  leg1->AddEntry(h_gj,"#gamma + jet","F");
  leg1->AddEntry(h_qcd,"QCD","F");   
  leg1->AddEntry(h_ggh,"GGH","F");   
  leg1->AddEntry(h_vh,"VH","F");   
  leg1->AddEntry(h_sum, "Bkg uncertainty", "F");
  
  
  
  leg1->SetTextSize(0.035);
  leg1->SetTextFont(42);
  leg1->SetBorderSize(0);
  leg1->SetFillStyle(0);
  leg1->Draw("same");
  
  int iPos=11 ;
  CMS_lumi( pad1,false,iPos );
  pad1->SetLogy(0);
  pad1->RedrawAxis();
  
  ctmp->cd();
  
  //-------pad 2------//
  TPad * pad2 = new TPad("pad2", "pad2",0.01,0.001,0.75,0.2);
  pad2->SetGrid();
  
  //pad2->SetTopMargin(0.01);
  pad2->SetBottomMargin(0.4);
  pad2->SetRightMargin(0.1);
  pad2->Draw();
  pad2->cd();
  
  Double_t xmax = h1_ratio1->GetXaxis()->GetXmax();
  Double_t xmin = h1_ratio1->GetXaxis()->GetXmin();
  TLine* line = new TLine(xmin,1.,xmax,1.);
  
  
  h1_ratio1->SetStats(0);
  
  h1_ratio1->Divide(h_sum);
  h1_ratio2->Divide(h_sum);
  h1_ratio1->SetMarkerStyle(20);
  h1_ratio1->SetMarkerSize(0.6);
  h1_ratio1->GetYaxis()->SetRangeUser(0., 2.); // cwr zoom
  h1_ratio1->GetYaxis()->SetNdivisions(2,false);
  h1_ratio1->GetYaxis()->SetTitle("Data/Bkg");
  h1_ratio1->GetYaxis()->SetTitleFont(42);
  h1_ratio1->GetXaxis()->SetTitleFont(42);
  h1_ratio1->GetXaxis()->SetTitle("m_{#gamma#gamma} [GeV]");
  
  h1_ratio1->GetXaxis()->SetTitleSize(0.2);
  h1_ratio1->GetXaxis()->SetLabelSize(0.16);
  h1_ratio1->GetXaxis()->SetLabelOffset(5.16);
  h1_ratio1->GetYaxis()->SetLabelSize(0.16);
  h1_ratio1->GetYaxis()->SetTitleSize(0.15);
  h1_ratio1->GetYaxis()->SetTitleOffset(0.45);
  h1_ratio1->GetXaxis()->SetTitleOffset(0.8);
  
  
  
  for(int j = 0;j<=h1_ratio1->GetNbinsX();j++){
    
    if(h_sum->GetBinContent(j))  h1_ratio1->SetBinError(j,sqrt(pow(h_data->GetBinError(j)/h_sum->GetBinContent(j), 2)+ pow(h_data->GetBinContent(j)*h_sum->GetBinError(j)/(h_sum->GetBinContent(j)*h_sum->GetBinContent(j)),2)));
    else h1_ratio1->SetBinError(j,0.);
  }
  h1_ratio1->Draw("PEX0");
  

  
  for(int j = 0;j<=h1_ratio2->GetNbinsX();j++){
      if(h_sum->GetBinContent(j)) h1_ratio2->SetBinError(j,h_sum->GetBinError(j)/h_sum->GetBinContent(j));
      else h1_ratio2->SetBinError(j,0.);
  }
  h1_ratio2->Draw("E2same");
  
  line->SetLineWidth(1.);
  line->Draw("same");
  
  ctmp->SaveAs("~/www/plotsMonoH/13TeV/DATA_MC_MET_"+TString::Format("cat%d.png", c));
  ctmp->SaveAs("~/www/plotsMonoH/13TeV/DATA_MC_MET_"+TString::Format("cat%d.pdf", c));
  
   pad1->SetLogy(1);
   ctmp->SaveAs("~/www/plotsMonoH/13TeV/DATA_MC_MET_"+TString::Format("cat%d_LOG.png", c));
   ctmp->SaveAs("~/www/plotsMonoH/13TeV/DATA_MC_MET_"+TString::Format("cat%d_LOG.pdf", c));

}



void MakePlotNVTXDataMC(RooWorkspace* w ) {
  TString inDir = "";

  // Luminosity:
  Float_t Lum = 40.0;  
  RooRealVar lumi("lumi","lumi",Lum);
  w->import(lumi); 
  
  // Variables
  RooArgSet* ntplVars = defineVariables();
  RooRealVar* nvtx = w->var("nvtx");
  // common preselection cut
  TString mainCut = "(nvtx>0 && nvtx<50 && mgg>100&&mgg<1000&&hltDiphoton30Mass95)";   //130-2000
  RooPlot*  plotPhotonsNvtxDataMC;
  
  //**********DATA***************//
  TFile file("data/50ns_betaV4/DoubleEG.root");
  TTree* dataTree = (TTree*) file.Get("DiPhotonTree");
   
  //**********G+jets***************//
  TChain* gjTree = new TChain();
  gjTree->Add("data/50ns_betaV4/GJets.root/DiPhotonTree");

  //**********QCD***************//
  TChain* qcdTree = new TChain();
  qcdTree->Add("data/50ns_betaV4/QCD.root/DiPhotonTree");
  
  //**********DIPHOT***************//
  TChain* diphotTree = new TChain();
  diphotTree->Add("data/50ns_betaV4/DiPhoton.root/DiPhotonTree");

  //**********GGH***************//
  TChain* gghTree = new TChain();
  gghTree->Add("data/50ns_betaV4/GluGluHToGG.root/DiPhotonTree");
  
  //**********VH***************//
  TChain* vhTree = new TChain();
  vhTree->Add("data/50ns_betaV4/VH.root/DiPhotonTree");
 
  //**********M1***************//
  TChain* m1Tree = new TChain();
  m1Tree->Add("data/50ns_betaV4/DMHtoGG_M1.root/DiPhotonTree");
  ///**********M10***************//
  TChain* m10Tree = new TChain();
  m10Tree->Add("data/50ns_betaV4/DMHtoGG_M10.root/DiPhotonTree");
  //**********M100***************//
  TChain* m100Tree = new TChain();
  m100Tree->Add("data/50ns_betaV4/DMHtoGG_M100.root/DiPhotonTree");
  //**********M1000***************//
  TChain* m1000Tree = new TChain();
  m1000Tree->Add("data/50ns_betaV4/DMHtoGG_M1000.root/DiPhotonTree");


  TH1F* h_data;
 
  TH1F*  h_gj;
  TH1F*  h_qcd;
  TH1F*  h_diphot;
  TH1F*  h_ggh;
  TH1F*  h_vh;
  TH1F*  h_m1;
  TH1F*  h_m10;
  TH1F*  h_m100;
  TH1F*  h_m1000;

  TH1F* h_sum; 
 
  
  TCanvas* ctmp = new TCanvas("ctmp","PhotonsNvtx Background Categories",1);
  Int_t nbin = 60;
  Double_t  min = 0;
  Double_t  max = 60;

  int c = 0;
  h_data= new TH1F(TString::Format("h_data_cat%d",c), TString::Format("h_data_cat%d",c), nbin, min, max);
  h_gj= new TH1F(TString::Format("h_gj_cat%d",c), TString::Format("h_gj_cat%d",c), nbin, min, max);
  h_qcd= new TH1F(TString::Format("h_qcd_cat%d",c), TString::Format("h_qcd_cat%d",c), nbin, min, max);
  h_diphot= new TH1F(TString::Format("h_diphot_cat%d",c), TString::Format("h_diphot_cat%d",c), nbin, min, max);
  h_ggh= new TH1F(TString::Format("h_ggh_cat%d",c), TString::Format("h_ggh_cat%d",c), nbin, min, max);
  h_vh= new TH1F(TString::Format("h_vh_cat%d",c), TString::Format("h_vh_cat%d",c), nbin, min, max);
  h_m1= new TH1F(TString::Format("h_m1_cat%d",c), TString::Format("h_m1_cat%d",c), nbin, min, max);
  h_m10= new TH1F(TString::Format("h_m10_cat%d",c), TString::Format("h_m10_cat%d",c), nbin, min, max);
  h_m100= new TH1F(TString::Format("h_m100_cat%d",c), TString::Format("h_m100_cat%d",c), nbin, min, max);
  h_m1000= new TH1F(TString::Format("h_m1000_cat%d",c), TString::Format("h_m1000_cat%d",c), nbin, min, max);

  
  
  dataTree->Draw("nvtx>>h_data_cat0", "("+mainCut+")*1");
  gjTree->Draw("nvtx>>h_gj_cat0", "("+mainCut+")*weight");
  qcdTree->Draw("nvtx>>h_qcd_cat0", "("+mainCut+")*weight");
  diphotTree->Draw("nvtx>>h_diphot_cat0","("+ mainCut+")*weight");
  gghTree->Draw("nvtx>>h_ggh_cat0","("+ mainCut+")*weight*0.002");
  vhTree->Draw("nvtx>>h_vh_cat0","("+ mainCut+")*weight*0.002");
  m1Tree->Draw("nvtx>>h_m1_cat0","("+ mainCut+")*weight");
  m10Tree->Draw("nvtx>>h_m10_cat0","("+ mainCut+")*weight");
  m100Tree->Draw("nvtx>>h_m100_cat0","("+ mainCut+")*weight");
  m1000Tree->Draw("nvtx>>h_m1000_cat0","("+ mainCut+")*weight");
 
  
  h_gj->SetFillColor(kAzure+8);
  h_gj->Sumw2();
  h_qcd->SetFillColor(kYellow+8);
  h_qcd->Sumw2();  
  h_diphot->SetFillColor(kSpring+7);
  h_diphot->Sumw2();
  h_ggh->SetFillColor(kMagenta+7);
  h_ggh->Sumw2();
  h_vh->SetFillColor(kPink+7);
  h_vh->Sumw2();
  
   
  h_sum = (TH1F*) h_gj->Clone();
  h_sum->Add(h_qcd);
  h_sum->Add(h_diphot);
  h_sum->Add(h_ggh);
  h_sum->Add(h_vh);
  h_sum->Sumw2();
  h_sum->SetFillColor(kBlack);
  h_sum->SetFillStyle(3003);
  h_sum->SetMarkerSize(0);

  //make kolmogorov test between data and MC
  Double_t CHI2ndf = h_data->Chi2Test(h_sum, "UWPCHI2/NDF");
  TPaveText* label = new TPaveText(0.6, 0.67, 0.85, 0.7, "brNDC" );
  label->SetFillColor(kWhite);
  label->SetBorderSize(0.);
  label->SetTextSize(0.038);
  label->SetTextAlign(11);
  label->SetTextFont(42);
  // label->AddText(TString::Format("#chi^{2}/NDF: %.3f", CHI2ndf));
     
  THStack hs("hs","hs");
  hs.Add(h_vh);
  hs.Add(h_ggh); 
  hs.Add(h_diphot); 
  hs.Add(h_qcd); 
  hs.Add(h_gj); 
  
  std::cout<<h_sum->Integral()<<std::endl;
  std::cout<<h_data->Integral()<<std::endl;
  std::cout<<h_gj->Integral()<<" %: "<<h_gj->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<h_qcd->Integral()<<" %: "<<h_qcd->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<h_diphot->Integral()<<" %: "<<h_diphot->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<h_ggh->Integral()<<" %: "<<h_ggh->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<h_vh->Integral()<<" %: "<<h_vh->Integral()/h_sum->Integral()<<std::endl;
  
   
 
  ctmp->cd();
  h_data->Sumw2();
  h_sum->Sumw2();

  TH1F* h1_ratio1 = (TH1F*)h_data->Clone();
  TH1F* h1_ratio2 = (TH1F*)h_sum->Clone();

  ctmp->Clear();
  //-------pad 1-------//
  TPad * pad1 = new TPad("pad1", "pad1",0.01,0.13,0.75,1.);  
  
  
  pad1->SetRightMargin(0.1);
  
  pad1->SetLogy();
  pad1->Draw();
  pad1->cd();
  
  h_data->SetMarkerSize(0.6);
  h_data->Draw("pe");
  
  h_data->GetYaxis()->SetTitle(TString::Format("Events /10 GeV", (max-min)/nbin));
  h_data->GetXaxis()->SetTitle("m_{#gamma #gamma} [GeV]");
  h_data->GetYaxis()->SetRangeUser(0.1, h_data->GetMaximum()*2);
  hs.Draw("histsame");
  h_data->Draw("pesame");
  h_sum->Draw("E2same");
   
  
  
  TLegend *leg1;
  if(c!=4)leg1 = new TLegend(0.6075,0.6536441,0.8575,0.9340678, TString::Format("Category %d",c), "brNDC");
  else leg1 = new TLegend(0.5075,0.6536441,0.8075,0.9340678, TString::Format("All classes combined",c), "brNDC");
  leg1->AddEntry(h_data  ,"Data","PE");
  leg1->AddEntry(h_diphot,"#gamma + #gamma", "F");
  leg1->AddEntry(h_gj,"#gamma + jet","F");
  leg1->AddEntry(h_qcd,"QCD","F");   
  leg1->AddEntry(h_ggh,"GGH","F");   
  leg1->AddEntry(h_vh,"VH","F");   
  leg1->AddEntry(h_sum, "Bkg uncertainty", "F");
  
  
  
  leg1->SetTextSize(0.035);
  leg1->SetTextFont(42);
  leg1->SetBorderSize(0);
  leg1->SetFillStyle(0);
  leg1->Draw("same");
  
  int iPos=11 ;
  CMS_lumi( pad1,false,iPos );
  pad1->SetLogy(0);
  pad1->RedrawAxis();
  
  ctmp->cd();
  
  //-------pad 2------//
  TPad * pad2 = new TPad("pad2", "pad2",0.01,0.001,0.75,0.2);
  pad2->SetGrid();
  
  //pad2->SetTopMargin(0.01);
  pad2->SetBottomMargin(0.4);
  pad2->SetRightMargin(0.1);
  pad2->Draw();
  pad2->cd();
  
  Double_t xmax = h1_ratio1->GetXaxis()->GetXmax();
  Double_t xmin = h1_ratio1->GetXaxis()->GetXmin();
  TLine* line = new TLine(xmin,1.,xmax,1.);
  
  
  h1_ratio1->SetStats(0);
  
  h1_ratio1->Divide(h_sum);
  h1_ratio2->Divide(h_sum);
  h1_ratio1->SetMarkerStyle(20);
  h1_ratio1->SetMarkerSize(0.6);
  h1_ratio1->GetYaxis()->SetRangeUser(0., 2.); // cwr zoom
  h1_ratio1->GetYaxis()->SetNdivisions(2,false);
  h1_ratio1->GetYaxis()->SetTitle("Data/Bkg");
  h1_ratio1->GetYaxis()->SetTitleFont(42);
  h1_ratio1->GetXaxis()->SetTitleFont(42);
  h1_ratio1->GetXaxis()->SetTitle("m_{#gamma#gamma} [GeV]");
  
  h1_ratio1->GetXaxis()->SetTitleSize(0.2);
  h1_ratio1->GetXaxis()->SetLabelSize(0.16);
  h1_ratio1->GetXaxis()->SetLabelOffset(5.16);
  h1_ratio1->GetYaxis()->SetLabelSize(0.16);
  h1_ratio1->GetYaxis()->SetTitleSize(0.15);
  h1_ratio1->GetYaxis()->SetTitleOffset(0.45);
  h1_ratio1->GetXaxis()->SetTitleOffset(0.8);
  
  
  
  for(int j = 0;j<=h1_ratio1->GetNbinsX();j++){
    
    if(h_sum->GetBinContent(j))  h1_ratio1->SetBinError(j,sqrt(pow(h_data->GetBinError(j)/h_sum->GetBinContent(j), 2)+ pow(h_data->GetBinContent(j)*h_sum->GetBinError(j)/(h_sum->GetBinContent(j)*h_sum->GetBinContent(j)),2)));
    else h1_ratio1->SetBinError(j,0.);
  }
  h1_ratio1->Draw("PEX0");
  

  
  for(int j = 0;j<=h1_ratio2->GetNbinsX();j++){
      if(h_sum->GetBinContent(j)) h1_ratio2->SetBinError(j,h_sum->GetBinError(j)/h_sum->GetBinContent(j));
      else h1_ratio2->SetBinError(j,0.);
  }
  h1_ratio2->Draw("E2same");
  
  line->SetLineWidth(1.);
  line->Draw("same");
  
  ctmp->SaveAs("~/www/plotsMonoH/13TeV/DATA_MC_NVTX_"+TString::Format("cat%d.png", c));
  ctmp->SaveAs("~/www/plotsMonoH/13TeV/DATA_MC_NVTX_"+TString::Format("cat%d.pdf", c));
  
  // pad1->SetLogy(1);
  //ctmp->SaveAs("~/www/plotsMonoH/13TeV/DATA_MC_NVTX_"+TString::Format("cat%d_LOG.png", c));
  //ctmp->SaveAs("~/www/plotsMonoH/13TeV/DATA_MC_NVTX_"+TString::Format("cat%d_LOG.pdf", c));

}











void MakePlotCutFlowDataMC(RooWorkspace* w ) {
  TString inDir = "";

  // Luminosity:
  Float_t Lum = 40.0;  
  RooRealVar lumi("lumi","lumi",Lum);
  w->import(lumi); 
  
  
  //**********DATA***************//
  TFile file("data/50ns_betaV4/DoubleEG.root");
  TH1F* h_data = (TH1F*) file.Get("h_selection");
   
  //**********G+jets***************//
  TFile gjTree("data/50ns_betaV4/GJets.root");
  TH1F* h_gj = (TH1F*) gjTree.Get("h_selection");
  //**********QCD***************//
  TFile qcdTree("data/50ns_betaV4/QCD.root");
  TH1F* h_qcd = (TH1F*) qcdTree.Get("h_selection");
  //**********DIPHOT***************//
  TFile diphotTree("data/50ns_betaV4/DiPhoton.root");
  TH1F* h_diphot = (TH1F*) diphotTree.Get("h_selection");
  //**********GGH***************//
  TFile gghTree("data/50ns_betaV4/GluGluHToGG.root");
  TH1F* h_ggh = (TH1F*) gghTree.Get("h_selection");
  //**********VH***************//
  TFile vhTree("data/50ns_betaV4/VH.root");
  TH1F* h_vh = (TH1F*) vhTree.Get("h_selection");
  //**********M1***************//
  TFile m1Tree("data/50ns_betaV4/DMHtoGG_M1.root");
  TH1F* h_m1 = (TH1F*) m1Tree.Get("h_selection");
  ///**********M10***************//
  TFile m10Tree("data/50ns_betaV4/DMHtoGG_M10.root");
  TH1F* h_m10 = (TH1F*) m10Tree.Get("h_selection");
  //**********M100***************//
  TFile m100Tree("data/50ns_betaV4/DMHtoGG_M100.root");
  TH1F* h_m100 = (TH1F*) m100Tree.Get("h_selection");
  //**********M1000***************//
  TFile m1000Tree("data/50ns_betaV4/DMHtoGG_M1000.root");
  TH1F* h_m1000 = (TH1F*) m1000Tree.Get("h_selection");


  TH1F* h_sum; 
 
  
  TCanvas* ctmp = new TCanvas("ctmp","PhotonsMass Background Categories",1);
 
   
  h_gj->SetFillColor(kAzure+8);
  h_gj->Sumw2();
  h_qcd->SetFillColor(kYellow+8);
  h_qcd->Sumw2();  
  h_diphot->SetFillColor(kSpring+7);
  h_diphot->Sumw2();
  h_ggh->SetFillColor(kMagenta+7);
  h_ggh->Sumw2();
  h_vh->SetFillColor(kPink+7);
  h_vh->Sumw2();
  // h_gj->Scale(40);
  //h_qcd->Scale(40);
  //h_diphot->Scale(40);
  h_ggh->Scale(0.002);
  h_vh->Scale(0.002);
  //h_m1->Scale(40);
  //h_m10->Scale(40);
  //h_m100->Scale(40);
  //h_m1000->Scale(40);
   
  h_sum = (TH1F*) h_gj->Clone();
  h_sum->Add(h_qcd);
  h_sum->Add(h_diphot);
  h_sum->Add(h_ggh);
  h_sum->Add(h_vh);
  h_sum->Sumw2();
  h_sum->SetFillColor(kBlack);
  h_sum->SetFillStyle(3003);
  h_sum->SetMarkerSize(0);

  //make kolmogorov test between data and MC
  Double_t CHI2ndf = h_data->Chi2Test(h_sum, "UWPCHI2/NDF");
  TPaveText* label = new TPaveText(0.6, 0.67, 0.85, 0.7, "brNDC" );
  label->SetFillColor(kWhite);
  label->SetBorderSize(0.);
  label->SetTextSize(0.038);
  label->SetTextAlign(11);
  label->SetTextFont(42);
  // label->AddText(TString::Format("#chi^{2}/NDF: %.3f", CHI2ndf));
     
  THStack hs("hs","hs");
  hs.Add(h_vh);
  hs.Add(h_ggh); 
  hs.Add(h_diphot); 
  hs.Add(h_qcd); 
  hs.Add(h_gj); 
  
  std::cout<<h_sum->Integral()<<std::endl;
  std::cout<<h_data->Integral()<<std::endl;
  std::cout<<"gj"<<h_gj->Integral()<<" %: "<<h_gj->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<"qcd "<<h_qcd->Integral()<<" %: "<<h_qcd->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<"diphot "<<h_diphot->Integral()<<" %: "<<h_diphot->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<"ggh "<<h_ggh->Integral()<<" %: "<<h_ggh->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<"vh "<<h_vh->Integral()<<" %: "<<h_vh->Integral()/h_sum->Integral()<<std::endl;
  
   
 
  ctmp->cd();
  h_data->Sumw2();
  h_sum->Sumw2();

  TH1F* h1_ratio1 = (TH1F*)h_data->Clone();
  TH1F* h1_ratio2 = (TH1F*)h_sum->Clone();

  ctmp->Clear();
  //-------pad 1-------//
  TPad * pad1 = new TPad("pad1", "pad1",0.01,0.13,0.75,1.);  
  
  
  pad1->SetRightMargin(0.1);
  
  pad1->SetLogy();
  pad1->Draw();
  pad1->cd();
  
  h_data->SetMarkerSize(0.6);
  h_data->Draw("pe");
  
  h_data->GetYaxis()->SetTitle("Events");
  h_data->GetXaxis()->SetTitle("Cut Flow");
  h_data->GetYaxis()->SetRangeUser(0.1, h_data->GetMaximum()*2);
  hs.Draw("histsame");
  h_data->Draw("pesame");
  h_sum->Draw("E2same");
   
  
  
  TLegend *leg1;

   leg1 = new TLegend(0.5075,0.6536441,0.8075,0.9340678, "", "brNDC");
  leg1->AddEntry(h_data  ,"Data","PE");
  leg1->AddEntry(h_diphot,"#gamma + #gamma", "F");
  leg1->AddEntry(h_gj,"#gamma + jet","F");
  leg1->AddEntry(h_qcd,"QCD","F");   
  leg1->AddEntry(h_ggh,"GGH","F");   
  leg1->AddEntry(h_vh,"VH","F");   
  leg1->AddEntry(h_sum, "Bkg uncertainty", "F");
  
  
  
  leg1->SetTextSize(0.035);
  leg1->SetTextFont(42);
  leg1->SetBorderSize(0);
  leg1->SetFillStyle(0);
  leg1->Draw("same");
  
  int iPos=11 ;
  CMS_lumi( pad1,false,iPos );
  pad1->SetLogy(0);
  pad1->RedrawAxis();
  
  ctmp->cd();
  
  //-------pad 2------//
  TPad * pad2 = new TPad("pad2", "pad2",0.01,0.001,0.75,0.2);
  pad2->SetGrid();
  
  //pad2->SetTopMargin(0.01);
  pad2->SetBottomMargin(0.4);
  pad2->SetRightMargin(0.1);
  pad2->Draw();
  pad2->cd();
  
  Double_t xmax = h1_ratio1->GetXaxis()->GetXmax();
  Double_t xmin = h1_ratio1->GetXaxis()->GetXmin();
  TLine* line = new TLine(xmin,1.,xmax,1.);
  
  
  h1_ratio1->SetStats(0);
  
  h1_ratio1->Divide(h_sum);
  h1_ratio2->Divide(h_sum);
  h1_ratio1->SetMarkerStyle(20);
  h1_ratio1->SetMarkerSize(0.6);
  h1_ratio1->GetYaxis()->SetRangeUser(0., 2.); // cwr zoom
  h1_ratio1->GetYaxis()->SetNdivisions(2,false);
  h1_ratio1->GetYaxis()->SetTitle("Data/Bkg");
  h1_ratio1->GetYaxis()->SetTitleFont(42);
  h1_ratio1->GetXaxis()->SetTitleFont(42);
  h1_ratio1->GetXaxis()->SetTitle("m_{#gamma#gamma} [GeV]");
  
  h1_ratio1->GetXaxis()->SetTitleSize(0.2);
  h1_ratio1->GetXaxis()->SetLabelSize(0.16);
  h1_ratio1->GetXaxis()->SetLabelOffset(5.16);
  h1_ratio1->GetYaxis()->SetLabelSize(0.16);
  h1_ratio1->GetYaxis()->SetTitleSize(0.15);
  h1_ratio1->GetYaxis()->SetTitleOffset(0.45);
  h1_ratio1->GetXaxis()->SetTitleOffset(0.8);
  
  
  
  for(int j = 0;j<=h1_ratio1->GetNbinsX();j++){
    
    if(h_sum->GetBinContent(j))  h1_ratio1->SetBinError(j,sqrt(pow(h_data->GetBinError(j)/h_sum->GetBinContent(j), 2)+ pow(h_data->GetBinContent(j)*h_sum->GetBinError(j)/(h_sum->GetBinContent(j)*h_sum->GetBinContent(j)),2)));
    else h1_ratio1->SetBinError(j,0.);
  }
  h1_ratio1->Draw("PEX0");
  

  
  for(int j = 0;j<=h1_ratio2->GetNbinsX();j++){
      if(h_sum->GetBinContent(j)) h1_ratio2->SetBinError(j,h_sum->GetBinError(j)/h_sum->GetBinContent(j));
      else h1_ratio2->SetBinError(j,0.);
  }
  h1_ratio2->Draw("E2same");
  
  line->SetLineWidth(1.);
  line->Draw("same");
  int c = 0;
  ctmp->SaveAs("~/www/plotsMonoH/13TeV/DATA_MC_CutFlow_"+TString::Format("cat%d.png", c));
  ctmp->SaveAs("~/www/plotsMonoH/13TeV/DATA_MC_CutFlow_"+TString::Format("cat%d.pdf", c));
  
   pad1->SetLogy(1);
   ctmp->SaveAs("~/www/plotsMonoH/13TeV/DATA_MC_CutFlow_"+TString::Format("cat%d_LOG.png", c));
   ctmp->SaveAs("~/www/plotsMonoH/13TeV/DATA_MC_CutFlow_"+TString::Format("cat%d_LOG.pdf", c));

}

