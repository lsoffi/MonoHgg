#include "ReweightPU.hh"

ReweightPU::ReweightPU(const TString MC, const TString Data, const Double_t lumi, const Int_t nBins, const TString indir, const TString outdir) {

  // save samples for PU weighting
  fMCName = MC;
  fDataName = Data;
 
  fInDir = indir;
 
  std::cout << "Comparing " << fMCName.Data() << " vs. " << fDataName.Data() <<std::endl;
  // save lumi
  fLumi = lumi;

  // set nBins for nvtx distribution
  fNBins = nBins;

  std::cout << "lumi " << lumi << " nbins " << nBins << std::endl;

  // set outputs
  fOutDir  = outdir;
  MakeOutDirectory(outdir); 
  fOutType = "png";

  // Initialize output TH1D's for data
  fOutDataNvtx = new TH1D("nvtx_data","",fNBins,0.,Double_t(fNBins));
  fOutDataNvtx->Sumw2();

  // Initialize outputs for MC
  fOutMCNvtx = new TH1D("nvtx_mc","",fNBins,0.,Double_t(fNBins));
  fOutMCNvtx->Sumw2();

  // Intialize Ratio Hist
  fOutDataOverMCNvtx = new TH1D("nvtx_dataOverMC","",fNBins,0.,Double_t(fNBins));
  fOutDataOverMCNvtx->Sumw2();

  // Initialize Output root file to be used by other macros ... eventually could integrate... no need now
  fOutFile = new TFile(Form("%s/PURW_%s.root",fOutDir.Data(),fMCName.Data()),"RECREATE");
}

ReweightPU::~ReweightPU() {
  //delete hists
  delete fOutDataNvtx;
  delete fOutMCNvtx;
  delete fOutDataOverMCNvtx;

  //delete
  delete fOutFile;
}

DblVec ReweightPU::GetPUWeights() {

  DblVec puweights; // return weights

  // get vtx distribution for data first
  // files + trees + tmp hist for data
  TString filename = Form("%s%s.root",fInDir.Data(),fDataName.Data());
  TFile * fileData = TFile::Open(filename.Data());
  CheckValidFile(fileData,filename);

  TTree * treeData = (TTree*)fileData->Get("DiPhotonTree");      
  CheckValidTree(treeData,"DiPhotonTree",filename);      
  TH1D * tmpnvtxData = new TH1D("tmpnvtxData","",fNBins,0.,Double_t(fNBins));
  tmpnvtxData->Sumw2();

  // fill each input data nvtx
  std::cout << "Reading data nvtx: " << filename.Data() << std::endl;
  treeData->Draw("nvtx>>tmpnvtxData");
 
  // add input data hist to total data hist
  fOutDataNvtx->Add(tmpnvtxData);
  //tmpnvtxData->Draw();
  //fOutDataNvtx->Draw();
  fOutDataNvtx->Print();

  // delete objects
  delete tmpnvtxData;
  delete treeData;
  delete fileData;

  // get vtx distribution for mc second
  // files + trees for mc + tmp hists
  filename = Form("%s%s.root",fInDir.Data(),fMCName.Data());
  TFile * fileMC = TFile::Open(filename.Data());
  CheckValidFile(fileMC,filename);

  TTree * treeMC = (TTree*)fileMC->Get("DiPhotonTree");      
  CheckValidTree(treeMC,"DiPhotonTree",filename);            
  TH1D * tmpnvtxMC = new TH1D("tmpnvtxMC","",fNBins,0.,Double_t(fNBins));
  tmpnvtxMC->Sumw2();

  // fill each input mc nvtx
  std::cout << "Reading MC nvtx: " << filename.Data() << std::endl;
  treeMC->Draw("nvtx>>tmpnvtxMC");

  // add input mc hist to total mc hist
  fOutMCNvtx->Add(tmpnvtxMC);
  fOutMCNvtx->Print();

  // delete objects
  delete tmpnvtxMC;
  delete treeMC;
  delete fileMC;
  

  // Set line colors
  fOutDataNvtx->SetLineColor(kRed);
  fOutMCNvtx->SetLineColor(kBlue);
  
  // use these for scaling and rescaling
  const Double_t int_DataNvtx = fOutDataNvtx->Integral();
  const Double_t int_MCNvtx   = fOutMCNvtx->Integral();
  //std::cout << "DataNvtx = " << int_DataNvtx << " MCNvtx = " << int_MCNvtx << std::endl;

  TCanvas * c0 = new TCanvas(); // Draw before reweighting --> unscaled
  c0->cd();
  c0->SetTitle("Before PU Reweighting Unnormalized");

  // draw and save in output directory --> appended by what selection we used for this pu reweight
  fOutDataNvtx->Draw("PE");
  fOutMCNvtx->Draw("HIST SAME");

  CMSLumi(c0,0,fLumi);

  c0->SetLogy(1); // save log
  c0->SaveAs(Form("%snvtx_%s_beforePURW_unnorm_log.%s",fOutDir.Data(),fMCName.Data(),fOutType.Data()));

  c0->SetLogy(0); // save lin
  c0->SaveAs(Form("%snvtx_%s_beforePURW_unnorm.%s",fOutDir.Data(),fMCName.Data(),fOutType.Data()));
  
  /////////////////////////////////////////////
  //       SCALE HERE TO GET REWEIGHTING     //
  /////////////////////////////////////////////
  // scale to unit area to not bias against data
  fOutDataNvtx->Scale(1.0/int_DataNvtx);  
  fOutMCNvtx->Scale(1.0/int_MCNvtx);

  // Draw before reweighting -- scaled
  TCanvas * c1 = new TCanvas();
  c1->cd();
  c1->SetTitle("Before PU Reweighting Normalized");

  // draw and save in output directory --> appended by what selection we used for this pu reweight
  fOutDataNvtx->Draw("PE");
  fOutMCNvtx->Draw("HIST SAME");

  CMSLumi(c1,0,fLumi);

  c1->SetLogy(1); // save log
  c1->SaveAs(Form("%snvtx_%s_beforePURW_norm_log.%s",fOutDir.Data(),fMCName.Data(),fOutType.Data()));

  c1->SetLogy(0); // save lin
  c1->SaveAs(Form("%snvtx_%s_beforePURW_norm.%s",fOutDir.Data(),fMCName.Data(),fOutType.Data()));



  /////////////////////////////////////////////
  //      DIVIDE HERE TO GET REWEIGHTING     //
  /////////////////////////////////////////////

  // copy fOutDataNvtx to save output of reweights properly
  for (Int_t ibin = 1; ibin <= fNBins; ibin++) {
    fOutDataOverMCNvtx->SetBinContent(ibin,fOutDataNvtx->GetBinContent(ibin));
  }

  // divide Data/MC after copy, now this original hist will be used for reweighting 
  fOutDataOverMCNvtx->Divide(fOutMCNvtx);

  /////////////////////////////////////////////
  //      STORE HERE TO USE REWEIGHTING      //
  /////////////////////////////////////////////

  // push back reweights and then scale MC to demonstrate that it works
  for (Int_t ibin = 1; ibin <= fNBins; ibin++) {
    // push back reweights 
    puweights.push_back(fOutDataOverMCNvtx->GetBinContent(ibin)); 

    // scale MC appropriately
    Double_t tmp = fOutMCNvtx->GetBinContent(ibin);
    fOutMCNvtx->SetBinContent(ibin,puweights[ibin-1]*tmp); 
  }

  fOutFile->cd();
  fOutDataOverMCNvtx->Write();

  TCanvas * c = new TCanvas();
  fOutDataOverMCNvtx->Draw("PE");
  CMSLumi(c,0,fLumi);
  c->SetLogy(1);
  c->SaveAs(Form("%spurw_%s_log.%s",fOutDir.Data(),fMCName.Data(),fOutType.Data()));
  c->SetLogy(0);
  c->SaveAs(Form("%spurw_%s.%s",fOutDir.Data(),fMCName.Data(),fOutType.Data()));


  // Draw after reweighting 
  TCanvas * c2 = new TCanvas();
  c2->cd();
  c2->SetTitle("After PU Reweighting Normalized");

  // draw output and save it, see comment above about selection
  fOutDataNvtx->Draw("PE");
  fOutMCNvtx->Draw("HIST SAME");

  CMSLumi(c2,0,fLumi);

  c2->SetLogy(1); // save log
  c2->SaveAs(Form("%snvtx_%s_afterPURW_norm_log.%s",fOutDir.Data(),fMCName.Data(),fOutType.Data()));

  c2->SetLogy(0); // save lin
  c2->SaveAs(Form("%snvtx_%s_afterPURW_norm.%s",fOutDir.Data(),fMCName.Data(),fOutType.Data()));

  TCanvas * c3 = new TCanvas(); // Draw before reweighting --> unscaled
  c3->cd();
  c3->SetTitle("After PU Reweighting Unnormalized"); 

  // now that the reweighting is applied, see total events again
  fOutDataNvtx->Scale(int_DataNvtx);
  fOutMCNvtx->Scale(int_MCNvtx);
  
  fOutDataNvtx->Draw("PE");
  fOutMCNvtx->Draw("HIST SAME");

  CMSLumi(c3,0,fLumi);

  c3->SetLogy(1); // save log
  c3->SaveAs(Form("%snvtx_%s_afterPURW_unnorm_log.%s",fOutDir.Data(),fMCName.Data(),fOutType.Data()));

  c3->SetLogy(0); // save lin
  c3->SaveAs(Form("%snvtx_%s_afterPURW_unnorm.%s",fOutDir.Data(),fMCName.Data(),fOutType.Data()));
  
  delete c0;
  delete c1;
  delete c2;
  delete c3;

  return puweights;
}
