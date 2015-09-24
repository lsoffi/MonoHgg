#define photonIdStudies_cxx
#include "photonIdStudies.h"
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <iostream> 

using namespace std;

void photonIdStudies::Loop()
{
  // ------------------------------------
  // chiara: to be set
  bool isSignal   = false;
  bool highR9only = false;
  bool preselOnly = false;
  bool noGapOnly  = true;
  // ------------------------------------


  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;

  // booking histos: kinematics
  TH1F *HEB_pt = new TH1F("HEB_pt","HEB_pt",100,0.,3000.);
  TH1F *HEE_pt = new TH1F("HEE_pt","HEE_pt",100,0.,3000.);

  // booking histos: ID variables
  TH1F *HEB_sieie_0_500     = new TH1F("HEB_sieie_0_500","HEB_sieie_0_500",100,0.,0.02);
  TH1F *HEE_sieie_0_500     = new TH1F("HEE_sieie_0_500","HEE_sieie_0_500",100,0.,0.055);
  TH1F *HEB_sieie_500_1000  = new TH1F("HEB_sieie_500_1000","HEB_sieie_500_1000",100,0.,0.02);
  TH1F *HEE_sieie_500_1000  = new TH1F("HEE_sieie_500_1000","HEE_sieie_500_1000",100,0.,0.055);
  TH1F *HEB_sieie_1000_2000 = new TH1F("HEB_sieie_1000_2000","HEB_sieie_1000_2000",100,0.,0.02);
  TH1F *HEE_sieie_1000_2000 = new TH1F("HEE_sieie_1000_2000","HEE_sieie_1000_2000",100,0.,0.055);


  // loop over entries
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    // minimal pT (30 GeV) and eta selection applied at the dumper level

    // restricting the analysis to photons matching or not the MC truth
    if (isSignal  && mctruth_truePt<-800) continue;          
    if (!isSignal && mctruth_truePt>-800) continue;             

    // restricting to good photons: high R9 photons
    if (highR9only && identificationNoZS_r9noZS<0.9) continue;

    // restricting the analysis to photons passing the miniAOD preselection
    bool isPreselected = false;
    if ( identification_r9>0.8 || isolation_chHadIso<20 || (isolation_chHadIso/kinematics_pt)<0.3 ) isPreselected = true;
    if (preselOnly && !isPreselected) continue;

    // extended gaps 
    bool inExtEBPhiGap = false;
    bool inExtEBEtaGap = false;
    int ieta = abs(tree5x5_ieta[12]);
    if ( ieta==0 ||  ieta==1 ||  ieta==2 ||  
	 ieta==23 || ieta==24 || ieta==25 || ieta==26 || ieta==27 || 
	 ieta==43 || ieta==44 || ieta==45 || ieta==46 || ieta==47 ||
	 ieta==63 || ieta==64 || ieta==65 || ieta==66 || ieta==67 ||
	 ieta==83 || ieta==84 || ieta==85) inExtEBEtaGap = true;
    
    // restricting to good photons: out of gaps
    if (noGapOnly) {
      if (kinematics_isEBEtaGap || kinematics_isEBPhiGap || 
	  kinematics_isEERingGap || kinematics_isEEDeeGap || 
	  kinematics_isEBEEGap || inExtEBEtaGap || inExtEBPhiGap ) continue;
    }


    // several H/E variables
    // float hoetrue  = identification_hoe*(energy_energy/mctruth_trueEnergy);
    // float htoetrue = identification_htoe*(energy_energy/mctruth_trueEnergy);


    // filling histos
    if (fabs(supercluster_scEta)<1.5) HEB_pt -> Fill(kinematics_pt);
    if (fabs(supercluster_scEta)>1.5) HEE_pt -> Fill(kinematics_pt);
    
    if (kinematics_pt>0 && kinematics_pt<=500) { 
      if (fabs(supercluster_scEta)<1.5) {
	HEB_sieie_0_500 -> Fill(identificationNoZS_sieienoZS);
      } else {
	HEE_sieie_0_500 -> Fill(identificationNoZS_sieienoZS);
      }
    } else if (kinematics_pt>500 && kinematics_pt<=1000) {
      if (fabs(supercluster_scEta)<1.5) {
	HEB_sieie_500_1000 -> Fill(identificationNoZS_sieienoZS);
      } else {
	HEE_sieie_500_1000 -> Fill(identificationNoZS_sieienoZS);
      }
    } else if (kinematics_pt>1000 && kinematics_pt<=2000) {
      if (fabs(supercluster_scEta)<1.5) {
	HEB_sieie_1000_2000 -> Fill(identificationNoZS_sieienoZS);
      } else {
	HEE_sieie_1000_2000 -> Fill(identificationNoZS_sieienoZS);
      }
    }

  }  // loop over entries

  // Plots
  gStyle->SetOptStat(0);

  TCanvas c("c","c",1);
  c.Divide(2,1);
  c.cd(1); HEB_pt->Draw();
  c.cd(2); HEE_pt->Draw();
  c.SaveAs("pT.png");

  TCanvas c1a("c1a","c1a",1);
  HEB_sieie_0_500 -> Draw();
  c1a.SaveAs("HEB_sieie_0_500.png");
  TCanvas c1b("c1b","c1a",1);
  HEE_sieie_0_500 -> Draw();
  c1b.SaveAs("HEE_sieie_0_500.png");

  TCanvas c2a("c2a","c2a",1);
  HEB_sieie_500_1000 -> Draw();
  c2a.SaveAs("HEB_sieie_500_1000.png");
  TCanvas c2b("c2b","c2a",1);
  HEE_sieie_500_1000 -> Draw();
  c2b.SaveAs("HEE_sieie_500_1000.png");

  TCanvas c3a("c3a","c3a",1);
  HEB_sieie_1000_2000 -> Draw();
  c3a.SaveAs("HEB_sieie_1000_2000.png");
  TCanvas c3b("c3b","c3a",1);
  HEE_sieie_1000_2000 -> Draw();
  c3b.SaveAs("HEE_sieie_1000_2000.png");


  TFile myOut("outputFile.root","RECREATE");
  myOut.cd();
  HEB_sieie_0_500     -> Write();
  HEE_sieie_0_500     -> Write();
  HEB_sieie_500_1000  -> Write();
  HEE_sieie_500_1000  -> Write();
  HEB_sieie_1000_2000 -> Write();
  HEE_sieie_1000_2000 -> Write();
  myOut.Close();
}
