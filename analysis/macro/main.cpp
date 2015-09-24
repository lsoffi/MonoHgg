
  // Compile:
  // make clean
  // make
  //
  // Call this script:
  // ./main
  //



#include "Plotter.hh"
#include "Combiner.hh"
#include "ReweightPU.hh"
#include "ABCDMethod.hh"
#include "Style.hh"


#include "TROOT.h"
#include "TStyle.h"

#include <iostream>

typedef std::pair<TString,Double_t>  SampleYieldPair;
typedef std::vector<SampleYieldPair> SampleYieldPairVec;

static bool sortByYield(const SampleYieldPair& mcpair1, const SampleYieldPair& mcpair2) {
  return mcpair1.second<=mcpair2.second;
}

int main(){
  setTDRStyle();

  TString inDir = "./data/50ns_betaV4/";
  TString outDir = "./diPhoPlots/50ns_betaV4/";

  bool doFakeData = false;	// use FakeData to test combiner
  bool doTest = false;		// run plotter on test sample
  bool makePURWfiles = false;	// recompute PURW and make files
  bool doReweightPU = true;	// use PURW from old files if !makePURWfiles
  bool doPlots = true;		// make plots for each sample individually
  bool doComb = true;		// make stack/overlay plots
  bool doABCD = true;		// run ABCD method 

  Double_t lumi = 41.64; // in pb^-1 
  UInt_t nBins_vtx = 60; // number of bins for PURW 
  
  //for CMSSW_7_0_pre9: run with root
  //gROOT->LoadMacro("Plotter.cpp++g");
  //Plotter * test1 = new Plotter("./data/ALL_nosel/diPhotons","./diPhoPlots/ALL_nosel/","DMHtoGG",30);

  /////////////////////////////////////////////////////
  //
  // Pile up reweighting
  //
  // Inputs to ReweightPU
  // 1st : MC Sample to weight
  // 2nd : Data Sample
  // 3rd : lumi
  // 4th : number of bins for nvtx
  // 5th : input directory of samples
  // 6th : output directory
  //
  /////////////////////////////////////////////////////

  DblVec	puweights_Data;

  DblVec 	puweights_QCD;
  DblVec 	puweights_GJets;
  DblVec	puweights_GGHGG;
  DblVec	puweights_WZH;
  DblVec	puweights_GG;

  DblVec	puweights_sig1;
  DblVec	puweights_sig10;
  DblVec	puweights_sig100;
  DblVec	puweights_sig1000;	

  // no puweight for data 
  for (UInt_t i=1; i<=nBins_vtx; i++){ puweights_Data.push_back(1.0); }

  if (doReweightPU){
    if (makePURWfiles){ 
      std::cout << "Doing PU Reweighting QCD" << std::endl;
      ReweightPU * reweightQCD = new ReweightPU("QCD","DoubleEG",lumi, nBins_vtx, inDir, outDir+"purw/");
      puweights_QCD = reweightQCD->GetPUWeights();
      delete reweightQCD;

      std::cout << "Doing PU Reweighting DMHtoGG_M1000" << std::endl;
      ReweightPU * reweightDMH1000 = new ReweightPU("DMHtoGG_M1000","DoubleEG",lumi, nBins_vtx, inDir, outDir+"purw/");
      puweights_sig1000 = reweightDMH1000->GetPUWeights();
      delete reweightDMH1000;

      puweights_WZH = puweights_QCD;
      puweights_GGHGG = puweights_QCD;
      puweights_GJets = puweights_QCD;
      puweights_GG = puweights_QCD;
      puweights_sig100 = puweights_sig1000;
      puweights_sig10 = puweights_sig1000;
      puweights_sig1 = puweights_sig1000;

    }

    else{ //load PURW from already made files
      TString fSigName = Form("%spurw/PURW_DMHtoGG_M1000.root",outDir.Data());
      TString fBkgName = Form("%spurw/PURW_QCD.root",outDir.Data());
      TFile *fSig = TFile::Open(fSigName.Data());
      CheckValidFile(fSig,fSigName);
      TFile *fBkg = TFile::Open(fBkgName.Data());
      CheckValidFile(fBkg,fBkgName);
      TH1D *fSigRatio = (TH1D*)fSig->Get("nvtx_dataOverMC");  
      CheckValidTH1D(fSigRatio,"nvtx_dataOverMC",fSigName);
      TH1D *fBkgRatio = (TH1D*)fBkg->Get("nvtx_dataOverMC");  
      CheckValidTH1D(fBkgRatio,"nvtx_dataOverMC",fBkgName);
       
      for (UInt_t i=1; i<=nBins_vtx; i++){
        puweights_QCD.push_back(fBkgRatio->GetBinContent(i));
        puweights_GJets.push_back(fBkgRatio->GetBinContent(i));
        puweights_GGHGG.push_back(fBkgRatio->GetBinContent(i));
        puweights_WZH.push_back(fBkgRatio->GetBinContent(i));
        puweights_GG.push_back(fBkgRatio->GetBinContent(i));

        puweights_sig1000.push_back(fSigRatio->GetBinContent(i));
        puweights_sig100.push_back(fSigRatio->GetBinContent(i));
        puweights_sig10.push_back(fSigRatio->GetBinContent(i));
        puweights_sig1.push_back(fSigRatio->GetBinContent(i));
      }
    }
  }// end doReweightPU 

  else{ // if not doReweightPU, set puweights to 1
    std::cout << "No PU Reweighting applied" << std::endl;
    for (UInt_t i=1; i<=nBins_vtx; i++){
      puweights_QCD.push_back(1.0);
      puweights_GJets.push_back(1.0);
      puweights_GGHGG.push_back(1.0);
      puweights_WZH.push_back(1.0);
      puweights_GG.push_back(1.0);
      puweights_sig1000.push_back(1.0);
      puweights_sig100.push_back(1.0);
      puweights_sig10.push_back(1.0);
      puweights_sig1.push_back(1.0);
    }
  }  

//  std::cout << "PU reweight values "<<std::endl;
//  for (UInt_t i=1; i<=nBins_vtx; i++){
//    std::cout << "puweights_Data    " << puweights_Data[i]    << std::endl;
//    std::cout << "puweights_QCD     " << puweights_QCD[i]     << std::endl;   
//    std::cout << "puweights_GJets   " << puweights_GJets[i]   << std::endl;
//    std::cout << "puweights_GGHGG   " << puweights_GGHGG[i]   << std::endl;
//    std::cout << "puweights_GG      " << puweights_GG[i]	<< std::endl;
//    std::cout << "puweights_sig1000 " << puweights_sig1000[i] << std::endl; 
//    std::cout << "puweights_sig100  " << puweights_sig100[i]  << std::endl;
//    std::cout << "puweights_sig10   " << puweights_sig10[i]   << std::endl;
//    std::cout << "puweights_sig1    " << puweights_sig1[i]    << std::endl;
//  }

  std::cout << "Finished PU Reweighting" << std::endl;

  /////////////////////////////////////////////////////
  //
  // Make plots for each sample
  //
  // Arguments to Plotter:
  // 1st : location of input data
  // 2nd : output data location
  // 3rd : name of sample 
  // 4th : lumi of data
  // 5th : bool isSigMC (overrides trigger)
  //
  /////////////////////////////////////////////////////

  if (doTest && doPlots){
    std::cout << "Working on test sample" << std::endl;
    Plotter * test = new Plotter(inDir,outDir,"GJets",puweights_GJets,lumi,false);
    test->DoPlots();
    delete test;
    std::cout << "Finished test sample" << std::endl;
  }
  if (doFakeData && doPlots){
    std::cout << "Working on FakeData sample" << std::endl;
    Plotter * FakeData = new Plotter(inDir,outDir,"FakeData",puweights_Data,lumi,false);
    FakeData->DoPlots();
    delete FakeData;
    std::cout << "Finished FakeData sample" << std::endl;
  }
  if (doPlots){
    std::cout << "Working on DoubleEG sample" << std::endl;
    Plotter * dEG = new Plotter(inDir,outDir,"DoubleEG",puweights_Data,lumi,false);
    dEG->DoPlots();
    delete dEG;
    std::cout << "Finished DoubleEG sample" << std::endl;

    std::cout << "Working on GJets sample" << std::endl;
    Plotter * GJets = new Plotter(inDir,outDir,"GJets",puweights_GJets,lumi,false);
    GJets->DoPlots();
    delete GJets;
    std::cout << "Finished GJets sample" << std::endl;

    std::cout << "Working on QCD sample" << std::endl;
    Plotter * QCD = new Plotter(inDir,outDir,"QCD",puweights_QCD,lumi,false);
    QCD->DoPlots();
    delete QCD;
    std::cout << "Finished QCD sample" << std::endl;

    std::cout << "Working on WZH sample" << std::endl;
    Plotter * WZH = new Plotter(inDir,outDir,"VH",puweights_WZH,lumi,false);
    WZH->DoPlots();
    delete WZH;
    std::cout << "Finished WZH sample" << std::endl;

    std::cout << "Working on GluGluH sample" << std::endl;
    Plotter * GGHGG = new Plotter(inDir,outDir,"GluGluHToGG",puweights_GGHGG,lumi,false);
    GGHGG->DoPlots();
    delete GGHGG;
    std::cout << "Finished GluGluH sample" << std::endl;
  
    std::cout << "Working on DiPhoton sample" << std::endl;
    Plotter * GG = new Plotter(inDir,outDir,"DiPhoton",puweights_GG,lumi,false);
    GG->DoPlots();
    delete GG;
    std::cout << "Finished GluGluH sample" << std::endl;

    std::cout << "Working on DMHgg M1000 sample" << std::endl;
    Plotter * DMH_M1000 = new Plotter(inDir,outDir,"DMHtoGG_M1000",puweights_sig1000,lumi,true);
    DMH_M1000->DoPlots();
    delete DMH_M1000;
    std::cout << "Finished DMHgg M1000 sample" << std::endl;
  
    std::cout << "Working on DMHgg M100 sample" << std::endl;
    Plotter * DMH_M100 = new Plotter(inDir,outDir,"DMHtoGG_M100",puweights_sig100,lumi,true);
    DMH_M100->DoPlots();
    delete DMH_M100;
    std::cout << "Finished DMHgg M100 sample" << std::endl;
  
    std::cout << "Working on DMHgg M10 sample" << std::endl;
    Plotter * DMH_M10 = new Plotter(inDir,outDir,"DMHtoGG_M10",puweights_sig10,lumi,true);
    DMH_M10->DoPlots();
    delete DMH_M10;
    std::cout << "Finished DMHgg M10 sample" << std::endl;
  
    std::cout << "Working on DMHgg M1 sample" << std::endl;
    Plotter * DMH_M1 = new Plotter(inDir,outDir,"DMHtoGG_M1",puweights_sig1,lumi,true);
    DMH_M1->DoPlots();
    delete DMH_M1;
    std::cout << "Finished DMHgg M1 sample" << std::endl;

  }// end doPlots

  //clear the vectors after they have been used
  puweights_Data.clear();
  puweights_QCD.clear();
  puweights_GJets.clear();
  puweights_GGHGG.clear();
  puweights_WZH.clear();
  puweights_GG.clear();
  puweights_sig1000.clear();
  puweights_sig100.clear();
  puweights_sig10.clear();
  puweights_sig1.clear();

  // setup all samples for Combiner and ABCD
  ColorMap colorMap;
  colorMap["QCD"] 			= kYellow;
  colorMap["GJets"] 			= kGreen;
  colorMap["VH"]			= kCyan+3;
  colorMap["GluGluHToGG"]		= kCyan;
  colorMap["DiPhoton"]			= kOrange-2;
  colorMap["DMHtoGG_M1"]		= kRed;
  colorMap["DMHtoGG_M10"]		= kRed+1;
  colorMap["DMHtoGG_M100"]		= kMagenta+1;
  colorMap["DMHtoGG_M1000"]		= kMagenta;
  colorMap["DoubleEG"]			= kBlack;
  if (doFakeData) colorMap["FakeData"]	= kBlack; 

  SamplePairVec Samples; // vector to also be used for stack plots
  Samples.push_back(SamplePair("QCD",1)); 
  Samples.push_back(SamplePair("VH",1));
  Samples.push_back(SamplePair("GJets",1)); 
  Samples.push_back(SamplePair("GluGluHToGG",1)); 
  Samples.push_back(SamplePair("DiPhoton",1));
  Samples.push_back(SamplePair("DMHtoGG_M1",0)); 
  Samples.push_back(SamplePair("DMHtoGG_M10",0)); 
  Samples.push_back(SamplePair("DMHtoGG_M100",0)); 
  Samples.push_back(SamplePair("DMHtoGG_M1000",0)); 
  Samples.push_back(SamplePair("DoubleEG",5));
  if (doFakeData) Samples.push_back(SamplePair("FakeData",5));

  UInt_t nbkg = 0;
  UInt_t nsig = 0;
  UInt_t ndata = 0;
  
  for (SamplePairVecIter iter=Samples.begin(); iter != Samples.end(); ++iter){
    std::cout << "Analyzing Sample: "<< (*iter).first.Data() << std::endl;
    if ((*iter).second == 1) {nbkg++;}
    else if ((*iter).second == 0) {nsig++;}
    else {ndata++;} 
  }
  UInt_t nsamples = nbkg + nsig + ndata;
   
  SamplePairVec BkgSamples;
  SamplePairVec SigSamples;
  SamplePairVec DataSamples;
  for (UInt_t isample = 0; isample < nsamples; isample++){
    if (Samples[isample].second == 0) SigSamples.push_back(Samples[isample]);
    else if (Samples[isample].second == 1) BkgSamples.push_back(Samples[isample]);
    else  DataSamples.push_back(Samples[isample]);
  }

  // to sort MC by smallest to largest for nice stacked plots
  SampleYieldPairVec tmp_mcyields;
  for (UInt_t mc = 0; mc < nbkg; mc++) {
      // open mc file first
      TString mcfilename = Form("%s%s/plots_%s.root",outDir.Data(),BkgSamples[mc].first.Data(),BkgSamples[mc].first.Data());
      TFile * tmp_mcfile = TFile::Open(mcfilename.Data());
      // open nvtx plot
      TH1D * tmpnvtx = (TH1D*)tmp_mcfile->Get("nvtx_n-1");
      // get yield and push back with corresponding sample name
      tmp_mcyields.push_back(SampleYieldPair(BkgSamples[mc].first,tmpnvtx->Integral()));
  
      delete tmpnvtx;
      delete tmp_mcfile;
   }
  
   std::sort(tmp_mcyields.begin(),tmp_mcyields.end(),sortByYield);
   std::cout << "Finished sorting MC, now put samples in right order to be processed" << std::endl;
   BkgSamples.clear();
    for (UInt_t mc = 0; mc < nbkg; mc++) { // init mc double hists
      BkgSamples.push_back(SamplePair(tmp_mcyields[mc].first,1));
    }
  
  Samples.clear();
  for (UInt_t data = 0; data < ndata; data++ ) {
    Samples.push_back(DataSamples[data]);
  }
  for (UInt_t mc = 0; mc < nbkg; mc++ ) {
    Samples.push_back(BkgSamples[mc]);
  }
  for (UInt_t mc = 0; mc < nsig; mc++) {
    Samples.push_back(SigSamples[mc]);
  }

  ////////////////////////////////////////////////////
  //
  // Make comb (stack & overlay) plots w/ all samples 
  //
  // Arguments of Combiner
  // 1st : SamplePairVec (Samples) that has Name,VALUE
  // 2rd : lumi
  // 3rd : ColorMap for samples
  // 4th : output directory
  // 5th : bool do N-1 plots 
  //
  ////////////////////////////////////////////////////

  if (doComb){
    // make overlayed and stack plots
    // Combiner( Samples, lumi, colorMap , outDir, doNmin1plots )
    Combiner *combAll = new Combiner(Samples,lumi,colorMap,outDir,false);
    combAll->DoComb();
    delete combAll;   
  
    Combiner *combAlln1 = new Combiner(Samples,lumi,colorMap,outDir,true);
    combAlln1->DoComb();
    delete combAlln1;   
  }// end doComb

  ////////////////////////////////////////////////////
  //
  // Make comb (stack & overlay) plots w/ all samples 
  //
  // Arguments of Combiner
  // 1st : SamplePairVec (Samples) that has Name,VALUE
  // 2rd : lumi
  // 3th : output directory
  //
  ////////////////////////////////////////////////////

  if (doABCD){
    ABCDMethod *abcd = new ABCDMethod(Samples,lumi,outDir);
    abcd->DoAnalysis();
    delete abcd; 
  }// end doABCD

}// end main
