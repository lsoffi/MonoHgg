 
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
  
  
  /*MakePlotMassDataMC(w, "mgg", 30, 100., 180.,0);
  MakePlotMassDataMC(w, "mgg", 30, 100., 180.,1);
  MakePlotMassDataMC(w, "ptgg", 30, 0., 300.,0);
  MakePlotMassDataMC(w, "pt1", 30, 0., 300.,0);
  MakePlotMassDataMC(w, "pt2", 30, 0., 300.,0);
  MakePlotMassDataMC(w, "eta1", 20, -3., 3.,0);
  MakePlotMassDataMC(w, "eta2", 20, -3., 3.,0);
  MakePlotMassDataMC(w, "phi1", 20, -4., 4.,0);
  MakePlotMassDataMC(w, "phi2", 20, -4., 4.,0);
  //  MakePlotMassDataMC(w, "nvtx", 40, 0., 40.,0);
  MakePlotMassDataMC(w, "t1pfmet", 25, 0., 200.,0);
  
  MakePlotCutFlowDataMC(w);*/
  ComputeABCDnumbers(w,"t1pfmet:mgg");

  return;
}



void MakePlotMassDataMC(RooWorkspace* w, std::string var, int BINS, double MIN, double MAX, bool metCut ) {
  TString inDir = "";

  // Luminosity:
  Float_t Lum = 40.0;  
  RooRealVar lumi("lumi","lumi",Lum);
  w->import(lumi); 
  
  // Variables
  RooArgSet* ntplVars = defineVariables();
 
  // common preselection cut
  TString mainCut = "(mgg>=100 && mgg<200)&& hltDiphoton30Mass95";   //130-2000 abs(sceta1)>1.4442&&abs(sceta2)>1.4442&&
  if(metCut) mainCut+="&&t1pfmet>100"; 
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
  Int_t nbin = BINS;
  Double_t  min = MIN;
  Double_t  max = MAX;

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

  
  
  dataTree->Draw((var+">>h_data_cat0").c_str(), "("+mainCut+")*1");
  gjTree->Draw((var+">>h_gj_cat0").c_str(), "("+mainCut+")*weight");
  qcdTree->Draw((var+">>h_qcd_cat0").c_str(), "("+mainCut+")*weight");
  diphotTree->Draw((var+">>h_diphot_cat0").c_str(),"("+ mainCut+")*weight");
  dyTree->Draw((var+">>h_dy_cat0").c_str(),"("+ mainCut+")*weight");
  gghTree->Draw((var+">>h_ggh_cat0").c_str(),"("+ mainCut+")*weight*0.002");
  vhTree->Draw((var+">>h_vh_cat0").c_str(),"("+ mainCut+")*weight*0.002");
  m1Tree->Draw((var+">>h_m1_cat0").c_str(),"("+ mainCut+")*weight");
  m10Tree->Draw((var+">>h_m10_cat0").c_str(),"("+ mainCut+")*weight");
  m100Tree->Draw((var+">>h_m100_cat0").c_str(),"("+ mainCut+")*weight");
  m1000Tree->Draw((var+">>h_m1000_cat0").c_str(),"("+ mainCut+")*weight");
 
  
  h_gj->SetFillColor(kGreen-9);
  h_gj->Sumw2();
  h_qcd->SetFillColor(kYellow+8);
  h_qcd->Sumw2();  
  h_diphot->SetFillColor(kTeal-1);
  h_diphot->Sumw2();
  h_dy->SetFillColor(kTeal-7);
  h_dy->Sumw2();
  h_ggh->SetFillColor(kOrange-2);
  h_ggh->Sumw2();
  h_vh->SetFillColor(kOrange-3);
  h_vh->Sumw2();
  
  h_m1->SetLineColor(kPink-2);
  h_m10->SetLineColor(kPink-6);
  h_m100->SetLineColor(kPink+6);
  h_m1000->SetLineColor(kPink+8);
  h_m1->SetLineWidth(2);
  h_m10->SetLineWidth(2);
  h_m100->SetLineWidth(2);
  h_m1000->SetLineWidth(2);
   
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
  hs.Add(h_gj); 
  hs.Add(h_qcd); 

  
  std::cout<<h_sum->Integral()<<std::endl;
  std::cout<<h_data->Integral()<<std::endl;

   
  std::cout<<"gj "<<h_gj->Integral()<<" %: "<<h_gj->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<"qcd "<<h_qcd->Integral()<<" %: "<<h_qcd->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<"diphot "<<h_diphot->Integral()<<" %: "<<h_diphot->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<"dy "<<h_dy->Integral()<<" %: "<<h_dy->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<"ggh "<<h_ggh->Integral()<<" %: "<<h_ggh->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<"vh "<<h_vh->Integral()<<" %: "<<h_vh->Integral()/h_sum->Integral()<<std::endl;
 
  std::cout<<"M1 "<<h_m1->Integral()<<std::endl;
  std::cout<<"M10 "<<h_m10->Integral()<<std::endl;
  std::cout<<"M100 "<<h_m100->Integral()<<std::endl;
  std::cout<<"M1000 "<<h_m1000->Integral()<<std::endl; 
 
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
  
  h_data->GetYaxis()->SetTitle(TString::Format("Events /%.2f GeV", double((max-min))/nbin));
  h_data->GetXaxis()->SetTitle("m_{#gamma #gamma} [GeV]");
  h_data->GetYaxis()->SetRangeUser(0.001, h_data->GetMaximum()*2);
  hs.Draw("histsame");
  h_data->Draw("pesame");
  h_sum->Draw("E2same");
  h_m1->Draw("histsame");
  h_m10->Draw("histsame");
  h_m100->Draw("histsame");
  h_m1000->Draw("histsame");
  
  
  TLegend *leg1;
  leg1 = new TLegend(0.5575,0.56441,0.8575,0.9340678,"", "brNDC");
 

  leg1->AddEntry(h_m1,"m_{#chi} = 1 GeV","L");   
  leg1->AddEntry(h_m10,"m_{#chi} = 10 GeV","L");   
  leg1->AddEntry(h_m100,"m_{#chi} = 100 GeV","L");   
  leg1->AddEntry(h_m1000,"m_{#chi} = 1000 GeV","L");   

  leg1->AddEntry(h_ggh,"GGH","F");   
  leg1->AddEntry(h_vh,"VH","F");   
  
  leg1->AddEntry(h_diphot,"#gamma + #gamma", "F");
  leg1->AddEntry(h_dy,"DYJets","F"); 
  leg1->AddEntry(h_gj,"#gamma + jet","F");

  
  leg1->AddEntry(h_qcd,"QCD","F");   
  leg1->AddEntry(h_data  ,"Data","PE");
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
  if(var == "ptgg")h1_ratio1->GetXaxis()->SetTitle("p_{T_{#gamma#gamma}} [GeV]");
  if(var == "pt1")h1_ratio1->GetXaxis()->SetTitle("p_{T1} [GeV]");
  if(var == "pt2")h1_ratio1->GetXaxis()->SetTitle("p_{T2} [GeV]");
  if(var == "eta1")h1_ratio1->GetXaxis()->SetTitle("#eta_{1}");
  if(var == "eta2")h1_ratio1->GetXaxis()->SetTitle("#eta_{2}");
  if(var == "phi1")h1_ratio1->GetXaxis()->SetTitle("#phi_{1}");
  if(var == "phi2")h1_ratio1->GetXaxis()->SetTitle("#phi_{2}");
  if(var == "mgg")h1_ratio1->GetXaxis()->SetTitle("m_{#gamma#gamma} [GeV]");
  if(var == "t1pfmet")h1_ratio1->GetXaxis()->SetTitle("MET [GeV]");
  if(var == "nvtx")h1_ratio1->GetXaxis()->SetTitle("nvtx");
  
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
  if(metCut){ 
  ctmp->SaveAs(("~/www/plotsMonoH/13TeV/DATA_MC_"+var+"_met100.png").c_str());
  ctmp->SaveAs(("~/www/plotsMonoH/13TeV/DATA_MC_"+var+"_met100.pdf").c_str());
  pad1->cd();
  pad1->SetLogy(1);
  h_data->GetYaxis()->SetRangeUser(0.001, h_data->GetMaximum()*200);
  ctmp->SaveAs(("~/www/plotsMonoH/13TeV/DATA_MC_"+var+"_LOG_met100.png").c_str());
  ctmp->SaveAs(("~/www/plotsMonoH/13TeV/DATA_MC_"+var+"_LOG_met100.pdf").c_str());
  }else{
  ctmp->SaveAs(("~/www/plotsMonoH/13TeV/DATA_MC_"+var+".png").c_str());
  ctmp->SaveAs(("~/www/plotsMonoH/13TeV/DATA_MC_"+var+".pdf").c_str());
  pad1->cd();
  pad1->SetLogy(1);
  h_data->GetYaxis()->SetRangeUser(0.001, h_data->GetMaximum()*20000);
  if(var == "nvtx") h_data->GetYaxis()->SetRangeUser(0.01, h_data->GetMaximum()*200);
  ctmp->SaveAs(("~/www/plotsMonoH/13TeV/DATA_MC_"+var+"_LOG.png").c_str());
  ctmp->SaveAs(("~/www/plotsMonoH/13TeV/DATA_MC_"+var+"_LOG.pdf").c_str());

  }
  //print numbers in signal region


    // prepare table latex
    double ggIntErr;
    double ggInt; 
    double ppIntErr;
    double ppInt;  
    double pfIntErr;
    double pfInt; 
    double dyIntErr;
    double dyInt; 
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
  
    if(var=="mgg"&&metCut){
      ppInt = h_qcd->IntegralAndError(h_qcd->FindBin(110),h_qcd->FindBin(130), ppIntErr, "");
      pfInt = h_gj->IntegralAndError(h_gj->FindBin(110),h_gj->FindBin(130), pfIntErr, ""); 
      ggInt = h_diphot->IntegralAndError(h_diphot->FindBin(110),h_diphot->FindBin(130), ggIntErr, ""); 
      dyInt = h_dy->IntegralAndError(h_dy->FindBin(110),h_dy->FindBin(130), dyIntErr, ""); 
      hggInt = h_ggh->IntegralAndError(h_ggh->FindBin(110),h_ggh->FindBin(130), hggIntErr, "");
      VhInt = h_vh->IntegralAndError(h_vh->FindBin(110),h_vh->FindBin(130), VhIntErr, ""); 
      sumInt = h_sum->IntegralAndError(h_sum->FindBin(110),h_sum->FindBin(130), sumIntErr, "");   
      sig1Int = h_m1->IntegralAndError(h_m1->FindBin(110),h_m1->FindBin(130), sig1IntErr, "");   
      sig10Int = h_m10->IntegralAndError(h_m10->FindBin(110),h_m10->FindBin(130), sig10IntErr, "");   
      sig100Int = h_m100->IntegralAndError(h_m100->FindBin(110),h_m100->FindBin(130), sig100IntErr, "");   
      sig1000Int = h_m1000->IntegralAndError(h_m1000->FindBin(110),h_m1000->FindBin(130), sig1000IntErr, "");
    } 
  
    if(h_gj->GetEntries()==0)pfInt = 0;
    if(h_gj->GetEntries()==0)pfIntErr = 0;
    RooRealVar gg("gg","gg",ggInt, "");
    gg.setError(ggIntErr);

    RooRealVar pp("pp","pp",ppInt, "");
    pp.setError(ppIntErr);

    RooRealVar pf("pf","pf",pfInt, "");
    pf.setError(pfIntErr);
    
    RooRealVar dy("dy","dy",dyInt, "");
    dy.setError(dyIntErr);
  
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
    std::cout<<"$\\sqrt{s}$ = 13 TeV; L = 40 $fb^{-1}$ - 50 ns"<<" \\\\" <<std::endl;
    std::cout<<"$m_{\\gamma \\gamma}$ in $[110-130]$ and MET $>$ 100 GeV"<<" \\\\" <<std::endl;
    std::cout<<"\\hline" <<std::endl;
    std::cout<<"QCD &  "<<*(pp.format(2,"EXPF"))<<" \\\\"<<std::endl;
    std::cout<<"DYJets &  "<<*(dy.format(2,"EXPF"))<<" \\\\"<<std::endl;
    std::cout<<"\\gamma + jets &  "<<*(pf.format(2,"EXPF"))<<" \\\\"<<std::endl;
    std::cout<<"$\\gamma \\gamma$ &  "<<*(gg.format(2,"EXPF"))<<" \\\\"<<std::endl;
    std::cout<<"$h \\rightarrow  \\gamma \\gamma$  (ggH)&  "<<*(hgg.format(2,"EXPF"))<<" \\\\"<<std::endl;
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

    

      

     std::cout<<"----------------------------------------------------------"<<std::endl;




}





void ComputeABCDnumbers(RooWorkspace* w, std::string var ) {
 TString inDir = "";

  // Luminosity:
  Float_t Lum = 40.0;  
  RooRealVar lumi("lumi","lumi",Lum);
  w->import(lumi); 
  
  // Variables
  RooArgSet* ntplVars = defineVariables();
 
  // common preselection cut
  TString mainCut = "(mgg>=100 && mgg<300)&& hltDiphoton30Mass95";   //130-2000 abs(sceta1)>1.4442&&abs(sceta2)>1.4442&&

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

  TH2F*  h_diphot;
  TH2F*  h_hgg;
  TH2F*  h_Vh;
  TH2F*  h_dy;
  TH2F* h_sig1;
  TH2F* h_sig10;
  TH2F* h_sig100;
  TH2F* h_sig1000;
  TH2F* h_sum; 
  TH2F* h_pp;
  TH2F* h_pf;
  Int_t xnbin = 120;
  Double_t  xmin = 100;
  Double_t  xmax = 300;
  Int_t ynbin = 180;
  Double_t  ymin = 0;
  Double_t  ymax = 300;
  int c = 1;
  h_pf= new TH2F(TString::Format("h_pf",c), TString::Format("h_pf",c), xnbin, xmin, xmax, ynbin, ymin, ymax);
  h_pp= new TH2F(TString::Format("h_pp",c), TString::Format("h_pp",c), xnbin, xmin, xmax, ynbin, ymin, ymax);
  h_diphot= new TH2F(TString::Format("h_diphot",c), TString::Format("h_diphot",c), xnbin, xmin, xmax, ynbin, ymin, ymax);  
  h_dy= new TH2F(TString::Format("h_dy",c), TString::Format("h_dy",c), xnbin, xmin, xmax, ynbin, ymin, ymax);
  h_hgg= new TH2F(TString::Format("h_hgg",c), TString::Format("h_hgg",c), xnbin, xmin, xmax, ynbin, ymin, ymax);
  h_Vh= new TH2F(TString::Format("h_Vh",c), TString::Format("h_Vh",c), xnbin, xmin, xmax, ynbin, ymin, ymax);
  
  h_sig1= new TH2F(TString::Format("h_sig1",c), TString::Format("h_sig1",c), xnbin, xmin, xmax, ynbin, ymin, ymax);
  h_sig10= new TH2F(TString::Format("h_sig10",c), TString::Format("h_sig10",c), xnbin, xmin, xmax, ynbin, ymin, ymax);
  h_sig100= new TH2F(TString::Format("h_sig100",c), TString::Format("h_sig100",c), xnbin, xmin, xmax, ynbin, ymin, ymax);
  h_sig1000= new TH2F(TString::Format("h_sig1000",c), TString::Format("h_sig1000",c), xnbin, xmin, xmax, ynbin, ymin, ymax);
 
 
  gjTree->Draw((var+">>h_pf").c_str(), "("+mainCut+")*weight");
  qcdTree->Draw((var+">>h_pp").c_str(), "("+mainCut+")*weight");
  diphotTree->Draw((var+">>h_diphot").c_str(),"("+ mainCut+")*weight");
  dyTree->Draw((var+">>h_dy").c_str(),"("+ mainCut+")*weight");
  gghTree->Draw((var+">>h_hgg").c_str(),"("+ mainCut+")*weight*0.002");
  vhTree->Draw((var+">>h_Vh").c_str(),"("+ mainCut+")*weight*0.002");
  m1Tree->Draw((var+">>h_sig1").c_str(),"("+ mainCut+")*weight");
  m10Tree->Draw((var+">>h_sig10").c_str(),"("+ mainCut+")*weight");
  m100Tree->Draw((var+">>h_sig100").c_str(),"("+ mainCut+")*weight");
  m1000Tree->Draw((var+">>h_sig1000").c_str(),"("+ mainCut+")*weight");
 
  std::cout<<"flag"<<std::endl;
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


  
 h_sum = (TH2F*) h_dy->Clone();
 h_sum->Add(h_diphot);
 h_sum->Add(h_pf);
 h_sum->Add(h_pp);
    

    int mggA1min = h_sum->GetXaxis()->FindBin(100);
    int mggA1max = h_sum->GetXaxis()->FindBin(110)-1;
    int mggB1min = h_sum->GetXaxis()->FindBin(100);
    int mggB1max = h_sum->GetXaxis()->FindBin(110)-1;
    int mggA2min = h_sum->GetXaxis()->FindBin(130)+1;
    int mggA2max = h_sum->GetXaxis()->FindBin(180);
    int mggB2min = h_sum->GetXaxis()->FindBin(130)+1;
    int mggB2max = h_sum->GetXaxis()->FindBin(180);
   
    int mggDCmin = h_sum->GetXaxis()->FindBin(110);
    int mggDCmax = h_sum->GetXaxis()->FindBin(130);
    
    int metCmin = h_sum->GetYaxis()->FindBin(0);
    int metCmax = h_sum->GetYaxis()->FindBin(100)-1;
    int metDmin = h_sum->GetYaxis()->FindBin(100);
    int metDmax = h_sum->GetYaxis()->FindBin(400);


    double ppNCerr;
    double ppNDerr;
    double ppNA1err;
    double ppNA2err;
    double ppNB1err;
    double ppNB2err;
    double ppNAerr;
    double ppNBerr;
  
    double ppNC = h_pp->IntegralAndError(mggDCmin, mggDCmax,metCmin, metCmax, ppNCerr, "");    
    double ppND = h_pp->IntegralAndError(mggDCmin, mggDCmax,metDmin, metDmax, ppNDerr, "");
    double ppNA1 = h_pp->IntegralAndError(mggA1min, mggA1max,metDmin, metDmax, ppNA1err, "");
    double ppNB1 = h_pp->IntegralAndError(mggB1min, mggB1max,metCmin, metCmax, ppNB1err, "");
    double ppNA2 = h_pp->IntegralAndError(mggA2min, mggA2max,metDmin, metDmax, ppNA2err, "");
    double ppNB2 = h_pp->IntegralAndError(mggB2min, mggB2max,metCmin, metCmax, ppNB2err, "");
    ppNAerr=sqrt(ppNA1err*ppNA1err+ppNA2err*ppNA2err);
    ppNBerr=sqrt(ppNB1err*ppNB1err+ppNB2err*ppNB2err);
    double ppNA=ppNA1+ppNA2;
    double ppNB=ppNB1+ppNB2;
    double ppcorr = h_pp->GetCorrelationFactor(1,2);

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
    std::cout<<"\\begin{table}[bthp]" <<std::endl;
    std::cout<<"\\begin{tabular}{cccccccc}" <<std::endl;
    std::cout<<"\\hline \\hline" <<std::endl;
    std::cout<<"$\\sqrt{s}$ = 13 TeV; L = 40 $fb^{-1}$ - 50 ns"<<" \\\\" <<std::endl;
    std::cout<<"\\hline" <<std::endl;
    
    std::cout<<"Process & $f_{corr}$&$N_A$&$N_B$&$N_C$&$N_D$&$N_D$ Exp.& $\Delta N_D$\\\\"<<std::endl;
    std::cout<<"\\hline" <<std::endl;
    std::cout<<"QCD & "<<ppcorr<<" & "<<*ppA.format(2,"EXPP")<<"&"<<*ppB.format(2,"EXPP")<<"&"<<*ppC.format(2,"EXPP")<<"&"<<*ppD.format(2,"EXPP")<<"&"<<*ppDexp.format(2,"EXPP")<<"&"<<ppDiff<<"\\\\"<<std::endl;
  double dyNCerr;
    double dyNDerr;
    double dyNA1err;
    double dyNA2err;
    double dyNB1err;
    double dyNB2err;
    double dyNAerr;
    double dyNBerr;
  

    double dyNC = h_dy->IntegralAndError(mggDCmin, mggDCmax,metCmin, metCmax, dyNCerr, "");    
    double dyND = h_dy->IntegralAndError(mggDCmin, mggDCmax,metDmin, metDmax, dyNDerr, "");
    double dyNA1 = h_dy->IntegralAndError(mggA1min, mggA1max,metDmin, metDmax, dyNA1err, "");
    double dyNB1 = h_dy->IntegralAndError(mggB1min, mggB1max,metCmin, metCmax, dyNB1err, "");
    double dyNA2 = h_dy->IntegralAndError(mggA2min, mggA2max,metDmin, metDmax, dyNA2err, "");
    double dyNB2 = h_dy->IntegralAndError(mggB2min, mggB2max,metCmin, metCmax, dyNB2err, "");
    dyNAerr=sqrt(dyNA1err*dyNA1err+dyNA2err*dyNA2err);
    dyNBerr=sqrt(dyNB1err*dyNB1err+dyNB2err*dyNB2err);
    double dyNA=dyNA1+dyNA2;
    double dyNB=dyNB1+dyNB2;
    double dycorr = h_dy->GetCorrelationFactor(1,2);

    double dyExpND = dyNC*dyNA/dyNB;
    double dyExpNDerr = sqrt((dyNCerr*dyNCerr*dyNA*dyNA/(dyNB*dyNB))+(dyNAerr*dyNAerr*dyNC*dyNC/(dyNB*dyNB))+(dyNBerr*dyNBerr*dyNC*dyNC*dyNA*dyNA/(pow(dyNB,4))));
    double dyDiff = TMath::Abs((dyNC*dyNA/dyNB-dyND)/(dyNC*dyNA/dyNB));
    RooRealVar dyA("dyA","dyA",dyNA,"");
    dyA.setError(dyNAerr);
    RooRealVar dyB("dyB","dyB",dyNB,"");
    dyB.setError(dyNBerr);
    RooRealVar dyC("dyC","dyC",dyNC,"");
    dyC.setError(dyNCerr);
    RooRealVar dyD("dyD","dyD",dyND,"");
    dyD.setError(dyNDerr);
    RooRealVar dyDexp("dyDexp","dyDexp",dyExpND,"");
    dyDexp.setError(dyExpNDerr);
    std::cout<<"DYJets & "<<dycorr<<" & "<<*dyA.format(2,"EXDY")<<"&"<<*dyB.format(2,"EXDY")<<"&"<<*dyC.format(2,"EXDY")<<"&"<<*dyD.format(2,"EXDY")<<"&"<<*dyDexp.format(2,"EXDY")<<"&"<<dyDiff<<"\\\\"<<std::endl;
 



  
    double pfNCerr;
    double pfNDerr;
    double pfNA1err;
    double pfNA2err;
    double pfNB1err;
    double pfNB2err;
    double pfNAerr;
    double pfNBerr;
  

    double pfNC = h_pf->IntegralAndError(mggDCmin, mggDCmax,metCmin, metCmax, pfNCerr, "");    
    double pfND = h_pf->IntegralAndError(mggDCmin, mggDCmax,metDmin, metDmax, pfNDerr, "");
    double pfNA1 = h_pf->IntegralAndError(mggA1min, mggA1max,metDmin, metDmax, pfNA1err, "");
    double pfNB1 = h_pf->IntegralAndError(mggB1min, mggB1max,metCmin, metCmax, pfNB1err, "");
    double pfNA2 = h_pf->IntegralAndError(mggA2min, mggA2max,metDmin, metDmax, pfNA2err, "");
    double pfNB2 = h_pf->IntegralAndError(mggB2min, mggB2max,metCmin, metCmax, pfNB2err, "");
    pfNAerr=sqrt(pfNA1err*pfNA1err+pfNA2err*pfNA2err);
    pfNBerr=sqrt(pfNB1err*pfNB1err+pfNB2err*pfNB2err);
    double pfNA=pfNA1+pfNA2;
    double pfNB=pfNB1+pfNB2;
    double pfcorr = h_pf->GetCorrelationFactor(1,2); 

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
    std::cout<<"$\\gamma$ + jets & "<<pfcorr<<" & "<<*pfA.format(2,"EXPF")<<"&"<<*pfB.format(2,"EXPF")<<"&"<<*pfC.format(2,"EXPF")<<"&"<<*pfD.format(2,"EXPF")<<"&"<<*pfDexp.format(2,"EXPF")<<"&"<<pfDiff<<"\\\\"<<std::endl;
 



  

    double diphotNCerr;
    double diphotNDerr;
    double diphotNA1err;
    double diphotNA2err;
    double diphotNB1err;
    double diphotNB2err;
    double diphotNAerr;
    double diphotNBerr;
  

    double diphotNC = h_diphot->IntegralAndError(mggDCmin, mggDCmax,metCmin, metCmax, diphotNCerr, "");    
    double diphotND = h_diphot->IntegralAndError(mggDCmin, mggDCmax,metDmin, metDmax, diphotNDerr, "");
    double diphotNA1 = h_diphot->IntegralAndError(mggA1min, mggA1max,metDmin, metDmax, diphotNA1err, "");
    double diphotNB1 = h_diphot->IntegralAndError(mggB1min, mggB1max,metCmin, metCmax, diphotNB1err, "");
    double diphotNA2 = h_diphot->IntegralAndError(mggA2min, mggA2max,metDmin, metDmax, diphotNA2err, "");
    double diphotNB2 = h_diphot->IntegralAndError(mggB2min, mggB2max,metCmin, metCmax, diphotNB2err, "");
    diphotNAerr=sqrt(diphotNA1err*diphotNA1err+diphotNA2err*diphotNA2err);
    diphotNBerr=sqrt(diphotNB1err*diphotNB1err+diphotNB2err*diphotNB2err);
    double diphotNA=diphotNA1+diphotNA2;
    double diphotNB=diphotNB1+diphotNB2;
    double diphotcorr = h_diphot->GetCorrelationFactor(1,2);

    double diphotExpND = diphotNC*diphotNA/diphotNB;
    double diphotExpNDerr = sqrt((diphotNCerr*diphotNCerr*diphotNA*diphotNA/(diphotNB*diphotNB))+(diphotNAerr*diphotNAerr*diphotNC*diphotNC/(diphotNB*diphotNB))+(diphotNBerr*diphotNBerr*diphotNC*diphotNC*diphotNA*diphotNA/(pow(diphotNB,4))));
    double diphotDiff = TMath::Abs((diphotNC*diphotNA/diphotNB-diphotND)/(diphotNC*diphotNA/diphotNB));
    RooRealVar diphotA("diphotA","diphotA",diphotNA,"");
    diphotA.setError(diphotNAerr);
    RooRealVar diphotB("diphotB","diphotB",diphotNB,"");
    diphotB.setError(diphotNBerr);
    RooRealVar diphotC("diphotC","diphotC",diphotNC,"");
    diphotC.setError(diphotNCerr);
    RooRealVar diphotD("diphotD","diphotD",diphotND,"");
    diphotD.setError(diphotNDerr);
    RooRealVar diphotDexp("diphotDexp","diphotDexp",diphotExpND,"");
    diphotDexp.setError(diphotExpNDerr);
    std::cout<<"$\\gamma \\gamma$ & "<<diphotcorr<<" & "<<*diphotA.format(2,"EXPF")<<"&"<<*diphotB.format(2,"EXPF")<<"&"<<*diphotC.format(2,"EXPF")<<"&"<<*diphotD.format(2,"EXPF")<<"&"<<*diphotDexp.format(2,"EXPF")<<"&"<<diphotDiff<<"\\\\"<<std::endl;






 
   double sumNCerr;
   double sumNDerr;
   double sumNA1err;
   double sumNA2err;
   double sumNB1err;
   double sumNB2err;
   double sumNAerr;
   double sumNBerr;
  
   
    std::cout<<"\\hline"<<std::endl;
    double sumNC = h_sum->IntegralAndError(mggDCmin, mggDCmax,metCmin, metCmax, sumNCerr, "");    
    double sumND = h_sum->IntegralAndError(mggDCmin, mggDCmax,metDmin, metDmax, sumNDerr, "");
    double sumNA1 = h_sum->IntegralAndError(mggA1min, mggA1max,metDmin, metDmax, sumNA1err, "");
    double sumNB1 = h_sum->IntegralAndError(mggB1min, mggB1max,metCmin, metCmax, sumNB1err, "");
    double sumNA2 = h_sum->IntegralAndError(mggA2min, mggA2max,metDmin, metDmax, sumNA2err, "");
    double sumNB2 = h_sum->IntegralAndError(mggB2min, mggB2max,metCmin, metCmax, sumNB2err, "");
    sumNAerr=sqrt(sumNA1err*sumNA1err+sumNA2err*sumNA2err);
    sumNBerr=sqrt(sumNB1err*sumNB1err+sumNB2err*sumNB2err);
    double sumNA=sumNA1+sumNA2;
    double sumNB=sumNB1+sumNB2;
    double sumcorr = h_sum->GetCorrelationFactor(1,2);
    
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
 
    std::cout<<"\\hline \\hline" <<std::endl;
    std::cout<<"\\end{tabular}" <<std::endl;
    std::cout<<"\\end{table}" <<std::endl;


    
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
 //**********DY***************//
  TFile dyTree("data/50ns_betaV4/DYJetsToLL.root");
  TH1F* h_dy = (TH1F*) dyTree.Get("h_selection");
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
 
    
  h_gj->SetFillColor(kGreen-9);
  h_gj->Sumw2();
  h_qcd->SetFillColor(kYellow+8);
  h_qcd->Sumw2();  
  h_diphot->SetFillColor(kTeal-1);
  h_diphot->Sumw2();
  h_dy->SetFillColor(kTeal-7);
  h_dy->Sumw2();
  h_ggh->SetFillColor(kOrange-2);
  h_ggh->Sumw2();
  h_vh->SetFillColor(kOrange-3);
  h_vh->Sumw2();
  
  h_m1->SetLineColor(kPink-2);
  h_m10->SetLineColor(kPink-6);
  h_m100->SetLineColor(kPink+6);
  h_m1000->SetLineColor(kPink+8);
  h_m1->SetLineWidth(2);
  h_m10->SetLineWidth(2);
  h_m100->SetLineWidth(2);
  h_m1000->SetLineWidth(2);
 
  h_ggh->Scale(0.002);
  h_vh->Scale(0.002);
   
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
  hs.Add(h_gj); 
  hs.Add(h_qcd); 

  
  std::cout<<h_sum->Integral()<<std::endl;
  std::cout<<h_data->Integral()<<std::endl;
  std::cout<<"gj"<<h_gj->Integral()<<" %: "<<h_gj->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<"qcd "<<h_qcd->Integral()<<" %: "<<h_qcd->Integral()/h_sum->Integral()<<std::endl;
  std::cout<<"dy "<<h_dy->Integral()<<" %: "<<h_dy->Integral()/h_sum->Integral()<<std::endl;
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
  leg1->AddEntry(h_dy,"DYJets","F");
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
  // h1_ratio1->GetXaxis()->SetTitle("m_{#gamma#gamma} [GeV]");
  
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
  ctmp->SaveAs("~/www/plotsMonoH/13TeV/DATA_MC_CutFlow.png");
  ctmp->SaveAs("~/www/plotsMonoH/13TeV/DATA_MC_CutFlow.pdf");
  
  pad1->SetLogy(1);
  ctmp->SaveAs("~/www/plotsMonoH/13TeV/DATA_MC_CutFlow_LOG.png");
  ctmp->SaveAs("~/www/plotsMonoH/13TeV/DATA_MC_CutFlow_LOG.pdf");

}

