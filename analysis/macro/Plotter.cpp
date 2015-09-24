#include "Plotter.hh"
#include "Style.hh"
#include "../../../DataFormats/Math/interface/deltaPhi.h"
//#include "mkPlotsLivia/CMS_lumi.C"

Plotter::Plotter( TString inName, TString outName, TString inSpecies, const DblVec puweights, const Double_t lumi, Bool_t sigMC){

  // Get ROOT file
  name = inName;
  species = inSpecies;
  isSigMC = sigMC;
  inFile = TFile::Open(Form("%s%s.root",name.Data(),species.Data()));
  CheckValidFile(inFile,Form("%s%s.root",name.Data(),species.Data()));  
  // Open Tree from inFile
  tpho = (TTree*)inFile->Get("DiPhotonTree"); 
  CheckValidTree(tpho,"DiPhotonTree",Form("%s%s.root",name.Data(),species.Data()));

  fLumi = lumi;
  fPUWeights = puweights;

  // Make output directory
  fName = outName;
  TString FullPath = fName.Data();
  FullPath+=species.Data();
  FullPath+="/";
  MakeOutDirectory(FullPath.Data());

  // Make output ROOT file
  outFile = new TFile(Form("%s/%s/plots_%s.root",fName.Data(),species.Data(),species.Data()),"RECREATE");
  CheckValidFile(outFile,Form("%s/%s/plots_%s.root",fName.Data(),species.Data(),species.Data()));

  // set all the branch addresses appropriately
  Plotter::SetBranchAddresses();

}// end Plotter::Plotter


Plotter::~Plotter(){
  std::cout << "Finished & Deleting" <<std::endl;
  std::cout << "Deleting inTree" <<std::endl;
  delete tpho;
  std::cout << "Deleting inFile" <<std::endl;
  delete inFile;
  // Write and Close output ROOT file
  //Plotter::DeleteBranches();
  std::cout << "Delete histos" <<std::endl;
  Plotter::DeleteHists();
  std::cout << "Deleting outFile" <<std::endl;
  delete outFile;
  std::cout << "Finished Deleting" <<std::endl;
}// end Plotter::~Plotter


void Plotter::DoPlots(){
  Plotter::SetUpPlots();
 
  nentries = tpho->GetEntries(); 

  
  Double_t effPUn[60]={0};
  Double_t effPUd[60]={0};
  Double_t effptn[60]={0};
  Double_t effptd[60]={0};

  fTH1DMap["hlt"]->Fill(0.5,nentries);

  for (UInt_t entry = 0; entry < nentries; entry++){
    tpho->GetEntry(entry);

    // calculate the weight
    Double_t Weight = (weight)*fPUWeights[nvtx];

    if (hltPhoton26Photon16Mass60 == 1) fTH1DMap["hlt"]->Fill(1.5,1);
    if (hltPhoton36Photon22Mass15 == 1) fTH1DMap["hlt"]->Fill(2.5,1);
    if (hltPhoton42Photon25Mass15 == 1) fTH1DMap["hlt"]->Fill(3.5,1);
    if (hltDiphoton30Mass95 == 1)   fTH1DMap["hlt"]->Fill(4.5,1);
    if (hltDiphoton30Mass70 == 1)   fTH1DMap["hlt"]->Fill(5.5,1);
    if (hltDiphoton30Mass55 == 1)   fTH1DMap["hlt"]->Fill(6.5,1);
    if (hltDiphoton30Mass55PV == 1) fTH1DMap["hlt"]->Fill(7.5,1);
    if (hltDiphoton30Mass55EB == 1) fTH1DMap["hlt"]->Fill(8.5,1);

    fTH1DMap["eff_sel"]->Fill(0.5,Weight);

    Bool_t passCH1  = false;
    Bool_t passCH2  = false;
    Bool_t passNH1  = false;
    Bool_t passNH2  = false;
    Bool_t passPH1  = false;
    Bool_t passPH2  = false;
    Bool_t passS1   = false;
    Bool_t passS2   = false;
    Bool_t passHE1  = false;
    Bool_t passHE2  = false;
    Bool_t passAll1 = false;
    Bool_t passAll2 = false;
    Bool_t passBoth = false;

    if (passCHiso1==1) passCH1 = true; 
    if (passCHiso2==1) passCH2 = true; 
    if (passNHiso1==1) passNH1 = true;
    if (passNHiso2==1) passNH2 = true;
    if (passPHiso1==1) passPH1 = true;
    if (passPHiso2==1) passPH2 = true;
    if (passSieie1==1) passS1  = true;
    if (passSieie2==1) passS2  = true;
    if (passHoe1==1)   passHE1 = true; 
    if (passHoe2==1)   passHE2 = true; 

    if (passCH1 && passNH1 && passPH1 && passS1 && passHE1) passAll1 = true;
    if (passCH2 && passNH2 && passPH2 && passS2 && passHE2) passAll2 = true;
    if (passAll1 && passAll2) passBoth = true;

    //start full selection for plots
    if ( pt1 > mgg/3 && pt2 > mgg/4){
      fTH1DMap["eff_sel"]->Fill(1.5,Weight);
      if ( isSigMC ||  hltDiphoton30Mass95==1){ //passes trigger

        fTH1DMap["eff_sel"]->Fill(2.5,Weight);
        //Fill histograms
        fTH1DMap["mgg"]->Fill(mgg,Weight);
        fTH1DMap["nvtx"]->Fill(nvtx,Weight);
        fTH1DMap["ptgg"]->Fill(ptgg,Weight);
        fTH1DMap["pt1"]->Fill(pt1,Weight);
        fTH1DMap["pt2"]->Fill(pt2,Weight);
        fTH1DMap["t1pfmet"]->Fill(t1pfmet,Weight);
        fTH1DMap["t1pfmetphi"]->Fill(t1pfmetphi,Weight);
        fTH1DMap["pfmet"]->Fill(pfmet,Weight);
        fTH1DMap["pfmetphi"]->Fill(pfmetphi,Weight);
        fTH1DMap["calomet"]->Fill(calomet,Weight);
        fTH1DMap["calometphi"]->Fill(calometphi,Weight);
        fTH1DMap["phi1"]->Fill(phi1,Weight);
        fTH1DMap["phi2"]->Fill(phi2,Weight);
        fTH1DMap["eta1"]->Fill(eta1,Weight);
        fTH1DMap["eta2"]->Fill(eta2,Weight);
        fTH1DMap["chiso1"]->Fill(chiso1,Weight);
        fTH1DMap["chiso2"]->Fill(chiso2,Weight);
        fTH1DMap["neuiso1"]->Fill(neuiso1,Weight);
        fTH1DMap["neuiso2"]->Fill(neuiso2,Weight);
        fTH1DMap["phoiso1"]->Fill(phoiso1,Weight);
        fTH1DMap["phoiso2"]->Fill(phoiso2,Weight);
        fTH1DMap["sieie1"]->Fill(sieie1,Weight);
        fTH1DMap["sieie2"]->Fill(sieie2,Weight);
        fTH1DMap["hoe1"]->Fill(hoe1,Weight);
        fTH1DMap["hoe2"]->Fill(hoe2,Weight);
        fTH1DMap["r91"]->Fill(r91,Weight);
        fTH1DMap["r92"]->Fill(r92,Weight);
        fTH1DMap["t1pfmet_zoom"]->Fill(t1pfmet,Weight);


        fTH2DMap["mgg_PU"]->Fill(nvtx,mgg,Weight);
        fTH2DMap["mgg_ptgg"]->Fill(ptgg,mgg,Weight);
        fTH2DMap["t1pfmet_PU"]->Fill(nvtx,t1pfmet,Weight);
        fTH2DMap["t1pfmet_ptgg"]->Fill(ptgg,t1pfmet,Weight);
   



        //std::cout << passCH1 <<" "<< passNH1 <<" "<< passPH1 <<" "<< passHE1 <<" "<< passS1 << std::endl; 
        //std::cout << passCH2 <<" "<< passNH2 <<" "<< passPH2 <<" "<< passHE2 <<" "<< passS2 << std::endl; 
        //std::cout << passAll1 <<" "<< passAll2 <<" "<< passBoth << std::endl;

        //fill n-1 plots for the photon ID selection variables
        if (passCH1 && passNH1 && passPH1 && passS1)  fTH1DMap["hoe1_n-1"]->Fill(hoe1,Weight); 
        if (passCH1 && passNH1 && passPH1 && passHE1) fTH1DMap["sieie1_n-1"]->Fill(sieie1,Weight);
        if (passCH1 && passNH1 && passHE1 && passS1)  fTH1DMap["phoiso1_n-1"]->Fill(phoiso1,Weight);
        if (passCH1 && passPH1 && passHE1 && passS1)  fTH1DMap["neuiso1_n-1"]->Fill(neuiso1,Weight);
        if (passPH1 && passNH1 && passHE1 && passS1)  fTH1DMap["chiso1_n-1"]->Fill(chiso1,Weight);

        if (passCH2 && passNH2 && passPH2 && passS2)  fTH1DMap["hoe2_n-1"]->Fill(hoe2,Weight); 
        if (passCH2 && passNH2 && passPH2 && passHE2) fTH1DMap["sieie2_n-1"]->Fill(sieie2,Weight);
        if (passCH2 && passNH2 && passHE2 && passS2)  fTH1DMap["phoiso2_n-1"]->Fill(phoiso2,Weight);
        if (passCH2 && passPH2 && passHE2 && passS2)  fTH1DMap["neuiso2_n-1"]->Fill(neuiso2,Weight);
        if (passPH2 && passNH2 && passHE2 && passS2)  fTH1DMap["chiso2_n-1"]->Fill(chiso2,Weight);

        if (passAll1){// fill pho1 plots if these photons pass phoID
          fTH1DMap["pt1_n-1"]->Fill(pt1,Weight);
          fTH1DMap["r91_n-1"]->Fill(r91,Weight);
          fTH1DMap["phi1_n-1"]->Fill(phi1,Weight);
          fTH1DMap["eta1_n-1"]->Fill(eta1,Weight);
        }
        if (passAll2){// fill pho2 plots if these photons pass phoID
          fTH1DMap["pt2_n-1"]->Fill(pt2,Weight);
          fTH1DMap["r92_n-1"]->Fill(r92,Weight);
          fTH1DMap["phi2_n-1"]->Fill(phi2,Weight);
          fTH1DMap["eta2_n-1"]->Fill(eta2,Weight);
        } 
        if (passBoth){
          fTH2DMap["t1pfmet_mgg"]->Fill(mgg,t1pfmet,Weight);
          fTH1DMap["nvtx_n-1"]->Fill(nvtx,Weight);
          fTH1DMap["mgg_n-1"]->Fill(mgg,Weight);  
          fTH1DMap["ptgg_n-1"]->Fill(ptgg,Weight);  
          fTH1DMap["t1pfmet_n-1"]->Fill(t1pfmet,Weight);  
          fTH1DMap["t1pfmetphi_n-1"]->Fill(t1pfmetphi,Weight);  
          fTH1DMap["pfmet_n-1"]->Fill(pfmet,Weight);
          fTH1DMap["pfmetphi_n-1"]->Fill(pfmetphi,Weight);
          fTH1DMap["calomet_n-1"]->Fill(calomet,Weight);
          fTH1DMap["calometphi_n-1"]->Fill(calometphi,Weight);
          if (mgg >= 110 && mgg <= 130) fTH1DMap["t1pfmet_selmgg"]->Fill(t1pfmet,Weight); 
          if (t1pfmet >= 100) fTH1DMap["mgg_selt1pfmet"]->Fill(mgg,Weight); 
        }

        if (passCH1 && passCH2){
          fTH1DMap["eff_sel"]->Fill(3.5,Weight);
          if (passNH1 && passNH2){
            fTH1DMap["eff_sel"]->Fill(4.5,Weight);
            if (passPH1 && passPH2){
              fTH1DMap["eff_sel"]->Fill(5.5,Weight);
              if (passS1 && passS2){ 
                fTH1DMap["eff_sel"]->Fill(6.5,Weight);
         	if (passHE1 && passHE2){
                  fTH1DMap["eff_sel"]->Fill(7.5,Weight);
                  if (mgg >= 110 && mgg <= 130){
            	    fTH1DMap["eff_sel"]->Fill(8.5,Weight);
            	    if (t1pfmet >= 100){
            	      fTH1DMap["eff_sel"]->Fill(9.5,Weight);
            	    }
                  }
                }
              }
            }
          }
        }

        for (UInt_t i = 0; i < 60; i++){
          if (nvtx == i){
            effPUd[i]++;
            if (passBoth) effPUn[i]++;
          }
          if (ptgg >= 10*i && ptgg < 10*(i+1)){
            effptd[i]++;
            if (passBoth) effptn[i]++;
          }
        }
 
        // calculate phi of the Higgs
        Float_t phigg = TMath::ATan((pt1*TMath::Sin(phi1) - pt2*TMath::Sin(phi2)) / (pt1*TMath::Cos(phi1) - pt2*TMath::Cos(phi2)));
        fTH1DMap["phigg"]->Fill(phigg,Weight);
        // calculate delta phi of Higgs & MET
        Float_t dphi_ggmet = deltaPhi(phigg,t1pfmetphi);
        fTH1DMap["dphi_ggmet"]->Fill(dphi_ggmet,Weight);

      }// end if passes trigger
    }// end if passes pt cuts 
    
    if ( isSigMC ||  hltDiphoton30Mass95==1){ //passes trigger
      if(passAll2 && pt2 > mgg/4) fTH1DMap["phi1_pho2pass"]->Fill(phi1,Weight);
      if(passAll1 && pt1 > mgg/3) fTH1DMap["phi2_pho1pass"]->Fill(phi2,Weight);
    }
  }// end loop over entries in tree

  Double_t effPU = 0;
  Double_t effpt = 0;
  Double_t bin = 0;
  for (UInt_t i=0; i<60; i++){
    bin = (Double_t)i;
    if (effPUd[i] > 0) effPU = (Double_t)effPUn[i]/(Double_t)effPUd[i];
    if (effptd[i] > 0) effpt = (Double_t)effptn[i]/(Double_t)effptd[i];
    fTH1DMap["eff_PU"]->Fill(bin,effPU); 
    fTH1DMap["eff_pt"]->Fill(bin*10,effpt); 
  }

  fTH1DMap["eff_sel"]->GetXaxis()->SetBinLabel(1,"nentries");
  fTH1DMap["eff_sel"]->GetXaxis()->SetBinLabel(2,"passPt");
  fTH1DMap["eff_sel"]->GetXaxis()->SetBinLabel(3,"passTrigger");
  fTH1DMap["eff_sel"]->GetXaxis()->SetBinLabel(4,"passCHiso");
  fTH1DMap["eff_sel"]->GetXaxis()->SetBinLabel(5,"passNHiso");
  fTH1DMap["eff_sel"]->GetXaxis()->SetBinLabel(6,"passPHiso");
  fTH1DMap["eff_sel"]->GetXaxis()->SetBinLabel(7,"passSieie");
  fTH1DMap["eff_sel"]->GetXaxis()->SetBinLabel(8,"passHoe");
  fTH1DMap["eff_sel"]->GetXaxis()->SetBinLabel(9,"passMgg");
  fTH1DMap["eff_sel"]->GetXaxis()->SetBinLabel(10,"passMet");  


  fTH1DMap["hlt"]->GetXaxis()->SetBinLabel(1,"nentries");
  fTH1DMap["hlt"]->GetXaxis()->SetBinLabel(2,"Pho26Pho16M60");
  fTH1DMap["hlt"]->GetXaxis()->SetBinLabel(3,"Pho36Pho22M15");
  fTH1DMap["hlt"]->GetXaxis()->SetBinLabel(4,"Pho42Pho25M15");
  fTH1DMap["hlt"]->GetXaxis()->SetBinLabel(5,"Dipho30M95");
  fTH1DMap["hlt"]->GetXaxis()->SetBinLabel(6,"Dipho30M70");
  fTH1DMap["hlt"]->GetXaxis()->SetBinLabel(7,"Dipho30M55");
  fTH1DMap["hlt"]->GetXaxis()->SetBinLabel(8,"Dipho30M55PV");
  fTH1DMap["hlt"]->GetXaxis()->SetBinLabel(9,"Dipho30M55EB");

  std::cout << "phi1 " << fTH1DMap["phi1_n-1"]->Integral() <<  " phi2 " << fTH1DMap["phi2_n-1"]->Integral() << std::endl;

  Plotter::SavePlots();

}// end Plotter::DoPlots


void Plotter::SetUpPlots(){
  // fill all plots from tree
  fTH1DMap["nvtx"]		= Plotter::MakeTH1DPlot("nvtx","",60,0.,60.,"nvtx","");
  fTH1DMap["mgg"]		= Plotter::MakeTH1DPlot("mgg","",40,100.,300.,"m_{#gamma#gamma} (GeV)","");  
  fTH1DMap["ptgg"]		= Plotter::MakeTH1DPlot("ptgg","",100,0.,1000.,"p_{T,#gamma#gamma} (GeV)","");
  fTH1DMap["t1pfmet"]		= Plotter::MakeTH1DPlot("t1pfmet","",100,0.,1000,"t1PF MET (GeV)","");
  fTH1DMap["t1pfmetphi"]	= Plotter::MakeTH1DPlot("t1pfmetphi","",80,-4.,4.,"t1PF MET #phi","");
  fTH1DMap["pfmet"]		= Plotter::MakeTH1DPlot("pfmet","",100,0.,1000,"PF MET (GeV)","");
  fTH1DMap["pfmetphi"]		= Plotter::MakeTH1DPlot("pfmetphi","",80,-4.,4.,"PF MET #phi","");
  fTH1DMap["calomet"]		= Plotter::MakeTH1DPlot("calomet","",100,0.,1000,"calo MET (GeV)","");
  fTH1DMap["calometphi"]	= Plotter::MakeTH1DPlot("calometphi","",80,-4.,4.,"calo MET #phi","");
  fTH1DMap["phi1"]		= Plotter::MakeTH1DPlot("phi1","",80,-4.,4.,"#phi(#gamma1)","");
  fTH1DMap["phi2"]		= Plotter::MakeTH1DPlot("phi2","",80,-4.,4.,"#phi(#gamma2)","");
  fTH1DMap["eta1"]		= Plotter::MakeTH1DPlot("eta1","",100,-5.,5.,"#eta(#gamma1)","");
  fTH1DMap["eta2"]		= Plotter::MakeTH1DPlot("eta2","",100,-5.,5.,"#eta(#gamma2)","");
  fTH1DMap["pt1"]		= Plotter::MakeTH1DPlot("pt1","",50,0.,500.,"p_{T,#gamma1} (GeV)","");
  fTH1DMap["pt2"]		= Plotter::MakeTH1DPlot("pt2","",50,0.,500.,"p_{T,#gamma2} (GeV)","");
  fTH1DMap["chiso1"]		= Plotter::MakeTH1DPlot("chiso1","",75,-5.,15.,"CHiso(#gamma1)","");
  fTH1DMap["chiso2"]		= Plotter::MakeTH1DPlot("chiso2","",75,-5.,15.,"CHiso(#gamma2)","");
  fTH1DMap["neuiso1"]		= Plotter::MakeTH1DPlot("neuiso1","",75,-5.,15.,"NHiso(#gamma1)","");
  fTH1DMap["neuiso2"]		= Plotter::MakeTH1DPlot("neuiso2","",75,-5.,15.,"NHiso(#gamma2)","");
  fTH1DMap["phoiso1"]		= Plotter::MakeTH1DPlot("phoiso1","",75,-5.,15.,"PHiso(#gamma1)",""); 
  fTH1DMap["phoiso2"]		= Plotter::MakeTH1DPlot("phoiso2","",75,-5.,15.,"PHiso(#gamma2)",""); 
  fTH1DMap["sieie1"]		= Plotter::MakeTH1DPlot("sieie1","",150,0.,0.03,"#sigma_{i#eta i#eta}(#gamma1)",""); 
  fTH1DMap["sieie2"]		= Plotter::MakeTH1DPlot("sieie2","",150,0.,0.03,"#sigma_{i#eta i#eta}(#gamma2)",""); 
  fTH1DMap["hoe1"]		= Plotter::MakeTH1DPlot("hoe1","",125,0.,0.025,"H/E(#gamma1)","");
  fTH1DMap["hoe2"]		= Plotter::MakeTH1DPlot("hoe2","",125,0.,0.025,"H/E(#gamma2)","");
  fTH1DMap["r91"]		= Plotter::MakeTH1DPlot("r91","",100,0.,1.1,"R9(#gamma1)","");
  fTH1DMap["r92"]		= Plotter::MakeTH1DPlot("r92","",100,0.,1.1,"R9(#gamma2)","");
  fTH1DMap["t1pfmet_zoom"]	= Plotter::MakeTH1DPlot("t1pfmet_zoom","",60,0.,300.,"t1PF MET (GEV)","");

  fTH1DMap["phi1_pho2pass"]     = Plotter::MakeTH1DPlot("phi1_pho2pass","",80,-4.,4.,"","");
  fTH1DMap["phi2_pho1pass"]     = Plotter::MakeTH1DPlot("phi2_pho1pass","",80,-4.,4.,"","");

  // n minus 1 plots
  fTH1DMap["nvtx_n-1"]		= Plotter::MakeTH1DPlot("nvtx_n-1","",60,0.,60.,"nvtx","");
  fTH1DMap["mgg_n-1"]		= Plotter::MakeTH1DPlot("mgg_n-1","",40,100.,300.,"m_{#gamma#gamma} (GeV)","");  
  fTH1DMap["ptgg_n-1"]		= Plotter::MakeTH1DPlot("ptgg_n-1","",100,0.,1000.,"p_{T,#gamma#gamma} (GeV)","");
  fTH1DMap["t1pfmet_n-1"]	= Plotter::MakeTH1DPlot("t1pfmet_n-1","",100,0.,1000.,"t1PF MET (GeV)","");
  fTH1DMap["t1pfmetphi_n-1"]	= Plotter::MakeTH1DPlot("t1pfmetphi_n-1","",80,-4.,4.,"t1PF MET #phi","");
  fTH1DMap["pfmet_n-1"]		= Plotter::MakeTH1DPlot("pfmet_n-1","",100,0.,1000,"PF MET (GeV)","");
  fTH1DMap["pfmetphi_n-1"]	= Plotter::MakeTH1DPlot("pfmetphi_n-1","",80,-4.,4.,"PF MET #phi","");
  fTH1DMap["calomet_n-1"]	= Plotter::MakeTH1DPlot("calomet_n-1","",100,0.,1000,"calo MET (GeV)","");
  fTH1DMap["calometphi_n-1"]	= Plotter::MakeTH1DPlot("calometphi_n-1","",80,-4.,4.,"calo MET #phi","");
  fTH1DMap["phi1_n-1"]		= Plotter::MakeTH1DPlot("phi1_n-1","",80,-4.,4.,"#phi(#gamma1)","");
  fTH1DMap["phi2_n-1"]		= Plotter::MakeTH1DPlot("phi2_n-1","",80,-4.,4.,"#phi(#gamma2)","");
  fTH1DMap["eta1_n-1"]		= Plotter::MakeTH1DPlot("eta1_n-1","",100,-5.,5.,"#eta(#gamma1)","");
  fTH1DMap["eta2_n-1"]		= Plotter::MakeTH1DPlot("eta2_n-1","",100,-5.,5.,"#eta(#gamma2)","");
  fTH1DMap["pt1_n-1"]		= Plotter::MakeTH1DPlot("pt1_n-1","",50,0.,500.,"p_{T,#gamma1} (GeV)","");
  fTH1DMap["pt2_n-1"]		= Plotter::MakeTH1DPlot("pt2_n-1","",50,0.,500.,"p_{T,#gamma2} (GeV)","");
  fTH1DMap["chiso1_n-1"]	= Plotter::MakeTH1DPlot("chiso1_n-1","",75,-5.,15.,"CHiso(#gamma1)","");
  fTH1DMap["chiso2_n-1"]	= Plotter::MakeTH1DPlot("chiso2_n-1","",75,-5.,15.,"CHiso(#gamma2)","");
  fTH1DMap["neuiso1_n-1"]	= Plotter::MakeTH1DPlot("neuiso1_n-1","",75,-5.,15.,"NHiso(#gamma1)","");
  fTH1DMap["neuiso2_n-1"]	= Plotter::MakeTH1DPlot("neuiso2_n-1","",75,-5.,15.,"NHiso(#gamma2)","");
  fTH1DMap["phoiso1_n-1"]	= Plotter::MakeTH1DPlot("phoiso1_n-1","",75,-5.,15.,"PHiso(#gamma1)",""); 
  fTH1DMap["phoiso2_n-1"]	= Plotter::MakeTH1DPlot("phoiso2_n-1","",75,-5.,15.,"PHiso(#gamma2)",""); 
  fTH1DMap["sieie1_n-1"]	= Plotter::MakeTH1DPlot("sieie1_n-1","",150,0.,0.03,"#sigma_{i#eta i#eta}(#gamma1)",""); 
  fTH1DMap["sieie2_n-1"]	= Plotter::MakeTH1DPlot("sieie2_n-1","",150,0.,0.03,"#sigma_{i#eta i#eta}(#gamma2)",""); 
  fTH1DMap["hoe1_n-1"]		= Plotter::MakeTH1DPlot("hoe1_n-1","",125,0.,0.025,"H/E(#gamma1)","");
  fTH1DMap["hoe2_n-1"]		= Plotter::MakeTH1DPlot("hoe2_n-1","",125,0.,0.025,"H/E(#gamma2)","");
  fTH1DMap["r91_n-1"]		= Plotter::MakeTH1DPlot("r91_n-1","",100,0.,1.1,"R9(#gamma1)","");
  fTH1DMap["r92_n-1"]		= Plotter::MakeTH1DPlot("r92_n-1","",100,0.,1.1,"R9(#gamma2)","");

  // special plots
  fTH1DMap["phigg"]		= Plotter::MakeTH1DPlot("phigg","",80,-4.,4.,"#phi(#gamma#gamma)","");
  fTH1DMap["dphi_ggmet"]	= Plotter::MakeTH1DPlot("dphi_ggmet","",80,-4.,4.,"#Delta#phi(#gamma#gamma,MET)","");
  fTH1DMap["t1pfmet_selmgg"]	= Plotter::MakeTH1DPlot("t1pfmet_selmgg","",100,0.,1000.,"t1PF MET (GeV)","");
  fTH1DMap["mgg_selt1pfmet"]	= Plotter::MakeTH1DPlot("mgg_selt1pfmet","",40,100.,300.,"m_{#gamma#gamma} (GeV)","");

  // efficiency plots
  fTH1DMap["eff_sel"]		= Plotter::MakeTH1DPlot("eff_sel","",10,0.,10.,"","");
  fTH1DMap["eff_PU"]		= Plotter::MakeTH1DPlot("eff_PU","",60,0.,60.,"","");
  fTH1DMap["eff_pt"]		= Plotter::MakeTH1DPlot("eff_pt","",60,0.,600.,"","");
  fTH1DMap["hlt"]		= Plotter::MakeTH1DPlot("hlt","",10,0.,10,"","");

  // 2D plots
  fTH2DMap["mgg_PU"]		= Plotter::MakeTH2DPlot("mgg_PU","",60,0.,60.,40,100.,300.,"nvtx","m_{#gamma#gamma} (GeV)");
  fTH2DMap["mgg_ptgg"] 		= Plotter::MakeTH2DPlot("mgg_ptgg","",50,0.,500.,40,100.,300.,"p_{T,#gamma#gamma} (GeV)","m_{#gamma#gamma}");
  fTH2DMap["t1pfmet_PU"]	= Plotter::MakeTH2DPlot("t1pfmet_PU","",60,50.,300.,100,0.,1000.,"nvtx","MET (GeV)");
  fTH2DMap["t1pfmet_ptgg"]	= Plotter::MakeTH2DPlot("t1pfmet_ptgg","",60,0.,60.,100,0.,1000.,"p_{T,#gamma#gamma} (GeV)","MET (GeV)");
  fTH2DMap["t1pfmet_mgg"]	= Plotter::MakeTH2DPlot("t1pfmet_mgg","",80,100.,180.,1000,0.,1000,"m_{#gamma#gamma} (GeV)","MET (GeV)");

}// end Plotter::SetUpPlots

TH1D * Plotter::MakeTH1DPlot(const TString hname, const TString htitle, const Int_t nbins, const Double_t xlow, const Double_t xhigh, const TString xtitle, const TString ytitle){
  TH1D * hist = new TH1D(hname.Data(),htitle.Data(),nbins,xlow,xhigh);
  hist->GetXaxis()->SetTitle(xtitle.Data());
  hist->GetYaxis()->SetTitle(ytitle.Data());
  hist->Sumw2();
  return hist;
}// end Plotter::MakeTH1DPlot

TH2D * Plotter::MakeTH2DPlot(const TString hname, const TString htitle, const Int_t xnbins, const Double_t xlow, const Double_t xhigh, const Int_t ynbins, const Double_t ylow, const Double_t yhigh, const TString xtitle, const TString ytitle){
  TH2D * hist = new TH2D(hname.Data(),htitle.Data(),xnbins,xlow,xhigh,ynbins,ylow,yhigh);
  hist->GetXaxis()->SetTitle(xtitle.Data());
  hist->GetYaxis()->SetTitle(ytitle.Data());
  return hist;
}// end Plotter::MakeTH2DPlot

TH1D * DrawOverflowBin(const TH1D * h){
    Int_t nbin = h->GetNbinsX()+1;
    Double_t overflow = h->GetBinContent(nbin); 

}


void Plotter::SavePlots(){
  outFile->cd();

  TCanvas * canv = new TCanvas();

  for (TH1DMapIter mapiter = fTH1DMap.begin(); mapiter != fTH1DMap.end(); mapiter++){
    canv->Clear();

    if ((*mapiter).second == (TH1D*) NULL)	{std::cout << "TH1D Null" << std::endl;}
    if (outFile == (TFile*) NULL)		{std::cout << "OutFile Null" << std::endl;}
    if (canv == (TCanvas*) NULL)		{std::cout << "Canvas Null" << std::endl;}

    //fTH1DNewMap[(*mapiter).first].second = DrawOverflowBin( (*mapiter).second );   

    (*mapiter).second->Write(); // save histos to root file 
    canv->cd();
    (*mapiter).second->Draw("HIST");

    CMSLumi(canv,0,fLumi);

    canv->SetLogy(0);
    canv->SaveAs(Form("%s%s/%s.png",fName.Data(),species.Data(),(*mapiter).first.Data()));

    canv->SetLogy(1);
    canv->SaveAs(Form("%s%s/%s_log.png",fName.Data(),species.Data(),(*mapiter).first.Data())); 
  }// end of loop over mapiter for 1d plots
  delete canv;

  TCanvas * canv2d = new TCanvas();

  for (TH2DMapIter mapiter = fTH2DMap.begin(); mapiter != fTH2DMap.end(); mapiter++){
    canv->Clear();

    if ((*mapiter).second == (TH2D*) NULL)	{std::cout << "TH2D Null" << std::endl;}
    if (outFile == (TFile*) NULL)		{std::cout << "OutFile Null" << std::endl;}
    if (canv == (TCanvas*) NULL)		{std::cout << "Canvas Null" << std::endl;}

    (*mapiter).second->Write(); // save histos to root file 
    canv2d->cd();
    (*mapiter).second->Draw("colz");

    CMSLumi(canv2d,0,fLumi);

    canv2d->SetLogy(0);
    canv2d->SaveAs(Form("%s%s/%s.png",fName.Data(),species.Data(),(*mapiter).first.Data()));
  }// end of loop over mapiter for 2d plots
  delete canv2d;

}// end Plotter::SavePlots

void Plotter::DeleteHists(){
  for (TH1DMapIter mapiter = fTH1DMap.begin(); mapiter != fTH1DMap.end(); mapiter++){
    delete ((*mapiter).second);
  }
  fTH1DMap.clear();

  for (TH2DMapIter mapiter = fTH2DMap.begin(); mapiter != fTH2DMap.end(); mapiter++){
    delete ((*mapiter).second);
  }
  fTH2DMap.clear();

}// end Plotter::DeleteHists

void Plotter::SetBranchAddresses(){
  tpho->SetBranchAddress("weight", &weight,  &b_weight);
  tpho->SetBranchAddress("nvtx",   &nvtx,    &b_nvtx);
  tpho->SetBranchAddress("mgg",    &mgg,     &b_mgg);
  tpho->SetBranchAddress("ptgg",   &ptgg,    &b_ptgg);
  tpho->SetBranchAddress("t1pfmet", &t1pfmet, &b_t1pfmet);   
  tpho->SetBranchAddress("t1pfmetPhi", &t1pfmetphi, &b_t1pfmetPhi);   
  tpho->SetBranchAddress("t1pfmetSumEt", &t1pfmetSumEt, &b_t1pfmetSumEt);   
  tpho->SetBranchAddress("pfmet", &pfmet, &b_pfmet);   
  tpho->SetBranchAddress("pfmetPhi", &pfmetphi, &b_pfmetPhi);   
  tpho->SetBranchAddress("pfmetSumEt", &pfmetSumEt, &b_pfmetSumEt);   
  tpho->SetBranchAddress("calomet", &calomet, &b_calomet);   
  tpho->SetBranchAddress("calometPhi", &calometphi, &b_calometPhi);   
  tpho->SetBranchAddress("calometSumEt", &calometSumEt, &b_calometSumEt);   
  tpho->SetBranchAddress("pt1", &pt1, &b_pt1);   
  tpho->SetBranchAddress("pt2", &pt2, &b_pt2);   
  tpho->SetBranchAddress("chiso1", &chiso1, &b_chiso1);   
  tpho->SetBranchAddress("chiso2", &chiso2, &b_chiso2);   
  tpho->SetBranchAddress("neuiso1", &neuiso1, &b_neuiso1);   
  tpho->SetBranchAddress("neuiso2", &neuiso2, &b_neuiso2);   
  tpho->SetBranchAddress("phoiso1", &phoiso1, &b_phoiso1);   
  tpho->SetBranchAddress("phoiso2", &phoiso2, &b_phoiso2);   
  tpho->SetBranchAddress("sieie1", &sieie1, &b_sieie1);   
  tpho->SetBranchAddress("sieie2", &sieie2, &b_sieie2);   
  tpho->SetBranchAddress("hoe1", &hoe1, &b_hoe1);   
  tpho->SetBranchAddress("hoe2", &hoe2, &b_hoe2);   
  tpho->SetBranchAddress("r91", &r91, &b_r91);   
  tpho->SetBranchAddress("r92", &r92, &b_r92);   
  tpho->SetBranchAddress("phi1", &phi1, &b_phi1);   
  tpho->SetBranchAddress("phi2", &phi2, &b_phi2);   
  tpho->SetBranchAddress("eta1", &eta1, &b_eta1);   
  tpho->SetBranchAddress("eta2", &eta2, &b_eta2);   
  tpho->SetBranchAddress("passCHiso1", &passCHiso1, &b_passCHiso1);   
  tpho->SetBranchAddress("passCHiso2", &passCHiso2, &b_passCHiso2);   
  tpho->SetBranchAddress("passNHiso1", &passNHiso1, &b_passNHiso1);   
  tpho->SetBranchAddress("passNHiso2", &passNHiso2, &b_passNHiso2);   
  tpho->SetBranchAddress("passPHiso1", &passPHiso1, &b_passNHiso1);   
  tpho->SetBranchAddress("passPHiso2", &passPHiso2, &b_passNHiso2);   
  tpho->SetBranchAddress("passSieie1", &passSieie1, &b_passSieie1);
  tpho->SetBranchAddress("passSieie2", &passSieie2, &b_passSieie2);
  tpho->SetBranchAddress("passHoe1", &passHoe1, &b_passHoe1);
  tpho->SetBranchAddress("passHoe2", &passHoe2, &b_passHoe2);
  tpho->SetBranchAddress("hltPhoton26Photon16Mass60", &hltPhoton26Photon16Mass60, &b_hltPhoton26Photon16Mass60);
  tpho->SetBranchAddress("hltPhoton36Photon22Mass15", &hltPhoton36Photon22Mass15, &b_hltPhoton36Photon22Mass15);
  tpho->SetBranchAddress("hltPhoton42Photon25Mass15", &hltPhoton42Photon25Mass15, &b_hltPhoton42Photon25Mass15);
  tpho->SetBranchAddress("hltDiphoton30Mass95", &hltDiphoton30Mass95, &b_hltDiphoton30Mass95);
  tpho->SetBranchAddress("hltDiphoton30Mass70", &hltDiphoton30Mass70, &b_hltDiphoton30Mass70);
  tpho->SetBranchAddress("hltDiphoton30Mass55", &hltDiphoton30Mass55, &b_hltDiphoton30Mass55);
  tpho->SetBranchAddress("hltDiphoton30Mass55PV", &hltDiphoton30Mass55PV, &b_hltDiphoton30Mass55PV);
  tpho->SetBranchAddress("hltDiphoton30Mass55EB", &hltDiphoton30Mass55EB, &b_hltDiphoton30Mass55EB);


  //tpho->SetBranchAddress("", &, &b_);
  
}// end Plotter::SetBranchAddresses


void Plotter::DeleteBranches(){
  delete b_weight;
  delete b_nvtx;
  delete b_mgg;
  delete b_ptgg;
  delete b_pt1;
  delete b_pt2;
}// end Plotter::DeleteBranches


void Plotter::FindMinAndMax(TH1F *& h, int plotLog){
  Float_t max = h->GetMaximum();
  if (plotLog==1) h->SetMaximum(10*max);
  if (plotLog==0) h->SetMaximum(2*max);

  Float_t min = 1000;
  Bool_t newmin = false;

  for (Int_t bin=1; bin <= h->GetNbinsX(); bin++){
    Float_t tmpmin = h->GetBinContent(bin);
    if ((tmpmin < min) && (tmpmin > 0)){
      min = tmpmin;
      newmin = true;
    }
  }

  if (newmin){
    h->SetMinimum(0.90*min);
  }
}// end Plotter::FindMinAndMax



